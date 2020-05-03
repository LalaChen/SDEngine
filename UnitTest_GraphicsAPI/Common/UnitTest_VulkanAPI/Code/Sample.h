#pragma once

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"
#include "VulkanWrapper.h"

using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;
using SDE::Graphics::Color4f;
using SDE::Graphics::Resolution;

class VulkanAPITestManager;

class BasicUniformBuffer
{
public:
    BasicUniformBuffer();
    ~BasicUniformBuffer();
public:
    Matrix4X4f m_clip;
    Matrix4X4f m_proj;
    Matrix4X4f m_view;
    Matrix4X4f m_worid;
    Matrix4X4f m_normal;
    Vector3f m_view_eye;
};

class LightUniformBuffer
{
public:
    LightUniformBuffer();
    ~LightUniformBuffer();
public:
    Color4f m_ambient;
    Color4f m_diffuse;
    Color4f m_specular;
    Vector3f m_position;
    Vector3f m_direction;
    float m_spot_exp;
    float m_spot_cos_cutoff;
    float m_constant_attenuation;
    float m_linear_attenuation;
    float m_quadratic_attenuation;
    float m_shininess;
    int32_t m_kind;
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample);

class Sample : public Object
{
public:
    explicit Sample(const ObjectName &i_object_name, VulkanAPITestManager *i_mgr);
    virtual ~Sample();
public:
    virtual void Initialize() = 0;
    virtual void Render() = 0;
    virtual void Destroy() = 0;
    virtual void Resize(Size_ui32 i_width, Size_ui32 i_height) = 0;
public:
    virtual VkImage GetColorBuffer() = 0;
    virtual VkImageView GetColorBufferImageView() = 0;
protected:
    virtual void CreateCommandBufferAndPool() = 0;
    virtual void CreateRenderPassAndFramebuffer() = 0;
protected:
    VulkanAPITestManager *m_mgr;
    Resolution m_current_res;
};