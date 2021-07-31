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

VRCameraComponent::VRCameraComponent(const ObjectName& i_object_name)
: CameraComponentBase(i_object_name)
, m_workspace_type(CameraWorkspaceType_Forward)
, m_follow_resolution(true)
, m_initialized(false)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
{
    m_buffer_size = GraphicsManager::GetRef().GetScreenResolution();
}

VRCameraComponent::~VRCameraComponent()
{
    ClearWorkspace();
}

void VRCameraComponent::SetEyeCenters(Vector3f i_eye_centers[VREye_Both])
{

    for (uint32_t eid = VREye_Left; eid < VREye_Both; ++eid) {
        m_eye_centers[eid] = i_eye_centers[eid];
    }
    OnGeometryChanged(EventArg());
}

void VRCameraComponent::SetProjectionMatrices(Matrix4X4f i_proj_mats[VREye_Both])
{

    for (uint32_t eid = VREye_Left; eid < VREye_Both; ++eid) {
        m_proj_mats[eid] = i_proj_mats[eid];
    }
    OnGeometryChanged(EventArg());
}

void VRCameraComponent::Initialize()
{
    InitializeDescriptorSetAndPool();

    if (m_workspace_type == CameraWorkspaceType_Forward) {
        InitializeWorkspaceForForwardPass();
    }
    else if (m_workspace_type == CameraWorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPass();
    }

    m_geo_comp = SD_GET_COMP_WREF(m_entity, TransformComponent);

    SD_WREF(m_geo_comp).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<VRCameraComponent>(
            "VRCameraComponent::OnGeometryChanged",
            GetThisWeakPtrByType<VRCameraComponent>(),
            &VRCameraComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());
}

void VRCameraComponent::Resize()
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

    OnGeometryChanged(EventArg());

    m_initialized = true;
}

void VRCameraComponent::RecordCommand(
    const CommandBufferWeakReferenceObject &i_cb,
    const std::list<LightComponentWeakReferenceObject> &i_light_comps,
    const std::list<MeshRenderComponentWeakReferenceObject> &i_mesh_render_comps)
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
        for (mr_iter = i_mesh_render_comps.begin(); mr_iter != i_mesh_render_comps.end(); ++mr_iter) {
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

        GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(i_cb, secondary_cbs);
        SD_SREF(m_render_flow).EndRenderFlow(i_cb);
    }
    else if (m_workspace_type == CameraWorkspaceType_Deferred) {

    }
}

//------------------ Private Part ---------------
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
            ImageSize(m_buffer_size.GetWidth(), m_buffer_size.GetHeight(), 1));

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
        Resolution current_res = GraphicsManager::GetRef().GetScreenResolution();
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("VRCameraColorBuffer");
        SD_SREF(m_color_buffer).InitializeVRColorOrDepthBuffer(
            current_res.GetWidth(), current_res.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("VRCameraDepthBuffer");
        SD_SREF(m_depth_buffer).InitializeVRColorOrDepthBuffer(
            current_res.GetWidth(), current_res.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    else {
        SDLOGE("Deferred render pass doesn't exist. Please check!!!");
    }
}

bool VRCameraComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_ub.IsNull() == false) {
        Transform node_xform = SD_WREF(m_geo_comp).GetWorldTransform();
        
        for (uint32_t eyeID = 0; eyeID < VREye_Both; ++eyeID) {
            Vector3f eye_position;
            Transform eye_xform;
            eye_position =
                node_xform.GetRight().scale(m_eye_centers[eyeID].m_vec.x) +
                node_xform.GetTop().scale(m_eye_centers[eyeID].m_vec.y) +
                node_xform.GetForward().scale(m_eye_centers[eyeID].m_vec.z);
            eye_xform = node_xform;
            eye_xform.AddTranslation(eye_position);

            SD_WREF(m_ub).SetMatrix4X4f("views", eye_xform.MakeViewMatrix(), eyeID);
            SD_WREF(m_ub).SetMatrix4X4f("projs", m_proj_mats[eyeID], eyeID);
            SD_WREF(m_ub).SetVector3f("viewEyes", eye_xform.m_position, eyeID);
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

______________SD_END_GRAPHICS_NAMESPACE______________