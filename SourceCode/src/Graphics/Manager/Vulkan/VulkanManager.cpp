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

const uint32_t VulkanManager::MaxImgAcqirationTime = 2000000000;

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
, m_VK_desired_pre_mode_list{VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR}
, m_VK_main_cmd_buffer_number(1)
// device(phy and logical)
, m_VK_physical_device(VK_NULL_HANDLE)
, m_VK_picked_queue_family_id(-1)
, m_VK_logic_device(VK_NULL_HANDLE)
, m_VK_present_queue(VK_NULL_HANDLE)
// swap chain
, m_VK_swap_chain(VK_NULL_HANDLE)
, m_VK_acq_img_semaphore(VK_NULL_HANDLE)
, m_VK_present_semaphore(VK_NULL_HANDLE)
, m_screen_size{0,0}
, m_VK_final_present_mode(VK_PRESENT_MODE_RANGE_SIZE_KHR)
// main command pool
, m_VK_main_cmd_pool(VK_NULL_HANDLE)
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
			InitializeCommandPoolAndBuffers();
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

	if (m_VK_main_cmd_buffers.size() > 0) {
		vkFreeCommandBuffers(m_VK_logic_device, m_VK_main_cmd_pool, static_cast<uint32_t>(m_VK_main_cmd_buffers.size()), m_VK_main_cmd_buffers.data());
		m_VK_main_cmd_buffers.clear();
	}

	if (m_VK_main_cmd_pool != VK_NULL_HANDLE) {
		vkDestroyCommandPool(m_VK_logic_device, m_VK_main_cmd_pool, nullptr);
		m_VK_main_cmd_pool = VK_NULL_HANDLE;
	}

	if (m_VK_acq_img_semaphore != VK_NULL_HANDLE) {
		vkDestroySemaphore(m_VK_logic_device, m_VK_acq_img_semaphore, nullptr);
		m_VK_acq_img_semaphore = VK_NULL_HANDLE;
	}

	if (m_VK_present_semaphore != VK_NULL_HANDLE) {
		vkDestroySemaphore(m_VK_logic_device, m_VK_present_semaphore, nullptr);
		m_VK_present_semaphore = VK_NULL_HANDLE;
	}

	for (auto &iv : m_VK_sc_image_views) {
		if (iv != VK_NULL_HANDLE) {
			vkDestroyImageView(m_VK_logic_device, iv, nullptr);
			iv = VK_NULL_HANDLE;
		}
	}
	m_VK_sc_image_views.clear();

	if (m_VK_swap_chain != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(m_VK_logic_device, m_VK_swap_chain, nullptr);
		m_VK_swap_chain = VK_NULL_HANDLE;
	}

	if (m_VK_logic_device != VK_NULL_HANDLE) {
		vkDestroyDevice(m_VK_logic_device, nullptr);
		m_VK_logic_device = VK_NULL_HANDLE;
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

void VulkanManager::RenderBegin()
{
	
}

void VulkanManager::RenderToScreen()
{
	//Get SwapChain Image.
	uint32_t image_index;

	VkResult result = vkAcquireNextImageKHR(
		m_VK_logic_device, m_VK_swap_chain, MaxImgAcqirationTime, 
		m_VK_acq_img_semaphore, nullptr, &image_index);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		SDLOGW("We can't get nxt swapchain image.");
		return;
	}
	//Begin Command Buffer
	VkCommandBufferBeginInfo cmd_buf_c_info = {};
	cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmd_buf_c_info.pNext = nullptr;
	cmd_buf_c_info.pInheritanceInfo = nullptr;
	cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(m_VK_main_cmd_buffers[0], &cmd_buf_c_info) != VK_SUCCESS) {
		SDLOGW("We can't begin command buffer(%x)!!!", m_VK_main_cmd_buffers[0]);
		return;
	}
	//

	//End Command Buffer
	if (vkEndCommandBuffer(m_VK_main_cmd_buffers[0]) != VK_SUCCESS) {
		SDLOGW("We can't end command buffer(%x)!!!", m_VK_main_cmd_buffers[0]);
		return;
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

	if (vkQueuePresentKHR(m_VK_present_queue, &p_info) != VK_SUCCESS) {
		SDLOGW("We can't present image by queue.");
		return;
	}

}

void VulkanManager::RenderEnd()
{

}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}