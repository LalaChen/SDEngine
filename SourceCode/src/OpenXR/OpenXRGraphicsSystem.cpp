#include "OpenXRGraphicsSystem.h"

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>

#include "OpenXRAPIManager.h"
#include "OpenXRVulkanManager.h"

#include "ECSManager.h"
#include "OpenXRSpaceComponent.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;

______________SD_START_OPENXR_NAMESPACE______________

OpenXRGraphicsSystem::OpenXRGraphicsSystem(const ObjectName &i_object_name)
: GraphicsSystem(i_object_name)
{
}

OpenXRGraphicsSystem::~OpenXRGraphicsSystem()
{
}

void OpenXRGraphicsSystem::Initialize()
{
    //1. initialize graphics system.
    GraphicsSystem::Initialize();
    //2. initialize action space.
    //3. create space component group.
    m_space_eg = ECSManager::GetRef().AddEntityGroup(
        "SpaceGroup",
        {
            std::type_index(typeid(OpenXRSpaceComponent))
        }
    );
}

void OpenXRGraphicsSystem::Update()
{
    //1. begin frame.
    XrFrameWaitInfo frame_wait_info = {XR_TYPE_FRAME_WAIT_INFO};
    OpenXRAPIManager::GetRef().WaitFrame(frame_wait_info);

    XrFrameBeginInfo frame_begin_info = {XR_TYPE_FRAME_BEGIN_INFO};
    OpenXRAPIManager::GetRef().BeginFrame(frame_begin_info);
    //2. update space by frame.predictTime.
    std::list<EntityWeakReferenceObject> space_entity_list = SD_SREF(m_space_eg).GetEntities();
    for (EntityWeakReferenceObject ce : space_entity_list) {
        SD_WREF(SD_GET_COMP_WREF(ce, CameraComponentBase)).Resize();
    }
    //3. update graphics system.
    GraphicsSystem::Update();
    //4. collect layer info from swapchain.

    //5. end frame.
    XrFrameEndInfo frame_end_info = {XR_TYPE_FRAME_END_INFO};
 
    OpenXRAPIManager::GetRef().EndFrame(frame_end_info);
}

void OpenXRGraphicsSystem::Destroy()
{
    GraphicsSystem::Destroy();
}

void OpenXRGraphicsSystem::Resize()
{
    GraphicsSystem::Resize();
}

_______________SD_END_OPENXR_NAMESPACE_______________