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

void VulkanManager::RenderTexture2DToScreen(const TextureWeakReferenceObject &i_tex)
{
    const TextureIdentity &tex_identity = GetIdentity(i_tex);
    //Get swapchain image.
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(
        m_device_handle, m_sc_handle, m_vulkan_config.m_max_img_acq_time,
        m_acq_img_sema_handle, VK_NULL_HANDLE, &image_index);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        SDLOGW("We can't get nxt swapchain image. error(%d)", result);
        return;
    }
    //Begin command buffer
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(m_main_cb_handle, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!! error(%d)", m_main_cb_handle, result);
        return;
    }

    //Begin RenderPass.
    VkRect2D render_area = {};
    render_area.offset = { 0, 0 };
    render_area.extent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };

    VkImageBlit blit_param = {};
    blit_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.srcSubresource.baseArrayLayer = 0;
    blit_param.srcSubresource.mipLevel = 0;
    blit_param.srcSubresource.layerCount = 1;
    blit_param.srcOffsets[0].x = 0;
    blit_param.srcOffsets[0].y = 0;
    blit_param.srcOffsets[0].z = 0;
    blit_param.srcOffsets[1].x = m_screen_size.GetWidth();
    blit_param.srcOffsets[1].y = m_screen_size.GetHeight();
    blit_param.srcOffsets[1].z = 1;
    blit_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.dstSubresource.baseArrayLayer = 0;
    blit_param.dstSubresource.mipLevel = 0;
    blit_param.dstSubresource.layerCount = 1;
    blit_param.dstOffsets[0].x = 0;
    blit_param.dstOffsets[0].y = 0;
    blit_param.dstOffsets[0].z = 0;
    blit_param.dstOffsets[1].x = m_screen_size.GetWidth();
    blit_param.dstOffsets[1].y = m_screen_size.GetHeight();
    blit_param.dstOffsets[1].z = 1;

    if (tex_identity.m_image_handle != VK_NULL_HANDLE) {
        vkCmdBlitImage(m_main_cb_handle,
            reinterpret_cast<VkImage>(tex_identity.m_image_handle),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_sc_img_handles[image_index],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit_param,
            VK_FILTER_NEAREST);
    }

    //End command buffer
    result = vkEndCommandBuffer(m_main_cb_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!! error(%d)", m_main_cb_handle, result);
        return;
    }

    //Push command buffer to queue.
    VkSemaphore wait_semaphores[1] = { m_acq_img_sema_handle };
    VkPipelineStageFlags submit_wait_flags[1] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores; //wait acq image.
    submit_info.pWaitDstStageMask = submit_wait_flags;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &m_pre_sema_handle; //set present semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_main_cb_handle;

    result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer to PresentQueue(%p) failure(%d)!!!", m_present_q_handle, result);
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, m_vulkan_config.m_max_fence_wait_time);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!! error(%d).", result);
    }

    //Present to screen
    VkPresentInfoKHR p_info = {};
    p_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    p_info.pNext = nullptr;
    p_info.waitSemaphoreCount = 1;
    p_info.pWaitSemaphores = &m_pre_sema_handle;
    p_info.swapchainCount = 1;
    p_info.pSwapchains = &m_sc_handle;
    p_info.pImageIndices = &image_index;
    p_info.pResults = nullptr;

    result = vkQueuePresentKHR(m_present_q_handle, &p_info);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        SDLOGW("We can't present image by queue. error(%d).", result);
    }

    m_fps_counter.AddCount();
}


void VulkanManager::RenderToScreen()
{
    //Get swapchain image.
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(
        m_device_handle, m_sc_handle, m_vulkan_config.m_max_img_acq_time,
        m_acq_img_sema_handle, VK_NULL_HANDLE, &image_index);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        SDLOGW("We can't get nxt swapchain image.");
        return;
    }
    //Begin command buffer
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(m_main_cb_handle, &cmd_buf_c_info) != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_main_cb_handle);
        return;
    }

    //Begin RenderPass.
    VkRect2D render_area = {};
    render_area.offset = { 0, 0 };
    render_area.extent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };

    VkRenderPassBeginInfo rp_begin_info = {};
    rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.pNext = nullptr;
    rp_begin_info.renderPass = m_pre_rp_handle;
    rp_begin_info.framebuffer = m_sc_fb_handles[image_index];
    rp_begin_info.renderArea = render_area;
    rp_begin_info.clearValueCount = 1;
    rp_begin_info.pClearValues = &sClearColor;

    vkCmdBeginRenderPass(m_main_cb_handle, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    //Render to screen (To do : Compose buffers.)

    //End RenderPass.
    vkCmdEndRenderPass(m_main_cb_handle);
    //End command buffer
    if (vkEndCommandBuffer(m_main_cb_handle) != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_main_cb_handle);
        return;
    }

    //Push command buffer to queue.
    VkSemaphore wait_semaphores[1] = { m_acq_img_sema_handle };
    VkPipelineStageFlags submit_wait_flags[1] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores; //wait acq image.
    submit_info.pWaitDstStageMask = submit_wait_flags;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &m_pre_sema_handle; //set present semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_main_cb_handle;

    if (vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle) != VK_SUCCESS) {
        SDLOGW("Submit command buffer to PresentQueue(%p) failure!!!", m_present_q_handle);
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, m_vulkan_config.m_max_fence_wait_time);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return;
    }

    //Reset main command buffer sync.
    if (vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle) != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
    }

    //Present to screen
    VkPresentInfoKHR p_info = {};
    p_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    p_info.pNext = nullptr;
    p_info.waitSemaphoreCount = 1;
    p_info.pWaitSemaphores = &m_pre_sema_handle;
    p_info.swapchainCount = 1;
    p_info.pSwapchains = &m_sc_handle;
    p_info.pImageIndices = &image_index;
    p_info.pResults = nullptr;

    if (vkQueuePresentKHR(m_present_q_handle, &p_info) != VK_SUCCESS) {
        SDLOGW("We can't present image by queue.");
        return;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________