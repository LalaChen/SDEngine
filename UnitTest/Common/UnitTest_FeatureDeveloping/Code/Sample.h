#pragma once

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using SDE::Graphics::Texture;
using SDE::Graphics::TextureStrongReferenceObject;
using SDE::Graphics::TextureWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample);

class Sample : public Object
{
public:
    explicit Sample(const ObjectName &i_object_name);
    virtual ~Sample();
public:
    virtual void InitializeScene() = 0;
    virtual void UpdateScene() = 0;
    virtual void DestroyScene() = 0;
};