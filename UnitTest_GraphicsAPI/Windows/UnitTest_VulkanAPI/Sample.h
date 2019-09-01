#pragma once

#include <vulkan/vulkan.h>

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using SDE::Math::Matrix4X4f;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;

class VulkanAPITestManager;

class BasicUniformBuffer
{
public:
    Matrix4X4f m_clip;
    Matrix4X4f m_proj;
    Matrix4X4f m_view;
    Matrix4X4f m_worid;
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
protected:
    virtual void CreateCommandBufferAndPool() = 0;
    virtual void CreateRenderPassAndFramebuffer() = 0;
protected:
    VulkanAPITestManager *m_mgr;
protected: //prepare for camera.
    VkCommandPool m_VK_cmd_pool;
    VkCommandBuffer m_VK_cmd_buffer;
    VkFramebuffer m_VK_frame_buffer;
};