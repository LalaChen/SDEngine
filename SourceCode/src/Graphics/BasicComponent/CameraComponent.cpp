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

#include "CameraComponent.h"
#include "MathConstant.h"
#include "MathAlgoritm.h"
#include "BasicUniforms.h"
#include "ECSManager.h"
#include "GraphicsSystem.h"
#include "GraphicsManager.h"
#include "LogManager.h"


using SDE::Basic::ECSManager;
using SDE::Basic::MemberFunctionSlot;

_____________SD_START_GRAPHICS_NAMESPACE_____________

CameraComponent::CameraComponent(const ObjectName &i_object_name)
: CameraComponentBase(i_object_name)
{
}

CameraComponent::~CameraComponent()
{
    ClearWorkspace();
}

//------------------ Initialization Part ---------------
void CameraComponent::InitializeImpl()
{
    InitializeDescriptorSetAndPool();

    if (m_workspace_type == CameraWorkspaceType_Forward) {
        InitializeWorkspaceForForwardPass();
    }
    else if (m_workspace_type == CameraWorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPass();
    }

    if (m_layer_id != UINT32_MAX) {
        m_layer = GraphicsManager::GetRef().RegisterLayer(
            SDE::Basic::StringFormat("%s_layer", m_object_name.c_str()),
            m_color_buffer, m_depth_buffer, m_layer_id);
    }
    else {
        SDLOG("Camera(%s)'s layer id is invalid, don't need to draw in screen", m_object_name.c_str());
    }

    m_xform = SD_GET_COMP_WREF(m_entity, TransformComponent);

    SD_WREF(m_xform).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<CameraComponent>(
            "CameraComponent::OnGeometryChanged",
            GetThisWeakPtrByType<CameraComponent>(),
            &CameraComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());

    m_ws_initialized = true;
}

void CameraComponent::ResizeImpl()
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
    float height = m_frustum.t - m_frustum.b;
    float width  = m_buffer_size.GetRatio() * height;
    float center_w = (m_frustum.r + m_frustum.l) / 2.0f;
    m_frustum.r = center_w + width / 2.0f;
    m_frustum.l = center_w - width / 2.0f;
    SetFrustum(m_frustum);
    NotifyEvent(sCameraSizeChangedEventName, EventArg());
    OnGeometryChanged(EventArg());
}


void CameraComponent::InitializeDescriptorSetAndPool()
{
    m_dp = new DescriptorPool("CameraPool");
    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    //1. Collect all descriptor set layouts used at this component.
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    DescriptorSetLayoutWeakReferenceObject camera_dsl = GraphicsManager::GetRef().GetBasicDescriptorSetLayout(sUniformDescriptorSetLayout_Camera);
    SD_SREF(camera_dsl).GetUniformDescriptorCounts(desc_counts);
    SD_SREF(m_dp).Initialize(desc_counts, 1, false);
    //2. Allocate descriptor set.
    m_ds = SD_SREF(m_dp).AllocateDescriptorSet(camera_dsl);
    SD_WREF(m_ds).GetAllocatedUniformVariables(uvs);

    if (m_ds.IsNull() == false) {
        SD_SREF(m_ds).WriteDescriptor();
    }

    if (uvs.find(sUniformBuffer_Camera) != uvs.end()) {
        m_ub = uvs[sUniformBuffer_Camera].DynamicCastTo<UniformBuffer>();
    }

    if (m_ub.IsNull() == true) {
        SDLOGE("We can find camera uniform buffer.");
    }
}

void CameraComponent::InitializeWorkspaceForForwardPass()
{
    RenderPassWeakReferenceObject forward_rp = GraphicsManager::GetRef().GetRenderPass(sRenderPass_Forward);

    if (forward_rp.IsNull() == false) {
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("CameraColorBuffer");
        SD_SREF(m_color_buffer).Initialize2DColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("CameraDepthBuffer");
        SD_SREF(m_depth_buffer).Initialize2DColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        m_render_flow = new RenderFlow("RenderFlow", ImageOffset(0, 0, 0),
            ImageSize(m_buffer_size.GetWidth(), m_buffer_size.GetHeight(), 1));

        SD_SREF(m_render_flow).RegisterRenderPass(forward_rp);
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
    RenderPassWeakReferenceObject deferred_rp = GraphicsManager::GetRef().GetRenderPass("DeferredPass");

    if (deferred_rp.IsNull() == false) {
        if (m_color_buffer.IsNull() == false) {
            m_color_buffer.Reset();
        }
        m_color_buffer = new Texture("CameraColorBuffer");
        SD_SREF(m_color_buffer).Initialize2DColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buffer.IsNull() == false) {
            m_depth_buffer.Reset();
        }
        m_depth_buffer = new Texture("CameraDepthBuffer");
        SD_SREF(m_depth_buffer).Initialize2DColorOrDepthBuffer(
            m_buffer_size.GetWidth(), m_buffer_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }
    else {
        SDLOGE("Deferred render pass doesn't exist. Please check!!!");
    }
}

void CameraComponent::ClearWorkspace()
{
    if (m_layer.IsNull() == false) {
        GraphicsManager::GetRef().UnregisterLayer(m_layer);
        m_layer.Reset();
    }
    m_color_buffer.Reset();
    m_depth_buffer.Reset();
    m_render_flow.Reset();
}
//------------------ Common Part ---------------
void CameraComponent::SetPerspective(float i_fov, float i_near, float i_far)
{
    if (m_buffer_size.IsValid() == true) {
        m_proj_mat.perspective(i_fov, m_buffer_size.GetRatio(), i_near, i_far);
        m_frustum = Frustum(m_proj_mat, i_near, i_far, true);
        NotifyEvent(sCameraProjChangedEventName, EventArg());
        OnGeometryChanged(EventArg());
    }
}

void CameraComponent::SetFrustum(const Frustum &i_frustum)
{
    if (m_buffer_size.IsValid() == true) {
        m_frustum = i_frustum;
        if (m_frustum.p == true) {
            m_proj_mat.frustum(m_frustum.l, m_frustum.r, m_frustum.t, m_frustum.b, m_frustum.n, m_frustum.f);
        }
        else {
            m_proj_mat.ortho(m_frustum.l, m_frustum.r, m_frustum.t, m_frustum.b, m_frustum.n, m_frustum.f);
        }
        NotifyEvent(sCameraProjChangedEventName, EventArg());
        OnGeometryChanged(EventArg());
    }
}

void CameraComponent::SetCameraSize(const Resolution &i_size)
{
    m_buffer_size = i_size;
    m_follow_resolution = false;
    if (m_ws_initialized == true) {
        Resize();
    }
}

Ray CameraComponent::CalculateRay(const TouchButton &i_tb) const
{
    if (i_tb.m_x >= 0 && i_tb.m_x < m_buffer_size.GetWidth() &&
        i_tb.m_y >= 0 && i_tb.m_y < m_buffer_size.GetHeight()) {
        Transform node_trans = SD_WREF(m_xform).GetWorldTransform();
        Ray ray;
        ray.InitializeByScreen(m_proj_mat, node_trans.MakeViewMatrix(), m_buffer_size, i_tb, m_frustum.n);
        return ray;
    }
    else {
        return Ray();
    }
}

void CameraComponent::RecordCommand(
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

    uint32_t tID = 0;
    for (tID = 0; tID < scp_threads.size(); ++tID) {
        SD_SREF(scp_threads[tID]).Restart();
    }

    if (m_workspace_type == CameraWorkspaceType_Forward) {
        SD_SREF(m_render_flow).BeginRenderFlow(i_cb);
        std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> >::const_iterator g_iter = i_mr_groups.begin();
        for (g_iter = i_mr_groups.begin(); g_iter != i_mr_groups.end(); ++g_iter) {
            CommandBufferInheritanceInfo cb_inherit_info = SD_SREF(m_render_flow).GetCurrentInheritanceInfo();
            RenderPassWeakReferenceObject current_rp = cb_inherit_info.m_rp;

            GraphicsManager::GetRef().SetViewport(i_cb, vp);
            GraphicsManager::GetRef().SetScissor(i_cb, sr);

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

DepthArea2D CameraComponent::ConvertNCPAreaToWorldArea(const Area2D &i_ncp_area) const
{
    DepthArea2D da;
    float scale = 2.001f;
    float near_scale = m_frustum.n * scale;
    float width = m_frustum.r * scale - m_frustum.l * scale;
    float height = m_frustum.t * scale - m_frustum.b * scale;
    da.area.x = m_frustum.l * scale + i_ncp_area.x * width;
    da.area.y = m_frustum.b * scale + i_ncp_area.y * height;
    da.area.w = i_ncp_area.w * width;
    da.area.h = i_ncp_area.h * height;
    da.depth  = -near_scale;
    return da;
}

bool CameraComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_ub.IsNull() == false) {
        Transform node_trans = SD_WREF(m_xform).GetWorldTransform();
        SD_WREF(m_ub).SetMatrix4X4f("view", node_trans.MakeViewMatrix());
        SD_WREF(m_ub).SetMatrix4X4f("proj", m_proj_mat);
        SD_WREF(m_ub).SetVector3f("viewEye", node_trans.m_position);
        SD_WREF(m_ub).Update();
    }

    return true;
}

______________SD_END_GRAPHICS_NAMESPACE______________
