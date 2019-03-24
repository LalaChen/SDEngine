#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanAPITestManager.h"

using namespace::SDE::Basic;
using namespace::SDE::Graphics;

VulkanAPITestManager::VulkanAPITestManager()
: VulkanManager()
{
}

VulkanAPITestManager::~VulkanAPITestManager()
{
}

void VulkanAPITestManager::InitializeGraphicsSystem(const EventArg &i_arg)
{
    VulkanManager::InitializeGraphicsSystem(i_arg);
    //To do : initialize necessary graphics resources.
    m_draw_triangle.Initialize(m_VK_physical_device, m_VK_device);
    //
}

void VulkanAPITestManager::ReleaseGraphicsSystem()
{
    //To do : release graphics resources.

    //
    VulkanManager::ReleaseGraphicsSystem();
}

void VulkanAPITestManager::RenderToScreen()
{
    //Get swapchain image.
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(
        m_VK_device, m_VK_swap_chain, MaxImgAcqirationTime,
        m_VK_acq_img_semaphore, nullptr, &image_index);

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

    if (vkBeginCommandBuffer(m_VK_main_cmd_buffer, &cmd_buf_c_info) != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_VK_main_cmd_buffer);
        return;
    }

    //Begin RenderPass.
    VkRect2D render_area = {};
    render_area.offset = { 0, 0 };
    render_area.extent = m_screen_size;

    VkRenderPassBeginInfo rp_begin_info = {};
    rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.pNext = nullptr;
    rp_begin_info.renderPass = m_VK_present_render_pass;
    rp_begin_info.framebuffer = m_VK_sc_image_fbs[image_index];
    rp_begin_info.renderArea = render_area;
    rp_begin_info.clearValueCount = 1;
    rp_begin_info.pClearValues = &ClearColor;

    vkCmdBeginRenderPass(m_VK_main_cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    //Render debug.
    RenderDebug();

    //End RenderPass.
    vkCmdEndRenderPass(m_VK_main_cmd_buffer);

    //End command buffer
    if (vkEndCommandBuffer(m_VK_main_cmd_buffer) != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_VK_main_cmd_buffer);
        return;
    }
    //Push command buffer to queue.
    VkPipelineStageFlags submit_wait_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &m_VK_acq_img_semaphore; //wait acq image.
    submit_info.pWaitDstStageMask = &submit_wait_flag;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &m_VK_present_semaphore; //set present semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_VK_main_cmd_buffer;

    if (vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence) != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure!!!");
    }

    if (vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_TRUE, MaxFenceWaitTime) != VK_SUCCESS) {
        SDLOGW("Wait sync failure!!!");
    }

    //Reset main command buffer sync.
    if (vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence) != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
    }

    //Present to screen
    VkPresentInfoKHR p_info = {};
    p_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    p_info.pNext = nullptr;
    p_info.waitSemaphoreCount = 1;
    p_info.pWaitSemaphores = &m_VK_present_semaphore;
    p_info.swapchainCount = 1;
    p_info.pSwapchains = &m_VK_swap_chain;
    p_info.pImageIndices = &image_index;
    p_info.pResults = nullptr;

    if (vkQueuePresentKHR(m_VK_present_queue, &p_info) != VK_SUCCESS) {
        SDLOGW("We can't present image by queue.");
        return;
    }
}

void VulkanAPITestManager::RenderDebug()
{
    //Test 1. Draw Trangle.
    m_draw_triangle.Render();
}