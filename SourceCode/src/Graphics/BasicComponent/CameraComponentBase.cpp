#include "CameraComponentBase.h"

#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

const std::string CameraComponentBase::sCameraEyeChangedEventName = "CameraEyeChanged";
const std::string CameraComponentBase::sCameraProjChangedEventName = "CameraProjChanged";
const std::string CameraComponentBase::sCameraSizeChangedEventName = "CameraSizeChanged";

Frustum::Frustum()
: l(0.0f), r(0.0f), t(0.0f), b(0.0f), n(0.0f), f(0.0f), p(false)
{
}

Frustum::Frustum(float i_l, float i_r, float i_t, float i_b, float i_n, float i_f, bool i_p)
: l(i_l), r(i_r), t(i_t), b(i_b), n(i_n), f(i_f), p(i_p)
{
}

Frustum::Frustum(const Matrix4X4f &i_src, float i_n, float i_f, bool i_p)
: n(i_n), f(i_f), p(i_p)
{
    if (i_p == true) {
        r = (1.0f + i_src.m_matrix[2][0]) * n / i_src.m_matrix[0][0];
        l = (i_src.m_matrix[2][0] - 1.0f) * n / i_src.m_matrix[0][0];
        t = (1.0f + i_src.m_matrix[2][1]) * n / i_src.m_matrix[1][1];
        b = (i_src.m_matrix[2][1] - 1.0f) * n / i_src.m_matrix[1][1];
    }
    else {

    }
}

Frustum::~Frustum()
{
}

CameraComponentBase::CameraComponentBase(const ObjectName &i_object_name)
: Component(i_object_name)
, m_workspace_type(CameraWorkspaceType_Forward)
, m_follow_resolution(true)
, m_ws_initialized(false)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
{
    m_buffer_size = GraphicsManager::GetRef().GetScreenResolution();
    RegisterEvent(new Event(sCameraEyeChangedEventName));
    RegisterEvent(new Event(sCameraProjChangedEventName));
    RegisterEvent(new Event(sCameraSizeChangedEventName));
}

CameraComponentBase::~CameraComponentBase()
{
}

void CameraComponentBase::SetClearValues(ClearValue i_color, ClearValue i_d_and_s)
{
    m_clear_color = i_color;
    m_clear_d_and_s = i_d_and_s;
}

TextureWeakReferenceObject CameraComponentBase::GetColorBuffer() const
{
    if (IsInitialized() == true && m_ws_initialized == true) {
        return m_color_buffer;
    }
    else {
        return TextureWeakReferenceObject();
    }
}

TextureWeakReferenceObject CameraComponentBase::GetDepthBuffer() const
{
    if (IsInitialized() == true && m_ws_initialized == true) {
        return m_depth_buffer;
    }
    else {
        return TextureWeakReferenceObject();
    }
}

DepthArea2D CameraComponentBase::ConvertScreenAreaToWorldArea(
    AreaAlignOrientationEnum i_orientation,
    const Area2D &i_screen_area) const
{
    Area2D ncp_area;
    ncp_area.SetArea2DByOrientationAndSize(i_orientation, i_screen_area, m_buffer_size);
    return ConvertNCPAreaToWorldArea(ncp_area);
}

______________SD_END_GRAPHICS_NAMESPACE______________