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
// debug cbk
, m_VK_debug_report_cbk(VK_NULL_HANDLE)
// desired device properities
, m_VK_desired_queue_abilities(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT)
, m_VK_desired_sur_fmt{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }
, m_Vk_desired_pre_mode_list{VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR}
// device(phy and logical)
, m_VK_physical_device(VK_NULL_HANDLE)
, m_Vk_picked_queue_family_id(-1)
, m_VK_logic_device(VK_NULL_HANDLE)
, m_VK_present_queue(VK_NULL_HANDLE)
// swap chain
, m_VK_swap_chain(VK_NULL_HANDLE)
, m_screen_size{0,0}
, m_final_present_mode(VK_PRESENT_MODE_RANGE_SIZE_KHR)
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
		(PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_VK_instance, "vkDestroyDebugReportCallbackEXT");

	if (vkDestroyDebugReportCallbackEXT != nullptr && m_VK_debug_report_cbk != VK_NULL_HANDLE) {
		vkDestroyDebugReportCallbackEXT(m_VK_instance, m_VK_debug_report_cbk, nullptr);
		m_VK_debug_report_cbk = VK_NULL_HANDLE;
	}
	else {
		SDLOG("failed to load set up destroy debug messenger function!");
	}

	for (auto &iv : m_VK_sc_image_views) {
		vkDestroyImageView(m_VK_logic_device, iv, nullptr);
	}

	vkDestroySwapchainKHR(m_VK_logic_device, m_VK_swap_chain, nullptr);
	vkDestroySurfaceKHR(m_VK_instance, m_VK_surface, nullptr);
	vkDestroyInstance(m_VK_instance, nullptr);
	m_VK_instance = VK_NULL_HANDLE;

	m_VK_physical_device = VK_NULL_HANDLE;
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}