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

#include "OpenXRVulkanManager.h"

#include "Exception.h"
#include "ImageAspect_Vulkan.h"
#include "VulkanCreationArg.h"
#include "VulkanWrapper.h"
#include "LogManager.h"
#include "ManagerIdentity.h"
#include "OpenXRAPIManager.h"
#include "OpenXRSwapchain.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;

______________SD_START_OPENXR_NAMESPACE______________

OpenXRVulkanManager::OpenXRVulkanManager()
: VulkanManager()
{
}

OpenXRVulkanManager::~OpenXRVulkanManager()
{
}

void OpenXRVulkanManager::InitializeGraphicsSystemImpl(const EventArg &i_arg)
{
    SDLOG("Initialize OpenXRVulkanManager.");

    if (typeid(i_arg).hash_code() == typeid(VulkanCreationArg).hash_code()) {
        VulkanCreationArg vk_c_arg = dynamic_cast<const VulkanCreationArg&>(i_arg);

        m_instance = vk_c_arg.m_instance;
        m_surface = vk_c_arg.m_surface;

        if (m_instance != nullptr) {
            //egl like
            InitializeVulkanEnvironment();
            InitializeSettings();
            InitializeGraphicsQueues();
            InitializeVulkanSurface();
            //graphics
            InitializeSession();
            //
            PrintSystemInformation();
            //
        }
        else {
            throw std::runtime_error("VkInstance in arg is nullptr!!!");
        }
    }
}

void OpenXRVulkanManager::ReleaseGraphicsSystemImpl()
{
    SDLOG("Release OpenXRVulkanManager.");
    VulkanManager::ReleaseGraphicsSystem();
}

void OpenXRVulkanManager::InitializeSession()
{
    m_binding.type = XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR;
    m_binding.instance = m_instance;
    m_binding.physicalDevice = m_phy_device;
    m_binding.queueFamilyIndex = SD_SREF(m_present_queue).GetQueueFamilyID();
    m_binding.queueIndex = SD_SREF(m_present_queue).GetQueueID();

    XrSessionCreateInfo info = {};
    info.type = XR_TYPE_SESSION_CREATE_INFO;
    info.next = &m_binding;

    OpenXRAPIManager::GetRef().InitializeSession(info);
}

void OpenXRVulkanManager::InitializeMainSwapchain()
{
    Resolution recommend_size = OpenXRAPIManager::GetRef().GetRecommendSize();
    m_swapchain = new OpenXRSwapchain("OpenXRSwapchain", m_present_queue, recommend_size);
    SD_SREF(m_swapchain).Initialize();
}

void OpenXRVulkanManager::CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity)
{
    VkPhysicalDevice &phy_device   = reinterpret_cast<VkPhysicalDevice&>(io_identity.m_phy_device);
    VkDevice         &device       = reinterpret_cast<VkDevice&>(io_identity.m_device);
    phy_device = m_phy_device;
    device     = m_device;
    io_identity.m_layer_size = 2;
    io_identity.SetValid();
}

void OpenXRVulkanManager::GetReadyTextureOfSwapchain(
    const GraphicsSwapchainIdentity &i_identity,
    const GraphicsSemaphoreWeakReferenceObject &i_acq_sema,
    uint32_t &io_idx)
{
    XrSwapchain swapchain = reinterpret_cast<XrSwapchain>(i_identity.m_handle);
    XrResult result;
    result = OpenXRAPIManager::GetRef().GetReadyTextureOfSwapchain(swapchain, io_idx);
    if (result != XR_SUCCESS) {
        throw ReasonableException("Get Image idx failure");
    }
}

_______________SD_END_OPENXR_NAMESPACE_______________