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

#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkResult VulkanManager::CreateVkDescriptorSetLayout(
    VkDescriptorSetLayout &io_handle,
    const VkDescriptorSetLayoutCreateInfo &i_c_info)
{
    return vkCreateDescriptorSetLayout(m_device_handle, &i_c_info, nullptr, &io_handle);
}

void VulkanManager::DestroyVkDescriptorSetLayout(VkDescriptorSetLayout &io_handle)
{
    vkDestroyDescriptorSetLayout(m_device_handle, io_handle, nullptr);
    io_handle = VK_NULL_HANDLE;
}

VkResult VulkanManager::CreateVkDescriptorPool(
    VkDescriptorPool &io_handle,
    const VkDescriptorPoolCreateInfo &i_dp_c_info)
{
    return vkCreateDescriptorPool(m_device_handle, &i_dp_c_info, nullptr, &io_handle);
}

void VulkanManager::DestroyVkDescriptorPool(VkDescriptorPool &io_handle)
{
    vkDestroyDescriptorPool(m_device_handle, io_handle, nullptr);
    io_handle = SD_NULL_HANDLE;
}

VkResult VulkanManager::AllocateVkDescriptorSet(
    VkDescriptorSet &io_handle,
    const VkDescriptorSetAllocateInfo &i_a_info)
{
    return vkAllocateDescriptorSets(m_device_handle, &i_a_info, &io_handle);
}

void VulkanManager::UpdateVkDescriptorSet(
    const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos,
    const std::vector<VkCopyDescriptorSet> &i_descriptor_c_infos)
{
    vkUpdateDescriptorSets(m_device_handle,
        static_cast<uint32_t>(i_descriptor_w_infos.size()),
        i_descriptor_w_infos.data(),
        static_cast<uint32_t>(i_descriptor_c_infos.size()),
        i_descriptor_c_infos.data());
}

void VulkanManager::BindVkDescriptorSets(
    VkCommandBuffer i_cb_handle,
    VkPipelineBindPoint i_pipe_bind_point,
    VkPipelineLayout i_pipe_layout_handle,
    const std::vector<VkDescriptorSet> &i_ds_handles,
    const std::vector<uint32_t> &i_dynamic_offsets)
{
    vkCmdBindDescriptorSets(
        i_cb_handle,
        i_pipe_bind_point,
        i_pipe_layout_handle, 0,
        static_cast<uint32_t>(i_ds_handles.size()), i_ds_handles.data(),
        static_cast<uint32_t>(i_dynamic_offsets.size()), i_dynamic_offsets.data());
}

void VulkanManager::FreeVkDescriptorSet(VkDescriptorSet &io_handle, VkDescriptorPool i_dp_handle)
{
    vkFreeDescriptorSets(m_device_handle, i_dp_handle, 1, &io_handle);
    io_handle = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________
