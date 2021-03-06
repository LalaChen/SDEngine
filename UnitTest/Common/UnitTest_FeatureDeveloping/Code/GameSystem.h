#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "Sample.h"

using SDE::Basic::ObjectName;
using SDE::Basic::System;

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
    std::vector<SampleWeakReferenceObject> m_samples;
};
