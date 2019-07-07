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

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class VulkanManager
 *  In our system, VulkanManager is a integration for Vulkan graphics API.
 */
class SDENGINE_CLASS VulkanManager : public GraphicsManager
{
protected:
    static const uint32_t MaxImgAcqirationTime; //nanosecond.
    static const uint32_t MaxFenceWaitTime;
    static const VkClearValue ClearColor;
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
//----------- Vertex Buffer Interface Function ------------
    void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type) override;
    void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteVertexBuffer(VertexBufferIdentity &io_identity) override;
public:
    void Resize(int i_w, int i_h) override;
protected:
//----------- Vulkan buffer private Function ------------
    VkResult CreateVkBuffer(VkBufferUsageFlags i_buffer_usage, VkSharingMode i_sharing_mode, VkDeviceSize i_size, VkBuffer &io_buffer_handle);
    VkResult AllocatVkDeviceMemoryForBuffer(VkFlags i_memo_prop_flags, VkDeviceSize i_mem_offset, VkBuffer i_buffer_handle, VkDeviceMemory &io_memory_handle);
    VkResult RefreshDataInHostVisibleVkBuffer(VkBuffer i_buffer_handle, VkDeviceMemory i_memory_handle, void *i_data_ptr, Size_ui64 i_data_size);
    VkResult CopyDataToStaticVkBuffer(
        VkBuffer i_src_buffer_handle, VkAccessFlags i_src_access_flags, VkPipelineStageFlags i_src_pipe_stage_flags, 
        VkBuffer i_dst_buffer_handle, VkAccessFlags i_dst_access_flags, VkPipelineStageFlags i_dst_pipe_stage_flags,
        VkDeviceSize i_data_size);
    void FreeVkDeviceMemory(VkDeviceMemory i_memory_handle);
    void DestroyVkBuffer(VkBuffer i_buffer_handle);
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
    std::vector<VkImage> m_VK_sc_images;
    std::vector<VkImageView> m_VK_sc_image_views;
    std::vector<VkFramebuffer> m_VK_sc_image_fbs;
protected:
    VkCommandPool m_VK_main_cmd_pool; //main render thread use.
    VkCommandBuffer m_VK_main_cmd_buffer;
    VkFence m_VK_main_cmd_buf_fence;
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}