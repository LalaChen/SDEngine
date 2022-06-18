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

#define NOMINMAX //For avoid max and min redefinition in Windows.h

#include "VulkanSwapchain.h"

#include <list>
#include <algorithm>

#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VulkanSwapchain::VulkanSwapchain(const ObjectName &i_name, const GraphicsIdentityGetterWeakReferenceObject &i_id_getter)
: Object(i_name)
, m_swapchain(VK_NULL_HANDLE)
, m_device(VK_NULL_HANDLE)
, m_queue(VK_NULL_HANDLE)
, m_max_img_acq_time(0) //2s
, m_max_fence_wait_time(0) //17ms
, m_id_getter(i_id_getter)
{
}

VulkanSwapchain::~VulkanSwapchain()
{
    SD_SREF(m_pool).RecycleCommandBuffer(m_cmd_buffer);
    m_pool.Reset();

    if (m_wait_img_sema != VK_NULL_HANDLE) {
        vkDestroySemaphore(m_device, m_wait_img_sema, nullptr);
        m_wait_img_sema = VK_NULL_HANDLE;
    }

    if (m_present_sema != VK_NULL_HANDLE) {
        vkDestroySemaphore(m_device, m_present_sema, nullptr);
        m_present_sema = VK_NULL_HANDLE;
    }

    if (m_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        m_swapchain = VK_NULL_HANDLE;
    }
}

void VulkanSwapchain::Initialize(
    const VulkanConfig &i_config,
    VkPhysicalDevice i_phy_device,
    VkDevice i_device,
    VkSurfaceKHR i_surface,
    uint32_t i_present_q_fam_id,
    VkQueue i_present_queue)
{
    //1.
    InitializeVKSwapchain(i_config,
        i_phy_device, i_device, i_surface,
        i_present_q_fam_id, i_present_queue);
    //2. create command pool.
    m_pool = new CommandPool("VulkanSwapchainCmdPool");
    SD_SREF(m_pool).Initialize();
    m_cmd_buffer = SD_SREF(m_pool).AllocateCommandBuffer("VulkanSwapchainCmdBuffer");
}

void VulkanSwapchain::RenderTexture2DToScreen(const TextureIdentity &i_tex_identity)
{
    if (i_tex_identity.IsValid() == false) {
        return;
    }

    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(
        m_device, m_swapchain, m_max_img_acq_time,
        m_wait_img_sema, VK_NULL_HANDLE, &image_index);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        SDLOGW("We can't get nxt swapchain image. error(%d)", result);
        return;
    }

    SDLOG("RenderTexture2DToScreen");

    CommandBufferIdentity cmd_identity = SD_WREF(m_id_getter).GetIdentity(m_cmd_buffer);

    SD_WREF(m_cmd_buffer).Begin();

    VkRect2D render_area = {};
    render_area.offset = { 0, 0 };
    render_area.extent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };

    VkImageBlit blit_param = {};
    blit_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.srcSubresource.baseArrayLayer = 0;
    blit_param.srcSubresource.mipLevel = 0;
    blit_param.srcSubresource.layerCount = 1;
    blit_param.srcOffsets[0].x = 0;
    blit_param.srcOffsets[0].y = 0;
    blit_param.srcOffsets[0].z = 0;
    blit_param.srcOffsets[1].x = m_screen_size.GetWidth();
    blit_param.srcOffsets[1].y = m_screen_size.GetHeight();
    blit_param.srcOffsets[1].z = 1;
    blit_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.dstSubresource.baseArrayLayer = 0;
    blit_param.dstSubresource.mipLevel = 0;
    blit_param.dstSubresource.layerCount = 1;
    blit_param.dstOffsets[0].x = 0;
    blit_param.dstOffsets[0].y = 0;
    blit_param.dstOffsets[0].z = 0;
    blit_param.dstOffsets[1].x = m_screen_size.GetWidth();
    blit_param.dstOffsets[1].y = m_screen_size.GetHeight();
    blit_param.dstOffsets[1].z = 1;

    if (i_tex_identity.m_image_handle != VK_NULL_HANDLE) {
        vkCmdBlitImage(
            reinterpret_cast<VkCommandBuffer>(cmd_identity.m_handle),
            reinterpret_cast<VkImage>(i_tex_identity.m_image_handle),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_swapchain_images[image_index],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit_param,
            VK_FILTER_NEAREST);
    }

    SD_WREF(m_cmd_buffer).End();

    GraphicsManager::GetRef().SubmitCommandBuffersToQueue({ m_cmd_buffer });

    //Present to screen
    VkPresentInfoKHR p_info = {};
    p_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    p_info.pNext = nullptr;
    p_info.waitSemaphoreCount = 1;
    p_info.pWaitSemaphores = &m_present_sema;
    p_info.swapchainCount = 1;
    p_info.pSwapchains = &m_swapchain;
    p_info.pImageIndices = &image_index;
    p_info.pResults = nullptr;

    {
        std::lock_guard<std::mutex> lck(m_queue_mutex);
        SDLOG("RenderTexture2DToScreen -- SCBegin");
        result = vkQueuePresentKHR(m_queue, &p_info);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            SDLOGW("We can't present image by queue. error(%d).", result);
        }
        SDLOG("RenderTexture2DToScreen -- SCEnd");
    }
}

void VulkanSwapchain::InitializeVKSwapchain(
    const VulkanConfig &i_config,
    VkPhysicalDevice i_phy_device,
    VkDevice i_device,
    VkSurfaceKHR i_surface,
    uint32_t i_present_q_fam_id,
    VkQueue i_present_queue)
{
    m_device = i_device;
    m_queue = i_present_queue;
    m_max_img_acq_time = i_config.m_max_img_acq_time;
    m_max_fence_wait_time = i_config.m_max_fence_wait_time;
    //
    SDLOG("--- Vulkan initialize swap chains.(Create swap chain)");
    VkSurfaceCapabilitiesKHR sur_caps;
    SDLOGD("------- Get surface capability : ");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(i_phy_device, i_surface, &sur_caps);

    bool has_desired_sur_fmt = false;
    std::vector<VkSurfaceFormatKHR> sur_formats;
    uint32_t sur_format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(i_phy_device, i_surface, &sur_format_count, nullptr);

    if (sur_format_count != 0) {
        sur_formats.resize(sur_format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(i_phy_device, i_surface, &sur_format_count, sur_formats.data());
    }

    if (sur_format_count > 1) {
        for (VkSurfaceFormatKHR &fmt : sur_formats) {
            SDLOGD("Supported SurfaceFormat:(Format)%d, (colorSpace)%d", fmt.format, fmt.colorSpace);
        }

        for (const VkSurfaceFormatKHR &desired_fmt : i_config.m_desired_sur_formats) {
            for (VkSurfaceFormatKHR &fmt : sur_formats) {
                if (fmt.colorSpace == desired_fmt.colorSpace &&
                    fmt.format == desired_fmt.format) {
                    m_final_sur_format = fmt;
                    has_desired_sur_fmt = true;
                    break;
                }
            }
            if (has_desired_sur_fmt == true) {
                break;
            }
        }
    }
    else if (sur_format_count == 1) {
        if (sur_formats[0].format == VK_FORMAT_UNDEFINED) {
            has_desired_sur_fmt = true;
        }
    }

    if (has_desired_sur_fmt == false) {
        throw std::runtime_error("Desire surface format isn't exist.");
    }

    SDLOGD("Desired SurfaceFormat:(%d, %d)", m_final_sur_format.format, m_final_sur_format.colorSpace);

    std::vector<VkPresentModeKHR> supported_p_modes;
    uint32_t supported_p_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(i_phy_device, i_surface, &supported_p_mode_count, nullptr);

    if (supported_p_mode_count != 0) {
        supported_p_modes.resize(supported_p_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(i_phy_device, i_surface, &supported_p_mode_count, supported_p_modes.data());
    }
    else {
        throw std::runtime_error("No present mode supported!");
    }

    for (const VkPresentModeKHR& p_mode : supported_p_modes) {
        SDLOGD("Supported present mode : %d", p_mode);
    }

    for (uint32_t mode_id = 0; mode_id < i_config.m_desired_pre_modes.size(); mode_id++) {
        for (const VkPresentModeKHR& p_mode : supported_p_modes) {
            if (i_config.m_desired_pre_modes[mode_id] == p_mode) {
                m_final_p_mode = i_config.m_desired_pre_modes[mode_id];
                break;
            }
        }
        if (m_final_p_mode != VK_PRESENT_MODE_MAX_ENUM_KHR) {
            break;
        }
    }

    if (m_final_p_mode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
        throw std::runtime_error("No desired present mode supported!");
    }
    else {
        SDLOG("final present mode : %d", m_final_p_mode);
    }

    if (sur_caps.currentExtent.width != 0 && sur_caps.currentExtent.height != 0) {
        m_screen_size.SetResolution(sur_caps.currentExtent.width, sur_caps.currentExtent.height);
    }
    else {
        m_screen_size.SetResolution(
            std::max(sur_caps.minImageExtent.width, std::min(sur_caps.maxImageExtent.width, sur_caps.minImageExtent.width)),
            std::max(sur_caps.minImageExtent.height, std::min(sur_caps.maxImageExtent.height, sur_caps.minImageExtent.height)));
    }

    uint32_t image_count = sur_caps.minImageCount + 1;
    if (image_count > sur_caps.maxImageCount) {
        image_count = sur_caps.maxImageCount;
    }

    uint32_t present_queue_fam_id = static_cast<uint32_t>(i_present_q_fam_id);

    VkSwapchainCreateInfoKHR sw_c_info = {};
    sw_c_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    sw_c_info.pNext = nullptr;
    sw_c_info.flags = 0;
    sw_c_info.surface = i_surface;
    sw_c_info.minImageCount = image_count;
    sw_c_info.imageFormat = m_final_sur_format.format;
    sw_c_info.imageColorSpace = m_final_sur_format.colorSpace;
    sw_c_info.imageExtent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };
    sw_c_info.imageArrayLayers = 1;
    sw_c_info.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    sw_c_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    sw_c_info.queueFamilyIndexCount = 1;
    sw_c_info.pQueueFamilyIndices = &present_queue_fam_id;
    sw_c_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; //Use this for android.(If use currentTransform as presentTransform, we will see double rotation)
    sw_c_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    sw_c_info.presentMode = m_final_p_mode;
    sw_c_info.clipped = VK_TRUE;
    sw_c_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device, &sw_c_info, nullptr, &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    if (vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to get image number of swap chain!");
    }

    if (image_count > 0) {
        m_swapchain_images.resize(image_count);
        if (vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data()) == VK_SUCCESS) {
            SDLOG("SwapChainImages number : %u, ViewPort(%u,%u)", image_count, m_screen_size.GetWidth(), m_screen_size.GetHeight());
        }
        else {
            throw std::runtime_error("failed to get image handle of swap chain!");
        }
    }

    VkSemaphoreCreateInfo acq_sem_c_info = {};
    acq_sem_c_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    acq_sem_c_info.pNext = nullptr;
    acq_sem_c_info.flags = 0;

    if (vkCreateSemaphore(m_device, &acq_sem_c_info, nullptr, &m_wait_img_sema) != VK_SUCCESS) {
        throw std::runtime_error("failed to create acq img semaphore!");
    }

    VkSemaphoreCreateInfo present_sem_c_info = {};
    present_sem_c_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    present_sem_c_info.pNext = nullptr;
    present_sem_c_info.flags = 0;

    if (vkCreateSemaphore(m_device, &present_sem_c_info, nullptr, &m_present_sema) != VK_SUCCESS) {
        throw std::runtime_error("failed to create present semaphore!");
    }
}

Resolution VulkanSwapchain::GetResolution() const
{
    return m_screen_size;
}

______________SD_END_GRAPHICS_NAMESPACE______________