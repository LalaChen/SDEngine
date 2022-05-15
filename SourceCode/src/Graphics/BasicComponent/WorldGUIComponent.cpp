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

#include "MathAlgoritm.h"
#include "MaterialUniforms.h"
#include "Application.h"
#include "ECSManager.h"
#include "BasicShapeCreator.h"
#include "GraphicsSystem.h"
#include "CameraComponent.h"
#include "WorldGUIComponent.h"

using namespace SDE::Math;
using namespace SDE::GUI;
using namespace SDE::Basic;
using SDE::Basic::ECSManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

WorldGUIComponent::WorldGUIComponent(const ObjectName &i_object_name, uint32_t i_width, uint32_t i_height, float i_world_w, float i_world_h)
: Component(i_object_name)
, m_buffer_size{i_width, i_height}
, m_world_size{i_world_w, i_world_h}
, m_clear_color{ 0.0f, 0.0f, 0.0f, 0.0f }
, m_clear_d_and_s{ 1.0f, 1 }
, m_initialized(false)
{
}

WorldGUIComponent::~WorldGUIComponent()
{
    ClearWorkspace();
}

void WorldGUIComponent::SetBufferSize(uint32_t i_width, uint32_t i_height)
{
    if (m_initialized == false) {
        m_buffer_size[0] = i_width;
        m_buffer_size[1] = i_height;
    }
}

void WorldGUIComponent::SetWorldSize(float i_world_w, float i_world_h)
{
    if (m_initialized == false) {
        m_world_size[0] = i_world_w;
        m_world_size[1] = i_world_h;
        float hw = i_world_w / 2.0f, hh = i_world_h / 2.0f;
        m_UI_vertices[0] = Vector3f(-hw, -hh, 0.0f, 1.0f);
        m_UI_vertices[1] = Vector3f( hw, -hh, 0.0f, 1.0f);
        m_UI_vertices[2] = Vector3f( hw,  hh, 0.0f, 1.0f);
        m_UI_vertices[3] = Vector3f(-hw,  hh, 0.0f, 1.0f);
    }
}

void WorldGUIComponent::SetTouchDataByRay(const Ray &i_ray, const TouchButton &i_tb)
{
    if (m_transform.IsNull() == false && m_batch.IsNull() == false && i_ray.IsValid() == true) {
        Transform xform = SD_WREF(m_transform).GetWorldTransform();
        Matrix4X4f world_inv = xform.MakeWorldMatrix().inverse();
        Vector3f ray_obj_origin = world_inv * i_ray.m_origin;
        Vector3f ray_obj_orientation = world_inv * i_ray.m_orientation;
        bool hit_result = false;
        float distance, u, v;
        float fu = -1.0f, fv = -1.0f;
        Vector3f hitted_pt;
        hit_result = CalculateRayTriangleIntersection(
            distance, u, v, hitted_pt,
            m_UI_vertices[0], m_UI_vertices[1], m_UI_vertices[2],
            ray_obj_origin, ray_obj_orientation);

        if (hit_result == false) {
            hit_result = CalculateRayTriangleIntersection(
                distance, u, v, hitted_pt,
                m_UI_vertices[0], m_UI_vertices[2], m_UI_vertices[3],
                ray_obj_origin, ray_obj_orientation);
        }

        TouchButtonStateEnum pre_state = m_touch_data.m_state;
        if (hit_result == true) {
            fu = (hitted_pt.m_vec.x - m_UI_vertices[0].m_vec.x) / m_world_size[0];
            fv = (hitted_pt.m_vec.y - m_UI_vertices[0].m_vec.y) / m_world_size[1];

            m_touch_data.m_state = i_tb.m_state;
            m_touch_data.m_x = fu * static_cast<float>(m_buffer_size[0]);
            m_touch_data.m_y = (1.0f - fv) * static_cast<float>(m_buffer_size[1]);
            SD_WREF(m_batch).UpdateTouchButton(m_touch_data);
            if (pre_state != m_touch_data.m_state) {
                SDLOG("WGUI(%s) state(%d) at (%lf,%lf)", m_object_name.c_str(), i_tb.m_state, m_touch_data.m_x, m_touch_data.m_y);
            }
        }
        else {
            m_touch_data.m_state = TouchButtonState_RELEASED;
            m_touch_data.m_x = -1.0f;
            m_touch_data.m_y = -1.0f;
            SD_WREF(m_batch).UpdateTouchButton(m_touch_data);
            if (pre_state != m_touch_data.m_state) {
                SDLOG("WGUI(%s) Pressed at (%lf,%lf)", m_object_name.c_str(), m_touch_data.m_x, m_touch_data.m_y);
            }
        }
    }
}

bool WorldGUIComponent::LoadGUI(const IMGUIBatchLoadingCallback &i_load_func)
{
    return SD_SREF(m_batch).LoadGUI(i_load_func);
}

void WorldGUIComponent::Initialize()
{
    m_batch = new IMGUIBatch("GUIBatch", ImVec2(m_buffer_size[0], m_buffer_size[1]));
    SD_SREF(m_batch).Initialize();
    //1. Initialize RenderFlow.
    InitializeWorkspace();
    //2. Initialize GUI Component.
    m_GUI_mesh_render = SD_WREF(m_entity).GetComponentByType<MeshRenderComponent>().StaticCastTo<MeshRenderComponent>();
    if (m_GUI_mesh_render.IsNull() == true) {
        m_GUI_mesh_render = ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(
            m_entity, SDE::Basic::StringFormat("%s_GUIMeshRender", m_object_name.c_str())).DynamicCastTo<MeshRenderComponent>();
        SD_WREF(m_GUI_mesh_render).Initialize();
    }
    //2.1 create material.
    m_GUI_material = new Material("GUIMaterial");
    SD_SREF(m_GUI_material).BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("AlphaNoLightingShader"));
    //Set data done. Link with shader program.(Write descirptor)
    SD_SREF(m_GUI_material).LinkWithShaderProgram();
    MaterialUniforms mat_ub; //use default color.
    SD_SREF(m_GUI_material).SetDataToUniformBuffer(sUniformBuffer_Material, &mat_ub, sizeof(MaterialUniforms));
    SD_SREF(m_GUI_material).SetTexture(sUniformImages_Material_Textures, m_GUI_color_buffer, 0);
    SD_SREF(m_GUI_material).Update();
    //2.2 create mesh.
    m_GUI_mesh = BasicShapeCreator::GetRef().CreateWorldGUI(m_world_size[0], m_world_size[1]);
    SD_WREF(m_GUI_mesh_render).AppendMesh(m_GUI_mesh, m_GUI_material);
    //
    m_transform = SD_GET_TYPE_COMP_WREF(m_entity, TransformComponent);
}

void WorldGUIComponent::Update()
{
    GraphicsSystemWeakReferenceObject gs = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
    if (gs.IsNull() == false) {
        CameraComponentWeakReferenceObject camera = SD_WREF(gs).GetScreenCamera().DynamicCastTo<CameraComponent>();
        if (camera.IsNull() == false) {
            TouchButton tb = Application::GetRef().GetTouchButton(TouchButton_RIGHT);
            Ray ray = SD_WREF(camera).CalculateRay(tb);
            SetTouchDataByRay(ray, tb);
        }
        else {
            SDLOGW("Camera is null. We can't update ray.");
        }
    }

    IMGUIRenderer::GetRef().RecordGUICommands(
        m_GUI_render_flow,
        m_GUI_cb,
        m_batch);

    GraphicsManager::GetRef().SubmitCommandBuffersToQueue({ m_GUI_cb });
}

void WorldGUIComponent::InitializeWorkspace()
{
    RenderPassWeakReferenceObject gui_rp = GraphicsManager::GetRef().GetRenderPass(sRenderPass_GUI);

    if (m_GUI_color_buffer.IsNull() == false) {
        m_GUI_color_buffer.Reset();
    }
    m_GUI_color_buffer = new Texture("GUIColorBuffer");
    SD_SREF(m_GUI_color_buffer).SetSamplerFilterType(SamplerFilterType_NEAREST, SamplerFilterType_NEAREST);
    SD_SREF(m_GUI_color_buffer).Initialize2DColorOrDepthBuffer(
        m_buffer_size[0], m_buffer_size[1],
        GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
        ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

    if (m_GUI_depth_buffer.IsNull() == false) {
        m_GUI_depth_buffer.Reset();
    }
    m_GUI_depth_buffer = new Texture("GUIDepthBuffer");
    SD_SREF(m_GUI_depth_buffer).Initialize2DColorOrDepthBuffer(
        m_buffer_size[0], m_buffer_size[1],
        GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
        ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    m_GUI_render_flow = new RenderFlow(SDE::Basic::StringFormat("%s_RenderFlow", m_object_name.c_str()),
        ImageOffset(0, 0, 0), ImageSize(m_buffer_size[0], m_buffer_size[1], 1));

    SD_SREF(m_GUI_render_flow).RegisterRenderPass(gui_rp);
    SD_SREF(m_GUI_render_flow).AllocateFrameBuffer();
    SD_SREF(m_GUI_render_flow).RegisterBufferToFrameBuffer(m_GUI_color_buffer, 0, m_clear_color);
    SD_SREF(m_GUI_render_flow).RegisterBufferToFrameBuffer(m_GUI_depth_buffer, 1, m_clear_d_and_s);
    SD_SREF(m_GUI_render_flow).Initialize();

    m_GUI_cp = new CommandPool("WGUICmdPool");
    SD_REF(m_GUI_cp).Initialize();
    m_GUI_cb = SD_REF(m_GUI_cp).AllocateCommandBuffer("WGUICmdBuffer");

    m_initialized = true;
}

void WorldGUIComponent::ClearWorkspace()
{
    m_GUI_color_buffer.Reset();
    m_GUI_depth_buffer.Reset();
    m_GUI_render_flow.Reset();
    SD_SREF(m_GUI_cp).Clear();
    m_GUI_cp.Reset();
}

______________SD_END_GRAPHICS_NAMESPACE______________