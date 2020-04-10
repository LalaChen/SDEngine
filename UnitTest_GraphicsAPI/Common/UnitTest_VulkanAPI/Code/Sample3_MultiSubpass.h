#pragma once

#define OBJ_SIZE 4
#define COLOR_AND_DEPTH_BUFFER_SIZE 2

#include "Sample.h"

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample3_MultiSubpass);

class Sample3_MultiSubpass : public Sample
{
public:
    explicit Sample3_MultiSubpass(VulkanAPITestManager *i_mgr);
    virtual ~Sample3_MultiSubpass();
public:
    void Initialize() override;
    void Render() override;
    void Resize(Size_ui32 i_width, Size_ui32 i_height) override;
    void Destroy() override;
    VkImage GetColorBuffer() override { return m_VK_color_buffer; }
    VkImageView GetColorBufferImageView() override { return m_VK_color_buffer_image_views[0]; }
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
    VkBuffer m_VK_basic_uniform_buffers[OBJ_SIZE];
    VkDeviceMemory m_VK_basic_uniform_buffer_memories[OBJ_SIZE];
protected:
//Texture
    VkImage m_VK_main_texture;
    VkDeviceMemory m_VK_main_texture_memory;
    VkImageView m_VK_main_texture_image_view;
    VkSampler m_VK_main_texture_sampler;
protected:
//shader
    VkPipelineLayout m_VK_pipeline_layouts[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkShaderModule m_vert_modules[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkShaderModule m_frag_modules[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkDescriptorSetLayout m_VK_main_shader_set0_layouts[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkDescriptorSet m_VK_descriptor_set0_uniforms[OBJ_SIZE];
    VkDescriptorPool m_VK_descriptor_pools[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkPipeline m_VK_main_graphics_pipelines[COLOR_AND_DEPTH_BUFFER_SIZE];
protected:
//render pass.
    VkRenderPass m_VK_render_pass;
protected:
//for camera
    VkImage m_VK_color_buffer; //only use id 0
    VkDeviceMemory m_VK_color_buffer_memory;
    VkImageView m_VK_color_buffer_image_views[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkImage m_VK_depth_buffer; //only use id 0
    VkDeviceMemory m_VK_depth_buffer_memory;
    VkImageView m_VK_depth_buffer_image_views[COLOR_AND_DEPTH_BUFFER_SIZE];
    VkFramebuffer m_VK_frame_buffer;
protected:
//prepare for work thread or camera.
    VkCommandPool m_VK_cmd_pool;
    VkCommandBuffer m_VK_cmd_buffer;
protected:
    BasicUniformBuffer m_uniform_buffer_datas[4];
};