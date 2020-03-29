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
    cmd_pool_c_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VkCommandPoolCreateFlags
    cmd_pool_c_info.queueFamilyIndex = m_VK_picked_queue_family_id;
    return vkCreateCommandPool(m_VK_device, &cmd_pool_c_info, nullptr, &io_pool_handle);
}

VkResult VulkanManager::AllocateVkCommandBuffer(
    VkCommandBuffer& io_cmd_handle,
    VkCommandPool i_pool_handle,
    VkCommandBufferLevel i_level)
{
    VkCommandBufferAllocateInfo cmd_buf_a_info = {};
    cmd_buf_a_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_buf_a_info.pNext = nullptr;
    cmd_buf_a_info.commandPool = i_pool_handle;
    cmd_buf_a_info.level = i_level; //VkCommandBufferLevel
    cmd_buf_a_info.commandBufferCount = 1;
    return vkAllocateCommandBuffers(m_VK_device, &cmd_buf_a_info, &io_cmd_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________