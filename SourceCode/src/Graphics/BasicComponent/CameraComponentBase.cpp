#include "CameraComponentBase.h"

#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

const std::string CameraComponentBase::sCameraResizedEventName = "CameraResized";

CameraComponentBase::CameraComponentBase(const ObjectName &i_object_name)
: Component(i_object_name)
, m_workspace_type(CameraWorkspaceType_Forward)
, m_follow_resolution(true)
, m_ws_initialized(false)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
{
    m_buffer_size = GraphicsManager::GetRef().GetScreenResolution();
    RegisterEvent(new Event(sCameraResizedEventName));
}

CameraComponentBase::~CameraComponentBase()
{
}

void CameraComponentBase::ResizeImpl()
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

    NotifyEvent(sCameraResizedEventName, EventArg());
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