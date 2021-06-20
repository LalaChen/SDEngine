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

VkResult VulkanManager::CreateVKRenderPass(
    VkRenderPass &io_rp_handle,
    const VkRenderPassCreateInfo& i_rp_c_info)
{   
    return vkCreateRenderPass(m_device_handle, &i_rp_c_info, nullptr, &io_rp_handle);
}

void VulkanManager::BeginVkRenderPass(
    VkCommandBuffer i_cb_handle,
    VkRenderPass i_rp_handle,
    VkFramebuffer i_fb_handle,
    const VkRect2D &i_render_area,
    const std::vector<VkClearValue> &i_clear_values,
    VkSubpassContents i_sp_content)
{
    VkRenderPassBeginInfo rp_begin_info = {};
    rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.pNext = nullptr;
    rp_begin_info.renderPass = i_rp_handle;
    rp_begin_info.framebuffer = i_fb_handle;
    rp_begin_info.renderArea = i_render_area;
    rp_begin_info.clearValueCount = static_cast<uint32_t>(i_clear_values.size());
    rp_begin_info.pClearValues = i_clear_values.data();

    vkCmdBeginRenderPass(i_cb_handle, &rp_begin_info, i_sp_content);
}

void VulkanManager::GotoNextStepInVKRenderPass(VkCommandBuffer i_cb_handle, VkSubpassContents i_sp_content)
{
    vkCmdNextSubpass(i_cb_handle, i_sp_content);
}

void VulkanManager::EndVkRenderPass(VkCommandBuffer i_cb_handle)
{
    vkCmdEndRenderPass(i_cb_handle);
}

void VulkanManager::DestroyVKRenderPass(VkRenderPass &io_rp_handle)
{
    vkDestroyRenderPass(m_device_handle, io_rp_handle, nullptr);
    io_rp_handle = SD_NULL_HANDLE;
}


______________SD_END_GRAPHICS_NAMESPACE______________