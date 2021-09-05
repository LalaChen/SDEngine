#pragma once

using SDE::Basic::ObjectName;
using SDE::Basic::System;
using SDE::Basic::EntityGroup;
using SDE::Basic::EntityGroupWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GameSystem);

class GameSystem : public System
{
public:
    explicit GameSystem(const ObjectName &i_object_name);
    virtual ~GameSystem();
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
    void Resize() override;
public:
    bool OnAppEventTriggered(const EventArg &i_arg);
protected:
    uint32_t m_cur_sample_idx;
    std::vector<ObjectName> m_scene_names;
    EntityGroupWeakReferenceObject m_camera_motor_group;
    EntityGroupWeakReferenceObject m_HUD_group;
    EntityGroupWeakReferenceObject m_screen_ray_group;
};
