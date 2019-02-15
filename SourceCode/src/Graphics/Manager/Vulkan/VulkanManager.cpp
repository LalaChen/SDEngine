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

#include <vulkan/vulkan.h>

#include "Vulkan_Utils.h"
#include "VulkanCreationArg.h"
#include "LogManager.h"
#include "VulkanManager.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

const std::vector<const char*>& VulkanManager::GetValidLayers()
{
	return ValidLayers;
}

std::vector<const char*> VulkanManager::ValidLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

std::vector<const char*> VulkanManager::NecessaryExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


//------------------------------------------------

VulkanManager::VulkanManager()
: m_VK_instance(VK_NULL_HANDLE)
, m_VK_surface(VK_NULL_HANDLE)
, m_VK_debug_report_cbk(VK_NULL_HANDLE)
, m_VK_physical_device(VK_NULL_HANDLE)
, m_VK_logic_device(VK_NULL_HANDLE)
, m_VK_graphics_queue(VK_NULL_HANDLE)
, m_VK_present_queue(VK_NULL_HANDLE)
, m_VK_swap_chain(VK_NULL_HANDLE)
, m_scimg_format(0)
, m_viewport_width(0)
, m_viewport_height(0)
{
	m_library = GraphicsLibrary_Vulkan;
	SDLOG("New VulkanManager object.");
}

VulkanManager::~VulkanManager()
{
	SDLOG("Delete VulkanManager object.");
}

void VulkanManager::InitializeGraphicsSystem(const EventArg &i_arg)
{
	SDLOG("Initialize VulkanManager.");
	if (typeid(i_arg).hash_code() == typeid(VulkanCreationArg).hash_code())
	{
		VulkanCreationArg vk_c_arg = dynamic_cast<const VulkanCreationArg&>(i_arg);
		
		m_VK_instance = vk_c_arg.m_instance;
		m_VK_surface = vk_c_arg.m_surface;

		if (m_VK_instance != nullptr) {
			InitializeDebugMessage();
			InitializePhysicalDevice();
			InitializeLogicDevice();
			InitializeSwapChain();
			InitializeImageViews();
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
		(PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr((VkInstance)m_VK_instance, "vkDestroyDebugReportCallbackEXT");

	if (vkDestroyDebugReportCallbackEXT != nullptr && (VkDebugReportCallbackEXT)m_VK_debug_report_cbk != VK_NULL_HANDLE) {
		vkDestroyDebugReportCallbackEXT((VkInstance)m_VK_instance, (VkDebugReportCallbackEXT)m_VK_debug_report_cbk, nullptr);
		m_VK_debug_report_cbk = VK_NULL_HANDLE;
	}
	else {
		SDLOG("failed to load set up destroy debug messenger function!");
	}

	vkDestroySwapchainKHR((VkDevice)m_VK_logic_device, (VkSwapchainKHR)m_VK_swap_chain, nullptr);
	vkDestroySurfaceKHR((VkInstance)m_VK_instance, (VkSurfaceKHR)m_VK_surface, nullptr);
	vkDestroyInstance((VkInstance)m_VK_instance, nullptr);
	m_VK_instance = VK_NULL_HANDLE;

	m_VK_physical_device = VK_NULL_HANDLE;
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}