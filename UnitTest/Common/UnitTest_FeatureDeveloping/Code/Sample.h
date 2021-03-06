#pragma once

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using SDE::Basic::ObjectName;
using SDE::Basic::System;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample);

class Sample : public System
{
public:
    explicit Sample(const ObjectName &i_object_name);
    virtual ~Sample();
public:
    virtual void InitializeScene() = 0;
    virtual void UpdateScene() = 0;
    virtual void DestroyScene() = 0;
};