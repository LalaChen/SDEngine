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

VkResult VulkanManager::CreateVKFrameBuffer(
    VkFramebuffer &io_fb_handle,
    const VkRenderPass i_rp_handle,
    const std::vector<VkImageView> &i_iv_handles,
    Size_ui32 i_width,
    Size_ui32 i_height,
    Size_ui32 i_layers)
{
    VkResult result = VK_SUCCESS;
    VkFramebufferCreateInfo fbo_c_info = {};
    fbo_c_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbo_c_info.pNext = nullptr;
    fbo_c_info.flags = 0; //Reserved for future use
    fbo_c_info.renderPass = i_rp_handle;
    fbo_c_info.attachmentCount = static_cast<uint32_t>(i_iv_handles.size());
    fbo_c_info.pAttachments = i_iv_handles.data();
    fbo_c_info.width  = i_width;
    fbo_c_info.height = i_height;
    fbo_c_info.layers = i_layers;
    result = vkCreateFramebuffer(m_device_handle, &fbo_c_info, nullptr, &io_fb_handle);
    return result;
}

void VulkanManager::DestroyVkFrameBuffer(VkFramebuffer &io_fb_handle)
{
    vkDestroyFramebuffer(m_device_handle, io_fb_handle, nullptr);
    io_fb_handle = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________