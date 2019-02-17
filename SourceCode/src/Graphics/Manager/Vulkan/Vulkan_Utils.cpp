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

#include <list>
#include <algorithm>

#include "LogManager.h"
#include "Vulkan_Utils.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

SwapChainDetails::SwapChainDetails(VkPhysicalDevice i_p_device, VkSurfaceKHR i_surface)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(i_p_device, i_surface, &m_caps);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(i_p_device, i_surface, &format_count, nullptr);

	if (format_count != 0) {
		m_formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(i_p_device, i_surface, &format_count, m_formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(i_p_device, i_surface, &present_mode_count, nullptr);

	if (present_mode_count != 0) {
		m_present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(i_p_device, i_surface, &present_mode_count, m_present_modes.data());
	}
}

QueueFamilyIndices::QueueFamilyIndices(VkPhysicalDevice i_p_device, VkSurfaceKHR i_surface)
: m_graphics_fid(-1)
, m_present_fid(-1)
{
	static const VkFlags total_support_queue =
		VK_QUEUE_GRAPHICS_BIT |
		VK_QUEUE_COMPUTE_BIT |
		VK_QUEUE_TRANSFER_BIT |
		VK_QUEUE_SPARSE_BINDING_BIT;
	//I want all!!!

    uint32_t queue_families_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties((VkPhysicalDevice)i_p_device, &queue_families_count, nullptr);
	m_queue_families.resize(queue_families_count);
	vkGetPhysicalDeviceQueueFamilyProperties((VkPhysicalDevice)i_p_device, &queue_families_count, m_queue_families.data());

	for (uint32_t fam_id = 0; fam_id < m_queue_families.size(); fam_id++) {
	
		VkBool32 present_support = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR((VkPhysicalDevice)i_p_device, fam_id, (VkSurfaceKHR)i_surface, &present_support);

		if (m_queue_families[fam_id].queueFlags == total_support_queue &&
			m_graphics_fid == -1 &&
			m_queue_families[fam_id].queueCount > 0) {
			m_graphics_fid = fam_id;
		}

		if (present_support == VK_TRUE) {
			m_present_fid = fam_id;
		}

		if (m_graphics_fid != -1 && m_present_fid != -1) {
			break;
		}
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugReportFlagsEXT i_flags,
	VkDebugReportObjectTypeEXT i_object_type,
	uint64_t i_obj,
	size_t i_location,
	int32_t i_code,
	const char* i_layer_prefix,
	const char* i_msg,
	void* i_use_data)
{
	if (i_flags == VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		SDLOG("Vulkan Verbose : %s", i_msg);
	}
	else if (i_flags == VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		SDLOGD("Vulkan Info : %s", i_msg);
	}
	else if (i_flags == VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		SDLOGW("Vulkan Warning : %s", i_msg);
	}
	else if (i_flags == VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		SDLOGW("Vulkan Performance Warning : %s", i_msg);
	}
	else {
		SDLOGE("Vulkan Error : %s", i_msg);
	}

	return VK_FALSE;
}

bool CheckDeviceExtensionSupport(
	VkPhysicalDevice i_p_device,
	std::vector<const char*> i_necessary_exts
)
{
	uint32_t avaible_ext_counts;
	vkEnumerateDeviceExtensionProperties(i_p_device, nullptr, &avaible_ext_counts, nullptr);

	std::vector<VkExtensionProperties> avaible_exts(avaible_ext_counts);
	vkEnumerateDeviceExtensionProperties(i_p_device, nullptr, &avaible_ext_counts, avaible_exts.data());

	std::list<const char*> check_ext_names(i_necessary_exts.begin(), i_necessary_exts.end());

	for (VkExtensionProperties &avaible_ext : avaible_exts) {
		
		for (std::list<const char*>::iterator c_iter = check_ext_names.begin();
			c_iter != check_ext_names.end(); ){
			if (strcmp(avaible_ext.extensionName, (*c_iter)) == 0) {
				c_iter = check_ext_names.erase(c_iter);
				break;
			}
			else {
				c_iter++;
			}
		}
	}

	return check_ext_names.empty();
}

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &i_avaible_fmts)
{
	if (i_avaible_fmts.size() == 1 && i_avaible_fmts[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& fmt : i_avaible_fmts) {
		if (fmt.format == VK_FORMAT_B8G8R8A8_UNORM && fmt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return fmt;
		}
	}

	return i_avaible_fmts[0];
}

VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &i_avaible_present_modes)
{
	VkPresentModeKHR best_modes[VK_PRESENT_MODE_RANGE_SIZE_KHR] = {
		VK_PRESENT_MODE_FIFO_KHR, //Vsync
		VK_PRESENT_MODE_MAILBOX_KHR, //Triple buffers.
		VK_PRESENT_MODE_FIFO_RELAXED_KHR,//Vsync but will cause tearing.
		VK_PRESENT_MODE_IMMEDIATE_KHR //will cause tearing.
	};

	for (int mode_id = 0; mode_id < VK_PRESENT_MODE_RANGE_SIZE_KHR; mode_id++) {
		for (const VkPresentModeKHR& p_mode : i_avaible_present_modes) {
			if (best_modes[mode_id] == p_mode) {
				return best_modes[mode_id];
			}
		}
	}

	return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &i_capabilities)
{
	if (i_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return i_capabilities.currentExtent;
	}
	else {
		VkExtent2D actual_extent = { 0, 0 };

		actual_extent.width = std::max(i_capabilities.minImageExtent.width, std::min(i_capabilities.maxImageExtent.width, actual_extent.width));
		actual_extent.height = std::max(i_capabilities.minImageExtent.height, std::min(i_capabilities.maxImageExtent.height, actual_extent.height));

	    return actual_extent;
	}
}


//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}