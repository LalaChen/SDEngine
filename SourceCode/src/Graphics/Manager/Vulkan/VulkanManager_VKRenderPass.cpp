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

VkResult VulkanManager::CreateVKRenderPass(
    VkRenderPass &io_rp_handle,
    const std::vector<VkAttachmentDescription> &i_vk_att_descs,
    const std::vector<VkSubpassDescription> &i_vk_sps,
    const std::vector<VkSubpassDependency> &i_vk_sp_dependencies)
{
    VkRenderPassCreateInfo rp_c_info = {};
    rp_c_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_c_info.flags = 0;
    rp_c_info.pNext = nullptr;
    rp_c_info.attachmentCount = static_cast<uint32_t>(i_vk_att_descs.size());
    rp_c_info.pAttachments = i_vk_att_descs.data();
    rp_c_info.dependencyCount = static_cast<uint32_t>(i_vk_sp_dependencies.size());
    rp_c_info.pDependencies = i_vk_sp_dependencies.data();
    rp_c_info.subpassCount = static_cast<uint32_t>(i_vk_sps.size());
    rp_c_info.pSubpasses = i_vk_sps.data();
   
    return vkCreateRenderPass(m_VK_device, &rp_c_info, nullptr, &io_rp_handle);
}

void VulkanManager::DestroyVKRenderPass(VkRenderPass &io_rp_handle)
{
    vkDestroyRenderPass(m_VK_device, io_rp_handle, nullptr);
    io_rp_handle = SD_NULL_HANDLE;
}


______________SD_END_GRAPHICS_NAMESPACE______________