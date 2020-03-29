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

#include <vulkan/vulkan.h>
#include "GraphicsManager.h"

using SDE::Basic::UBytePtr;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class VulkanManager
 *  \brief In our system, VulkanManager is a integration for Vulkan graphics API.
 */
class SDENGINE_CLASS VulkanManager : public GraphicsManager
{
protected:
    static const uint32_t MaxImgAcqirationTime; //nanosecond.
    static const uint32_t MaxFenceWaitTime;
    static const VkClearValue ClearColor;
    static const VkClearValue ClearDepth;
protected:
    static std::vector<const char*> DesiredValidLayers;
    static std::vector<const char*> NecessaryExtensions;
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
public:
    void CreateCommandPool(CommandPoolIdentity &io_identity);
    void DestroyCommandPool(CommandPoolIdentity &io_identity);
    void AllocateCommandBuffer(CommandBufferIdentity &io_identity);
    void BeginCommandBuffer(CommandBufferIdentity &io_identity);
    void EndCommandBuffer(CommandBufferIdentity &io_identity);
    void FreeCommandBuffer(CommandBufferIdentity &io_identity);
public:
//----------- Vertex Buffer Interface Function ------------
    /*
     * We will set i_data_size to VertexBufferIdentity::m_buffer_size and assign allocated memory size to m_memory_size
     */
    void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type) override;
    void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteVertexBuffer(VertexBufferIdentity &io_identity) override;
    void MapBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) override;
    void UnmapBuffer(const VertexBufferIdentity &i_identity) override;
public:
    void CreateTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) override;
    void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum &i_dst_layout = ImageLayout_MAX_DEFINE_VALUE) override;
    void DeleteTextureImage(TextureIdentity &io_identity) override;
public:
    void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) override;
    void DeleteShaderModule(ShaderModuleIdentity &io_identity) override;
public:
    void CreateRenderPass(RenderPassIdentity &io_identity) override;
    void BeginRenderPass(const CompHandle i_cmd_buffer_handle, const FrameBufferIdentity &i_fb_identity, const ImageOffset &i_start_pos, const ImageSize &i_render_size, const std::vector<ClearValue> &i_clear_values) override;
    void GoToNextStepOfRenderPass(const CompHandle i_cmd_buffer_handle, const FrameBufferGroupIdentity &i_target_fbg_identity) override;
    void EndRenderPass(const CompHandle i_cmd_buffer_handle) override;
    void DestroyRenderPass(RenderPassIdentity &io_identity) override;
public:
    void CreateFrameBuffer(FrameBufferIdentity &io_identity, const std::vector<TextureWeakReferenceObject>& i_buf_wrefs) override;
    void CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity) override;
    void DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity) override;
    void DestroyFrameBuffer(FrameBufferIdentity &io_identity) override;
public:
    void Resize(CompHandle i_new_surface, Size_ui32 i_w, Size_ui32 i_h) override;
protected:
//------- Vulkan command buffer private Function --------
    VkResult CreateVkCommandPool(
        VkCommandPool &io_pool_handle,
        VkCommandPoolCreateFlags i_flag = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

    VkResult AllocateVkCommandBuffer(
        VkCommandBuffer &io_cmd_handle,
        VkCommandPool i_pool_handle,
        VkCommandBufferLevel i_level = VK_COMMAND_BUFFER_LEVEL_MAX_ENUM);

protected:
//----------- Vulkan buffer private Function ------------
    VkResult CreateVkBuffer(
        VkBuffer &io_buffer_handle, 
        VkDeviceSize i_size, 
        VkBufferUsageFlags i_buffer_usage,
        VkSharingMode i_sharing_mode = VK_SHARING_MODE_EXCLUSIVE);

    VkResult CopyVkBuffer(
        VkBuffer i_src_buffer_handle,
        VkDeviceSize i_data_size,
        VkBuffer i_dst_buffer_handle,
        VkAccessFlags i_src_access_flags = 0,
        VkAccessFlags i_dst_access_flags = 0,
        VkPipelineStageFlags i_src_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VkPipelineStageFlags i_dst_pipe_stage_flags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

    VkResult RefreshDataToHostVisibleVKDeviceMemory(
        VkDeviceMemory i_memory_handle,
        VkDeviceSize i_mem_allocated_size,
        VoidPtr i_data_ptr,
        Size_ui64 i_data_size);

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
        VkBuffer i_src_buffer_handle,
        VkDeviceSize i_data_size,
        VkImage i_dst_image_handle,
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
        VkSampler &io_sampler_handle,
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
        VkBool32 i_unnormalize_coord = VK_FALSE
    );

    void FreeVkSampler(SamplerIdentity &io_identity);
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
        VkShaderModule &io_shader_module_handle,
        VkShaderStageFlagBits i_stage,
        const UByte *i_binary_ptr,
        const Size_ui64 i_binary_size);

    void DestroyVKShaderModule(VkShaderModule &io_shader_module_handle);
protected:
//----------- Vulkan image layout changing Function ------------
    void SwitchVKImageLayout(
        VkCommandBuffer i_cmd_buffer,
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
        VkCommandBuffer i_cmd_buffer,
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
        const std::vector<VkAttachmentDescription> &i_vk_att_descs,
        const std::vector<VkSubpassDescription> &i_vk_sps,
        const std::vector<VkSubpassDependency> &i_vk_sp_dependencies);

    void BeginVkRenderPass(
        VkCommandBuffer i_cmd_buffer,
        VkRenderPass i_rp_handle,
        VkFramebuffer i_fb_handle,
        const VkRect2D &i_render_area,
        const std::vector<VkClearValue> &i_clear_values,
        VkSubpassContents i_sp_contents = VK_SUBPASS_CONTENTS_INLINE);

    void GotoNextStepInVKRenderPass(VkCommandBuffer i_cmd_buffer, VkSubpassContents i_sp_content = VK_SUBPASS_CONTENTS_INLINE);

    void EndVkRenderPass(VkCommandBuffer i_cmd_buffer);

    void DestroyVKRenderPass(VkRenderPass &io_rp_handle);
public:
//----------- Vulkan FrameBuffer Function ------------
    VkResult CreateVKFrameBuffer(
        VkFramebuffer &io_fb_handle,
        const VkRenderPass i_rp_handle,
        const std::vector<VkImageView> &i_iv_handles,
        Size_ui32 i_width,
        Size_ui32 i_height,
        Size_ui32 i_layers = 1);

    VkResult CreateVkImageView(
        VkImageView &io_iv_handle,
        const VkImage i_img_handle,
        VkImageViewType i_view_type,
        VkFormat i_img_format,
        VkComponentMapping i_comp_swizzle,
        VkImageSubresourceRange i_sub_src_range);

    void DestroyVkImageView(VkImageView &io_iv_handle);

    void DestroyVkFrameBuffer(VkFramebuffer& io_fb_handle);
public:
    bool IsContainerNecessaryQueueFlags(VkQueueFlags i_flags);
protected:
//--------------- Render Flow Function ------------------
    void RenderBegin() override;
    void RenderToScreen() override;
    void RenderEnd() override;
protected:
    void InitializeDebugMessage();
    void InitializePhysicalDevice();
    void InitializeLogicDevice();
    void InitializeSwapChain();
    void InitializePresentRenderPass();
    void InitializeSCImageViewsAndFBs();
    void InitializeCommandPoolAndBuffers();
protected:
    VkInstance m_VK_instance;
    VkSurfaceKHR m_VK_surface;
protected:
    VkDebugReportCallbackEXT m_VK_debug_report_cbk;
protected:
    std::vector<VkQueueFlagBits> m_VK_desired_queue_ability_lists;
    VkQueueFlags m_VK_desired_queue_abilities;
    std::vector<VkSurfaceFormatKHR> m_VK_desired_sur_fmts;
    VkSurfaceFormatKHR m_VK_final_sur_fmt;
    std::vector<VkPresentModeKHR> m_VK_desired_pre_mode_list;
protected:
    VkPhysicalDevice m_VK_physical_device;
    VkDevice m_VK_device;
    int32_t m_VK_picked_queue_family_id;
    VkQueue m_VK_present_queue;
protected:
    VkPresentModeKHR m_VK_final_present_mode;
    VkSwapchainKHR m_VK_swap_chain;
    VkRenderPass m_VK_present_render_pass;
    VkSemaphore m_VK_acq_img_semaphore; //GPU to GPU lock
    VkSemaphore m_VK_present_semaphore; //GPU to GPU lock
    std::vector<VkImage> m_VK_sc_images;
    std::vector<VkImageView> m_VK_sc_image_views;
    std::vector<VkFramebuffer> m_VK_sc_image_fbs;
protected:
    VkCommandPool m_VK_main_cmd_pool; //main render thread use.
    VkCommandBuffer m_VK_main_cmd_buffer;
    VkFence m_VK_main_cmd_buf_fence;
protected:
 //--------------- Configuration ------------------
    uint32_t m_queue_size; //use to allocate queues for multi-threads.
};

______________SD_END_GRAPHICS_NAMESPACE______________