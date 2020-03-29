#pragma once

#include "Sample1_DrawTriangle.h"
#include "Sample2_DrawScene.h"
#include "Sample3_MultiSubpass.h"
#include "VulkanManager.h"

class VulkanAPITestManager : public SDE::Graphics::VulkanManager
{
public:
    explicit VulkanAPITestManager();
    virtual ~VulkanAPITestManager();
public:
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    void ReleaseGraphicsSystem() override;
public:
    void Initialize() override;
    void Release() override;
public: //Memory Related.
    VkResult AllocateMemoryAndBindToBuffer(VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkBuffer i_VK_buffer, VkDeviceMemory &io_VK_memory);
    VkResult AllocateMemoryAndBindToImage(VkImage i_VK_img, VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkDeviceMemory &io_VK_memory);
    void* MapDeviceMemoryTest(VkDeviceMemory i_VK_memory, const VkDeviceSize &i_size);
    void UnmapDeviceMemoryTest(VkDeviceMemory i_VK_memory);
    void ReleaseMemory(VkDeviceMemory i_VK_memory);
public: //Buffer Related.
    VkResult CreateBuffer(VkBufferUsageFlags i_buffer_usage, VkSharingMode i_sharing_mode, VkDeviceSize i_size, VkBuffer &io_VK_buffer);
    VkResult RefreshLocalDeviceBufferData(VkBuffer i_VK_buffer, void *i_data_ptr, VkDeviceSize i_size);
    VkResult RefreshHostDeviceBufferData(VkBuffer i_VK_buffer, VkDeviceMemory i_VK_buffer_mem, void *i_data_ptr, VkDeviceSize i_size);
    void DestroyBuffer(VkBuffer i_VK_buffer);
public:
    VkResult CreateImage(const VkImageCreateInfo &i_info, VkImage &io_VK_img);
    VkResult RefreshLocalDeviceImage(VkImage i_VK_img, const void *i_data_ptr, uint32_t i_width, uint32_t i_height, uint32_t i_depth, VkDeviceSize i_data_size);
    VkResult RefreshHostDeviceImage(VkImage i_VK_img, VkDeviceMemory i_VK_img_mem, const void *i_data_ptr, VkDeviceSize i_size);
    void DestroyImage(VkImage i_VK_buffer);
public: //Shader Related.
    VkResult CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo &i_c_info, VkDescriptorSetLayout &io_layout);
    void DestroyDesciptorSetLayout(VkDescriptorSetLayout i_VK_layout);

    VkResult CreateShaderModule(const std::vector<SDE::Basic::UByte> &i_program_binary, VkShaderModule &io_VK_shader_module);
    void DestroyShaderModule(VkShaderModule i_VK_shader_module);
   
    VkResult CreatePipelineLayout(const VkPipelineLayoutCreateInfo &i_pipeline_layout_c_info, VkPipelineLayout &io_VK_pipeline_layout);
    void DestroyPipelineLayout(VkPipelineLayout i_VK_pipeline_layout);
 
    VkResult CreateGraphicsPipeline(const VkGraphicsPipelineCreateInfo &i_c_info, VkPipelineCache i_VK_pipeline_cache, VkPipeline &io_VK_pipeline);
    void DestroyGraphicsPipeline(VkPipeline i_VK_pipeline);

    VkResult CreateDescriptorPool(const std::vector<VkDescriptorPoolSize> &i_descriptor_pool_sizes, uint32_t i_max_set_num, bool i_free_individual_sets, VkDescriptorPool &io_descriptor_pool);
    void DestroyDescriptorPool(VkDescriptorPool i_VK_descriptor_pool);
    VkResult AllocateDescriptorSet(const VkDescriptorSetAllocateInfo &i_a_info, VkDescriptorSet &io_descriptor_set);
    void BindDescriptorSets(VkCommandBuffer i_VK_cmd_buffer, VkPipelineLayout i_VK_pipeline_layout, VkPipelineBindPoint i_VK_pipeline_type, uint32_t i_first_set_id, const std::vector<VkDescriptorSet> &i_VK_desc_sets, const std::vector<uint32_t> &i_dynamic_offsets);
    void UpdateDescriptorSet(const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos);

    VkResult CreateSampler(const VkSamplerCreateInfo &i_c_info, VkSampler &io_VK_sampler);
    void DestroySampler(VkSampler i_VK_sampler);

    VkResult CreateImageView(const VkImageViewCreateInfo &i_iv_info, VkImageView &io_VK_image_view);
    void DestroyImageView(VkImageView i_VK_image_view);
public:
    VkResult CreateRenderPass(const VkRenderPassCreateInfo &i_rp_c_info, VkRenderPass &io_rp);
    void BeginRenderPass(const VkRenderPassBeginInfo &i_rp_b_info, const VkCommandBuffer &i_cmd_buffer);
    void EndRenderPass(const VkCommandBuffer &i_cmd_buffer);
    void DestroyRenderPass(VkRenderPass i_render_pass);
public:
    VkResult CreateVkFramebuffer(const VkFramebufferCreateInfo &i_fb_c_info, VkFramebuffer &io_fb);
    void DestroyFramebuffer(VkFramebuffer i_framebuffer);
public:
    VkResult CreateCommandPool(const VkCommandPoolCreateInfo &i_cmd_p_c_info, VkCommandPool &io_cmd_pool);
    VkResult ResetCommandPool(VkCommandPool i_cmd_pool, bool i_clear_resource);
    void DestroyCommandPool(VkCommandPool i_cmd_pool);

    VkResult AllocateCommandBuffer(const VkCommandBufferAllocateInfo &i_cmd_buf_a_info, VkCommandBuffer &io_cmd_buffer);
    VkResult BeginCommandBuffer(const VkCommandBufferBeginInfo &i_cmd_buf_b_info, VkCommandBuffer i_cmd_buffer);
    VkResult EndCommandBuffer(VkCommandBuffer i_cmd_buffer);
    VkResult ResetCommandBuffer(VkCommandBuffer i_cmd_buffer, bool i_reset_resource);
    void FreeCommandBuffers(VkCommandPool i_target_cmd_pool, VkCommandBuffer *i_cmd_buffers, uint32_t i_buffer_size);
public: //Set Dynamic State Function.
    void SetViewportsDynamically(VkCommandBuffer i_cmd_buffer, const std::vector<VkViewport> &i_viewports);
    void SetMainViewportDynamically(VkCommandBuffer i_cmd_buffer, const VkViewport &i_viewport);
    void SetScissorDynamically(VkCommandBuffer i_cmd_buffer, const std::vector<VkRect2D> &i_rect);
    void SetMainScissorDynamically(VkCommandBuffer i_cmd_buffer, const VkRect2D &i_rect);
public: //Draw function.
    void BindVertexBuffer(VkCommandBuffer i_cmd_buffer, VkBuffer i_VK_buffer, VkDeviceSize i_VK_offset, uint32_t i_binding_id);
    void BindVertexBuffers(VkCommandBuffer i_cmd_buffer, const std::vector<VkBuffer> &i_va_buffers, const std::vector<VkDeviceSize> &i_va_offset, uint32_t i_first_bind_id);
    void BindIndiceBuffer(VkCommandBuffer i_cmd_buffer, VkBuffer i_VK_buffer, VkDeviceSize i_VK_offset, VkIndexType i_index_type);
    void BindGraphicsPipeline(VkCommandBuffer i_cmd_buffer, VkPipeline i_VK_graphics_pipeline);
    void DrawByIndice(VkCommandBuffer i_cmd_buffer, uint32_t i_indice_size, uint32_t i_instance_count, uint32_t i_first_index, int32_t i_vertex_offset, uint32_t i_first_instance);
public:
    VkResult SubmitCommandBufferToMainQueue(VkCommandBuffer i_cmd_buffer);
public:
    void RenderToScreen() override;
    void RenderDebug();
public:
    void SetCurrentSampleIndex(uint32_t i_sample_idx);
protected:
    uint32_t m_cur_sample_idx;
    std::vector<SampleStrongReferenceObject> m_samples;
};