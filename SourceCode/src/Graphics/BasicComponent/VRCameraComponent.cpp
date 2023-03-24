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
#include "VRCameraComponent.h"

using SDE::Basic::ECSManager;
using SDE::Basic::MemberFunctionSlot;

_____________SD_START_GRAPHICS_NAMESPACE_____________

VRCameraComponent::VRCameraComponent(const ObjectName &i_object_name)
: CameraComponentBase(i_object_name)
{
}

VRCameraComponent::~VRCameraComponent()
{
    ClearWorkspace();
}

//------------------ Private Part ---------------
void VRCameraComponent::InitializeImpl()
{
    InitializeDescriptorSetAndPool();

    if (m_workspace_type == CameraWorkspaceType_Forward) {
        InitializeWorkspaceForForwardPass();
    }
    else if (m_workspace_type == CameraWorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPass();
    }

    m_xform = SD_GET_COMP_WREF(m_entity, TransformComponent);

    SD_WREF(m_xform).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<VRCameraComponent>(
            "VRCameraComponent::OnGeometryChanged",
            GetThisWeakPtrByType<VRCameraComponent>(),
            &VRCameraComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());
    m_ws_initialized = true;
}

void VRCameraComponent::ResizeImpl()
{
    ClearWorkspace();

    if (m_follow_resolution == true) {

        m_buffer_size = GraphicsManager::GetRef().GetScreenResolution();

        if (m_workspace_type == CameraWorkspaceType_Forward) {
            InitializeWorkspaceForForwardPass();
        }
        else if (m_workspace_type == CameraWorkspaceType_Deferred) {
            InitializeWorkspaceForDeferredPass();
        }
    }

    m_ws_initialized = true;
    //
    OnGeometryChanged(EventArg());
}


void VRCameraComponent::InitializeDescriptorSetAndPool()
{
    m_dp = new DescriptorPool("VRCameraPool");
    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    //1. Collect all descriptor set layouts used at this component.
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    DescriptorSetLayoutWeakReferenceObject vrcamera_dsl = GraphicsManager::GetRef().GetBasicDescriptorSetLayout(sUniformDescriptorSetLayout_VRCamera);
    SD_SREF(vrcamera_dsl).GetUniformDescriptorCounts(desc_counts);
    SD_SREF(m_dp).Initialize(desc_counts, 1, false);
    //2. Allocate descriptor set.
    m_ds = SD_SREF(m_dp).AllocateDescriptorSet(vrcamera_dsl);
    SD_WREF(m_ds).GetAllocatedUniformVariables(uvs);

    if (m_ds.IsNull() == false) {
        SD_SREF(m_ds).WriteDescriptor();
    }

    if (uvs.find(sUniformBuffer_VRCamera) != uvs.end()) {
        m_ub = uvs[sUniformBuffer_VRCamera].DynamicCastTo<UniformBuffer>();
    }

    if (m_ub.IsNull() == true) {
        SDLOGE("We can find vrcamera uniform buffer.");
    }
}

void VRCameraComponent::InitializeWorkspaceForForwardPass()
{
    RenderPassWeakReferenceObject forward_rp = GraphicsManager::GetRef().GetRenderPass(sRenderPass_VRForward);

    if (forward_rp.IsNull() == false) {
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("VRCameraColorBuffer");
        SD_SREF(m_color_buffer).InitializeVRColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("VRCameraDepthBuffer");
        SD_SREF(m_depth_buffer).InitializeVRColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        m_render_flow = new RenderFlow("VRRenderFlow", ImageOffset(0, 0, 0),
            ImageSize(m_buffer_size.GetWidth(), m_buffer_size.GetHeight(), 2));

        SD_SREF(m_render_flow).RegisterRenderPass(forward_rp);
        SD_SREF(m_render_flow).AllocateFrameBuffer();
        SD_SREF(m_render_flow).RegisterBufferToFrameBuffer(m_color_buffer, 0, m_clear_color);
        SD_SREF(m_render_flow).RegisterBufferToFrameBuffer(m_depth_buffer, 1, m_clear_d_and_s);
        SD_SREF(m_render_flow).Initialize();
    }
    else {
        SDLOGE("VR forward render pass doesn't exist. Please check!!!");
    }
}

void VRCameraComponent::InitializeWorkspaceForDeferredPass()
{
    RenderPassWeakReferenceObject deferred_rp = GraphicsManager::GetRef().GetRenderPass("DeferredPass");

    if (deferred_rp.IsNull() == false) {
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("VRCameraColorBuffer");
        SD_SREF(m_color_buffer).InitializeVRColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("VRCameraDepthBuffer");
        SD_SREF(m_depth_buffer).InitializeVRColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    else {
        SDLOGE("Deferred render pass doesn't exist. Please check!!!");
    }
}

DepthArea2D VRCameraComponent::ConvertNCPAreaToWorldArea(const Area2D &i_ncp_area) const
{
    DepthArea2D da;
    float scale = 2.001f;
    float near_scale = m_frustums[VREye_Right].n * scale;
    float width = m_frustums[VREye_Right].r * scale - m_frustums[VREye_Right].l * scale;
    float height = m_frustums[VREye_Right].t * scale - m_frustums[VREye_Right].b * scale;
    da.area.x = m_frustums[VREye_Right].l * scale + i_ncp_area.x * width;
    da.area.y = m_frustums[VREye_Right].b * scale + i_ncp_area.y * height;
    da.area.w = i_ncp_area.w * width;
    da.area.h = i_ncp_area.h * height;
    da.depth = -near_scale;
    return da;
}


bool VRCameraComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_ub.IsNull() == false) {
        Transform node_xform = SD_WREF(m_xform).GetWorldTransform();
        Matrix4X4f node_mat = node_xform.MakeWorldMatrix();

        for (uint32_t eyeID = 0; eyeID < VREye_Both; ++eyeID) {
            Matrix4X4f eye_world_mat = m_eye_mats[eyeID] * node_mat;
            Vector3f eye_position = Vector3f(
                eye_world_mat.m_matrix[3][0],
                eye_world_mat.m_matrix[3][1],
                eye_world_mat.m_matrix[3][2]);
            eye_position.RepresentPosition();
            SD_WREF(m_ub).SetMatrix4X4f("views", eye_world_mat.inverse(), eyeID);
            SD_WREF(m_ub).SetMatrix4X4f("projs", m_proj_mats[eyeID], eyeID);
            SD_WREF(m_ub).SetVector3f("viewEyes", eye_position, eyeID);
        }
        SD_WREF(m_ub).Update();
    }

    return true;
}

void VRCameraComponent::ClearWorkspace()
{
    m_color_buffer.Reset();
    m_depth_buffer.Reset();
    m_render_flow.Reset();
}

void VRCameraComponent::SetEyeCenters(Vector3f i_eye_centers[VREye_Both])
{
    for (uint32_t eid = VREye_Left; eid < VREye_Both; ++eid) {
        m_eye_mats[eid].translate(i_eye_centers[eid]);
    }
    NotifyEvent(sCameraEyeChangedEventName, EventArg());
    OnGeometryChanged(EventArg());
}

void VRCameraComponent::SetEyeMatrices(Matrix4X4f i_eye_mats[VREye_Both])
{
    for (uint32_t eid = VREye_Left; eid < VREye_Both; ++eid) {
        m_eye_mats[eid] = i_eye_mats[eid];
    }

    NotifyEvent(sCameraEyeChangedEventName, EventArg());
    OnGeometryChanged(EventArg());
}

void VRCameraComponent::SetFrustums(const Frustum i_frustums[VREye_Both])
{
    for (uint32_t eid = VREye_Left; eid < VREye_Both; ++eid) {
        m_frustums[eid] = i_frustums[eid];
        if (m_frustums[eid].p == true) {
            m_proj_mats[eid].frustum(m_frustums[eid].l, m_frustums[eid].r, m_frustums[eid].t, m_frustums[eid].b, m_frustums[eid].n, m_frustums[eid].f);
        }
        else {
            m_proj_mats[eid].ortho(m_frustums[eid].l, m_frustums[eid].r, m_frustums[eid].t, m_frustums[eid].b, m_frustums[eid].n, m_frustums[eid].f);
        }
    }
    NotifyEvent(sCameraProjChangedEventName, EventArg());
    OnGeometryChanged(EventArg());
}

void VRCameraComponent::SetProjectionForEye(float i_fov, float i_near, float i_far, VREyeEnum i_enum)
{
    Resolution proj_res;
    proj_res.SetResolution(m_buffer_size.GetWidth() / 2, m_buffer_size.GetHeight() / 2);
    
    for (uint32_t eid = VREye_Left; eid < VREye_Both; ++eid) {
        m_proj_mats[eid].perspective(i_fov, proj_res.GetRatio(), i_near, i_far);
        m_frustums[eid] = Frustum(m_proj_mats[eid], i_near, i_far);
    }

    NotifyEvent(sCameraProjChangedEventName, EventArg());
    OnGeometryChanged(EventArg());
}

void VRCameraComponent::RecordCommand(
    const CommandBufferWeakReferenceObject &i_cb,
    const std::list<LightComponentWeakReferenceObject> &i_light_comps,
    const std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> > &i_mr_groups)
{
    GraphicsSystemWeakReferenceObject gs = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
    std::list<LightComponentWeakReferenceObject>::const_iterator light_iter;
    std::list<MeshRenderComponentWeakReferenceObject>::const_iterator mr_iter;

    const std::vector<SecondaryCommandPoolThreadStrongReferenceObject> &scp_threads = SD_WREF(gs).GetSecondaryCommandPool();
    std::list<CommandBufferWeakReferenceObject> secondary_cbs;
    Viewport vp;
    vp.m_x = 0.0f; vp.m_y = static_cast<float>(m_buffer_size.GetHeight());
    vp.m_width = static_cast<float>(m_buffer_size.GetWidth());
    vp.m_height = -1.0f * static_cast<float>(m_buffer_size.GetHeight());
    vp.m_min_depth = 0.0f;
    vp.m_max_depth = 1.0f;

    ScissorRegion sr;
    sr.m_x = 0.0f; sr.m_y = 0.0f;
    sr.m_width = vp.m_width;
    sr.m_height = static_cast<float>(m_buffer_size.GetHeight());

    if (m_workspace_type == CameraWorkspaceType_Forward) {
        SD_SREF(m_render_flow).BeginRenderFlow(i_cb);

        std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> >::const_iterator g_iter = i_mr_groups.begin();
        for (g_iter = i_mr_groups.begin(); g_iter != i_mr_groups.end(); ++g_iter) {
            CommandBufferInheritanceInfo cb_inherit_info = SD_SREF(m_render_flow).GetCurrentInheritanceInfo();
            RenderPassWeakReferenceObject current_rp = cb_inherit_info.m_rp;

            GraphicsManager::GetRef().SetViewport(i_cb, vp);
            GraphicsManager::GetRef().SetScissor(i_cb, sr);

            uint32_t tID = 0;
            for (tID = 0; tID < scp_threads.size(); ++tID) {
                SD_SREF(scp_threads[tID]).StartRecording(cb_inherit_info, vp, sr);
            }

            light_iter = i_light_comps.begin();
            tID = 0;
            for (mr_iter = (*g_iter).second.begin(); mr_iter != (*g_iter).second.end(); ++mr_iter) {
                DescriptorSetWeakReferenceObject light_ds = SD_WREF((*light_iter)).GetDescriptorSet();
                MeshRenderComponentWeakReferenceObject mr = (*mr_iter);
                std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, current_rp, light_ds, mr](const CommandBufferWeakReferenceObject& i_cb) {
                    SD_WREF(mr).RenderMesh(current_rp, i_cb, m_ds, light_ds, 0);
                };

                SD_SREF(scp_threads[tID]).AddTask(task_func);
                tID = (tID + 1) % scp_threads.size();
            }

            for (tID = 0; tID < scp_threads.size(); ++tID) {
                SD_SREF(scp_threads[tID]).WaitAndStopRecording(secondary_cbs);
            }
        }

        GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(i_cb, secondary_cbs);
        SD_SREF(m_render_flow).EndRenderFlow(i_cb);
    }
    else if (m_workspace_type == CameraWorkspaceType_Deferred) {

    }
}

______________SD_END_GRAPHICS_NAMESPACE______________