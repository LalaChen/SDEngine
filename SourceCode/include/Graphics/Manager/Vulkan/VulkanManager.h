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

#include "VulkanWrapper.h"
#include "VulkanStructureInitializer.h"
#include "GraphicsManager.h"

using SDE::Basic::UBytePtr;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class VulkanManager
 *  \brief In our system, VulkanManager is a integration for Vulkan graphics API.
 */
class SDENGINE_CLASS VulkanManager : public GraphicsManager
{
public:
    static VkBool32 ConvertBoolean(bool flag);
protected:
    static const uint32_t sMaxImgAcqirationTime; //nanosecond.
    static const uint32_t sMaxFenceWaitTime;
    static const VkClearValue sClearColor;
    static const VkClearValue sClearDepth;
protected:
    static std::vector<const char*> sDesiredValidLayers;
    static std::vector<const char*> sNecessaryExtensions;
public:
    static const std::vector<const char*>& GetDesiredValidLayers();
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
    void SubmitCommandBuffersToQueue(const std::vector<CommandBufferWeakReferenceObject> &i_cbs) override;
    void SubmitCommandBufferToQueue(const CommandBufferWeakReferenceObject &i_cb) override;
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
    void MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) override;
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
    void Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h) override;
public:
    void RenderTexture2DToScreen(const TextureWeakReferenceObject &i_tex) override;
protected:
//------- Vulkan descriptor set and pool private Function ------
    VkResult CreateVkDescriptorSetLayout(
        VkDescriptorSetLayout &io_handle,
        const VkDescriptorSetLayoutCreateInfo &i_c_info);

    void DestroyVkDescriptorSetLayout(VkDescriptorSetLayout &io_handle);

    VkResult CreateVkDescriptorPool(
        VkDescriptorPool &io_handle,
        const VkDescriptorPoolCreateInfo &i_dp_c_info);

    void DestroyVkDescriptorPool(VkDescriptorPool &io_handle);

    VkResult AllocateVkDescriptorSet(
        VkDescriptorSet &io_handle,
        const VkDescriptorSetAllocateInfo &i_a_info);

    void UpdateVkDescriptorSet(
        const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos,
        const std::vector<VkCopyDescriptorSet> &i_descriptor_c_infos);

    void BindVkDescriptorSets(
        VkCommandBuffer i_cb_handle,
        VkPipelineBindPoint i_pipe_bind_point,
        VkPipelineLayout i_pipe_layout_handle,
        const std::vector<VkDescriptorSet> &i_ds_handles,
        const std::vector<uint32_t> &i_dynamic_offsets = {});

    void FreeVkDescriptorSet(
        VkDescriptorSet &io_handle,
        VkDescriptorPool i_dp_handle);
protected:
//------- Vulkan command buffer and pool private Function --------
    VkResult CreateVkCommandPool(
        VkCommandPool &io_pool_handle,
        VkCommandPoolCreateFlags i_flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    void DestroyVkCommandPool(VkCommandPool &io_pool_handle);

    VkResult AllocateVkCommandBuffer(
        VkCommandBuffer &io_cb_handle,
        VkCommandPool i_cp_handle,
        VkCommandBufferLevel i_level = VK_COMMAND_BUFFER_LEVEL_MAX_ENUM);

    void FreeVkCommandBuffer(
        VkCommandBuffer &io_cb_handle,
        VkCommandPool i_cp_handle);

    VkResult BeginVkCommandBuffer(
        VkCommandBuffer i_cb_handle,
        const VkCommandBufferBeginInfo &i_info);

    VkResult EndVkCommandBuffer(VkCommandBuffer i_cb_handle);

    VkResult SubmitVkCommandBuffers(const std::vector<VkCommandBuffer> &i_cb_handles);

    void ExecuteVkSecondaryCommandBuffersToPrimaryVkCommandBuffer(
        VkCommandBuffer i_primary_cb_handle,
        const std::vector<VkCommandBuffer> &i_second_cb_handles);
protected:
//----------- Vulkan buffer private Function ------------
    VkResult CreateVkBuffer(
        VkBuffer &io_handle, 
        VkDeviceSize i_size, 
        VkBufferUsageFlags i_buffer_usage,
        VkSharingMode i_sharing_mode = VK_SHARING_MODE_EXCLUSIVE);

    VkResult CopyVkBuffer(
        VkCommandBuffer i_cb_handle,
        VkBuffer i_sb_handle,
        VkDeviceSize i_data_size,
        VkBuffer i_db_handle,
        VkAccessFlags i_src_access_flags = 0,
        VkAccessFlags i_dst_access_flags = 0,
        VkPipelineStageFlags i_src_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VkPipelineStageFlags i_dst_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

    VkResult RefreshDataToHostVisibleVKDeviceMemory(
        VkDeviceMemory i_memory_handle,
        VkDeviceSize i_allocated_size,
        const void *i_data_ptr,
        Size_ui64 i_data_size);

    void BindVkVertexBuffer(
        VkCommandBuffer i_cb_handle,
        VkBuffer i_vb_handle,
        uint32_t i_binding_id,
        VkDeviceSize i_offset);

    void BindVkIndexBuffer(
        VkCommandBuffer i_cb_handle,
        VkBuffer i_ib_handle,
        VkDeviceSize i_offset,
        VkIndexType i_type);

    void DestroyVkBuffer(VkBuffer &io_buffer_handle);

protected:
//----------- Vulkan image private Function -------------
    VkResult CreateVkImage(
        VkImage &io_image_handle,
        VkImageType i_type,
        VkFormat i_image_format,
        VkExtent3D i_image_size,
        VkImageUsageFlags i_usage_flags,
        VkSampleCountFlagBits i_sample_count = VK_SAMPLE_COUNT_1_BIT,
        VkImageLayout i_image_layout = VK_IMAGE_LAYOUT_UNDEFINED,
        uint32_t i_mipmap_levels = 1,
        uint32_t i_array_layers = 1,
        VkImageTiling i_tiling_mode = VK_IMAGE_TILING_OPTIMAL,
        VkSharingMode i_sharing_mode = VK_SHARING_MODE_EXCLUSIVE);

    VkResult CopyVkBufferToVkImage(
        VkCommandBuffer i_cb_handle,
        VkBuffer i_sb_handle,
        VkDeviceSize i_data_size,
        VkImage i_di_handle,
        VkOffset3D i_image_offset,
        VkExtent3D i_image_size,
        VkAccessFlags i_src_access_flags = 0,
        VkAccessFlags i_dst_access_flags = 0,
        VkImageLayout i_dst_image_original_layout = VK_IMAGE_LAYOUT_UNDEFINED,
        VkImageLayout i_dst_image_final_layout = VK_IMAGE_LAYOUT_UNDEFINED,
        VkPipelineStageFlags i_src_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VkPipelineStageFlags i_dst_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

    void DestroyVkImage(VkImage &io_image_handle);

protected:
//----------- Vulkan sampler private Function -------------
    /*! \fn VkResult CreateVkSampler( VkSampler &io_sampler, VkFilter i_mag_filter_type, VkFilter i_min_filter_type, VkSamplerMipmapMode i_mipmap_mode, VkSamplerAddressMode i_wrap_mode_s, VkSamplerAddressMode i_wrap_mode_t, VkSamplerAddressMode i_wrap_mode_r, float i_mip_lod_bias, VkBool32 i_enable_anisotropy, float i_max_anisotropy, VkBool32 i_enable_compare, VkCompareOp i_compare_op, float i_min_lod, float i_max_lod, VkBorderColor i_border_color, VkBool32 i_unnormalize_coord);
     *  \param [in] io_sampler Created handle. It's used to keep created handle.
     *  \param [in] i_mag_filter_type Mag filter type. Default type is nearset.
     *  \param [in] i_min_filter_type Min filter type. Default type is nearest.
     *  \param [in] i_mipmap_mode Mipmap type. Default is nearest.
     *  \param [in] i_wrap_mode_s Texture coordinate wrap mode. Default is clamp tp edge.
     *  \param [in] i_wrap_mode_t Texture coordinate wrap mode. Default is clamp tp edge.
     *  \param [in] i_wrap_mode_r Texture coordinate wrap mode. Default is clamp tp edge.
     *  \param [in] i_mip_lod_bias Mipmap bias. Default is 0.
     *  \param [in] i_enable_anisotropy Default false.
     *  \param [in] i_max_anisotropy Default 1.0f.
     *  \param [in] i_enable_compare Default false.
     *  \param [in] i_compare_op Default always.
     *  \param [in] i_min_lod Default 0.0f.
     *  \param [in] i_max_lod Default 1.0f.
     *  \param [in] i_border_color Default float opaque black.
     *  \param [in] i_unnormalize_coord Default false.(Normalize coord.)
     *  \brief Initialize graphics API. (link dll, ...)
     */
    VkResult CreateVkSampler(
        VkSampler &io_handle,
        VkFilter i_mag_filter_type = VK_FILTER_NEAREST,
        VkFilter i_min_filter_type = VK_FILTER_NEAREST,
        VkSamplerMipmapMode i_mipmap_mode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        VkSamplerAddressMode i_wrap_mode_s = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        VkSamplerAddressMode i_wrap_mode_t = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        VkSamplerAddressMode i_wrap_mode_r = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        float i_mip_lod_bias = 0.0f,
        VkBool32 i_enable_anisotropy = VK_FALSE,
        float i_max_anisotropy = 1.0f,
        VkBool32 i_enable_compare = VK_FALSE,
        VkCompareOp i_compare_op = VK_COMPARE_OP_ALWAYS,
        float i_min_lod = 0.0f,
        float i_max_lod = 1.0f,
        VkBorderColor i_border_color = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
        VkBool32 i_unnormalize_coord = VK_FALSE);

    void DestroyVkSampler(VkSampler &io_handle);
protected:
//----------- Vulkan memory private Function ------------
    VkResult AllocatVkDeviceMemoryForVkBuffer(
        VkDeviceMemory &io_memory_handle, 
        VkDeviceSize &io_allocated_size,
        VkBuffer i_buffer_handle, 
        VkDeviceSize i_mem_offset, 
        VkFlags i_memo_prop_flags);

    VkResult AllocateVkDeviceMemoryForVkImage(
        VkDeviceMemory &io_memory_handle,
        VkDeviceSize &io_allocated_size,
        VkImage &i_image_handle,
        VkDeviceSize i_mem_offset,
        VkFlags i_memo_prop_flags);

    VkResult MapVkDeviceMemory(
        VkDeviceMemory i_memory_handle,
        VkDeviceSize i_allocated_size, 
        VoidPtr &i_local_ptr);

    VkResult FlushMappedVkDeviceMemoryRanges(VkDeviceMemory i_memory_handle);

    void UnmapVkDeviceMemory(VkDeviceMemory i_memory_handle);

    void FreeVkDeviceMemory(VkDeviceMemory &io_memory_handle);
protected:
    VkResult CreateVKShaderModule(
        VkShaderModule &io_sm_handle,
        const UByte *i_binary_ptr,
        const Size_ui64 i_binary_size);

    void DestroyVKShaderModule(VkShaderModule &io_sm_handle);

    VkResult CreateVKDescriptorSetLayout(
        VkDescriptorSetLayout &io_layout_handle,
        const VkDescriptorSetLayoutCreateInfo &i_c_info);

    void DestroyVKDescriptorSetLayout(VkDescriptorSetLayout &io_layout_handle);

    VkResult CreateVKPipelineLayout(
        VkPipelineLayout &io_layout_handle,
        const VkPipelineLayoutCreateInfo &i_c_info);

    void DestroyVKPipelineLayout(VkPipelineLayout &io_layout_handle);

    VkResult CreateVKPipeline(
        VkPipeline &io_pipeline_handle,
        const VkGraphicsPipelineCreateInfo &i_c_info);

    void BindVkPipeline(
        VkCommandBuffer i_cb_handle,
        VkPipeline i_pipe_handle,
        VkPipelineBindPoint i_pipe_point);

    void DestroyVKPipeline(VkPipeline &io_pipeline_handle);
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
        VkRenderPass &io_rp_handle,
        const VkRenderPassCreateInfo &i_rp_c_info);

    void BeginVkRenderPass(
        VkCommandBuffer i_cb_handle,
        VkRenderPass i_rp_handle,
        VkFramebuffer i_fb_handle,
        const VkRect2D &i_render_area,
        const std::vector<VkClearValue> &i_clear_values,
        VkSubpassContents i_sp_contents = VK_SUBPASS_CONTENTS_INLINE);

    void GotoNextStepInVKRenderPass(
        VkCommandBuffer i_cb_handle,
        VkSubpassContents i_sp_content = VK_SUBPASS_CONTENTS_INLINE);

    void EndVkRenderPass(VkCommandBuffer i_cb_handle);

    void DestroyVKRenderPass(VkRenderPass &io_rp_handle);
public:
//----------- Vulkan ImageView Function ------------
    VkResult CreateVkImageView(
        VkImageView &io_iv_handle,
        const VkImage i_img_handle,
        VkImageViewType i_view_type,
        VkFormat i_img_format,
        VkComponentMapping i_comp_swizzle,
        VkImageSubresourceRange i_sub_src_range);

    void DestroyVkImageView(VkImageView &io_iv_handle);
public:
//----------- Vulkan FrameBuffer Function ------------
    VkResult CreateVKFrameBuffer(
        VkFramebuffer &io_fb_handle,
        const VkRenderPass i_rp_handle,
        const std::vector<VkImageView> &i_iv_handles,
        Size_ui32 i_width,
        Size_ui32 i_height,
        Size_ui32 i_layers = 1);

    void DestroyVkFrameBuffer(VkFramebuffer &io_fb_handle);
public:
    void SetVkViewport(
        VkCommandBuffer i_cb_handle,
        const VkViewport &i_viewport);

    void SetVkViewports(
        VkCommandBuffer i_cb_handle,
        const std::vector<VkViewport> &i_viewports);

    void SetVkScissor(
        VkCommandBuffer i_cb_handle,
        const VkRect2D &i_rect);

    void SetVkScissors(
        VkCommandBuffer i_cb_handle,
        const std::vector<VkRect2D>& i_rects);

    void DrawByVkIndexBuffer(
        VkCommandBuffer i_cb_handle,
        uint32_t i_indice_size,
        uint32_t i_instance_count,
        uint32_t i_first_index,
        int32_t i_vertex_offset,
        uint32_t i_first_instance);
public:
    bool IsContainerNecessaryQueueFlags(VkQueueFlags i_flags);
protected:
//--------------- Render Flow Function ------------------
    void RenderBegin() override;
    void RenderToScreen() override;
    void RenderEnd() override;
protected:
//-------------------- Vulkan Builder -------------------
    void InitializeDebugMessage();
    void InitializePhysicalDevice();
    void InitializeSettings();
    void InitializeDevice();
    void InitializeSwapChain();
    void InitializePresentRenderPass();
    void InitializeSCImageViewsAndFBs();
    void InitializeCommandPoolAndBuffers();
protected:
    //configuration
    std::vector<VkQueueFlagBits> m_desired_queue_abilities;
    std::vector<VkSurfaceFormatKHR> m_desired_sur_formats;
    std::vector<VkPresentModeKHR> m_desired_pre_modes;
protected:
    //Application Create.
    VkInstance m_ins_handle;
    VkSurfaceKHR m_sur_handle;
protected:
    VkDebugReportCallbackEXT m_debug_rp_cbk;
protected:
    VkQueueFlags m_final_q_abi_flag;
    VkSurfaceFormatKHR m_final_sur_format;
protected:
    VkPhysicalDevice m_phy_device_handle;
    VkDevice m_device_handle;
    int32_t m_final_queue_fam_id;
    VkQueue m_present_q_handle;
protected:
    VkPresentModeKHR m_final_p_mode;
    VkSwapchainKHR m_sc_handle;
    VkSemaphore m_acq_img_sema_handle; //GPU to GPU lock
    VkSemaphore m_pre_sema_handle; //GPU to GPU lock
    VkRenderPass m_pre_rp_handle;
    std::vector<VkImage> m_sc_img_handles;
    std::vector<VkImageView> m_sc_iv_handles;
    std::vector<VkFramebuffer> m_sc_fb_handles;
protected:
    VkCommandPool m_main_cp_handle; //main render thread use.
    VkCommandBuffer m_main_cb_handle;
    VkFence m_main_cb_fence_handle;
};

______________SD_END_GRAPHICS_NAMESPACE______________