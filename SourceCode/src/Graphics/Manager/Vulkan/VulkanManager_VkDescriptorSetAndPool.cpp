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

VkResult VulkanManager::CreateVkDescriptorPool(
    VkDescriptorPool &io_handle,
    const VkDescriptorPoolCreateInfo &i_dp_c_info)
{
    return vkCreateDescriptorPool(m_VK_device, &i_dp_c_info, nullptr, &io_handle);
}

void VulkanManager::DestroyVkDescriptorPool(VkDescriptorPool &io_handle)
{
    vkDestroyDescriptorPool(m_VK_device, io_handle, nullptr);
    io_handle = SD_NULL_HANDLE;
}

VkResult VulkanManager::AllocateVkDescriptorSet(
    VkDescriptorSet &io_handle,
    const VkDescriptorSetAllocateInfo &i_a_info)
{
    return vkAllocateDescriptorSets(m_VK_device, &i_a_info, &io_handle);
}

void VulkanManager::UpdateVkDescriptorSet(
    const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos,
    const std::vector<VkCopyDescriptorSet> &i_descriptor_c_infos)
{
    vkUpdateDescriptorSets(m_VK_device,
        i_descriptor_w_infos.size(),
        i_descriptor_w_infos.data(),
        i_descriptor_c_infos.size(),
        i_descriptor_c_infos.data());
}

void VulkanManager::FreeVkDescriptorSet(VkDescriptorSet &io_handle, VkDescriptorPool i_dp_handle)
{
    vkFreeDescriptorSets(m_VK_device, i_dp_handle, 1, &io_handle);
    io_handle = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________