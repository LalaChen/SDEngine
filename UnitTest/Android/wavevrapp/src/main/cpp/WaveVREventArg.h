#pragma once

#include "WaveCommonFunction.h"

class WaveVREventArg : public EventArg
{
public:
    static const std::string sWaveVREventName;
public:
    WaveVREventArg(){}
    ~WaveVREventArg(){}
public:
    WVR_Event m_event;
};