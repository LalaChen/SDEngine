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
#include "VulkanWrapper.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

const VkClearValue VulkanManager::sClearColor = { {0.2f, 0.5f, 0.8f, 1.0f} };
const VkClearValue VulkanManager::sClearDepth = { {1.0f, 0} };

VkBool32 VulkanManager::ConvertBoolean(bool flag)
{
    if (flag == true) {
        return VK_TRUE;
    }
    else {
        return VK_FALSE;
    }
}

//------------------------------------------------
VulkanManager::VulkanManager()
: m_desired_queue_abilities{ VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT }
// application created.
, m_ins_handle(VK_NULL_HANDLE)
, m_sur_handle(VK_NULL_HANDLE)
// debug cbk
, m_debug_rp_cbk(VK_NULL_HANDLE)
// desired device properities
, m_final_q_abi_flag(0)
, m_final_sur_format{VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }
// device(phy and logical)
, m_phy_device_handle(VK_NULL_HANDLE)
, m_device_handle(VK_NULL_HANDLE)
, m_final_queue_fam_id(-1)
, m_present_q_handle(VK_NULL_HANDLE)
// main command pool
, m_main_cp_handle(VK_NULL_HANDLE)
, m_main_cb_handle(VK_NULL_HANDLE)
, m_main_cb_fence_handle(VK_NULL_HANDLE)
{
    SDLOG("New VulkanManager object.");
    if (InitVulkanFunction() == false) {
        SDLOGE("Initialize vulkan function ptrs failure.");
    }
    else {
        SDLOG("Load vulkan library func successfully.");
    }

    m_vulkan_config.LoadFromFile("Common/Configs/VKConfig.json");

    m_swapchain = new VulkanSwapchain("VulkanSwapchain", m_graphics_identity_getter);
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

        m_ins_handle = vk_c_arg.m_instance;
        m_sur_handle = vk_c_arg.m_surface;

        if (m_ins_handle != nullptr) {
            //egl like
            InitializeDebugMessage();
            InitializePhysicalDevice();
            InitializeSettings();
            InitializeDevice();
            InitializeCommandPoolAndBuffers();
            //graphics
            InitializeSwapChain();
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

    m_swapchain.Reset();

    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_ins_handle, "vkDestroyDebugReportCallbackEXT");

    if (vkDestroyDebugReportCallbackEXT != nullptr && m_debug_rp_cbk != VK_NULL_HANDLE) {
        vkDestroyDebugReportCallbackEXT(m_ins_handle, m_debug_rp_cbk, nullptr);
        m_debug_rp_cbk = VK_NULL_HANDLE;
    }
    else {
        SDLOG("failed to load set up destroy debug messenger function!");
    }

    if (m_main_cb_handle != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(m_device_handle, m_main_cp_handle, 1, &m_main_cb_handle);
        m_main_cb_handle = VK_NULL_HANDLE;
    }

    if (m_main_cp_handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(m_device_handle, m_main_cp_handle, nullptr);
        m_main_cp_handle = VK_NULL_HANDLE;
    }

    if (m_device_handle != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device_handle, nullptr);
        m_device_handle = VK_NULL_HANDLE;
    }

    m_phy_device_handle = VK_NULL_HANDLE;

    if (m_sur_handle != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_ins_handle, m_sur_handle, nullptr);
        m_sur_handle = VK_NULL_HANDLE;
    }

    if (m_ins_handle != VK_NULL_HANDLE) {
        vkDestroyInstance(m_ins_handle, nullptr);
        m_ins_handle = VK_NULL_HANDLE;
    }
}

//----------------------- Render Flow -----------------------
void VulkanManager::Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h)
{
    m_swapchain.Reset();
    m_swapchain = new VulkanSwapchain("VulkanSwapchain", m_graphics_identity_getter);

    VkSurfaceKHR new_surface_handle = reinterpret_cast<VkSurfaceKHR>(i_ns_handle);
    if (m_sur_handle != new_surface_handle && new_surface_handle != SD_NULL_HANDLE) {
        SDLOG("Refresh surface for resize.");
        vkDestroySurfaceKHR(m_ins_handle, m_sur_handle, nullptr);
        m_sur_handle = new_surface_handle;
    }
    else {
        SDLOG("Surface no change. We don't need to refresh surface for resize.");
    }

    InitializeSwapChain();
}

Resolution VulkanManager::GetScreenResolution() const
{
    if (m_swapchain.IsNull() == false) {
        return SD_SREF(m_swapchain).GetResolution();
    }
    else {
        return Resolution();
    }
}

void VulkanManager::RenderBegin()
{

}

void VulkanManager::RenderEnd()
{
    //Reset command buffers in pool.
    if (vkResetCommandPool(m_device_handle, m_main_cp_handle, 0) != VK_SUCCESS) {
        SDLOGW("reset command buffer in main pool failure!!!");
    }
}

void VulkanManager::GetDesiredVulkanValidLayers(std::vector<const char*> &io_valid_layers) const
{
    for (const std::string &layer_name : m_vulkan_config.m_desire_valid_layers) {
        if (layer_name.empty() == false) {
            io_valid_layers.push_back(layer_name.c_str());
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________