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

#include "VulkanWrapper.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkResult VulkanManager::CreateVkCommandPool(VkCommandPool &io_pool_handle, VkCommandPoolCreateFlags i_flag)
{
    VkCommandPoolCreateInfo cmd_pool_c_info = {};
    cmd_pool_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_c_info.pNext = nullptr;
    cmd_pool_c_info.flags = i_flag; //VkCommandPoolCreateFlags
    cmd_pool_c_info.queueFamilyIndex = m_final_queue_fam_id;
    return vkCreateCommandPool(m_device_handle, &cmd_pool_c_info, nullptr, &io_pool_handle);
}

void VulkanManager::DestroyVkCommandPool(VkCommandPool &io_pool_handle)
{
    vkDestroyCommandPool(m_device_handle, io_pool_handle, nullptr);
    io_pool_handle = VK_NULL_HANDLE;
}

VkResult VulkanManager::AllocateVkCommandBuffer(
    VkCommandBuffer &io_cb_handle,
    VkCommandPool i_cp_handle,
    VkCommandBufferLevel i_level)
{
    VkCommandBufferAllocateInfo cmd_buf_a_info = {};
    cmd_buf_a_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_buf_a_info.pNext = nullptr;
    cmd_buf_a_info.commandPool = i_cp_handle;
    cmd_buf_a_info.level = i_level; //VkCommandBufferLevel
    cmd_buf_a_info.commandBufferCount = 1;
    return vkAllocateCommandBuffers(m_device_handle, &cmd_buf_a_info, &io_cb_handle);
}

void VulkanManager::FreeVkCommandBuffer(
    VkCommandBuffer &io_cb_handle,
    VkCommandPool i_cp_handle)
{
    vkFreeCommandBuffers(m_device_handle, i_cp_handle, 1, &io_cb_handle);
}

VkResult VulkanManager::BeginVkCommandBuffer(
    VkCommandBuffer i_cb_handle,
    const VkCommandBufferBeginInfo &i_info)
{
    return vkBeginCommandBuffer(i_cb_handle, &i_info);
}

VkResult VulkanManager::EndVkCommandBuffer(VkCommandBuffer i_cb_handle)
{
    return vkEndCommandBuffer(i_cb_handle);
}

VkResult VulkanManager::SubmitVkCommandBuffers(const std::vector<VkCommandBuffer> &i_cb_handles)
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
    submit_info.commandBufferCount = static_cast<uint32_t>(i_cb_handles.size());
    submit_info.pCommandBuffers = i_cb_handles.data();

    result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);

    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure(%d)!!!", result);
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, m_vulkan_config.m_max_fence_wait_time);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("reset command buffer fence failure(%d)!!!", result);
        return result;
    }

    return VK_SUCCESS;
}

void VulkanManager::ExecuteVkSecondaryCommandBuffersToPrimaryVkCommandBuffer(
    VkCommandBuffer i_primary_cb_handle,
    const std::vector<VkCommandBuffer> &i_second_cb_handles)
{
    vkCmdExecuteCommands(i_primary_cb_handle, static_cast<uint32_t>(i_second_cb_handles.size()), i_second_cb_handles.data());
}

______________SD_END_GRAPHICS_NAMESPACE______________