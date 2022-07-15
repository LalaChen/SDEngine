/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "nlohmann/json.hpp"

#include "LogManager.h"
#include "ECSManager.h"
#include "TransformComponent.h"
#include "Scene.h"

using SDE::Basic::ECSManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

Scene::Scene(const ObjectName & i_object_name, const FilePathString &i_fp)
: Object(i_object_name)
, m_scene_fp(i_fp)
, m_loading_status(LoadStatus_NONE)
{
}

Scene::~Scene()
{
}

EntityWeakReferenceObject Scene::GetRoot() const
{
    LoadStatusEnum cur_status = GetStatus();
    if (cur_status != LoadStatus_FINISHED) {
        return EntityWeakReferenceObject();
    }
    else {
        return (*m_entities.begin());
    }
}

bool Scene::Load()
{
    LoadStatusEnum cur_status = GetStatus();
    if (cur_status == LoadStatus_NONE) {
        SetStatus(LoadStatus_INPROGRESS);
        bool result = LoadImpl();
        if (result == true) {
            SetStatus(LoadStatus_FINISHED);
        }
        else {
            UnloadImpl();
            SetStatus(LoadStatus_NONE);
        }
        return result;
    }
    else if (cur_status == LoadStatus_INPROGRESS) {
        SDLOGW("Scene[%s] is loading now.", m_object_name.c_str());
        return false;
    }
    else  {
        SDLOGW("Scene[%s] has been loaded.", m_object_name.c_str());
        return false;
    }
}

bool Scene::Unload()
{
    LoadStatusEnum cur_status = GetStatus();
    if (cur_status == LoadStatus_FINISHED) {
        UnloadImpl();
        SetStatus(LoadStatus_NONE);
        return true;
    }
    else {
        SDLOGW("Scene[%s] han't been loaded.", m_object_name.c_str());
        return false;
    }
}

bool Scene::LoadImpl()
{
    EntityWeakReferenceObject root = ECSManager::GetRef().CreateEntity(SDE::Basic::StringFormat("%s_root", m_object_name.c_str()));
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(root, SDE::Basic::StringFormat("%s_Transform", SD_WREF(root).GetObjectName().c_str()));
    m_entities.push_back(root);
    return true;
}

void Scene::UnloadImpl()
{
    for (std::list<EntityWeakReferenceObject>::reverse_iterator e_iter = m_entities.rbegin(); e_iter != m_entities.rend();) {
        ECSManager::GetRef().DeleteEntity((*e_iter));
        m_entities.erase(std::next(e_iter).base());
    }
}

void Scene::SetStatus(LoadStatusEnum i_status)
{
    std::lock_guard<std::mutex> lck(m_ld_mutex);
    m_loading_status = i_status;
}

Scene::LoadStatusEnum Scene::GetStatus() const
{
    std::lock_guard<std::mutex> lck(m_ld_mutex);
    return m_loading_status;
}

______________SD_END_GRAPHICS_NAMESPACE______________