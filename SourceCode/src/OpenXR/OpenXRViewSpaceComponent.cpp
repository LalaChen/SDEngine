#include "OpenXRViewSpaceComponent.h"

#include "OpenXRAPIManager.h"
#include "LogManager.h"

using SDE::Graphics::VREye_Both;

______________SD_START_OPENXR_NAMESPACE______________

OpenXRViewSpaceComponent::OpenXRViewSpaceComponent(const ObjectName &i_object_name)
: OpenXRSpaceComponent(i_object_name, XR_REFERENCE_SPACE_TYPE_VIEW)
{
}

OpenXRViewSpaceComponent::~OpenXRViewSpaceComponent()
{
    OpenXRAPIManager::GetRef().DestroySpace(m_space);
}

void OpenXRViewSpaceComponent::InitializeImpl()
{
    //1. get transform component
    OpenXRSpaceComponent::InitializeImpl();
    //2. get camera component
    m_camera_comp = SD_GET_COMP_WREF(m_entity, VRCameraComponent);
    m_trans_comp = SD_GET_COMP_WREF(m_entity, TransformComponent);
    //3. create space.
    XrReferenceSpaceCreateInfo c_info = {};
    c_info.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
    c_info.poseInReferenceSpace.orientation.w = 1.0f;
    c_info.referenceSpaceType = m_space_type;

    OpenXRAPIManager::GetRef().CreateSpace(c_info, m_space);
}

void OpenXRViewSpaceComponent::UpdateImpl()
{
    if (m_space != XR_NULL_HANDLE) {
        //1. update camera info.
        if (m_camera_comp.IsNull() == false) {
            std::vector<XrView> views;
            views.resize(VREye_Both);
            XrViewState view_state{};
            view_state.type = XR_TYPE_VIEW_STATE;
            uint32_t view_c_input = (uint32_t)views.size();
            uint32_t view_c_output;

            XrViewLocateInfo view_info = {};
            view_info.type = XR_TYPE_VIEW_LOCATE_INFO;
            view_info.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
            view_info.displayTime = OpenXRAPIManager::GetRef().GetPredictedDisplayTime();
            view_info.space = m_space;

            XrResult result = OpenXRAPIManager::GetRef().LocateViews(view_info, view_state, views);
            if (result == XR_SUCCESS) {
                Matrix4X4f projs[VREye_Both];
                Matrix4X4f view_mats[VREye_Both];
                Transform view_trans[VREye_Both];
                Transform camera_trans;
                for (uint32_t i = 0; i < VREye_Both; ++i) {
                    OpenXRAPIManager::GetRef().ConvertXrViewToProjMatrix(views[i], 0.01f, 1000.0f, projs[i]);
                    OpenXRAPIManager::GetRef().CovertXrPoseToTransform(views[i].pose, view_trans[i]);
                }
                SD_WREF(m_camera_comp).SetProjectionMatrices(projs);
                //
                camera_trans.m_position = (view_trans[0].m_position + view_trans[1].m_position).scale(0.5);
                camera_trans.m_position.RepresentPosition();
                camera_trans.m_rotation = Quaternion::Slerp(view_trans[0].m_rotation, view_trans[1].m_rotation, 0.5f);
                SD_WREF(m_trans_comp).SetWorldTransform(camera_trans);

                for (uint32_t i = 0; i < VREye_Both; ++i) {
                    view_mats[i] = view_trans[i].MakeWorldMatrix() * camera_trans.MakeWorldMatrix().inverse();
                }

                SD_WREF(m_camera_comp).SetEyeMatrices(view_mats);
            }
        }
    }
}

_______________SD_END_OPENXR_NAMESPACE_______________