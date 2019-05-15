#pragma once

#include "Sample1_DrawTriangle.h"
#include "VulkanManager.h"

class VulkanAPITestManager : public SDE::Graphics::VulkanManager
{
public:
    explicit VulkanAPITestManager();
    virtual ~VulkanAPITestManager();
public:
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    void ReleaseGraphicsSystem() override;
public: //Buffer Related.
    VkResult CreateBuffer(VkBufferUsageFlags i_buffer_usage, VkSharingMode i_sharing_mode, VkDeviceSize i_size, VkBuffer &io_VK_buffer);
    VkResult AllocateMemoryAndBindToBuffer(VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkBuffer i_VK_buffer, VkDeviceMemory &io_VK_memory);
    VkResult RefreshLocalDeviceBufferData(VkBuffer i_VK_buffer, void *i_data_ptr, VkDeviceSize i_size);
    VkResult RefreshHostDeviceBufferData(VkBuffer i_VK_buffer, VkDeviceMemory i_VK_buffer_mem, void *i_data_ptr, VkDeviceSize i_size);
    void ReleaseMemory(VkDeviceMemory i_VK_memory);
    void DestroyBuffer(VkBuffer i_VK_buffer);
public: //Shader Related.
    VkResult CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo &i_c_info, VkDescriptorSetLayout &io_layout);
    void DestroyDesciptorSetLayout(VkDescriptorSetLayout i_VK_layout);
    VkResult CreateShaderModule(const std::vector<SDE::Basic::UByte> &i_program_binary, VkShaderModule &io_VK_shader_module);
    void DestroyShaderModule(VkShaderModule i_VK_shader_module);
    VkResult CreatePipelineLayout(const VkPipelineLayoutCreateInfo &i_pipeline_layout_c_info, VkPipelineLayout &io_VK_pipeline_layout);
    void DestroyPipelineLayout(VkPipelineLayout i_VK_pipeline_layout);
    VkResult CreateMainRenderPassGraphicsPipeline(const VkGraphicsPipelineCreateInfo &i_c_info, VkPipelineCache i_VK_pipeline_cache, uint32_t i_subpass_id, VkPipeline &io_VK_pipeline);
    void DestroyGraphicsPipeline(VkPipeline i_VK_pipeline);
    VkResult CreateDescriptorPool(const std::vector<VkDescriptorPoolSize> &i_descriptor_pool_sizes, uint32_t i_max_set_num, bool i_free_individual_sets, VkDescriptorPool &io_descriptor_pool);
    void DestroyDescriptorPool(VkDescriptorPool i_VK_descriptor_pool);
    VkResult AllocateDescriptorSet(const VkDescriptorSetAllocateInfo &i_a_info, VkDescriptorSet &io_descriptor_set);
    void BindDescriptorSets(VkPipelineLayout i_VK_pipeline_layout, VkPipelineBindPoint i_VK_pipeline_type, uint32_t i_first_set_id, const std::vector<VkDescriptorSet> &i_VK_desc_sets, const std::vector<uint32_t> &i_dynamic_offsets);
    void UpdateDescriptorSet(const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos);
public: //Set Dynamic State Function.
    void SetViewportsDynamically(const std::vector<VkViewport> &i_viewports);
    void SetMainViewportDynamically(const VkViewport &i_viewport);
public: //Draw function.
    void BindVertexBuffer(VkBuffer i_VK_buffer, VkDeviceSize i_VK_offset, uint32_t i_binding_id);
    void BindVertexBuffers(const std::vector<VkBuffer> &i_va_buffers, const std::vector<VkDeviceSize> &i_va_offset, uint32_t i_first_bind_id);
    void BindIndiceBuffer(VkBuffer i_VK_buffer, VkDeviceSize i_VK_offset, VkIndexType i_index_type);
    void BindGraphicsPipeline(VkPipeline i_VK_graphics_pipeline);
    void DrawByIndice(uint32_t i_indice_size, uint32_t i_instance_count, uint32_t i_first_index, int32_t i_vertex_offset, uint32_t i_first_instance);
public:
    void RenderToScreen() override;
    void RenderDebug();
protected:
    Sample1_DrawTriangle m_draw_triangle;
};