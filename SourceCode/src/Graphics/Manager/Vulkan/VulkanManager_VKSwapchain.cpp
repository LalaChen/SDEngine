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

#include "VulkanManager.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkResult VulkanManager::CreateVkSwapchain(VkSwapchainKHR &io_swapchain, VkDevice i_device, const VkSwapchainCreateInfoKHR &i_info)
{
    return vkCreateSwapchainKHR(i_device, &i_info, nullptr, &io_swapchain);
}

VkResult VulkanManager::CollectVkImageForVkSwapchain(std::vector<VkImage> &io_iamges, VkDevice i_device, VkSwapchainKHR i_swapchain)
{
    uint32_t image_count;
    VkResult result = VK_SUCCESS;
    result = vkGetSwapchainImagesKHR(i_device, i_swapchain, &image_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    if (image_count > 0) {
        io_iamges.resize(image_count);
        result = vkGetSwapchainImagesKHR(m_device, i_swapchain, &image_count, io_iamges.data());
        return result;
    }
    else {
        result = VK_ERROR_UNKNOWN;
        return result;
    }
}

VkResult VulkanManager::GetCurrentVkSwapchainIdx(
    uint32_t &io_image_idx,
    VkDevice i_device,
    VkSwapchainKHR i_swapchain,
    VkSemaphore i_wait_sema)
{
    VkResult result = vkAcquireNextImageKHR(
        i_device, i_swapchain,
        m_vulkan_config.m_max_img_acq_time,
        i_wait_sema, VK_NULL_HANDLE, &io_image_idx);

    return result;
}

void VulkanManager::BlitVkImages(
    VkCommandBuffer i_cmd_buffer,
    VkImage i_src_image,
    VkImage i_dst_image,
    const VkImageBlit &i_param,
    VkFilter i_filter)
{
    vkCmdBlitImage(
        reinterpret_cast<VkCommandBuffer>(i_cmd_buffer),
        i_src_image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        i_dst_image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &i_param,
        i_filter);
}

void VulkanManager::DestroyVkSwapchain(VkSwapchainKHR &io_swapchain, VkDevice i_device)
{
    vkDestroySwapchainKHR(i_device, io_swapchain, nullptr);
    io_swapchain = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________