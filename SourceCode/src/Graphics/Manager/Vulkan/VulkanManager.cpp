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

#include "VulkanCreationArg.h"
#include "LogManager.h"
#include "VulkanManager.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

const uint32_t VulkanManager::MaxImgAcqirationTime = 2000000000; //2s
const uint32_t VulkanManager::MaxFenceWaitTime = 17000000; //17ms
const VkClearValue VulkanManager::ClearColor = { 0.2f, 0.5f, 0.8f, 1.0f };

const std::vector<const char*>& VulkanManager::GetDesiredValidLayers()
{
    return DesiredValidLayers;
}

std::vector<const char*> VulkanManager::DesiredValidLayers = {
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_RENDERDOC_Capture"//,
    //"VK_LAYER_VALVE_steam_overlay",
    //"VK_LAYER_VALVE_steam_fossilize", // will create a lot of json file.
    //"VK_LAYER_NV_optimus"
};

std::vector<const char*> VulkanManager::NecessaryExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


//------------------------------------------------

VulkanManager::VulkanManager()
: m_VK_instance(VK_NULL_HANDLE)
, m_VK_surface(VK_NULL_HANDLE)
// debug cbk
, m_VK_debug_report_cbk(VK_NULL_HANDLE)
// desired device properities
, m_VK_desired_queue_abilities(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT)
, m_VK_desired_sur_fmt{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }
, m_VK_desired_pre_mode_list{ VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR }
// device(phy and logical)
, m_VK_physical_device(VK_NULL_HANDLE)
, m_VK_picked_queue_family_id(-1)
, m_VK_device(VK_NULL_HANDLE)
// queue
, m_VK_present_queue(VK_NULL_HANDLE)
// swap chain
, m_VK_swap_chain(VK_NULL_HANDLE)
, m_VK_present_render_pass(VK_NULL_HANDLE)
, m_VK_acq_img_semaphore(VK_NULL_HANDLE)
, m_VK_present_semaphore(VK_NULL_HANDLE)
, m_VK_final_present_mode(VK_PRESENT_MODE_RANGE_SIZE_KHR)
// main command pool
, m_VK_main_cmd_pool(VK_NULL_HANDLE)
, m_VK_main_cmd_buffer(VK_NULL_HANDLE)
, m_VK_main_cmd_buf_fence(VK_NULL_HANDLE)
// configuration
, m_queue_size(3)
{
    SDLOG("New VulkanManager object.");
}

VulkanManager::~VulkanManager()
{
    SDLOG("Delete VulkanManager object.");
}

void VulkanManager::InitializeGraphicsSystem(const EventArg &i_arg)
{
    SDLOG("Initialize VulkanManager.");
    if (typeid(i_arg).hash_code() == typeid(VulkanCreationArg).hash_code()) {
        VulkanCreationArg vk_c_arg = dynamic_cast<const VulkanCreationArg&>(i_arg);

        m_VK_instance = vk_c_arg.m_instance;
        m_VK_surface = vk_c_arg.m_surface;

        if (m_VK_instance != nullptr) {
            //egl like
            InitializeDebugMessage();
            InitializePhysicalDevice();
            InitializeLogicDevice();
            InitializeSwapChain();
            //graphics
            InitializeCommandPoolAndBuffers();
            InitializePresentRenderPass();
            InitializeSCImageViewsAndFBs();
            //
            PrintSystemInformation();
        }
        else {
            throw std::runtime_error("VkInstance in arg is nullptr!!!");
        }
    }
}

void VulkanManager::ReleaseGraphicsSystem()
{
    SDLOG("Release VulkanManager.");

    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_VK_instance, "vkDestroyDebugReportCallbackEXT");

    if (vkDestroyDebugReportCallbackEXT != nullptr && m_VK_debug_report_cbk != VK_NULL_HANDLE) {
        vkDestroyDebugReportCallbackEXT(m_VK_instance, m_VK_debug_report_cbk, nullptr);
        m_VK_debug_report_cbk = VK_NULL_HANDLE;
    }
    else {
        SDLOG("failed to load set up destroy debug messenger function!");
    }

    if (m_VK_present_render_pass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(m_VK_device, m_VK_present_render_pass, nullptr);
        m_VK_present_render_pass = VK_NULL_HANDLE;
    }

    if (m_VK_main_cmd_buffer != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(m_VK_device, m_VK_main_cmd_pool, 1, &m_VK_main_cmd_buffer);
        m_VK_main_cmd_buffer = VK_NULL_HANDLE;
    }

    if (m_VK_main_cmd_pool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(m_VK_device, m_VK_main_cmd_pool, nullptr);
        m_VK_main_cmd_pool = VK_NULL_HANDLE;
    }

    if (m_VK_acq_img_semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(m_VK_device, m_VK_acq_img_semaphore, nullptr);
        m_VK_acq_img_semaphore = VK_NULL_HANDLE;
    }

    if (m_VK_present_semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(m_VK_device, m_VK_present_semaphore, nullptr);
        m_VK_present_semaphore = VK_NULL_HANDLE;
    }

    for (VkFramebuffer &fbo : m_VK_sc_image_fbs) {
        if (fbo != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(m_VK_device, fbo, nullptr);
        }
        fbo = VK_NULL_HANDLE;
    }
    m_VK_sc_image_fbs.clear();

    for (VkImageView &iv : m_VK_sc_image_views) {
        if (iv != VK_NULL_HANDLE) {
            vkDestroyImageView(m_VK_device, iv, nullptr);
            iv = VK_NULL_HANDLE;
        }
    }
    m_VK_sc_image_views.clear();

    if (m_VK_swap_chain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_VK_device, m_VK_swap_chain, nullptr);
        m_VK_swap_chain = VK_NULL_HANDLE;
    }

    if (m_VK_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_VK_device, nullptr);
        m_VK_device = VK_NULL_HANDLE;
    }

    m_VK_physical_device = VK_NULL_HANDLE;

    if (m_VK_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_VK_instance, m_VK_surface, nullptr);
        m_VK_surface = VK_NULL_HANDLE;
    }

    if (m_VK_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_VK_instance, nullptr);
        m_VK_instance = VK_NULL_HANDLE;
    }
}

//----------------------- Render Flow -----------------------
void VulkanManager::Resize(Size_ui32 i_w, Size_ui32 i_h)
{
    for (VkFramebuffer &fbo : m_VK_sc_image_fbs) {
        if (fbo != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(m_VK_device, fbo, nullptr);
        }
        fbo = VK_NULL_HANDLE;
    }
    m_VK_sc_image_fbs.clear();

    for (VkImageView &iv : m_VK_sc_image_views) {
        if (iv != VK_NULL_HANDLE) {
            vkDestroyImageView(m_VK_device, iv, nullptr);
            iv = VK_NULL_HANDLE;
        }
    }
    m_VK_sc_image_views.clear();

    if (m_VK_swap_chain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_VK_device, m_VK_swap_chain, nullptr);
        m_VK_swap_chain = VK_NULL_HANDLE;
    }

    InitializeSwapChain();
    InitializeSCImageViewsAndFBs();
}

void VulkanManager::RenderBegin()
{
    
}

void VulkanManager::RenderToScreen()
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
    render_area.extent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };

    VkRenderPassBeginInfo rp_begin_info = {};
    rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.pNext = nullptr;
    rp_begin_info.renderPass = m_VK_present_render_pass;
    rp_begin_info.framebuffer = m_VK_sc_image_fbs[image_index];
    rp_begin_info.renderArea = render_area;
    rp_begin_info.clearValueCount = 1;
    rp_begin_info.pClearValues = &ClearColor;

    vkCmdBeginRenderPass(m_VK_main_cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    //Render to screen (To do : Compose buffers.)

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

void VulkanManager::RenderEnd()
{
    //Reset command buffers in pool.
    if (vkResetCommandPool(m_VK_device, m_VK_main_cmd_pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS) {
        SDLOGW("reset command buffer in main pool failure!!!");
    }
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}