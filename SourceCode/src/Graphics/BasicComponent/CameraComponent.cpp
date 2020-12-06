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

#include "BasicUniforms.h"
#include "ECSManager.h"
#include "GraphicsSystem.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "CameraComponent.h"

using SDE::Basic::ECSManager;
using SDE::Basic::MemberFunctionSlot;

_____________SD_START_GRAPHICS_NAMESPACE_____________

CameraComponent::CameraComponent(const ObjectName &i_object_name)
: Component(i_object_name)
, m_workspace_type(WorkspaceType_Forward)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
, m_fov(120.0f)
, m_near(0.01f)
, m_far(1000.0f)
{
    m_screen_size = GraphicsManager::GetRef().GetScreenResolution();
}

CameraComponent::~CameraComponent()
{
    ClearWorkspace();
}

void CameraComponent::Initialize()
{
    InitializeDescriptorSetAndPool();
 
    if (m_workspace_type == WorkspaceType_Forward) {
        InitializeWorkspaceForForwardPath();
    }
    else if (m_workspace_type == WorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPath();
    }

    m_geo_comp_wref = SD_GET_COMP_WREF(m_entity_wref, TransformComponent);

    SD_WREF(m_geo_comp_wref).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<CameraComponent>(
            "CameraComponent::OnGeometryChanged",
            GetThisWeakPtrByType<CameraComponent>(),
            &CameraComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());
}

void CameraComponent::Resize()
{
    ClearWorkspace();

    if (m_workspace_type == WorkspaceType_Forward) {
        InitializeWorkspaceForForwardPath();
    }
    else if (m_workspace_type == WorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPath();
    }

    SetPerspective(m_fov, m_near, m_far);
    OnGeometryChanged(EventArg());
}

void CameraComponent::RecordCommand(
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const std::list<LightComponentWeakReferenceObject> &i_light_list,
    const std::list<MeshRenderComponentWeakReferenceObject> &i_mesh_render_list)
{
    GraphicsSystemWeakReferenceObject gs_wref = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
    std::list<LightComponentWeakReferenceObject>::const_iterator light_wref_iter;
    std::list<MeshRenderComponentWeakReferenceObject>::const_iterator mr_wref_iter;

    const std::vector<SecondaryCommandPoolThreadStrongReferenceObject> &scp_threads = SD_WREF(gs_wref).GetSecondaryCommandPool();
    std::list<CommandBufferWeakReferenceObject> secondary_cb_wrefs;
    Viewport vp;
    vp.m_x = 0.0f; vp.m_y = static_cast<float>(m_screen_size.GetHeight());
    vp.m_width = static_cast<float>(m_screen_size.GetWidth());
    vp.m_height = -1.0f * static_cast<float>(m_screen_size.GetHeight());
    vp.m_min_depth = 0.0f;
    vp.m_max_depth = 1.0f;

    ScissorRegion sr;
    sr.m_x = 0.0f; sr.m_y = 0.0f;
    sr.m_width = vp.m_width;
    sr.m_height = static_cast<float>(m_screen_size.GetHeight());

    if (m_workspace_type == WorkspaceType_Forward) {
        SD_SREF(m_rf_sref).BeginRenderFlow(i_cb_wref);
        CommandBufferInheritanceInfo cb_inherit_info = SD_SREF(m_rf_sref).GetCurrentInheritanceInfo();
        RenderPassWeakReferenceObject current_rp = cb_inherit_info.m_rp_wref;

        GraphicsManager::GetRef().SetViewport(i_cb_wref, vp);
        GraphicsManager::GetRef().SetScissor(i_cb_wref, sr);

#if 0
        uint32_t tID = 0;
        for (tID = 0; tID < scp_threads.size(); ++tID) {
            SD_SREF(scp_threads[tID]).StartRecording(cb_inherit_info, vp, sr);
        }

        light_wref_iter = i_light_list.begin();
        tID = 0;
        for (mr_wref_iter = i_mesh_render_list.begin(); mr_wref_iter != i_mesh_render_list.end(); ++mr_wref_iter) {
            DescriptorSetWeakReferenceObject light_ds_wref = SD_WREF((*light_wref_iter)).GetDescriptorSet();
            MeshRenderComponentWeakReferenceObject mr_wref = (*mr_wref_iter);
            std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, current_rp, light_ds_wref, mr_wref](const CommandBufferWeakReferenceObject &i_cb_wref) {
                SD_WREF(mr_wref).RenderMesh(current_rp, i_cb_wref, m_ds_wref, light_ds_wref, 0);
            };

            SD_SREF(scp_threads[tID]).AddTask(task_func);
            tID = (tID + 1) % scp_threads.size();
        }

        for (tID = 0; tID < scp_threads.size(); ++tID) {
            SD_SREF(scp_threads[tID]).WaitAndStopRecording(secondary_cb_wrefs);
        }

        GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(i_cb_wref, secondary_cb_wrefs);
#else
        light_wref_iter = i_light_list.begin();
        for (mr_wref_iter = i_mesh_render_list.begin(); mr_wref_iter != i_mesh_render_list.end(); ++mr_wref_iter) {
            DescriptorSetWeakReferenceObject light_ds_wref = SD_WREF((*light_wref_iter)).GetDescriptorSet();
            MeshRenderComponentWeakReferenceObject mr_wref = (*mr_wref_iter);
            SD_WREF(mr_wref).RenderMesh(current_rp, i_cb_wref, m_ds_wref, light_ds_wref, 0);
        }
#endif

        SD_SREF(m_rf_sref).EndRenderFlow(i_cb_wref);
    }
    else if (m_workspace_type == WorkspaceType_Deferred) {

    }
}


//------------------ Private Part ---------------
void CameraComponent::InitializeDescriptorSetAndPool()
{
    m_dp_sref = new DescriptorPool("CameraPool");
    std::map<ObjectName, UniformVariableWeakReferenceObject> uv_wrefs;
    //1. Collect all descriptor set layouts used at this component.
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    DescriptorSetLayoutWeakReferenceObject dsl_wref = GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Camera");
    SD_SREF(dsl_wref).GetUniformDescriptorCounts(desc_counts);
    SD_SREF(m_dp_sref).Initialize(desc_counts, 1, false);
    //2. Allocate descriptor set.
    m_ds_wref = SD_SREF(m_dp_sref).AllocateDescriptorSet(dsl_wref);
    SD_WREF(m_ds_wref).GetAllocatedUniformVariables(uv_wrefs);

    if (m_ds_wref.IsNull() == false) {
        SD_SREF(m_ds_wref).WriteDescriptor();
    }

    if (uv_wrefs.find("camera") != uv_wrefs.end()) {
        m_buffer_wref = uv_wrefs["camera"].DynamicCastTo<UniformBuffer>();
    }

    if (m_buffer_wref.IsNull() == true) {
        SDLOGE("We can find camera uniform buffer.");
    }
}

void CameraComponent::InitializeWorkspaceForForwardPath()
{
    RenderPassWeakReferenceObject forward_rp_wref = GraphicsManager::GetRef().GetRenderPass("ForwardPath");

    if (forward_rp_wref.IsNull() == false) {
        if (m_color_buf_sref.IsNull() == false) {
            m_color_buf_sref.Reset();
        }
        m_color_buf_sref = new Texture("CameraColorBuffer");
        SD_SREF(m_color_buf_sref).Initialize2DColorOrDepthBuffer(
            m_screen_size.GetWidth(), m_screen_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buf_sref.IsNull() == false) {
            m_depth_buf_sref.Reset();
        }
        m_depth_buf_sref = new Texture("CameraDepthBuffer");
        SD_SREF(m_depth_buf_sref).Initialize2DColorOrDepthBuffer(
            m_screen_size.GetWidth(), m_screen_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        m_rf_sref = new RenderFlow("RenderFlow", ImageOffset(0, 0, 0),
            ImageSize(m_screen_size.GetWidth(), m_screen_size.GetHeight(), 1));

        SD_SREF(m_rf_sref).RegisterRenderPass(forward_rp_wref);
        SD_SREF(m_rf_sref).AllocateFrameBuffer();
        SD_SREF(m_rf_sref).RegisterBufferToFrameBuffer(m_color_buf_sref, 0, m_clear_color);
        SD_SREF(m_rf_sref).RegisterBufferToFrameBuffer(m_depth_buf_sref, 1, m_clear_d_and_s);
        SD_SREF(m_rf_sref).Initialize();
    }
    else {
        SDLOGE("Forward render pass doesn't exist. Please check!!!");
    }
}

void CameraComponent::InitializeWorkspaceForDeferredPath()
{
    RenderPassWeakReferenceObject deferred_rp_wref = GraphicsManager::GetRef().GetRenderPass("DeferredPath");

    if (deferred_rp_wref.IsNull() == false) {
        Resolution current_res = GraphicsManager::GetRef().GetScreenResolution();
        if (m_color_buf_sref.IsNull() == false) {
            m_color_buf_sref.Reset();
        }
        m_color_buf_sref = new Texture("CameraColorBuffer");
        SD_SREF(m_color_buf_sref).Initialize2DColorOrDepthBuffer(
            current_res.GetWidth(), current_res.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buf_sref.IsNull() == false) {
            m_depth_buf_sref.Reset();
        }
        m_depth_buf_sref = new Texture("CameraDepthBuffer");
        SD_SREF(m_depth_buf_sref).Initialize2DColorOrDepthBuffer(
            current_res.GetWidth(), current_res.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    else {
        SDLOGE("Deferred render pass doesn't exist. Please check!!!");
    }
}

bool CameraComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_buffer_wref.IsNull() == false) {
        Transform node_trans = SD_WREF(m_geo_comp_wref).GetWorldTransform();
        //SD_WREF(m_buffer_wref).SetMatrix4X4f("view", node_trans.MakeViewMatrix());
        //SD_WREF(m_buffer_wref).SetMatrix4X4f("proj", m_proj_mat);
        //SD_WREF(m_buffer_wref).SetVector3f("viewEye", node_trans.m_position);
        CameraUniforms cu;
        cu.m_proj = m_proj_mat;
        cu.m_view = node_trans.MakeViewMatrix();
        cu.m_view_eye = node_trans.m_position;
        SDLOG("Proj:%s", cu.m_proj.ToString().c_str());
        SDLOG("View:%s", cu.m_view.ToString().c_str());
        SD_WREF(m_buffer_wref).SetBufferData(&cu, sizeof(CameraUniforms));
        SD_WREF(m_buffer_wref).Update();
    }

    return true;
}

void CameraComponent::ClearWorkspace()
{
    m_color_buf_sref.Reset();
    m_depth_buf_sref.Reset();
    m_rf_sref.Reset();
}

______________SD_END_GRAPHICS_NAMESPACE______________
