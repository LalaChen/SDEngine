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
    void CreateTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity, VoidPtr i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size) override;
    void DeleteTextureImage(TextureIdentity &io_identity) override;
public:
    void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) override;
    void DeleteShaderModule(ShaderModuleIdentity &io_identity) override;
public:
    void Resize(Size_ui32 i_w, Size_ui32 i_h) override;
protected:
//----------- Vulkan buffer private Function ------------
    VkResult CreateVkBuffer(
        VkBuffer &io_buffer_handle, 
        VkDeviceSize i_size, 
        VkBufferUsageFlags i_buffer_usage,
        VkSharingMode i_sharing_mode);

    VkResult CopyVkBuffer(
        VkBuffer i_src_buffer_handle,
        VkAccessFlags i_src_access_flags,
        VkPipelineStageFlags i_src_pipe_stage_flags, 
        VkBuffer i_dst_buffer_handle, 
        VkAccessFlags i_dst_access_flags,
        VkPipelineStageFlags i_dst_pipe_stage_flags,
        VkDeviceSize i_data_size);

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
        VkSharingMode i_sharing_mode,
        VkImageType i_type,
        VkImageUsageFlags i_usage_flags,
        uint32_t i_mipmap_levels,
        uint32_t i_array_layers,
        VkImageTiling i_tiling_mode,
        VkExtent3D i_image_size,
        VkFormat i_image_format);

    VkResult CopyVkBufferToVkImage(
        VkBuffer i_src_buffer_handle,
        VkAccessFlags i_src_access_flags,
        VkPipelineStageFlags i_src_pipe_stage_flags,
        VkImage i_dst_image_handle,
        VkAccessFlags i_dst_access_flags,
        VkPipelineStageFlags i_dst_pipe_stage_flags,
        VkOffset3D i_image_offset,
        VkExtent3D i_image_size,
        VkDeviceSize i_data_size);

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
        VkSampler &io_sampler,
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

    VkResult AllocateVkDeviceMemortForVkImage(
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
    VkQueueFlags m_VK_desired_queue_abilities;
    VkSurfaceFormatKHR m_VK_desired_sur_fmt;
    std::vector<VkPresentModeKHR> m_VK_desired_pre_mode_list;
protected:
    VkInstance m_VK_instance;
    VkSurfaceKHR m_VK_surface;
protected:
    VkDebugReportCallbackEXT m_VK_debug_report_cbk;
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
    VkSemaphore m_VK_render_scene_semaphore;
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