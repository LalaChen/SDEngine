/* ==============  SD Engine License ==============
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

#include "Application.h"
#include "ECSManager.h"
#include "VRCameraComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "MeshRenderComponent.h"
#include "GraphicsSystem.h"

using SDE::Basic::StringFormat;
using SDE::Basic::Application;
using SDE::Basic::ECSManager;

using SDE::Basic::MemberFunctionSlot;

using SDE::Basic::Entity;
using SDE::Basic::EntityStrongReferenceObject;
using SDE::Basic::EntityWeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsSystem::GraphicsSystem(const ObjectName &i_object_name)
: System(i_object_name)
, m_scene_changed(true)
, m_update_every_frame(true)
{
}

GraphicsSystem::~GraphicsSystem()
{
}

void GraphicsSystem::Initialize()
{
    //1. Initialize command buffer and pools.
    m_graphics_cp = new CommandPool("GSCmdPool");
    SD_REF(m_graphics_cp).Initialize();
    m_graphics_cb = SD_REF(m_graphics_cp).AllocateCommandBuffer("GSCmdBuffer");

    //2. Initialize necessary group.
    m_camera_eg = ECSManager::GetRef().AddEntityGroup(
        "CameraBaseGroup",
        {
            std::type_index(typeid(CameraComponentBase))
        }
    );

    m_mesh_render_eg = ECSManager::GetRef().AddEntityGroup(
        "MeshRenderGroup",
        {
            std::type_index(typeid(MeshRenderComponent))
        }
    );

    m_light_eg = ECSManager::GetRef().AddEntityGroup(
        "LightGroup",
        {
            std::type_index(typeid(LightComponent))
        }
    );

    //3. initialize rec_thread.
    uint32_t max_threads = std::thread::hardware_concurrency();
    m_rec_threads.resize(max_threads);
    for (uint32_t tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID] = new SecondaryCommandPoolThread(StringFormat("RecordThread_%d", tID));
        m_rec_threads[tID].GetRef().Initialize();
    }

    //4. decide update every frame or not.
    if (Application::GetRef().GetAdoptLibrary() == GraphicsLibrary_Vulkan) {
        m_update_every_frame = false;
    }
}

void GraphicsSystem::Update()
{
    if (m_update_every_frame == true || m_scene_changed == true) {
        RecordCommand();
        m_scene_changed = false;
    }

    GraphicsManager::GetRef().SubmitCommandBuffersToQueue({ m_graphics_cb });

    std::list<EntityWeakReferenceObject> camera_entity_list = SD_WREF(m_camera_eg).GetEntities();
    std::list<CameraComponentBaseWeakReferenceObject> camera_list;
    for (EntityWeakReferenceObject &ce : camera_entity_list) {
        camera_list.push_back(SD_GET_COMP_WREF(ce, CameraComponentBase));
    }

    GraphicsManager::GetRef().RenderTexture2DToScreen(SD_WREF((*camera_list.begin())).GetColorBuffer());
}

void GraphicsSystem::Destroy()
{
    for (uint32_t tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].Reset();
    }

    SD_SREF(m_graphics_cp).Clear();
    m_graphics_cp.Reset();

}

void GraphicsSystem::Resize()
{
    std::list<EntityWeakReferenceObject> camera_entity_list = m_camera_eg.GetRef().GetEntities();
    for (EntityWeakReferenceObject ce : camera_entity_list) {
        SD_WREF(SD_GET_COMP_WREF(ce, CameraComponentBase)).Resize();
    }

    m_scene_changed = true;
}

void GraphicsSystem::RecordCommand()
{
    ClearValue clear_color = { 0.15f, 0.15f, 0.75f, 1.0f };
    ClearValue clear_dands = { 1.0f, 1 };
    Resolution current_res = GraphicsManager::GetRef().GetScreenResolution();
    //Negative viewport.
    Viewport vp;
    vp.m_x = 0.0f; vp.m_y = static_cast<float>(current_res.GetHeight());
    vp.m_width = static_cast<float>(current_res.GetWidth());
    vp.m_height = -1.0f * static_cast<float>(current_res.GetHeight());
    vp.m_min_depth = 0.0f;
    vp.m_max_depth = 1.0f;

    ScissorRegion sr;
    sr.m_x = 0.0f; sr.m_y = 0.0f;
    sr.m_width = vp.m_width; sr.m_height = static_cast<float>(current_res.GetHeight());

    std::list<CommandBufferWeakReferenceObject> secondary_cbs;
    std::list<EntityWeakReferenceObject> camera_entity_list = SD_WREF(m_camera_eg).GetEntities();
    std::list<EntityWeakReferenceObject> mesh_render_entity_list = SD_WREF(m_mesh_render_eg).GetEntities();
    std::list<EntityWeakReferenceObject> light_entity_list = SD_WREF(m_light_eg).GetEntities();
    std::list<LightComponentWeakReferenceObject> light_list;
    std::list<CameraComponentBaseWeakReferenceObject> camera_list;
    std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> > mr_group_map;
    //1. collect necessary components.
    for (EntityWeakReferenceObject &le : light_entity_list) {
        light_list.push_back(SD_GET_COMP_WREF(le, LightComponent));
    }

    for (EntityWeakReferenceObject &ce : camera_entity_list) {
        camera_list.push_back(SD_GET_COMP_WREF(ce, CameraComponentBase));
    }

    for (EntityWeakReferenceObject &mre : mesh_render_entity_list) {
        MeshRenderComponentWeakReferenceObject mr_comp = SD_GET_COMP_WREF(mre, MeshRenderComponent);
        uint32_t mr_order = SD_WREF(mr_comp).GetRenderOrder().GetOrder();
        std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> >::iterator mr_iter = mr_group_map.find(mr_order);
        if (mr_iter != mr_group_map.end()) {
            (*mr_iter).second.push_back(mr_comp);
        }
        else {
            mr_group_map[mr_order].push_back(mr_comp);
        }
    }

    SD_SREF(m_graphics_cb).Begin();
    GraphicsManager::GetRef().SetViewport(m_graphics_cb, vp);
    GraphicsManager::GetRef().SetScissor(m_graphics_cb, sr);

    //2. update shadow map.

    //3. update stencil buffer. (To Do)

    //4. record command for camera.
    for (CameraComponentBaseWeakReferenceObject &camera : camera_list) {
        SD_WREF(camera).RecordCommand(m_graphics_cb, light_list, mr_group_map);
    }
    
    SD_SREF(m_graphics_cb).End();
}

const std::vector<SecondaryCommandPoolThreadStrongReferenceObject>& GraphicsSystem::GetSecondaryCommandPool() const
{
    return m_rec_threads;
}

______________SD_END_GRAPHICS_NAMESPACE______________