#pragma once

#include <vulkan/vulkan.h>

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using SDE::Math::Matrix4X4f;

class VulkanAPITestManager;

class BasicUniformBuffer
{
public:
    Matrix4X4f m_clip;
    Matrix4X4f m_proj;
    Matrix4X4f m_view;
    Matrix4X4f m_worid;
};

class Sample1_DrawTriangle
{
public:
    explicit Sample1_DrawTriangle(VulkanAPITestManager *i_mgr);
    virtual ~Sample1_DrawTriangle();
public:
    void Initialize();
    void Render();
    void Destroy();
private:
    void CreateBuffers();
    void CreateUniformBuffer();
    void CreateShaderPrograms();
protected:
    VulkanAPITestManager *m_mgr;
protected:
    //Device Local memory buffer. (Memory type VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT can't use map and unmap)
    VkBuffer m_VK_vertice_buffer;
    VkDeviceMemory m_VK_vbuf_memory;
    VkBuffer m_VK_ver_color_buffer;
    VkDeviceMemory m_VK_ver_color_buf_memory;
    VkBuffer m_VK_indices_buffer;
    VkDeviceMemory m_VK_ibuf_memory;
protected:
    VkBuffer m_VK_basic_uniform_buffer;
    VkDeviceMemory m_VK_basic_uniform_buffer_memory;
protected:
    //shader
    VkPipelineLayout m_VK_pipeline_layout;
    VkShaderModule m_vert_module;
    VkShaderModule m_frag_module;
    VkDescriptorSetLayout m_VK_main_shader_set0_layout;
    VkDescriptorSet m_VK_descriptor_set0;
    VkDescriptorPool m_VK_descriptor_pool;
    VkPipeline m_VK_main_graphics_pipeline;
protected:
    BasicUniformBuffer m_uniform_buffer_data;
};