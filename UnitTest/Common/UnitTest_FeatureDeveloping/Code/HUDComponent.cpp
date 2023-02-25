#include "HUDComponent.h"

HUDComponent::HUDComponent(const ObjectName &i_name)
: Component(i_name)
{
}

HUDComponent::~HUDComponent()
{
}

void HUDComponent::InitializeImpl()
{
    EntityWeakReferenceObject entity = GetEntity();
    m_GUI = SD_GET_COMP_WREF(entity, WorldIMGUIComponent);
    m_transform = SD_GET_COMP_WREF(entity, TransformComponent);

    m_FPS_label = SD_WREF(m_GUI).GetGUINode<IMGUITextLabel>("FPSLabel");
    if (m_FPS_label.IsNull() == true) {
        SDLOGE("No FPSLabel !!!");
    }

    m_camera_pos_label = SD_WREF(m_GUI).GetGUINode<IMGUIVectorLabel>("CameraPosition");
    if (m_camera_pos_label.IsNull() == true) {
        SDLOGE("No m_camera_pos_label !!!");
    }

    m_camera_rot_label = SD_WREF(m_GUI).GetGUINode<IMGUIVectorLabel>("CameraRotation");
    if (m_camera_rot_label.IsNull() == true) {
        SDLOGE("No m_camera_rot_label !!!");
    }

    m_touch_btn_label = SD_WREF(m_GUI).GetGUINode<IMGUITextLabel>("TouchButton");
    if (m_touch_btn_label.IsNull() == true) {
        SDLOGE("No m_touch_btn_label !!!");
    }

    m_btn1 = SD_WREF(m_GUI).GetGUINode<IMGUIButton>("Button1");
    if (m_btn1.IsNull() == true) {
        SDLOGE("No m_btn1 !!!");
    }
    else {
        SD_WREF(m_btn1).RegisterSlotFunctionIntoEvent(IMGUIButton::sButtonEventName_Clicked, 
            new MemberFunctionSlot<HUDComponent>(
            "HUDComponent::OnBtn1Clicked",
            GetThisWeakPtrByType<HUDComponent>(),
            &HUDComponent::OnBtn1Clicked));
    }
}

void HUDComponent::UpdateImpl()
{
    if (m_FPS_label.IsNull() == false) {
        float fps = GraphicsManager::GetRef().GetFPS();
        SD_WREF(m_FPS_label).SetText(SDE::Basic::StringFormat("FPS : %lf", fps));
    }

    if (m_transform.IsNull() == false) {
        Transform camera_trans = SD_WREF(m_transform).GetWorldTransform();
        if (m_camera_pos_label.IsNull() == false) {
            SD_WREF(m_camera_pos_label).SetValue(camera_trans.m_position);
        }

        if (m_camera_rot_label.IsNull() == false) {
            SD_WREF(m_camera_rot_label).SetValue(camera_trans.m_rotation.toEulerianAngles());
        }
    }

    if (m_touch_btn_label.IsNull() == false && m_GUI.IsNull() == false) {
        TouchButton tb = SD_WREF(m_GUI).GetTouchButton();
        SD_WREF(m_touch_btn_label).SetText(SDE::Basic::StringFormat("tbInfo : (%d)(%lf,%lf)", tb.m_state, tb.m_x, tb.m_y));
    }
}

bool HUDComponent::OnBtn1Clicked(const EventArg &i_arg)
{
    SDLOG("HUDComponent::OnBtn1Clicked() : %s clicked.", SD_WREF(m_btn1).GetObjectName().c_str());
    return true;
}