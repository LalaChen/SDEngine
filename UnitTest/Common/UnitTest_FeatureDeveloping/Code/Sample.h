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
    virtual void Initialize() = 0;
    virtual void Render() = 0;
    virtual void Destroy() = 0;
    virtual void Resize(Size_ui32 i_width, Size_ui32 i_height) = 0;
public:
    virtual TextureWeakReferenceObject GetColorBuffer() const = 0;
protected:
    virtual void CreateCommandBufferAndPool() = 0;
    virtual void CreateRenderPass() = 0;
    virtual void CreateFramebuffer() = 0;
protected:
    Resolution m_current_res;
};