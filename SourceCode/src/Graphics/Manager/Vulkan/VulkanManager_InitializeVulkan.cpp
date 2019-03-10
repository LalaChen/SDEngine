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
    //I want all!!!
    VkDebugReportCallbackCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
    create_info.pfnCallback = VulkanDebugCallback;

    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
        (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_VK_instance, "vkCreateDebugReportCallbackEXT");
    if (vkCreateDebugReportCallbackEXT != nullptr) {
        VkResult res = vkCreateDebugReportCallbackEXT(m_VK_instance, &create_info, nullptr, &m_VK_debug_report_cbk);
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
    vkEnumeratePhysicalDevices(m_VK_instance, &dev_count, nullptr);
    if (dev_count == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(dev_count);
    vkEnumeratePhysicalDevices(m_VK_instance, &dev_count, devices.data());
    uint32_t pickup_dev_score = 0;
    for (VkPhysicalDevice &device : devices) {
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
        //Check all necenssary extensions are supported by this physical.
        SDLOGD("------ Extension ------");
        bool exts_support = false;
        uint32_t avaible_ext_counts;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &avaible_ext_counts, nullptr);

        std::vector<VkExtensionProperties> avaible_exts(avaible_ext_counts);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &avaible_ext_counts, avaible_exts.data());
        std::list<const char*> check_ext_names(NecessaryExtensions.begin(), NecessaryExtensions.end());

        for (VkExtensionProperties &avaible_ext : avaible_exts) {

            for (std::list<const char*>::iterator c_iter = check_ext_names.begin();
                c_iter != check_ext_names.end(); ) {
                if (strcmp(avaible_ext.extensionName, (*c_iter)) == 0) {
                    c_iter = check_ext_names.erase(c_iter);
                    break;
                }
                else {
                    c_iter++;
                }
            }
        }

        exts_support = check_ext_names.empty();
        //Calculate device score.
        if (dev_score > pickup_dev_score && exts_support == true) {
            m_VK_physical_device = device;
            pickup_dev_score = dev_score;
        }
    }

    if (m_VK_physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find physical device!");
    }

    VkPhysicalDeviceProperties picked_dev_props;
    vkGetPhysicalDeviceProperties(m_VK_physical_device, &picked_dev_props);
    SDLOG("Final choose dev : %s(%d:%d:%d) driver version : %d apiversion : %d, score : %d", 
        picked_dev_props.deviceName,
        picked_dev_props.vendorID, picked_dev_props.deviceID, picked_dev_props.deviceType,
        picked_dev_props.driverVersion,
        picked_dev_props.apiVersion,
        pickup_dev_score);
    //
    SDLOGD("Maximum Colot Attachment : %d.", picked_dev_props.limits.maxColorAttachments);
}

void VulkanManager::InitializeLogicDevice()
{
    SDLOG("--- Vulkan initialize device.(Create devices queue)");
    SDLOG("------ Get queue family properties.");
    std::vector<VkQueueFamilyProperties> queue_families;
    uint32_t queue_families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_VK_physical_device, &queue_families_count, nullptr);
    queue_families.resize(queue_families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(m_VK_physical_device, &queue_families_count, queue_families.data());

    SDLOG("------ Get avaiable layer again properties.");
    uint32_t layer_count = 0;
    std::vector<VkLayerProperties> avaiable_valid_layers;
    std::vector<const char*> desired_valid_layer_names;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (layer_count > 0) {
        avaiable_valid_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, avaiable_valid_layers.data());
        for (uint32_t ext_id = 0; ext_id < avaiable_valid_layers.size(); ext_id++) {
            for (const char *desired_name : DesiredValidLayers) {
                if (strcmp(desired_name, avaiable_valid_layers[ext_id].layerName) == 0) {
                    desired_valid_layer_names.push_back(desired_name);
                    break;
                }
            }
        }
    }
    
    for (uint32_t fam_id = 0; fam_id < queue_families.size(); fam_id++) {
        SDLOGD("Queue Family [%d]: TimeStamp[%d] Flag(%x) Count:%d Extent3D(%d,%d,%d) ",
            fam_id,
            queue_families[fam_id].timestampValidBits,
            queue_families[fam_id].queueFlags,
            queue_families[fam_id].queueCount,
            queue_families[fam_id].minImageTransferGranularity.width,
            queue_families[fam_id].minImageTransferGranularity.height,
            queue_families[fam_id].minImageTransferGranularity.depth);
    }

    for (uint32_t fam_id = 0; fam_id < queue_families.size(); fam_id++) {
        //pick up queue family with best ability.
        if (queue_families[fam_id].queueFlags == m_VK_desired_queue_abilities &&
            queue_families[fam_id].queueCount > 0) {
            //check this family can be used to present on screen.
            VkBool32 present_support = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_VK_physical_device, fam_id, m_VK_surface, &present_support);
            if (present_support == VK_TRUE) {
                m_VK_picked_queue_family_id = fam_id;
                break;
            }
        }
    }

    if (m_VK_picked_queue_family_id >= 0) {
        SDLOGD("Final Present Queue Family[%d] : TimeStamp[%d] Flag(%x) Count:%d Extent3D(%d,%d,%d) ",
            m_VK_picked_queue_family_id,
            queue_families[m_VK_picked_queue_family_id].timestampValidBits,
            queue_families[m_VK_picked_queue_family_id].queueFlags,
            queue_families[m_VK_picked_queue_family_id].queueCount,
            queue_families[m_VK_picked_queue_family_id].minImageTransferGranularity.width,
            queue_families[m_VK_picked_queue_family_id].minImageTransferGranularity.height,
            queue_families[m_VK_picked_queue_family_id].minImageTransferGranularity.depth);
    }
    else{
        throw std::runtime_error("Can't find desired queue family.");
    }

    //Create queues.
    float quene_priorities[1] = { 1.0f };
    VkDeviceQueueCreateInfo queue_c_info = {};
    queue_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_c_info.queueFamilyIndex = static_cast<uint32_t>(m_VK_picked_queue_family_id);
    queue_c_info.queueCount = 1; //use signal queue. extend in future.
    queue_c_info.pQueuePriorities = quene_priorities;

    VkPhysicalDeviceFeatures dev_features = {};
    vkGetPhysicalDeviceFeatures(m_VK_physical_device, &dev_features);

    VkDeviceCreateInfo logic_dev_c_info = {};
    logic_dev_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logic_dev_c_info.pQueueCreateInfos = &queue_c_info;
    logic_dev_c_info.queueCreateInfoCount = 1;
    logic_dev_c_info.pEnabledFeatures = &dev_features; //use all features physical device support.
    logic_dev_c_info.ppEnabledExtensionNames = NecessaryExtensions.data();
    logic_dev_c_info.enabledExtensionCount = static_cast<uint32_t>(NecessaryExtensions.size());

#ifdef _NDEBUG
    logic_dev_c_info.enabledLayerCount = 0;
    logic_dev_c_info.ppEnabledLayerNames = nullptr;
#else
    logic_dev_c_info.enabledLayerCount = static_cast<uint32_t>(desired_valid_layer_names.size());
    logic_dev_c_info.ppEnabledLayerNames = desired_valid_layer_names.data();
#endif

    if (vkCreateDevice(m_VK_physical_device, &logic_dev_c_info, nullptr, &m_VK_logic_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    if (m_VK_picked_queue_family_id >= 0) {
        vkGetDeviceQueue(m_VK_logic_device, static_cast<uint32_t>(m_VK_picked_queue_family_id), 0, &m_VK_present_queue);
    }
}

void VulkanManager::InitializeSwapChain()
{
    SDLOG("--- Vulkan initialize swap chains.(Create swap chain)");
    VkSurfaceCapabilitiesKHR sur_caps;
    SDLOGD("------- Get surface capability : ");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_VK_physical_device, m_VK_surface, &sur_caps);
    SDLOGD("ImageCount Min:%d Max:%d", sur_caps.minImageCount, sur_caps.maxImageCount);
    SDLOGD("ImageExtents Min:(%d,%d) Cur:(%d,%d) Max:(%d,%d)", 
        sur_caps.minImageExtent.width, sur_caps.minImageExtent.height,
        sur_caps.currentExtent.width, sur_caps.currentExtent.height, 
        sur_caps.maxImageExtent.width, sur_caps.maxImageExtent.height);
    SDLOGD("maxImageArrayLayers:%d", sur_caps.maxImageArrayLayers);
    SDLOGD("supportedTransforms:%d", sur_caps.supportedTransforms);
    SDLOGD("currentTransform:%x (VkSurfaceTransformFlagBitsKHR)", sur_caps.currentTransform);
    SDLOGD("supportedCompositeAlpha:%d", sur_caps.supportedCompositeAlpha);
    SDLOGD("supportedUsageFlags:%d", sur_caps.supportedUsageFlags);

    bool has_desired_sur_fmt = false;
    std::vector<VkSurfaceFormatKHR> sur_formats;
    uint32_t sur_format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_VK_physical_device, m_VK_surface, &sur_format_count, nullptr);

    if (sur_format_count != 0) {
        sur_formats.resize(sur_format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_VK_physical_device, m_VK_surface, &sur_format_count, sur_formats.data());
    }

    if (sur_format_count > 1) {
        for (VkSurfaceFormatKHR &fmt : sur_formats) {
            SDLOGD("Supported SurfaceFormat:(Format)%d, (colorSpace)%d", fmt.format, fmt.colorSpace);
        }

        for (VkSurfaceFormatKHR &fmt : sur_formats) {
            if (fmt.colorSpace == m_VK_desired_sur_fmt.colorSpace &&
                fmt.format == m_VK_desired_sur_fmt.format) {
                has_desired_sur_fmt = true;
                break;
            }
        }
    }
    else if (sur_format_count == 1) {
        if (sur_formats[0].format == VK_FORMAT_UNDEFINED) {
            has_desired_sur_fmt = true;
        }
    }

    if (has_desired_sur_fmt == false) {
        throw std::runtime_error("Desire surface format isn't exist.");
    }
    
    SDLOGD("Desired SurfaceFormat:(VK_FORMAT_B8G8R8A8_UNORM)%d, (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)%d", VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);

    std::vector<VkPresentModeKHR> supported_p_modes;
    uint32_t supported_p_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_VK_physical_device, m_VK_surface, &supported_p_mode_count, nullptr);

    if (supported_p_mode_count != 0) {
        supported_p_modes.resize(supported_p_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_VK_physical_device, m_VK_surface, &supported_p_mode_count, supported_p_modes.data());
    }
    else {
        throw std::runtime_error("No present mode supported!");
    }

    for (const VkPresentModeKHR &p_mode : supported_p_modes) {
        SDLOGD("Supported present mode : %d", p_mode);
    }

    for (int mode_id = 0; mode_id < VK_PRESENT_MODE_RANGE_SIZE_KHR; mode_id++) {
        for (const VkPresentModeKHR &p_mode : supported_p_modes) {
            if (m_VK_desired_pre_mode_list[mode_id] == p_mode) {
                m_VK_final_present_mode = m_VK_desired_pre_mode_list[mode_id];
                break;
            }
        }
    }

    if (m_VK_final_present_mode == VK_PRESENT_MODE_RANGE_SIZE_KHR) {
        throw std::runtime_error("No desired present mode supported!");
    }
    else {
        SDLOGD("final present mode : %d", m_VK_final_present_mode);
    }

    if (sur_caps.currentExtent.width != 0 && sur_caps.currentExtent.height != 0) {
        m_screen_size = sur_caps.currentExtent;
    }
    else {
        m_screen_size.width = std::max(sur_caps.minImageExtent.width, std::min(sur_caps.maxImageExtent.width, sur_caps.minImageExtent.width));
        m_screen_size.height = std::max(sur_caps.minImageExtent.height, std::min(sur_caps.maxImageExtent.height, sur_caps.minImageExtent.height));
    }

    uint32_t image_count = sur_caps.minImageCount + 1;
    if (image_count > sur_caps.maxImageCount) {
        image_count = sur_caps.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_VK_surface;
    createInfo.minImageCount = image_count;
    createInfo.imageFormat = m_VK_desired_sur_fmt.format;
    createInfo.imageColorSpace = m_VK_desired_sur_fmt.colorSpace;
    createInfo.imageExtent = m_screen_size;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = reinterpret_cast<const uint32_t*>(&m_VK_picked_queue_family_id);
    createInfo.preTransform = sur_caps.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = m_VK_final_present_mode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_VK_logic_device, &createInfo, nullptr, &m_VK_swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_VK_logic_device, m_VK_swap_chain, &image_count, nullptr);
    m_VK_sc_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_VK_logic_device, m_VK_swap_chain, &image_count, m_VK_sc_images.data());
    SDLOG("SwapChainImages number : %u, ViewPort(%d,%d)", image_count, m_screen_size.width, m_screen_size.height);

    VkSemaphoreCreateInfo semaphore_c_info = {};
    semaphore_c_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_c_info.pNext = nullptr;
    semaphore_c_info.flags = 0;

    if (vkCreateSemaphore(m_VK_logic_device, &semaphore_c_info, nullptr, &m_VK_acq_img_semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create acq img semaphore!");
    }

    if (vkCreateSemaphore(m_VK_logic_device, &semaphore_c_info, nullptr, &m_VK_present_semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create present semaphore!");
    }
}

void VulkanManager::InitializeSCImageViewsAndFBOs()
{
    SDLOG("--- Vulkan initialize image views.");
    m_VK_sc_image_views.resize(m_VK_sc_images.size());
    m_VK_sc_image_fbos.resize(m_VK_sc_images.size());
    for (size_t imgv_id = 0; imgv_id < m_VK_sc_image_views.size(); imgv_id++) {
        //create image view for sc img[id].
        VkImageViewCreateInfo image_view_c_info = {};
        image_view_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_c_info.image = m_VK_sc_images[imgv_id];
        image_view_c_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_c_info.format = m_VK_desired_sur_fmt.format;
        image_view_c_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_c_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_c_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_c_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_c_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_c_info.subresourceRange.baseMipLevel = 0;
        image_view_c_info.subresourceRange.levelCount = 1;
        image_view_c_info.subresourceRange.baseArrayLayer = 0;
        image_view_c_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_VK_logic_device, &image_view_c_info, nullptr, (VkImageView*)&m_VK_sc_image_views[imgv_id]) != VK_SUCCESS) {
            throw std::runtime_error(SDE::Basic::StringFormat("failed to create image views[%d]!", imgv_id).c_str());
        }
        //create fbo for sc img[id].
    }
}

void VulkanManager::InitializeCommandPoolAndBuffers()
{
    SDLOG("--- Vulkan initialize command pool and buffer.");
    VkCommandPoolCreateInfo cmd_pool_c_info = {};
    cmd_pool_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_c_info.pNext = nullptr;
    cmd_pool_c_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VkCommandPoolCreateFlags
    cmd_pool_c_info.queueFamilyIndex = m_VK_picked_queue_family_id;

    if (vkCreateCommandPool(m_VK_logic_device, &cmd_pool_c_info, nullptr, &m_VK_main_cmd_pool) != VK_SUCCESS) {
        throw std::runtime_error("Create main command pool failure!");
    }

    VkCommandBufferAllocateInfo cmd_buf_a_info = {};
    cmd_buf_a_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_buf_a_info.pNext = nullptr;
    cmd_buf_a_info.commandPool = m_VK_main_cmd_pool;
    cmd_buf_a_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //VkCommandBufferLevel
    cmd_buf_a_info.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(m_VK_logic_device, &cmd_buf_a_info, &m_VK_main_cmd_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Create main command buffer failure!");
    }

    VkFenceCreateInfo fence_c_info = {};
    fence_c_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_c_info.pNext = nullptr;
    fence_c_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(m_VK_logic_device, &fence_c_info, nullptr, &m_VK_main_cmd_buf_fence) != VK_SUCCESS) {
        throw std::runtime_error("Create main cmd buf fence failure failure!");
    }
}

void VulkanManager::InitializeRenderToScreenRenderPass()
{
    VkAttachmentDescription clr_attachment = {};
    clr_attachment.flags = 0;
    clr_attachment.format = m_VK_desired_sur_fmt.format;
    clr_attachment.samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    clr_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    clr_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    clr_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    clr_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    clr_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    clr_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}