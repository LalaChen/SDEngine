#pragma once

#include "Sample.h"

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample1_DrawTriangle);

class Sample1_DrawTriangle : public Sample
{
public:
    explicit Sample1_DrawTriangle(VulkanAPITestManager *i_mgr);
    virtual ~Sample1_DrawTriangle();
public:
    void Initialize() override;
    void Render() override;
    void Destroy() override;
    VkImage GetColorBuffer() override { return m_VK_color_buffer; }
protected:
    void CreateRenderPassAndFramebuffer() override;
    void CreateCommandBufferAndPool() override;
private:
    void CreateBuffers();
    void CreateTexture();
    void CreateUniformBuffer();
    void CreateShaderPrograms();
protected:
//Vertex Device Local memory buffer.
//(Memory type VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT can't use map and unmap)
    VkBuffer m_VK_ver_buffer;
    VkDeviceMemory m_VK_ver_buf_memory;
    VkBuffer m_VK_ver_color_buffer;
    VkDeviceMemory m_VK_ver_color_buf_memory;
    VkBuffer m_VK_ver_tex_buffer;
    VkDeviceMemory m_VK_ver_tex_buf_memory;
    VkBuffer m_VK_indices_buffer;
    VkDeviceMemory m_VK_ibuf_memory;
protected:
//Vertex Host memory buffer.
    VkBuffer m_VK_basic_uniform_buffer;
    VkDeviceMemory m_VK_basic_uniform_buffer_memory;
protected:
//Texture
    VkImage m_VK_main_texture;
    VkDeviceMemory m_VK_main_texture_memory;
    VkImageView m_VK_main_texture_image_view;
    VkSampler m_VK_main_texture_sampler;
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
//render pass.
    VkRenderPass m_VK_render_pass;
protected:
//for camera
    VkImage m_VK_color_buffer;
    VkDeviceMemory m_VK_color_buffer_memory;
    VkImageView m_VK_color_buffer_image_view;
    VkImage m_VK_depth_buffer;
    VkDeviceMemory m_VK_depth_buffer_memory;
    VkImageView m_VK_depth_buffer_image_view;
    VkFramebuffer m_VK_frame_buffer;
protected:
//prepare for work thread or camera.
    VkCommandPool m_VK_cmd_pool;
    VkCommandBuffer m_VK_cmd_buffer;
protected:
    BasicUniformBuffer m_uniform_buffer_data;
};