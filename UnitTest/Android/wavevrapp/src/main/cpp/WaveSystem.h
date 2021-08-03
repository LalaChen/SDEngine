#pragma once

#include <wvr/wvr_types.h>
#include <wvr/wvr_device.h>
#include <SDEngineMacro.h>
#include <SDEngineCommonType.h>
#include <SDEngine.h>

using SDE::Basic::ObjectName;
using SDE::Basic::System;
using SDE::Basic::EntityGroup;
using SDE::Basic::EntityGroupWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(WaveSystem);

class WaveSystem : public System
{
public:
    explicit WaveSystem(const ObjectName &i_object_name);
    virtual ~WaveSystem();
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
    void Resize() override;
public:
    bool OnAppEventTriggered(const EventArg &i_arg);
    bool OnWaveVREventTriggered(const EventArg &i_arg);
public:
    void UpdateCameraInnerParameter();
    void UpdateCameraPose();
	void UpdateWorldGUIs();
protected:
    std::vector<ObjectName> m_scene_names;
protected:
    WVR_DevicePosePair_t m_device_poses[WVR_DEVICE_COUNT_LEVEL_1];
    EntityGroupWeakReferenceObject m_camera_group;
	EntityGroupWeakReferenceObject m_HUD_group;
};
