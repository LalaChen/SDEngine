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

VkResult VulkanManager::CreateVkFence(VkFence &io_fence, VkDevice i_device, const VkFenceCreateInfo &i_info)
{
    VkResult result = vkCreateFence(i_device, &i_info, nullptr, &io_fence);
    return result;
}

VkResult VulkanManager::ResetVkFence(VkFence i_fence, VkDevice i_device)
{
    return vkResetFences(i_device, 1, &i_fence);
}

void VulkanManager::DestroyVkFence(VkFence &io_fence, VkDevice i_device)
{
    vkDestroyFence(i_device, io_fence, nullptr);
}

VkResult VulkanManager::CreateVkSemaphore(
    VkSemaphore &io_semaphore,
    VkDevice i_device,
    const VkSemaphoreCreateInfo &i_info)
{
    return vkCreateSemaphore(i_device, &i_info, nullptr, &io_semaphore);
}

void VulkanManager::DestroyVkSemaphore(
    VkSemaphore &io_semaphore,
    VkDevice i_device)
{
    vkDestroySemaphore(i_device, io_semaphore, nullptr);
}

void VulkanManager::GetVkQueue(VkDevice i_device, uint32_t i_q_fam_id, uint32_t i_q_id, VkQueue& io_queue)
{
    vkGetDeviceQueue(i_device, static_cast<uint32_t>(i_q_fam_id), i_q_id, &io_queue);
}

VkResult VulkanManager::SubmitVkCommandBuffersToQueue(
    VkDevice i_device,
    VkQueue i_queue,
    VkFence i_fence,
    const std::vector<VkCommandBuffer> &i_cbs)
{
    VkResult result;
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr; //wait acq image.
    submit_info.pWaitDstStageMask = nullptr;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    submit_info.commandBufferCount = static_cast<uint32_t>(i_cbs.size());
    submit_info.pCommandBuffers = i_cbs.data();

    result = vkQueueSubmit(i_queue, 1, &submit_info, i_fence); //m_main_cb_fence_handle need to move out for one queue

    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure(%d)!!!", result);
    }

    do {
        result = vkWaitForFences(i_device, 1, &i_fence, VK_TRUE, m_vulkan_config.m_max_fence_wait_time);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    result = vkResetFences(i_device, 1, &i_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("reset command buffer fence failure(%d)!!!", result);
        return result;
    }

    return VK_SUCCESS;
}

VkResult VulkanManager::PresentVkSwapchainToVkQueue(
    VkQueue i_queue,
    const VkPresentInfoKHR &i_info)
{
    return vkQueuePresentKHR(i_queue, &i_info);
}

______________SD_END_GRAPHICS_NAMESPACE______________