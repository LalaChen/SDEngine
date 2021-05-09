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
, m_follow_resolution(true)
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
        InitializeWorkspaceForForwardPass();
    }
    else if (m_workspace_type == WorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPass();
    }

    m_geo_comp = SD_GET_COMP_WREF(m_entity, TransformComponent);

    SD_WREF(m_geo_comp).RegisterSlotFunctionIntoEvent(
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

    if (m_follow_resolution == true) {

        m_screen_size = GraphicsManager::GetRef().GetScreenResolution();

        if (m_workspace_type == WorkspaceType_Forward) {
            InitializeWorkspaceForForwardPass();
        }
        else if (m_workspace_type == WorkspaceType_Deferred) {
            InitializeWorkspaceForDeferredPass();
        }
    }

    SetPerspective(m_fov, m_near, m_far);
    OnGeometryChanged(EventArg());
}

void CameraComponent::RecordCommand(
    const CommandBufferWeakReferenceObject &i_cb,
    const std::list<LightComponentWeakReferenceObject> &i_light_comps,
    const std::list<MeshRenderComponentWeakReferenceObject> &i_mesh_render_comps)
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
        SD_SREF(m_render_flow).BeginRenderFlow(i_cb);
        CommandBufferInheritanceInfo cb_inherit_info = SD_SREF(m_render_flow).GetCurrentInheritanceInfo();
        RenderPassWeakReferenceObject current_rp = cb_inherit_info.m_rp_wref;

        GraphicsManager::GetRef().SetViewport(i_cb, vp);
        GraphicsManager::GetRef().SetScissor(i_cb, sr);

        uint32_t tID = 0;
        for (tID = 0; tID < scp_threads.size(); ++tID) {
            SD_SREF(scp_threads[tID]).StartRecording(cb_inherit_info, vp, sr);
        }

        light_wref_iter = i_light_comps.begin();
        tID = 0;
        for (mr_wref_iter = i_mesh_render_comps.begin(); mr_wref_iter != i_mesh_render_comps.end(); ++mr_wref_iter) {
            DescriptorSetWeakReferenceObject light_ds_wref = SD_WREF((*light_wref_iter)).GetDescriptorSet();
            MeshRenderComponentWeakReferenceObject mr_wref = (*mr_wref_iter);
            std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, current_rp, light_ds_wref, mr_wref](const CommandBufferWeakReferenceObject &i_cb_wref) {
                SD_WREF(mr_wref).RenderMesh(current_rp, i_cb_wref, m_ds, light_ds_wref, 0);
            };

            SD_SREF(scp_threads[tID]).AddTask(task_func);
            tID = (tID + 1) % scp_threads.size();
        }

        for (tID = 0; tID < scp_threads.size(); ++tID) {
            SD_SREF(scp_threads[tID]).WaitAndStopRecording(secondary_cb_wrefs);
        }

        GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(i_cb, secondary_cb_wrefs);
        SD_SREF(m_render_flow).EndRenderFlow(i_cb);
    }
    else if (m_workspace_type == WorkspaceType_Deferred) {

    }
}


//------------------ Private Part ---------------
void CameraComponent::InitializeDescriptorSetAndPool()
{
    m_dp = new DescriptorPool("CameraPool");
    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    //1. Collect all descriptor set layouts used at this component.
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    DescriptorSetLayoutWeakReferenceObject camera_dsl = GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Camera");
    SD_SREF(camera_dsl).GetUniformDescriptorCounts(desc_counts);
    SD_SREF(m_dp).Initialize(desc_counts, 1, false);
    //2. Allocate descriptor set.
    m_ds = SD_SREF(m_dp).AllocateDescriptorSet(camera_dsl);
    SD_WREF(m_ds).GetAllocatedUniformVariables(uvs);

    if (m_ds.IsNull() == false) {
        SD_SREF(m_ds).WriteDescriptor();
    }

    if (uvs.find("camera") != uvs.end()) {
        m_ub = uvs["camera"].DynamicCastTo<UniformBuffer>();
    }

    if (m_ub.IsNull() == true) {
        SDLOGE("We can find camera uniform buffer.");
    }
}

void CameraComponent::InitializeWorkspaceForForwardPass()
{
    RenderPassWeakReferenceObject forward_rp_wref = GraphicsManager::GetRef().GetRenderPass("ForwardPass");

    if (forward_rp_wref.IsNull() == false) {
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("CameraColorBuffer");
        SD_SREF(m_color_buffer).Initialize2DColorOrDepthBuffer(
            m_screen_size.GetWidth(), m_screen_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("CameraDepthBuffer");
        SD_SREF(m_depth_buffer).Initialize2DColorOrDepthBuffer(
            m_screen_size.GetWidth(), m_screen_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        m_render_flow = new RenderFlow("RenderFlow", ImageOffset(0, 0, 0),
            ImageSize(m_screen_size.GetWidth(), m_screen_size.GetHeight(), 1));

        SD_SREF(m_render_flow).RegisterRenderPass(forward_rp_wref);
        SD_SREF(m_render_flow).AllocateFrameBuffer();
        SD_SREF(m_render_flow).RegisterBufferToFrameBuffer(m_color_buffer, 0, m_clear_color);
        SD_SREF(m_render_flow).RegisterBufferToFrameBuffer(m_depth_buffer, 1, m_clear_d_and_s);
        SD_SREF(m_render_flow).Initialize();
    }
    else {
        SDLOGE("Forward render pass doesn't exist. Please check!!!");
    }
}

void CameraComponent::InitializeWorkspaceForDeferredPass()
{
    RenderPassWeakReferenceObject deferred_rp_wref = GraphicsManager::GetRef().GetRenderPass("DeferredPass");

    if (deferred_rp_wref.IsNull() == false) {
        Resolution current_res = GraphicsManager::GetRef().GetScreenResolution();
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("CameraColorBuffer");
        SD_SREF(m_color_buffer).Initialize2DColorOrDepthBuffer(
            current_res.GetWidth(), current_res.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("CameraDepthBuffer");
        SD_SREF(m_depth_buffer).Initialize2DColorOrDepthBuffer(
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
    if (m_ub.IsNull() == false) {
        Transform node_trans = SD_WREF(m_geo_comp).GetWorldTransform();
        SD_WREF(m_ub).SetMatrix4X4f("view", node_trans.MakeViewMatrix());
        SD_WREF(m_ub).SetMatrix4X4f("proj", m_proj_mat);
        SD_WREF(m_ub).SetVector3f("viewEye", node_trans.m_position);
        SD_WREF(m_ub).Update();
    }

    return true;
}

void CameraComponent::ClearWorkspace()
{
    m_color_buffer.Reset();
    m_depth_buffer.Reset();
    m_render_flow.Reset();
}

______________SD_END_GRAPHICS_NAMESPACE______________
