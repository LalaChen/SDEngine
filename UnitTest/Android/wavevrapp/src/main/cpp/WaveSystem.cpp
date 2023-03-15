#include "HUDComponent.h"
#include "WaveVRControllerComponent.h"
#include "SampleDrawObjects.h"
#include "WaveVREventArg.h"
#include "WaveSystem.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

WaveSystem::WaveSystem(const ObjectName &i_object_name)
: System(i_object_name)
{
    m_scene_names.push_back("SampleDrawObjects");
}

WaveSystem::~WaveSystem()
{
}

void WaveSystem::Initialize()
{
    SD_WREF(Application::GetRef().GetEventNotifier()).RegisterSlotFunctionIntoEvent(
        sAppEventName,
        new MemberFunctionSlot<WaveSystem>(
            "WaveSystem::OnAppEventTriggered",
            GetThisWeakPtrByType<WaveSystem>(),
            &WaveSystem::OnAppEventTriggered));

    SD_WREF(Application::GetRef().GetEventNotifier()).RegisterSlotFunctionIntoEvent(
            WaveVREventArg::sWaveVREventName,
            new MemberFunctionSlot<WaveSystem>(
                    "WaveSystem::OnWaveVREventTriggered",
                    GetThisWeakPtrByType<WaveSystem>(),
                    &WaveSystem::OnWaveVREventTriggered));

    m_camera_group = ECSManager::GetRef().AddEntityGroup(
		"WaveSystem",
		{
			std::type_index(typeid(CameraComponentBase))
		}
    );

	m_HUD_group = ECSManager::GetRef().AddEntityGroup(
        "HUDComponent",
        {
            std::type_index(typeid(HUDComponent))
        }
    );

    m_ctrlers_group = ECSManager::GetRef().AddEntityGroup(
            "WaveVRControllerComponent",
            {
                std::type_index(typeid(WaveVRControllerComponent))
            });
}

void WaveSystem::Update()
{
    //1. get pose for devices.
    WVR_GetSyncPose(WVR_PoseOriginModel_OriginOnGround, m_device_poses, WVR_DEVICE_COUNT_LEVEL_1);
    //2. update camera pose.
	UpdateCameraPose();
	//3. update ctrler pose.

	//4. update world GUI.
	UpdateWorldGUIs();
}

void WaveSystem::Destroy()
{
    for (std::string &scene_name : m_scene_names) {
        SceneManager::GetRef().UnloadScene(m_scene_names[0]);
    }
}

void WaveSystem::Resize()
{
}

bool WaveSystem::OnAppEventTriggered(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(AppEventArg).hash_code()) {
        const AppEventArg &arg = dynamic_cast<const AppEventArg&>(i_arg);
        if (arg.m_app_event == AppEvent_GRAPHICS_INITIALIZED) {

            SceneManager::GetRef().RegisterScene(new SampleDrawObjects(m_scene_names[0]));
            SceneManager::GetRef().LoadScene("SampleDrawObjects");

            UpdateCameraInnerParameter();
        }

        return true;
    }
    else {
        SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
        return false;
    }
}

bool WaveSystem::OnWaveVREventTriggered(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(WaveVREventArg).hash_code()) {
        const WaveVREventArg &arg = dynamic_cast<const WaveVREventArg&>(i_arg);
        switch(arg.m_event.common.type) {
            case WVR_EventType_DeviceConnected:
                {
                    SDLOG("Device %u attached. Setting up controller cube.", arg.m_event.device.deviceType);
                }
                break;
            case WVR_EventType_DeviceDisconnected:
                {
                    SDLOG("Device %u detached.", arg.m_event.device.deviceType);
                }
                break;
            case WVR_EventType_DeviceStatusUpdate:
                {
                    SDLOG("Device %u updated.", arg.m_event.device.deviceType);
                }
                break;
            case WVR_EventType_IpdChanged:
                {
                    SDLOG("IPD is changed and renew the transform from eye to head.");
                    UpdateCameraInnerParameter();
                }
                break;
        }
        return true;
    }
    else {
        SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
        return false;
    }
}

void WaveSystem::UpdateCameraInnerParameter()
{
    Matrix4X4f eye_offsets[WVR_Eye_Both];
    eye_offsets[WVR_Eye_Left] = ConvertWVR_Matrix4f_tToMatrix4X4f(WVR_GetTransformFromEyeToHead(WVR_Eye_Left, WVR_NumDoF_6DoF));
    eye_offsets[WVR_Eye_Right] = ConvertWVR_Matrix4f_tToMatrix4X4f(WVR_GetTransformFromEyeToHead(WVR_Eye_Right, WVR_NumDoF_6DoF));

    Vector3f eye_positions[WVR_Eye_Both];
    for (uint32_t eye_id = 0; eye_id < WVR_Eye_Both; ++eye_id) {
        eye_positions[eye_id] = Vector3f(
                eye_offsets[eye_id].m_matrix[3][0],
                eye_offsets[eye_id].m_matrix[3][1],
                eye_offsets[eye_id].m_matrix[3][2],
                1.0f);
    }

    Frustum frustums[WVR_Eye_Both];
    for (uint32_t eye = 0; eye < WVR_Eye_Both; ++eye) {
        WVR_GetClippingPlaneBoundary((WVR_Eye)eye, &frustums[eye].l, &frustums[eye].r, &frustums[eye].t, &frustums[eye].b);
        frustums[eye].n = 0.01f;
        frustums[eye].l *= frustums[eye].n;
        frustums[eye].r *= frustums[eye].n;
        frustums[eye].t *= frustums[eye].n;
        frustums[eye].b *= frustums[eye].n;
        frustums[eye].f = 300.0f;
        frustums[eye].p = true;
    }

    std::list<EntityWeakReferenceObject> entities = SD_WREF(m_camera_group).GetEntities();
    if (entities.size() > 0) {
        for (EntityWeakReferenceObject &entity : entities) {
            VRCameraComponentWeakReferenceObject vrcamera = SD_GET_TYPE_COMP_WREF(entity, VRCameraComponent);
            if (vrcamera.IsNull() == false) {
                SD_WREF(vrcamera).SetEyeCenters(eye_positions);
                SD_WREF(vrcamera).SetFrustums(frustums);
            } else {
                SDLOGE("No VRCamera!!!!");
            }
        }
    } else {
        SDLOGE("No entity with VRCamera!!!!");
    }
}

void WaveSystem::UpdateCameraPose()
{
    std::list<EntityWeakReferenceObject> entities = SD_WREF(m_camera_group).GetEntities();
    for (EntityWeakReferenceObject &entity : entities) {
        TransformComponentWeakReferenceObject vrcamera_xform = SD_WREF(entity).GetComponent(typeid(TransformComponent)).DynamicCastTo<TransformComponent>();
        if (vrcamera_xform.IsNull() == false) {
            Transform xform;
            if (m_device_poses[WVR_DEVICE_HMD].pose.isValidPose == true) {
                xform = ConvertWVR_PoseState_tToMatrix4X4f(m_device_poses[WVR_DEVICE_HMD].pose);
            }
            else {
                xform = Transform();
            }
            SD_WREF(vrcamera_xform).SetWorldTransform(xform);
        }
    }
}

void WaveSystem::UpdateWorldGUIs()
{
    std::list<EntityWeakReferenceObject> entities = SD_WREF(m_HUD_group).GetEntities();
    for (EntityWeakReferenceObject &entity : entities) {
        HUDComponentWeakReferenceObject hud = SD_WREF(entity).GetComponent(typeid(HUDComponent)).DynamicCastTo<HUDComponent>();
        if (hud.IsNull() == false) {
            SD_WREF(hud).Update();
        }
    }
}

void WaveSystem::UpdateControllerPose()
{
    std::list<EntityWeakReferenceObject> entities = SD_WREF(m_ctrlers_group).GetEntities();
    for (EntityWeakReferenceObject &entity : entities) {
        WaveVRControllerComponentWeakReferenceObject ctrler_xform = SD_WREF(entity).GetComponent(typeid(WaveVRControllerComponent)).DynamicCastTo<WaveVRControllerComponent>();
        if (ctrler_xform.IsNull() == false) {
            Transform xform;
            if (m_device_poses[WVR_DEVICE_HMD].pose.isValidPose == true) {
                xform = ConvertWVR_PoseState_tToMatrix4X4f(m_device_poses[WVR_DEVICE_HMD].pose);
            }
            else {
                xform = Transform();
            }
            SD_WREF(ctrler_xform).SetControllerPose(xform);
        }
    }
}