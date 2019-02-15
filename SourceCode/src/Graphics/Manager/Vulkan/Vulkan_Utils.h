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

#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

class SDENGINE_CLASS SwapChainDetails
{
public:
	explicit SwapChainDetails(VkPhysicalDevice i_p_device, VkSurfaceKHR i_surface);
public:
	VkSurfaceCapabilitiesKHR m_caps;
	std::vector<VkSurfaceFormatKHR> m_formats;
	std::vector<VkPresentModeKHR> m_present_modes;
};

class SDENGINE_CLASS QueueFamilyIndices
{
public:
	explicit QueueFamilyIndices(VkPhysicalDevice i_p_device, VkSurfaceKHR i_surface);
public:
	int m_graphics_fid;
	int m_present_fid;
	std::vector<VkQueueFamilyProperties> m_queue_families;
};

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugReportFlagsEXT i_flags,
	VkDebugReportObjectTypeEXT i_object_type,
	uint64_t i_obj,
	size_t i_location,
	int32_t i_code,
	const char* i_layer_prefix,
	const char* i_msg,
	void* i_use_data);

SDENGINE_API bool CheckDeviceExtensionSupport(
	VkPhysicalDevice i_p_device,
	std::vector<const char*> i_necessary_exts
);

SDENGINE_API VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR>& i_avaible_fmts);

SDENGINE_API VkPresentModeKHR ChooseSwapPresentMode(
	const std::vector<VkPresentModeKHR> &i_avaible_present_modes);

SDENGINE_API VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &i_capabilities);

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}