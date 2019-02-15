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
#include <set>
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

void VulkanManager::InitializeDebugMessage()
{
	SDLOG("--- Vulkan initialize debug message.");

	VkDebugReportCallbackCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
	create_info.pfnCallback = VulkanDebugCallback;

	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
		(PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr((VkInstance)m_VK_instance, "vkCreateDebugReportCallbackEXT");
	if (vkCreateDebugReportCallbackEXT != nullptr) {
		VkResult res = vkCreateDebugReportCallbackEXT((VkInstance)m_VK_instance, &create_info, nullptr, (VkDebugReportCallbackEXT*)&m_VK_debug_report_cbk);
		if (res != VK_SUCCESS) {
			SDLOG("failed create debug messenger!");
		}
	}
	else {
		SDLOG("failed to load set up debug messenger function!");
	}
}

void VulkanManager::InitializePhysicalDevice()
{
	SDLOG("--- Vulkan initialize physical device.(Select graphics card)");

	uint32_t dev_count = 0;
	vkEnumeratePhysicalDevices((VkInstance)m_VK_instance, &dev_count, nullptr);
	if (dev_count == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	std::vector<VkPhysicalDevice> devices(dev_count);
	vkEnumeratePhysicalDevices((VkInstance)m_VK_instance, &dev_count, devices.data());
	uint32_t pickup_dev_score = 0;
	for (VkPhysicalDevice& device : devices) {
		VkPhysicalDeviceProperties dev_property;
		vkGetPhysicalDeviceProperties(device, &dev_property);
		SDLOGD("--- DeviceID[%d] name : %s DevType : %d.", dev_property.deviceID, dev_property.deviceName, dev_property.deviceType);
		VkPhysicalDeviceFeatures dev_features;
		uint32_t dev_score = 0;
		vkGetPhysicalDeviceFeatures(device, &dev_features);
		SDLOGD("------ Feature[alphaToOne] : %d", dev_features.alphaToOne);
		dev_score += dev_features.alphaToOne;
		SDLOGD("------ Feature[depthBiasClamp] : %d", dev_features.depthBiasClamp);
		dev_score += dev_features.depthBiasClamp;
		SDLOGD("------ Feature[depthBounds] : %d", dev_features.depthBounds);
		dev_score += dev_features.depthBiasClamp;
		SDLOGD("------ Feature[depthClamp] : %d", dev_features.depthClamp);
		dev_score += dev_features.depthClamp;
		SDLOGD("------ Feature[drawIndirectFirstInstance] : %d", dev_features.drawIndirectFirstInstance);
		dev_score += dev_features.drawIndirectFirstInstance;
		SDLOGD("------ Feature[dualSrcBlend] : %d", dev_features.dualSrcBlend);
		dev_score += dev_features.dualSrcBlend;
		SDLOGD("------ Feature[fillModeNonSolid] : %d", dev_features.fillModeNonSolid);
		dev_score += dev_features.fillModeNonSolid;
		SDLOGD("------ Feature[fragmentStoresAndAtomics] : %d", dev_features.fragmentStoresAndAtomics);
		dev_score += dev_features.fragmentStoresAndAtomics;
		SDLOGD("------ Feature[fullDrawIndexUint32] : %d", dev_features.fullDrawIndexUint32);
		dev_score += dev_features.fullDrawIndexUint32;
		SDLOGD("------ Feature[tessellationShader] : %d", dev_features.tessellationShader);
		dev_score += dev_features.tessellationShader;
		SDLOGD("------ Feature[geometryShader] : %d", dev_features.geometryShader);
		dev_score += dev_features.geometryShader;
		SDLOGD("------ Feature[imageCubeArray] : %d", dev_features.imageCubeArray);
		dev_score += dev_features.imageCubeArray;
		SDLOGD("------ Feature[independentBlend] : %d", dev_features.independentBlend);
		dev_score += dev_features.independentBlend;
		SDLOGD("------ Feature[inheritedQueries] : %d", dev_features.inheritedQueries);
		dev_score += dev_features.inheritedQueries;
		SDLOGD("------ Feature[largePoints] : %d", dev_features.largePoints);
		dev_score += dev_features.largePoints;
		SDLOGD("------ Feature[logicOp] : %d", dev_features.logicOp);
		dev_score += dev_features.logicOp;
		SDLOGD("------ Feature[multiDrawIndirect] : %d", dev_features.multiDrawIndirect);
		dev_score += dev_features.multiDrawIndirect;
		SDLOGD("------ Feature[multiViewport] : %d", dev_features.multiViewport);
		dev_score += dev_features.multiViewport;
		SDLOGD("------ Feature[occlusionQueryPrecise] : %d", dev_features.occlusionQueryPrecise);
		dev_score += dev_features.occlusionQueryPrecise;
		SDLOGD("------ Feature[pipelineStatisticsQuery] : %d", dev_features.pipelineStatisticsQuery);
		dev_score += dev_features.pipelineStatisticsQuery;
		SDLOGD("------ Feature[robustBufferAccess] : %d", dev_features.robustBufferAccess);
		dev_score += dev_features.robustBufferAccess;
		SDLOGD("------ Feature[samplerAnisotropy] : %d", dev_features.samplerAnisotropy);
		dev_score += dev_features.samplerAnisotropy;
		SDLOGD("------ Feature[sampleRateShading] : %d", dev_features.sampleRateShading);
		dev_score += dev_features.sampleRateShading;
		SDLOGD("------ Feature[shaderClipDistance] : %d", dev_features.shaderClipDistance);
		dev_score += dev_features.shaderClipDistance;
		SDLOGD("------ Feature[shaderCullDistance] : %d", dev_features.shaderCullDistance);
		dev_score += dev_features.shaderCullDistance;
		SDLOGD("------ Feature[shaderFloat64] : %d", dev_features.shaderFloat64);
		dev_score += dev_features.shaderFloat64;
		SDLOGD("------ Feature[shaderInt16] : %d", dev_features.shaderInt16);
		dev_score += dev_features.shaderInt16;
		SDLOGD("------ Feature[shaderInt64] : %d", dev_features.shaderInt64);
		dev_score += dev_features.shaderInt64;
		SDLOGD("------ Feature[shaderImageGatherExtended] : %d", dev_features.shaderImageGatherExtended);
		dev_score += dev_features.shaderImageGatherExtended;
		SDLOGD("------ Feature[shaderResourceMinLod] : %d", dev_features.shaderResourceMinLod);
		dev_score += dev_features.shaderResourceMinLod;
		SDLOGD("------ Feature[shaderResourceResidency] : %d", dev_features.shaderResourceResidency);
		dev_score += dev_features.shaderResourceResidency;
		SDLOGD("------ Feature[shaderSampledImageArrayDynamicIndexing] : %d", dev_features.shaderSampledImageArrayDynamicIndexing);
		dev_score += dev_features.shaderSampledImageArrayDynamicIndexing;
		SDLOGD("------ Feature[shaderStorageBufferArrayDynamicIndexing] : %d", dev_features.shaderStorageBufferArrayDynamicIndexing);
		dev_score += dev_features.shaderStorageBufferArrayDynamicIndexing;
		SDLOGD("------ Feature[shaderStorageImageArrayDynamicIndexing] : %d", dev_features.shaderStorageImageArrayDynamicIndexing);
		dev_score += dev_features.shaderStorageImageArrayDynamicIndexing;
		SDLOGD("------ Feature[shaderStorageImageExtendedFormats] : %d", dev_features.shaderStorageImageExtendedFormats);
		dev_score += dev_features.shaderStorageImageExtendedFormats;
		SDLOGD("------ Feature[shaderStorageImageMultisample] : %d", dev_features.shaderStorageImageMultisample);
		dev_score += dev_features.shaderStorageImageMultisample;
		SDLOGD("------ Feature[shaderStorageImageReadWithoutFormat] : %d", dev_features.shaderStorageImageReadWithoutFormat);
		dev_score += dev_features.shaderStorageImageReadWithoutFormat;
		SDLOGD("------ Feature[shaderStorageImageWriteWithoutFormat] : %d", dev_features.shaderStorageImageWriteWithoutFormat);
		dev_score += dev_features.shaderStorageImageWriteWithoutFormat;
		SDLOGD("------ Feature[shaderTessellationAndGeometryPointSize] : %d", dev_features.shaderTessellationAndGeometryPointSize);
		dev_score += dev_features.shaderTessellationAndGeometryPointSize;
		SDLOGD("------ Feature[shaderUniformBufferArrayDynamicIndexing] : %d", dev_features.shaderUniformBufferArrayDynamicIndexing);
		dev_score += dev_features.shaderUniformBufferArrayDynamicIndexing;
		SDLOGD("------ Feature[sparseBinding] : %d", dev_features.sparseBinding);
		dev_score += dev_features.sparseBinding;
		SDLOGD("------ Feature[sparseResidency2Samples] : %d", dev_features.sparseResidency2Samples);
		dev_score += dev_features.sparseResidency2Samples;
		SDLOGD("------ Feature[sparseResidency4Samples] : %d", dev_features.sparseResidency4Samples);
		dev_score += dev_features.sparseResidency4Samples;
		SDLOGD("------ Feature[sparseResidency8Samples] : %d", dev_features.sparseResidency8Samples);
		dev_score += dev_features.sparseResidency8Samples;
		SDLOGD("------ Feature[sparseResidency16Samples] : %d", dev_features.sparseResidency16Samples);
		dev_score += dev_features.sparseResidency16Samples;
		SDLOGD("------ Feature[sparseResidencyAliased] : %d", dev_features.sparseResidencyAliased);
		dev_score += dev_features.sparseResidencyAliased;
		SDLOGD("------ Feature[sparseResidencyBuffer] : %d", dev_features.sparseResidencyBuffer);
		dev_score += dev_features.sparseResidencyBuffer;
		SDLOGD("------ Feature[sparseResidencyImage2D] : %d", dev_features.sparseResidencyImage2D);
		dev_score += dev_features.sparseResidencyImage2D;
		SDLOGD("------ Feature[sparseResidencyImage3D] : %d", dev_features.sparseResidencyImage3D);
		dev_score += dev_features.sparseResidencyImage3D;
		SDLOGD("------ Feature[textureCompressionASTC_LDR] : %d", dev_features.textureCompressionASTC_LDR);
		dev_score += dev_features.textureCompressionASTC_LDR;
		SDLOGD("------ Feature[textureCompressionBC] : %d", dev_features.textureCompressionBC);
		dev_score += dev_features.textureCompressionBC;
		SDLOGD("------ Feature[textureCompressionETC2] : %d", dev_features.textureCompressionETC2);
		dev_score += dev_features.textureCompressionETC2;
		SDLOGD("------ Feature[variableMultisampleRate] : %d", dev_features.variableMultisampleRate);
		dev_score += dev_features.variableMultisampleRate;
		SDLOGD("------ Feature[vertexPipelineStoresAndAtomics] : %d", dev_features.vertexPipelineStoresAndAtomics);
		dev_score += dev_features.vertexPipelineStoresAndAtomics;
		//
		bool exts_support = CheckDeviceExtensionSupport(device, VulkanManager::NecessaryExtensions);
		//Calculate device score.
		if (dev_score > pickup_dev_score && exts_support == true) {
			m_VK_physical_device = device;
			pickup_dev_score = dev_score;
		}
	}
	VkPhysicalDeviceProperties picked_dev_props;
	vkGetPhysicalDeviceProperties((VkPhysicalDevice)m_VK_physical_device, &picked_dev_props);
	SDLOG("Final choose dev : %s(%d:%d:%d) driver version : %d apiversion : %d, score : %d", 
		picked_dev_props.deviceName,
		picked_dev_props.vendorID, picked_dev_props.deviceID, picked_dev_props.deviceType,
		picked_dev_props.driverVersion,
		picked_dev_props.apiVersion,
		pickup_dev_score);

	//
	VkPhysicalDeviceProperties picked_phy_device_prop = {};
	vkGetPhysicalDeviceProperties((VkPhysicalDevice)m_VK_physical_device, &picked_phy_device_prop);
	//
	SDLOGD("Maximum Colot Attachment : %d.", picked_phy_device_prop.limits.maxColorAttachments);
}

void VulkanManager::InitializeLogicDevice()
{
	SDLOG("--- Vulkan initialize device.(Create devices queue)");
	SDLOG("------ Get queue family properties.");
	QueueFamilyIndices fam_idices((VkPhysicalDevice)m_VK_physical_device, (VkSurfaceKHR)m_VK_surface);

	for (uint32_t fam_id = 0; fam_id < fam_idices.m_queue_families.size(); fam_id++) {
		SDLOGD("Queue TimeStamp[%d] Flag(%x) Count:%d Extent3D(%d,%d,%d) ",
			fam_idices.m_queue_families[fam_id].timestampValidBits,
			fam_idices.m_queue_families[fam_id].queueFlags,
			fam_idices.m_queue_families[fam_id].queueCount,
			fam_idices.m_queue_families[fam_id].minImageTransferGranularity.width,
			fam_idices.m_queue_families[fam_id].minImageTransferGranularity.height,
			fam_idices.m_queue_families[fam_id].minImageTransferGranularity.depth);
	}

	SDLOG("Selected Graphics Queue Family : Fid[%d] Flag(%x) Count:%d Extent3D(%d,%d,%d) ",
		fam_idices.m_graphics_fid,
		fam_idices.m_queue_families[fam_idices.m_graphics_fid].queueFlags,
		fam_idices.m_queue_families[fam_idices.m_graphics_fid].queueCount,
		fam_idices.m_queue_families[fam_idices.m_graphics_fid].minImageTransferGranularity.width,
		fam_idices.m_queue_families[fam_idices.m_graphics_fid].minImageTransferGranularity.height,
		fam_idices.m_queue_families[fam_idices.m_graphics_fid].minImageTransferGranularity.depth);

	SDLOG("Selected Present Queue Family : Fid[%d] Flag(%x) Count:%d Extent3D(%d,%d,%d) ",
		fam_idices.m_present_fid,
		fam_idices.m_queue_families[fam_idices.m_present_fid].queueFlags,
		fam_idices.m_queue_families[fam_idices.m_present_fid].queueCount,
		fam_idices.m_queue_families[fam_idices.m_present_fid].minImageTransferGranularity.width,
		fam_idices.m_queue_families[fam_idices.m_present_fid].minImageTransferGranularity.height,
		fam_idices.m_queue_families[fam_idices.m_present_fid].minImageTransferGranularity.depth);

	//Create queues.
	float quene_priority = 1.0f;
	std::vector< VkDeviceQueueCreateInfo > queue_c_infos;
	if (fam_idices.m_graphics_fid >= 0) {
		VkDeviceQueueCreateInfo queue_c_info = {};
		queue_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_c_info.queueFamilyIndex = fam_idices.m_graphics_fid;
		queue_c_info.queueCount = 1;
		queue_c_info.pQueuePriorities = &quene_priority;
		queue_c_infos.push_back(queue_c_info);
	}
	if (fam_idices.m_present_fid >= 0 && fam_idices.m_graphics_fid != fam_idices.m_present_fid) {
		VkDeviceQueueCreateInfo queue_c_info = {};
		queue_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_c_info.queueFamilyIndex = fam_idices.m_present_fid;
		queue_c_info.queueCount = 1;
		queue_c_info.pQueuePriorities = &quene_priority;
	}

	VkPhysicalDeviceFeatures dev_features = {};
	vkGetPhysicalDeviceFeatures((VkPhysicalDevice)m_VK_physical_device, &dev_features);

	VkDeviceCreateInfo logic_dev_c_info = {};
	logic_dev_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	logic_dev_c_info.pQueueCreateInfos = queue_c_infos.data();
	logic_dev_c_info.queueCreateInfoCount = static_cast<uint32_t>(queue_c_infos.size());

	logic_dev_c_info.pEnabledFeatures = &dev_features;

	logic_dev_c_info.ppEnabledExtensionNames = NecessaryExtensions.data();
	logic_dev_c_info.enabledExtensionCount = static_cast<uint32_t>(NecessaryExtensions.size());

#ifdef _NDEBUG
	logic_dev_c_info.enabledLayerCount = 0;
#else
	logic_dev_c_info.enabledLayerCount = static_cast<uint32_t>(ValidLayers.size());
	logic_dev_c_info.ppEnabledLayerNames = ValidLayers.data();
#endif

	if (vkCreateDevice((VkPhysicalDevice)m_VK_physical_device, &logic_dev_c_info, nullptr, (VkDevice*)&m_VK_logic_device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	if (fam_idices.m_graphics_fid >= 0) {
		vkGetDeviceQueue((VkDevice)m_VK_logic_device, static_cast<uint32_t>(fam_idices.m_graphics_fid), 0, (VkQueue*)&m_VK_graphics_queue);
	}
	if (fam_idices.m_present_fid >= 0) {
		vkGetDeviceQueue((VkDevice)m_VK_logic_device, static_cast<uint32_t>(fam_idices.m_present_fid), 0, (VkQueue*)&m_VK_present_queue);
	}
}

void VulkanManager::InitializeSwapChain()
{
	SDLOG("--- Vulkan initialize swap chains.(Create swap chain)");
	SwapChainDetails swap_chain((VkPhysicalDevice)m_VK_physical_device, (VkSurfaceKHR)m_VK_surface);
	SDLOGD("------- Initialize SwapChainInfo");
	SDLOGD("ImageCount Min:%d Max:%d", swap_chain.m_caps.minImageCount, swap_chain.m_caps.maxImageCount);
	SDLOGD("ImageExtents Min:(%d,%d) Cur:(%d,%d) Max:(%d,%d)", 
		swap_chain.m_caps.minImageExtent.width, swap_chain.m_caps.minImageExtent.height,
		swap_chain.m_caps.currentExtent.width, swap_chain.m_caps.currentExtent.height, 
		swap_chain.m_caps.maxImageExtent.width, swap_chain.m_caps.maxImageExtent.height);
	SDLOGD("maxImageArrayLayers:%d", swap_chain.m_caps.maxImageArrayLayers);
	SDLOGD("supportedTransforms:%d", swap_chain.m_caps.supportedTransforms);
	SDLOGD("currentTransform:%x (VkSurfaceTransformFlagBitsKHR)", swap_chain.m_caps.currentTransform);
	SDLOGD("supportedCompositeAlpha:%d", swap_chain.m_caps.supportedCompositeAlpha);
	SDLOGD("supportedUsageFlags:%d", swap_chain.m_caps.supportedUsageFlags);

	VkSurfaceFormatKHR surface_fmt = ChooseSwapSurfaceFormat(swap_chain.m_formats);
	SDLOGD("SurfaceFormat:(VkFormat)%d, (VkColorSpaceKHR)%d", surface_fmt.format, surface_fmt.colorSpace);

	VkPresentModeKHR present_mode = ChooseSwapPresentMode(swap_chain.m_present_modes);

	VkExtent2D extent = ChooseSwapExtent(swap_chain.m_caps);

	uint32_t image_count = swap_chain.m_caps.minImageCount + 1;
	if (image_count > swap_chain.m_caps.maxImageCount) {
		image_count = swap_chain.m_caps.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = (VkSurfaceKHR)m_VK_surface;
	createInfo.minImageCount = image_count;
	createInfo.imageFormat = surface_fmt.format;
	createInfo.imageColorSpace = surface_fmt.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices fam_idices((VkPhysicalDevice)m_VK_physical_device, (VkSurfaceKHR)m_VK_surface);
	uint32_t queue_fam_indices[] = { 
		static_cast<uint32_t>(fam_idices.m_graphics_fid),
		static_cast<uint32_t>(fam_idices.m_present_fid)
	};

	if (fam_idices.m_graphics_fid != fam_idices.m_present_fid) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queue_fam_indices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swap_chain.m_caps.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = present_mode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR((VkDevice)m_VK_logic_device, &createInfo, nullptr, (VkSwapchainKHR*)&m_VK_swap_chain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR((VkDevice)m_VK_logic_device, (VkSwapchainKHR)m_VK_swap_chain, &image_count, nullptr);
	m_VK_sc_images.resize(image_count);
	vkGetSwapchainImagesKHR((VkDevice)m_VK_logic_device, (VkSwapchainKHR)m_VK_swap_chain, &image_count, (VkImage*)m_VK_sc_images.data());
	m_scimg_format = surface_fmt.format;
	m_viewport_width = extent.width;
	m_viewport_height = extent.height;
	SDLOG("SwapChainImages number : %u, ViewPort(%d,%d)", image_count, m_viewport_width, m_viewport_height);
}

void VulkanManager::InitializeImageViews()
{
	SDLOG("--- Vulkan initialize image views.");
	for (size_t imgv_id = 0; m_VK_sc_image_views.size(); imgv_id++) {
		VkImageViewCreateInfo image_view_c_info = {};
		image_view_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_c_info.image = (VkImage)m_VK_sc_images[imgv_id];
		image_view_c_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_c_info.format = (VkFormat)m_scimg_format;
		image_view_c_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_c_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_c_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_c_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_c_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_c_info.subresourceRange.baseMipLevel = 0;
		image_view_c_info.subresourceRange.levelCount = 1;
		image_view_c_info.subresourceRange.baseArrayLayer = 0;
		image_view_c_info.subresourceRange.layerCount = 1;

		if (vkCreateImageView((VkDevice)m_VK_logic_device, &image_view_c_info, nullptr, (VkImageView*)&m_VK_sc_image_views[imgv_id]) != VK_SUCCESS) {
			throw std::runtime_error(SDE::Basic::StringFormat("failed to create image views[%d]!").c_str());
		}
	}
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}