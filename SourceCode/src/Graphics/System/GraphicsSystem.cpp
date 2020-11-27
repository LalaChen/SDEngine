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
#include "CameraComponent.h"
#include "LightComponent.h"
#include "MeshRenderComponent.h"
#include "GraphicsSystem.h"

using SDE::Basic::StringFormat;
using SDE::Basic::Application;
using SDE::Basic::ECSManager;

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
    m_graphics_cp_sref = new CommandPool("GSCmdPool");
    SD_REF(m_graphics_cp_sref).Initialize();
    m_graphics_cb_wref = SD_REF(m_graphics_cp_sref).AllocateCommandBuffer("GSCmdBuffer");

    //2. Initialize necessary group.
    m_camera_eg_wref = ECSManager::GetRef().AddEntityGroup(
        "CameraGroup",
        {
            std::type_index(typeid(CameraComponent))
        }
    );

    m_mesh_render_eg_wref = ECSManager::GetRef().AddEntityGroup(
        "MeshRenderGroup",
        {
            std::type_index(typeid(MeshRenderComponent))
        }
    );

    m_light_eg_wref = ECSManager::GetRef().AddEntityGroup(
        "LightGroup",
        {
            std::type_index(typeid(LightComponent))
        }
    );

    //3. initialize rec_thread.
    uint32_t max_threads = std::thread::hardware_concurrency();
    m_rec_thread_srefs.resize(max_threads);
    for (uint32_t tID = 0; tID < m_rec_thread_srefs.size(); ++tID) {
        m_rec_thread_srefs[tID] = new SecondaryCommandPoolThread(StringFormat("RecordThread_%d", tID));
        m_rec_thread_srefs[tID].GetRef().Initialize();
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

    GraphicsManager::GetRef().SubmitCommandBuffersToQueue({ m_graphics_cb_wref });
}

void GraphicsSystem::Destroy()
{
    for (uint32_t tID = 0; tID < m_rec_thread_srefs.size(); ++tID) {
        m_rec_thread_srefs[tID].Reset();
    }

    SD_SREF(m_graphics_cp_sref).Clear();
    m_graphics_cp_sref.Reset();

}

void GraphicsSystem::Resize()
{
    std::list<EntityWeakReferenceObject> camera_entity_list = m_camera_eg_wref.GetRef().GetEntities();
    for (EntityWeakReferenceObject ce_wref : camera_entity_list) {
        SD_WREF(SD_GET_COMP_WREF(ce_wref, CameraComponent).DynamicCastTo<CameraComponent>()).Resize();
    }
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

    std::list<CommandBufferWeakReferenceObject> secondary_cb_wrefs;
    std::list<EntityWeakReferenceObject> camera_entity_list = SD_WREF(m_camera_eg_wref).GetEntities();
    std::list<EntityWeakReferenceObject> mesh_render_entity_list = SD_WREF(m_mesh_render_eg_wref).GetEntities();
    std::list<EntityWeakReferenceObject> light_entity_list = SD_WREF(m_light_eg_wref).GetEntities();
    std::list<LightComponentWeakReferenceObject> light_list;
    std::list<MeshRenderComponentWeakReferenceObject> mesh_render_list;
    std::list<CameraComponentWeakReferenceObject> camera_list;
    //1. collect necessary components.
    for (EntityWeakReferenceObject &le_wref : light_entity_list) {
        light_list.push_back(SD_GET_COMP_WREF(le_wref, LightComponent));
    }

    for (EntityWeakReferenceObject &ce_wref : camera_entity_list) {
        camera_list.push_back(SD_GET_COMP_WREF(ce_wref, CameraComponent));
    }

    for (EntityWeakReferenceObject &mre_wref : mesh_render_entity_list) {
        mesh_render_list.push_back(SD_GET_COMP_WREF(mre_wref, MeshRenderComponent));
    }

    SD_SREF(m_graphics_cb_wref).Begin();
    GraphicsManager::GetRef().SetViewport(m_graphics_cb_wref, vp);
    GraphicsManager::GetRef().SetScissor(m_graphics_cb_wref, sr);

    //2. update shadow map.

    //3. update stencil buffer. (To Do)

    //4. record command for camera.
    for (CameraComponentWeakReferenceObject &camera_wref : camera_list) {
        SD_WREF(camera_wref).RecordCommand(m_graphics_cb_wref, light_list, mesh_render_list);
    }
    
    SD_SREF(m_graphics_cb_wref).End();
}

const std::vector<SecondaryCommandPoolThreadStrongReferenceObject>& GraphicsSystem::GetSecondaryCommandPool() const
{
    return m_rec_thread_srefs;
}

______________SD_END_GRAPHICS_NAMESPACE______________