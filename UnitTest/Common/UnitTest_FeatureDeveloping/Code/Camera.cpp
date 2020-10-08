#include "Camera.h"

Camera::Camera(const ObjectName &i_object_name)
: Object(i_object_name)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
, m_fov(120.0f)
, m_aspect(1.0f)
, m_near(0.01f)
, m_far(1000.0f)
{
}

Camera::~Camera()
{
    m_rf_sref.Reset();
    m_color_buffer.Reset();
    m_depth_buffer.Reset();
}

void Camera::SetPerspective(float i_angle, float i_aspect, float i_near, float i_far)
{
    m_fov = i_angle;
    m_aspect = i_aspect;
    m_near = i_near;
    m_far = i_far;
    m_proj_mat.perspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::SetRenderPass(const RenderPassWeakReferenceObject &i_rp_wref)
{
    m_custom_rp_wref = i_rp_wref;
}

void Camera::Initialize()
{
    Resolution current_res = GraphicsManager::GetRef().GetScreenResolution();
    if (m_color_buffer.IsNull() == false) {
        m_color_buffer.Reset();
    }
    m_color_buffer = new Texture("CameraColorBuffer");
    m_color_buffer.GetRef().Initialize2DColorOrDepthBuffer(
        current_res.GetWidth(), current_res.GetHeight(),
        GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
        ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

    if (m_depth_buffer.IsNull() == false) {
        m_depth_buffer.Reset();
    }
    m_depth_buffer = new Texture("CameraDepthBuffer");
    m_depth_buffer.GetRef().Initialize2DColorOrDepthBuffer(
        current_res.GetWidth(), current_res.GetHeight(),
        GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
        ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    // Create render flow.
    if (m_custom_rp_wref.IsNull() == false) {
        SDLOG("Use custom render pass(%s)", m_custom_rp_wref.GetRef().GetObjectName().c_str());
        m_rf_sref = new RenderFlow("RenderFlow", ImageOffset(0, 0, 0),
            ImageSize(current_res.GetWidth(), current_res.GetHeight(), 1));
        m_rf_sref.GetRef().RegisterRenderPass(m_custom_rp_wref);
        m_rf_sref.GetRef().AllocateFrameBuffer();
        m_rf_sref.GetRef().RegisterBufferToFrameBuffer(m_color_buffer, 0, m_clear_color);
        m_rf_sref.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer, 1, m_clear_d_and_s);
        m_rf_sref.GetRef().Initialize();
    }
    else {
    }
}

void Camera::Resize()
{
    m_color_buffer.Reset();
    m_depth_buffer.Reset();
    m_rf_sref.Reset();

    Initialize();
}

//------------ Temporary Transform Method ----------------
void Camera::AddTranslation(const Vector3f &i_offset)
{
    m_trans.AddTranslation(i_offset);
}

void Camera::AddRotationX(float i_angle)
{
    m_trans.AddRotation(m_trans.GetRight(), i_angle);
}

void Camera::AddRotationY(float i_angle)
{
    m_trans.AddRotation(m_trans.GetTop(), i_angle);
}

void Camera::AddRotationZ(float i_angle)
{
    m_trans.AddRotation(m_trans.GetForward().negative(), i_angle);
}

void Camera::SetTransform(const Transform &i_trans)
{
    m_trans = i_trans;
}