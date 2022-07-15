#include "SceneManager.h"
#include "LogManager.h"
#include "ECSManager.h"

#include "GraphicsSystem.h"

#include "TransformComponent.h"

using SDE::Basic::ECSManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_SINGLETON_DECLARATION_IMPL(SceneManager);

SceneManager::SceneManager()
{
    SD_SINGLETON_DECLARATION_REGISTER;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
    m_dispatcher = new SceneLoadingDispatcher("SceneLoadingDispatcher");
    SD_SREF(m_dispatcher).Run();

    m_global_root = ECSManager::GetRef().CreateEntity("GlobalRoot");
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_global_root, SDE::Basic::StringFormat("%s_Transform", SD_WREF(m_global_root).GetObjectName().c_str()));
}

void SceneManager::Terminate()
{
    for (std::map<ObjectName, SceneStrongReferenceObject>::iterator scene_iter = m_scene_maps.begin(); scene_iter != m_scene_maps.end(); ) {
        SD_SREF((*scene_iter).second).Unload();
        (*scene_iter).second.Reset();
        scene_iter = m_scene_maps.erase(scene_iter);
    }
    ECSManager::GetRef().DeleteEntity(m_global_root);

    m_dispatcher.Reset();
}

bool SceneManager::RegisterScene(const FilePathString &i_fp)
{
    return false;
}

bool SceneManager::RegisterScene(const SceneStrongReferenceObject &i_scene)
{
    if (i_scene.IsNull() == true) {
        return false;
    }

    ObjectName target_scene = SD_SREF(i_scene).GetObjectName();

    std::map<ObjectName, SceneStrongReferenceObject>::iterator scene_iter =
        m_scene_maps.find(target_scene);
    if (scene_iter != m_scene_maps.end()) {
        SDLOGW("Scene[%s] is existed", target_scene.c_str());
        return false;
    }
    else {
        m_scene_maps[target_scene] = i_scene;
        return true;
    }
}

bool SceneManager::LoadScene(const ObjectName &i_scene_name)
{
    std::map<ObjectName, SceneStrongReferenceObject>::iterator scene_iter =
        m_scene_maps.find(i_scene_name);
    if (scene_iter == m_scene_maps.end()) {
        SDLOGE("We can't find scene[%s].", i_scene_name.c_str());
        return false;
    }
    bool result = SD_SREF((*scene_iter).second).Load();
    if (result == true) {
        EntityWeakReferenceObject scene_root = SD_SREF((*scene_iter).second).GetRoot();
        SD_COMP_WREF(m_global_root, TransformComponent).AddChild(SD_GET_COMP_WREF(scene_root, TransformComponent));
        GraphicsSystemWeakReferenceObject gs = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
        if (gs.IsNull() == false) {
            SD_WREF(gs).NotifySceneChanged();
        }
        else {
            SDLOGW("Load scene [%s]. GS is not existed.", i_scene_name.c_str());
        }
    }
    else {
        SDLOGE("Load scene[%s] failure.", i_scene_name.c_str());
    }
    return result;
}

void SceneManager::LoadSceneAsync(const ObjectName &i_scene_name)
{
    std::map<ObjectName, SceneStrongReferenceObject>::iterator scene_iter =
        m_scene_maps.find(i_scene_name);
    if (scene_iter == m_scene_maps.end()) {
        SDLOGE("We can't find scene[%s].", i_scene_name.c_str());
    }
    else {
        SceneWeakReferenceObject s_wref = (*scene_iter).second;
        std::function<void()> task = [s_wref] {
            bool result = SD_WREF(s_wref).Load();
            if (result == true) {
                GraphicsSystemWeakReferenceObject gs = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
                if (gs.IsNull() == false) {
                    SD_WREF(gs).NotifySceneChanged();
                }
                else {
                    SDLOGW("Load scene async[%s]. GS is not existed.", SD_WREF(s_wref).GetObjectName().c_str());
                }
            }
            else {
                SDLOGE("Load scene async[%s] failure.", SD_WREF(s_wref).GetObjectName().c_str());
            }
        };

        SD_SREF(m_dispatcher).AddTask(task);
    }
}

bool SceneManager::UnloadScene(const ObjectName &i_scene_name)
{
    std::map<ObjectName, SceneStrongReferenceObject>::iterator scene_iter =
        m_scene_maps.find(i_scene_name);
    if (scene_iter == m_scene_maps.end()) {
        SDLOGE("We can't find scene[%s].", i_scene_name.c_str());
        return false;
    }
    return SD_SREF((*scene_iter).second).Unload();
}

______________SD_END_GRAPHICS_NAMESPACE______________