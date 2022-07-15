/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*! \file      VulkanManager.h
 *  \brief     Introduce of class VulkanManager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/09
 *  \copyright MIT License.
 */

#pragma once

#include "VulkanConfig.h"
#include "GraphicsManager.h"

#include "VulkanQueueFamily.h"

using SDE::Basic::UBytePtr;

extern std::mutex m_queue_mutex;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class VulkanManager
 *  \brief In our system, VulkanManager is a integration for Vulkan graphics API.
 */
class SDENGINE_CLASS VulkanManager : public GraphicsManager
{
public:
    friend class VulkanSwapchain;
public:
    static VkBool32 ConvertBoolean(bool flag);
protected:
    static const VkClearValue sClearColor;
    static const VkClearValue sClearDepth;
public:
    /*! \fn VulkanManager();
     *  \brief The constructor of VulkanManager Class.
     */
    VulkanManager();

    /*! \fn virtual ~VulkanManager();
     *  \brief The destructor of VulkanManager Class.
     */
    virtual ~VulkanManager();
public:
    /*! \fn void InitializeGraphicsSystem(const EventArg &i_arg) override;
     *  \param [in] i_arg Vulkan creating arguments.
     *  \brief Initialize graphics API. (link dll, ...)
     */
    void InitializeGraphicsSystem(const EventArg &i_arg) override;

    /*! \fn void ReleaseGraphicsSystem() override;
     *  \brief Release graphics API.
     */
    void ReleaseGraphicsSystem() override;
public:
    /*! \fn void PrintSystemInformation() override;
     *  \brief Print system information.
     */
    void PrintSystemInformation() override;
public:
    void GetDesiredVulkanValidLayers(std::vector<const char*> &io_valid_layers) const override;
//----------- Descriptor Set and Pool Function.
public:
    void CreateDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity,  const std::vector<UniformVariableDescriptorWeakReferenceObject> &i_uvds) override;
    void DestroyDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity) override;
    void CreateDescriptorPool(DescriptorPoolIdentity &io_identity) override;
    void DestroyDescriptorPool(DescriptorPoolIdentity &io_identity) override;
    void AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool, const DescriptorSetLayoutWeakReferenceObject &i_layout) override;
    void WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uvs) override;
    void FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool) override;
//----------- Command Buffer and Pool Function
public:
    void CreateCommandPool(CommandPoolIdentity &io_identity) override;
    void DestroyCommandPool(CommandPoolIdentity &io_identity) override;
    void AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool) override;
    void BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info) override;
    void EndCommandBuffer(const CommandBufferIdentity &i_identity) override;
    void FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool) override;
    void ExecuteCommandsToPrimaryCommandBuffer(const CommandBufferWeakReferenceObject &i_primary_cb, const std::list<CommandBufferWeakReferenceObject> &i_secondary_cbs) override;
public:
//----------- Vertex Buffer Interface Function ------------
    /*
     * We will set i_data_size to VertexBufferIdentity::m_buffer_size and assign allocated memory size to m_memory_size
     */
    void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size) override;
    void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteVertexBuffer(VertexBufferIdentity &io_identity) override;
    void MapVertexBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) override;
    void UnmapVertexBuffer(const VertexBufferIdentity &i_identity) override;
//----------- Index Buffer Interface Function ------------
public:
    void CreateIndexBuffer(IndexBufferIdentity &io_identity, Size_ui64 i_data_size) override;
    void RefreshStaticIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteIndexBuffer(IndexBufferIdentity &io_identity) override;
    void MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr & io_buffer) override;
    void UnmapIndexBuffer(const IndexBufferIdentity &i_identity) override;
public:
//----------- Uniform Buffer Interface Function ------------
    void CreateUniformBuffer(UniformBufferIdentity &io_identity) override;
    void MapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub, VoidPtr &io_buffer_handle) override;
    void UnmapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub) override;
    void DeleteUnifromBuffer(UniformBufferIdentity &io_identity) override;
public:
    void CreateTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) override;
    void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum &i_dst_layout = ImageLayout_MAX_DEFINE_VALUE) override;
    void DeleteTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) override;
public:
    void BindVertexBuffer(const VertexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, uint32_t i_binding_id, Size_ui64 i_offset) override;
    void BindIndexBuffer(const IndexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset) override;
public:
    void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) override;
    void DeleteShaderModule(ShaderModuleIdentity &io_identity) override;
public:
    void CreateGraphicsPipeline(GraphicsPipelineIdentity &io_identity, const ShaderModules &i_shaders, const RenderPassWeakReferenceObject &i_rp, const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_dsls) override;
    void BindGraphicsPipeline(const GraphicsPipelineIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, const std::vector<DescriptorSetWeakReferenceObject> &i_dss) override;
    void DestroyGraphicsPipeline(GraphicsPipelineIdentity &io_identity) override;
public:
    void CreateRenderPass(RenderPassIdentity &io_identity) override;
    void BeginRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, const RenderPassWeakReferenceObject &i_rp, const ImageOffset &i_start_pos, const ImageSize &i_render_size) override;
    void GoToNextStepOfRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, uint32_t i_sp_id) override;
    void EndRenderPass(const CommandBufferWeakReferenceObject &i_cb) override;
    void DestroyRenderPass(RenderPassIdentity &io_identity) override;
public:
    void CreateFrameBuffer(FrameBufferIdentity &io_identity, const RenderPassWeakReferenceObject &i_rp, const std::vector<TextureWeakReferenceObject> &i_bufs) override;
    void CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity) override;
    void DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity) override;
    void DestroyFrameBuffer(FrameBufferIdentity &io_identity) override;
public:
    void SetViewport(const CommandBufferWeakReferenceObject &i_cb, const Viewport &i_vp) override;
    void SetViewports(const CommandBufferWeakReferenceObject &i_cb, const std::vector<Viewport> &i_vps) override;
    void SetScissor(const CommandBufferWeakReferenceObject &i_cb, const ScissorRegion &i_region) override;
    void SetScissors(const CommandBufferWeakReferenceObject &i_cb, const std::vector<ScissorRegion> &i_regions) override;
    void DrawByIndices(const CommandBufferWeakReferenceObject &i_cb, const IndexBufferWeakReferenceObject &i_ib, uint32_t i_first_id, int32_t i_offset, uint32_t i_first_ins_id, uint32_t i_ins_number) override;
public:
    void CreateQueue(GraphicsQueueIdentity &io_identity) override;
    void SubmitCommandBuffersToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsFenceWeakReferenceObject &i_fence, const std::vector<CommandBufferWeakReferenceObject> &i_cbs) override;
    void PresentToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsSwapchainIdentity &i_sw_identity, uint32_t i_img_id, const std::vector<GraphicsSemaphoreWeakReferenceObject> &i_semas) override;
    void DestroyQueue(GraphicsQueueIdentity &io_identity) override;
public:
    void CreateFence(GraphicsFenceIdentity &io_identity) override;
    void ResetFence(const GraphicsFenceIdentity &i_identity) override;
    void DestroyFence(GraphicsFenceIdentity &io_identity) override;
public:
    void CreateSemaphoreObject(GraphicsSemaphoreIdentity &io_identity) override;
    void DestroySemaphoreObject(GraphicsSemaphoreIdentity &io_identity) override;
public:
    void CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) override;
    void RenderTextureToSwapchain(const GraphicsSwapchainIdentity &i_identity, const GraphicsQueueWeakReferenceObject &i_queue, const CommandBufferWeakReferenceObject &i_cmd_buffer, const GraphicsSemaphoreWeakReferenceObject &i_acq_sema, const GraphicsSemaphoreWeakReferenceObject &i_present_sema, const TextureWeakReferenceObject &i_texture) override;
    void DestroyGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) override;
public:
    Resolution GetScreenResolution() const override;
public:
    void Resize(CompHandle i_new_surface, Size_ui32 i_w, Size_ui32 i_h) override;
public:
    void RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex) override;
protected:
//------- Vulkan descriptor set and pool private Function ------
    VkResult CreateVkDescriptorSetLayout(
        VkDescriptorSetLayout &io_ds_layout,
        VkDevice i_device,
        const VkDescriptorSetLayoutCreateInfo &i_c_info);

    void DestroyVkDescriptorSetLayout(
        VkDescriptorSetLayout &io_layout,
        VkDevice i_device);

    VkResult CreateVkDescriptorPool(
        VkDescriptorPool &io_pool,
        VkDevice i_device,
        const VkDescriptorPoolCreateInfo &i_c_info);

    void DestroyVkDescriptorPool(
        VkDescriptorPool &io_pool,
        VkDevice i_device);

    VkResult AllocateVkDescriptorSet(
        VkDescriptorSet & io_set, 
        VkDevice i_device,
        const VkDescriptorSetAllocateInfo & i_info);

    void UpdateVkDescriptorSet(
        VkDevice i_device,
        const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos,
        const std::vector<VkCopyDescriptorSet> &i_descriptor_c_infos);

    void BindVkDescriptorSets(
        VkCommandBuffer i_cmd_buffer,
        VkPipelineBindPoint i_pipe_bind_point,
        VkPipelineLayout i_pipe_layout,
        const std::vector<VkDescriptorSet> &i_sets,
        const std::vector<uint32_t> &i_dynamic_offsets = {});

    void FreeVkDescriptorSet(
        VkDescriptorSet &io_set, 
        VkDevice i_device,
        VkDescriptorPool i_pool);
protected:
//------- Vulkan command buffer and pool private Function --------
    VkResult CreateVkCommandPool(
        VkCommandPool &io_cmd_pool,
        VkDevice i_device,
        VkCommandPoolCreateFlags i_flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    void DestroyVkCommandPool(
        VkCommandPool &io_cmd_pool,
        VkDevice i_device);

    VkResult AllocateVkCommandBuffer(
        VkCommandBuffer &io_cmd_buffer, 
        VkDevice i_device,
        VkCommandPool i_cmd_pool,
        VkCommandBufferLevel i_level = VK_COMMAND_BUFFER_LEVEL_MAX_ENUM);

    void FreeVkCommandBuffer(
        VkCommandBuffer &io_cmd_buffer,
        VkDevice i_device,
        VkCommandPool i_cmd_pool);

    VkResult BeginVkCommandBuffer(
        VkCommandBuffer i_cmd_buffer,
        const VkCommandBufferBeginInfo &i_info);

    VkResult EndVkCommandBuffer(VkCommandBuffer i_cmd_buffer);

    void ExecuteVkSecondaryCommandBuffersToPrimaryVkCommandBuffer(
        VkCommandBuffer i_primary_cmd_buffer,
        const std::vector<VkCommandBuffer> &i_second_cmd_buffers);
protected:
//----------- Vulkan buffer private Function ------------
    VkResult CreateVkBuffer(
        VkBuffer &io_buffer,
        VkDevice i_device,
        const VkBufferCreateInfo & i_info);

    VkResult PrepareCopyVkBufferCommand(
        VkCommandBuffer i_cmd_buffer,
        VkBuffer i_src_buffer,
        VkDeviceSize i_data_size,
        VkBuffer i_dst_buffer,
        VkAccessFlags i_src_access_flags = 0,
        VkAccessFlags i_dst_access_flags = 0,
        VkPipelineStageFlags i_src_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VkPipelineStageFlags i_dst_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

    VkResult RefreshDataToHostVisibleVKDeviceMemory(
        VkDeviceMemory i_memory,
        VkDevice i_device,
        VkDeviceSize i_allocated_size,
        const void *i_data_ptr,
        Size_ui64 i_data_size);

    void BindVkVertexBuffer(
        VkCommandBuffer i_cmd_buffer,
        VkBuffer i_buffer,
        uint32_t i_binding_id,
        VkDeviceSize i_offset);

    void BindVkIndexBuffer(
        VkCommandBuffer i_cmd_buffer,
        VkBuffer i_buffer,
        VkDeviceSize i_offset,
        VkIndexType i_type);

    void DestroyVkBuffer(
        VkBuffer &io_buffer,
        VkDevice i_device);
protected:
    VkResult CreateVkFence(
        VkFence &io_fence,
        VkDevice i_device,
        const VkFenceCreateInfo &i_info);
    
    VkResult ResetVkFence(
        VkFence i_fence,
        VkDevice i_device);

    void DestroyVkFence(
        VkFence &io_fence,
        VkDevice i_device);

    VkResult CreateVkSemaphore(
        VkSemaphore &io_semaphore,
        VkDevice i_device,
        const VkSemaphoreCreateInfo &i_info);

    void DestroyVkSemaphore(
        VkSemaphore &io_semaphore,
        VkDevice i_device);
protected:
//----------- Vulkan image private Function -------------
    VkResult CreateVkImage(
        VkImage &io_image,
        VkDevice i_device,
        const VkImageCreateInfo &i_info);

    void GetVkImageMemoryRequirement(
        VkMemoryRequirements &io_mem_req,
        VkDevice i_device,
        VkImage i_image);

    VkResult PrepareCopyVkBufferToVkImage(
        VkCommandBuffer i_cmd_buffer,
        VkBuffer i_src_buffer,
        VkDeviceSize i_data_size,
        VkImage i_dst_image,
        VkOffset3D i_image_offset,
        VkExtent3D i_image_size,
        VkAccessFlags i_src_access_flags = 0,
        VkAccessFlags i_dst_access_flags = 0,
        VkImageLayout i_dst_image_original_layout = VK_IMAGE_LAYOUT_UNDEFINED,
        VkImageLayout i_dst_image_final_layout = VK_IMAGE_LAYOUT_UNDEFINED,
        VkPipelineStageFlags i_src_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VkPipelineStageFlags i_dst_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

    void DestroyVkImage(
        VkImage &io_image,
        VkDevice i_device);

protected:
//----------- Vulkan sampler private Function -------------
    VkResult CreateVkSampler(
        VkSampler &io_sampler,
        VkDevice i_device,
        const VkSamplerCreateInfo &i_info);

    void DestroyVkSampler(
        VkSampler &io_sampler,
        VkDevice i_device);
protected:
//----------- Vulkan memory private Function ------------
    VkResult AllocatVkDeviceMemoryForVkBuffer(
        VkDeviceMemory &io_memory,
        VkDeviceSize &io_allocated_size,
        VkDevice i_device,
        VkBuffer i_buffer, 
        VkDeviceSize i_mem_offset,
        VkFlags i_memo_prop_flags);

    VkResult AllocateVkDeviceMemoryForVkImage(
        VkDeviceMemory &io_memory,
        VkDevice i_device,
        VkImage i_image,
        VkDeviceSize i_mem_offset,
        const VkMemoryAllocateInfo &i_info);

    VkResult MapVkDeviceMemory(
        VkDeviceMemory i_memory,
        VkDevice i_device,
        VkDeviceSize i_allocated_size,
        VoidPtr &i_local_ptr);

    VkResult FlushMappedVkDeviceMemoryRanges(
        VkDeviceMemory i_memory,
        VkDevice i_device);

    void UnmapVkDeviceMemory(
        VkDeviceMemory i_memory,
        VkDevice i_device);

    void FreeVkDeviceMemory(
        VkDeviceMemory &io_memory,
        VkDevice i_device);
protected:
    VkResult CreateVKShaderModule(
        VkShaderModule &io_module,
        VkDevice i_device,
        const UByte *i_binary_ptr,
        const Size_ui64 i_binary_size);

    void DestroyVKShaderModule(
        VkShaderModule &io_module,
        VkDevice i_device);

    VkResult CreateVKDescriptorSetLayout(
        VkDescriptorSetLayout &io_layout,
        VkDevice i_device,
        const VkDescriptorSetLayoutCreateInfo &i_info);

    void DestroyVKDescriptorSetLayout(
        VkDescriptorSetLayout &io_layout,
        VkDevice i_device);

    VkResult CreateVKPipelineLayout(
        VkPipelineLayout &io_layout,
        VkDevice i_device,
        const VkPipelineLayoutCreateInfo &i_info);

    void DestroyVKPipelineLayout(
        VkPipelineLayout &io_layout,
        VkDevice i_device);

    VkResult CreateVKPipeline(
        VkPipeline &io_pipeline,
        VkDevice i_device,
        const VkGraphicsPipelineCreateInfo &i_info);

    void BindVkPipeline(
        VkCommandBuffer i_cmd_buffer,
        VkPipeline i_pipeline,
        VkPipelineBindPoint i_pipe_point);

    void DestroyVKPipeline(
        VkPipeline &io_pipeline,
        VkDevice i_device);
protected:
//----------- Vulkan image layout changing Function ------------
    void SwitchVKImageLayout(
        VkCommandBuffer i_cb_handle,
        VkImage i_image_handle,
        uint32_t i_sub_src_range_aspect_mask,
        uint32_t i_sub_src_range_base_mip_lv,
        uint32_t i_sub_src_range_mip_lv_count,
        uint32_t i_sub_src_range_base_layer_lv,
        uint32_t i_sub_src_range_layer_lv_count,
        VkImageLayout i_old_layout,
        VkImageLayout i_new_layout,
        VkPipelineStageFlags i_src_pipeline_stage,
        VkPipelineStageFlags i_dst_pipeline_stage,
        VkAccessFlags i_src_access_mask = 0,
        VkAccessFlags i_dst_access_mask = 0,
        uint32_t i_src_queue_family_id = VK_QUEUE_FAMILY_IGNORED,
        uint32_t i_dst_queue_family_id = VK_QUEUE_FAMILY_IGNORED);

    void SwitchVKBufferLayout(
        VkCommandBuffer i_cb_handle,
        VkBuffer i_buffer_handle,
        VkDeviceSize i_offset,
        VkDeviceSize i_size,
        VkPipelineStageFlags i_src_pipeline_stage,
        VkPipelineStageFlags i_dst_pipeline_stage,
        VkAccessFlags i_src_access_mask = 0,
        VkAccessFlags i_dst_access_mask = 0,
        uint32_t i_src_queue_family_id = VK_QUEUE_FAMILY_IGNORED,
        uint32_t i_dst_queue_family_id = VK_QUEUE_FAMILY_IGNORED);
protected:
//----------- Vulkan RenderPass Function ------------
    VkResult CreateVKRenderPass(
        VkRenderPass &io_render_pass,
        VkDevice i_device,
        const VkRenderPassCreateInfo &i_info);

    void BeginVkRenderPass(
        VkCommandBuffer i_cmd_buffer,
        VkRenderPass i_render_pass,
        VkFramebuffer i_framebuffer,
        const VkRect2D &i_render_area,
        const std::vector<VkClearValue> &i_clear_values,
        VkSubpassContents i_sp_contents = VK_SUBPASS_CONTENTS_INLINE);

    void GotoNextStepInVKRenderPass(
        VkCommandBuffer i_cmd_buffer,
        VkSubpassContents i_sp_content = VK_SUBPASS_CONTENTS_INLINE);

    void EndVkRenderPass(VkCommandBuffer i_cmd_buffer);

    void DestroyVKRenderPass(
        VkRenderPass &io_render_pass,
        VkDevice i_device);
protected:
//----------- Vulkan ImageView Function ------------
    VkResult CreateVkImageView(
        VkImageView &io_image_view,
        VkDevice i_device,
        const VkImageViewCreateInfo &i_info);

    void DestroyVkImageView(
        VkImageView &io_image_view,
        VkDevice i_device);
protected:
//----------- Vulkan FrameBuffer Function ------------
    VkResult CreateVKFrameBuffer(
        VkFramebuffer &io_framebuffer,
        VkDevice i_device,
        const VkRenderPass i_render_pass,
        const std::vector<VkImageView> &i_img_views,
        Size_ui32 i_width,
        Size_ui32 i_height,
        Size_ui32 i_layers = 1);

    void DestroyVkFrameBuffer(
        VkFramebuffer &io_framebuffer,
        VkDevice i_device);
protected:
    void SetVkViewport(
        VkCommandBuffer i_cmd_buffer,
        const VkViewport &i_viewport);

    void SetVkViewports(
        VkCommandBuffer i_cmd_buffer,
        const std::vector<VkViewport> &i_viewports);

    void SetVkScissor(
        VkCommandBuffer i_cmd_buffer,
        const VkRect2D &i_rect);

    void SetVkScissors(
        VkCommandBuffer i_cmd_buffer,
        const std::vector<VkRect2D> &i_rects);

    void DrawByVkIndexBuffer(
        VkCommandBuffer i_cmd_buffer,
        uint32_t i_indice_size,
        uint32_t i_instance_count,
        uint32_t i_first_index,
        int32_t i_vertex_offset,
        uint32_t i_first_instance);
protected:
    void GetVkQueue(
        VkDevice i_device,
        uint32_t i_q_fam_id,
        uint32_t i_q_id,
        VkQueue &io_queue);

    VkResult SubmitVkCommandBuffersToQueue(
        VkDevice i_device,
        VkQueue i_queue,
        VkFence i_fence,
        const std::vector<VkCommandBuffer> &i_cbs);

    VkResult PresentVkSwapchainToVkQueue(
        VkQueue i_queue,
        const VkPresentInfoKHR &i_info);
protected:
    VkResult CreateVkSwapchain(
        VkSwapchainKHR &io_swapchain,
        VkDevice i_device,
        const VkSwapchainCreateInfoKHR &i_info);

    VkResult CollectVkImageForVkSwapchain(
        std::vector<VkImage> &io_iamges,
        VkDevice i_device,
        VkSwapchainKHR i_swapchain);

    VkResult GetCurrentVkSwapchainIdx(
        uint32_t &io_image_idx,
        VkDevice i_device,
        VkSwapchainKHR i_swapchain,
        VkSemaphore i_wait_sema);

    void BlitVkImages(
        VkCommandBuffer i_cmd_buffer,
        VkImage i_src_image,
        VkImage i_dst_image,
        const VkImageBlit &i_param,
        VkFilter i_filter = VK_FILTER_NEAREST);

    void DestroyVkSwapchain(
        VkSwapchainKHR &io_swapchain,
        VkDevice i_device);
public:
    bool IsContainerNecessaryQueueFlags(VkQueueFlags i_flags);
    int32_t GetSuitableMemoryTypeID(const VkMemoryRequirements &i_mem_req, VkFlags i_mem_prop_flags);
protected:
//-------------------- Vulkan Builder -------------------
    void InitializeVulkanEnvironment();
    void InitializeVulkanSurface();
    void InitializeSettings();
    void InitializeGraphicsQueues();
    void InitializeSwapChain();
protected:
    //configuration
    std::vector<VkQueueFlagBits> m_desired_queue_abilities;
    VulkanConfig m_vulkan_config;
protected:
    //Application Create.
    VkInstance m_instance;
    VkPhysicalDevice m_phy_device;
    VkDebugReportCallbackEXT m_debug_rp_cbk;
    VkPhysicalDeviceMemoryProperties m_phy_dev_memory_props;
    VkPhysicalDeviceFeatures m_phy_dev_features;
    VkPhysicalDeviceProperties m_picked_dev_props;
    VkDevice m_device;
    VkSurfaceKHR m_surface;
    VkSurfaceFormatKHR m_final_sur_format;
    VkSurfaceCapabilitiesKHR m_surface_capabilities;
    VkPresentModeKHR m_final_p_mode;
    VulkanQueueFamily m_queue_family;
};

______________SD_END_GRAPHICS_NAMESPACE______________