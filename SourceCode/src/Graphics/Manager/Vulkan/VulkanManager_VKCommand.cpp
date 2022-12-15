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

VkResult VulkanManager::CreateVkCommandPool(VkCommandPool &io_cmd_pool, VkDevice i_device, VkCommandPoolCreateFlags i_flag)
{
    VkCommandPoolCreateInfo cmd_pool_c_info = {};
    cmd_pool_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_c_info.pNext = nullptr;
    cmd_pool_c_info.flags = i_flag; //VkCommandPoolCreateFlags
    cmd_pool_c_info.queueFamilyIndex = m_queue_family.GetQueueFamilyID();
    return vkCreateCommandPool(i_device, &cmd_pool_c_info, nullptr, &io_cmd_pool);
}

void VulkanManager::DestroyVkCommandPool(VkCommandPool &io_cmd_pool, VkDevice i_device)
{
    vkDestroyCommandPool(i_device, io_cmd_pool, nullptr);
    io_cmd_pool = VK_NULL_HANDLE;
}

VkResult VulkanManager::AllocateVkCommandBuffer(
    VkCommandBuffer &io_cmd_buffer, VkDevice i_device, VkCommandPool i_cmd_pool, VkCommandBufferLevel i_level)
{
    VkCommandBufferAllocateInfo cmd_buf_a_info = {};
    cmd_buf_a_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_buf_a_info.pNext = nullptr;
    cmd_buf_a_info.commandPool = i_cmd_pool;
    cmd_buf_a_info.level = i_level; //VkCommandBufferLevel
    cmd_buf_a_info.commandBufferCount = 1;
    return vkAllocateCommandBuffers(i_device, &cmd_buf_a_info, &io_cmd_buffer);
}

void VulkanManager::FreeVkCommandBuffer(
    VkCommandBuffer &io_cmd_buffer, VkDevice i_device, VkCommandPool i_cmd_pool)
{
    vkFreeCommandBuffers(i_device, i_cmd_pool, 1, &io_cmd_buffer);
}

VkResult VulkanManager::BeginVkCommandBuffer(
    VkCommandBuffer i_cmd_buffer,
    const VkCommandBufferBeginInfo &i_info)
{
    return vkBeginCommandBuffer(i_cmd_buffer, &i_info);
}

VkResult VulkanManager::EndVkCommandBuffer(VkCommandBuffer i_cmd_buffer)
{
    return vkEndCommandBuffer(i_cmd_buffer);
}

void VulkanManager::ExecuteVkSecondaryCommandBuffersToPrimaryVkCommandBuffer(
    VkCommandBuffer i_primary_cmd_buffer,
    const std::vector<VkCommandBuffer>& i_second_cmd_buffers)
{
    vkCmdExecuteCommands(i_primary_cmd_buffer, static_cast<uint32_t>(i_second_cmd_buffers.size()), i_second_cmd_buffers.data());
}

______________SD_END_GRAPHICS_NAMESPACE______________