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

#define NOMINMAX //For avoid max and min redefinition in Windows.h

#include "VulkanManager.h"

#include <list>
#include <algorithm>

#include "LogManager.h"
#include "TextureFormat_Vulkan.h"
#include "VulkanCreationArg.h"
#include "VulkanWrapper.h"


_____________SD_START_GRAPHICS_NAMESPACE_____________

//------------- Debug Callback.
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
        SDLOG("L[%s] O(%x) T:%d VK Verbose : %s", i_layer_prefix, i_obj, i_object_type, i_msg);
    }
    else if (i_flags == VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        SDLOGD("L[%s] O(%x) T:%d VK Info : %s", i_layer_prefix, i_obj, i_object_type, i_msg);
    }
    else if (i_flags == VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        SDLOGW("L[%s] O(%x) T:%d VK Warning : %s", i_layer_prefix, i_obj, i_object_type, i_msg);
    }
    else if (i_flags == VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        SDLOGW("L[%s] O(%x) T:%d VK Performance Warning : %s", i_layer_prefix, i_obj, i_object_type, i_msg);
    }
    else {
        SDLOGE("Vulkan Error : %s", i_msg);
    }

    return VK_FALSE;
}

void VulkanManager::InitializeVulkanEnvironment()
{
    {
        SDLOG("--- Vulkan initialize debug message.");
        //I want all!!!
        VkDebugReportCallbackCreateInfoEXT create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
        create_info.pfnCallback = VulkanDebugCallback;

        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
            (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
        if (vkCreateDebugReportCallbackEXT != nullptr) {
            VkResult res = vkCreateDebugReportCallbackEXT(m_instance, &create_info, nullptr, &m_debug_rp_cbk);
            if (res != VK_SUCCESS) {
                SDLOG("failed create debug messenger!");
            }
        }
        else {
            SDLOG("failed to load set up debug messenger function!");
        }
    }

    {
        SDLOG("--- Vulkan initialize physical device.(Select graphics card)");
        uint32_t dev_count = 0;
        vkEnumeratePhysicalDevices(m_instance, &dev_count, nullptr);
        if (dev_count == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> phy_devices(dev_count);
        vkEnumeratePhysicalDevices(m_instance, &dev_count, phy_devices.data());
        uint32_t pickup_dev_score = 0;
        for (VkPhysicalDevice& phy_device : phy_devices) {
            VkPhysicalDeviceProperties dev_property;
            vkGetPhysicalDeviceProperties(phy_device, &dev_property);
            SDLOGD("--- DeviceID[%d] name : %s DevType : %d.", dev_property.deviceID, dev_property.deviceName, dev_property.deviceType);
            VkPhysicalDeviceFeatures dev_features;
            uint32_t dev_score = 0;
            vkGetPhysicalDeviceFeatures(phy_device, &dev_features);
            SDLOGD("------ Feature[alphaToOne] : %d", dev_features.alphaToOne);
            dev_score += dev_features.alphaToOne;
            SDLOGD("------ Feature[depthBiasClamp] : %d", dev_features.depthBiasClamp);
            dev_score += dev_features.depthBiasClamp;
            SDLOGD("------ Feature[depthBounds] : %d", dev_features.depthBounds);
            dev_score += dev_features.depthBounds;
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
            vkEnumerateDeviceExtensionProperties(phy_device, nullptr, &avaible_ext_counts, nullptr);

            std::vector<VkExtensionProperties> avaible_exts(avaible_ext_counts);
            vkEnumerateDeviceExtensionProperties(phy_device, nullptr, &avaible_ext_counts, avaible_exts.data());
            std::list<const char*> check_ext_names;
            for (std::string& ext : m_vulkan_config.m_necessary_extensions) {
                check_ext_names.push_back(ext.c_str());
            }

            for (VkExtensionProperties &avaible_ext : avaible_exts) {
                SDLOGD("Extension(%s)(%d)", avaible_ext.extensionName, avaible_ext.specVersion);
                for (std::list<const char*>::iterator c_iter = check_ext_names.begin(); c_iter != check_ext_names.end(); ) {
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
            if (dev_property.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && exts_support == true) {
                m_phy_device = phy_device;
                pickup_dev_score = dev_score;
                break;
            }
            else {
                if (dev_score > pickup_dev_score && exts_support == true) {
                    m_phy_device = phy_device;
                    pickup_dev_score = dev_score;
                }
            }
        }

        if (m_phy_device == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find physical device!");
        }

        vkGetPhysicalDeviceProperties(m_phy_device, &m_picked_dev_props);
        SDLOG("Final choose dev : %s(%d:%d:%d) driver version(%d, %d, %d) apiversion(%d, %d, %d), score : %d",
            m_picked_dev_props.deviceName,
            m_picked_dev_props.vendorID, m_picked_dev_props.deviceID, m_picked_dev_props.deviceType,
            VK_VERSION_MAJOR(m_picked_dev_props.apiVersion), VK_VERSION_MINOR(m_picked_dev_props.apiVersion), VK_VERSION_PATCH(m_picked_dev_props.apiVersion),
            VK_VERSION_MAJOR(m_picked_dev_props.driverVersion), VK_VERSION_MINOR(m_picked_dev_props.driverVersion), VK_VERSION_PATCH(m_picked_dev_props.driverVersion),
            pickup_dev_score);

        vkGetPhysicalDeviceMemoryProperties(m_phy_device, &m_phy_dev_memory_props);

        vkGetPhysicalDeviceFeatures(m_phy_device, &m_phy_dev_features);
    }

    {
        SDLOG("--- Vulkan initialize device.(Create devices queue)");
        SDLOG("------ Get avaiable layer again properties.");
        uint32_t layer_count = 0;
        std::vector<VkLayerProperties> avaiable_valid_layers;
        std::vector<const char*> desired_valid_layer_names;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        if (layer_count > 0) {
            avaiable_valid_layers.resize(layer_count);
            vkEnumerateInstanceLayerProperties(&layer_count, avaiable_valid_layers.data());
            for (uint32_t ext_id = 0; ext_id < avaiable_valid_layers.size(); ext_id++) {
                for (std::string& desired_name : m_vulkan_config.m_desire_valid_layers) {
                    if (strcmp(desired_name.c_str(), avaiable_valid_layers[ext_id].layerName) == 0) {
                        desired_valid_layer_names.push_back(desired_name.c_str());
                        break;
                    }
                }
            }
        }

        SDLOG("------ Get queue family properties.");
        int final_queue_fam_id = -1;
        std::vector<VkQueueFamilyProperties> queue_families;

        uint32_t queue_families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_phy_device, &queue_families_count, nullptr);
        queue_families.resize(queue_families_count);
        vkGetPhysicalDeviceQueueFamilyProperties(m_phy_device, &queue_families_count, queue_families.data());

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
            if (IsContainerNecessaryQueueFlags(queue_families[fam_id].queueFlags) == true &&
                queue_families[fam_id].queueCount > 0) {
                //check this family can be used to present on screen.
                VkBool32 present_support = VK_FALSE;
                VkResult rst = vkGetPhysicalDeviceSurfaceSupportKHR(m_phy_device, fam_id, m_surface, &present_support);
                if (present_support == VK_TRUE && rst == VK_SUCCESS) {
                    final_queue_fam_id = static_cast<int32_t>(fam_id);
                    break;
                }
            }
        }

        if (final_queue_fam_id < 0) {
            SDLOGW("Can't find desired queue family. use family [0]");
            final_queue_fam_id = 0;
        }

        m_queue_family.Initialize(
            queue_families[final_queue_fam_id],
            final_queue_fam_id, 3, true);

        m_queue_family.SetQueuePriority(0, 1.0f); //Present Queue
        m_queue_family.SetQueuePriority(1, 1.0f); //Graphics Queue
        m_queue_family.SetQueuePriority(2, 0.7f); //Loading Data Queue

        std::vector<const char*> target_ext_names;
        for (std::string& ext : m_vulkan_config.m_necessary_extensions) {
            target_ext_names.push_back(ext.c_str());
        }

        //Create queues.
        VkDeviceQueueCreateInfo queue_c_info = {};
        queue_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_c_info.pNext = nullptr;
        queue_c_info.flags = 0;
        queue_c_info.queueFamilyIndex = static_cast<uint32_t>(final_queue_fam_id);
        queue_c_info.queueCount = m_queue_family.GetQueueNumber(); //use signal queue. extend in future.
        queue_c_info.pQueuePriorities = m_queue_family.GetQueuePriorities();

        VkDeviceCreateInfo dev_c_info = {};
        dev_c_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        dev_c_info.pNext = nullptr;
        dev_c_info.flags = 0;
        dev_c_info.pQueueCreateInfos = &queue_c_info;
        dev_c_info.queueCreateInfoCount = 1;
        dev_c_info.pEnabledFeatures = nullptr; //use all features physical device support.
        dev_c_info.ppEnabledExtensionNames = target_ext_names.data();
        dev_c_info.enabledExtensionCount = static_cast<uint32_t>(target_ext_names.size());

    #ifdef NDEBUG
        dev_c_info.enabledLayerCount = 0;
        dev_c_info.ppEnabledLayerNames = nullptr;
    #else
        dev_c_info.enabledLayerCount = static_cast<uint32_t>(desired_valid_layer_names.size());
        dev_c_info.ppEnabledLayerNames = desired_valid_layer_names.data();
    #endif

        if (vkCreateDevice(m_phy_device, &dev_c_info, nullptr, &m_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }
    }
}

void VulkanManager::InitializeVulkanSurface()
{
    SDLOG("------ Get Vulkan Surface Information.");
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_phy_device, m_surface, &m_surface_capabilities);

    bool has_desired_sur_fmt = false;
    std::vector<VkSurfaceFormatKHR> sur_formats;
    uint32_t sur_format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_phy_device, m_surface, &sur_format_count, nullptr);

    if (sur_format_count != 0) {
        sur_formats.resize(sur_format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_phy_device, m_surface, &sur_format_count, sur_formats.data());
    }

    if (sur_format_count > 1) {
        for (VkSurfaceFormatKHR &fmt : sur_formats) {
            SDLOGD("Supported SurfaceFormat:(Format)%d, (colorSpace)%d", fmt.format, fmt.colorSpace);
        }

        for (const VkSurfaceFormatKHR &desired_fmt : m_vulkan_config.m_desired_sur_formats) {
            for (VkSurfaceFormatKHR &fmt : sur_formats) {
                if (fmt.colorSpace == desired_fmt.colorSpace &&
                    fmt.format == desired_fmt.format) {
                    m_final_sur_format = fmt;
                    has_desired_sur_fmt = true;
                    break;
                }
            }
            if (has_desired_sur_fmt == true) {
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

    SDLOGD("Desired SurfaceFormat:(%d, %d)", m_final_sur_format.format, m_final_sur_format.colorSpace);

    std::vector<VkPresentModeKHR> supported_p_modes;
    uint32_t supported_p_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_phy_device, m_surface, &supported_p_mode_count, nullptr);

    if (supported_p_mode_count != 0) {
        supported_p_modes.resize(supported_p_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_phy_device, m_surface, &supported_p_mode_count, supported_p_modes.data());
    }
    else {
        throw std::runtime_error("No present mode supported!");
    }

    for (const VkPresentModeKHR &p_mode : supported_p_modes) {
        SDLOGD("Supported present mode : %d", p_mode);
    }

    for (uint32_t mode_id = 0; mode_id < m_vulkan_config.m_desired_pre_modes.size(); mode_id++) {
        for (const VkPresentModeKHR &p_mode : supported_p_modes) {
            if (m_vulkan_config.m_desired_pre_modes[mode_id] == p_mode) {
                m_final_p_mode = m_vulkan_config.m_desired_pre_modes[mode_id];
                break;
            }
        }
        if (m_final_p_mode != VK_PRESENT_MODE_MAX_ENUM_KHR) {
            break;
        }
    }

    if (m_final_p_mode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
        throw std::runtime_error("No desired present mode supported!");
    }
    else {
        SDLOG("final present mode : %d", m_final_p_mode);
    }

    //Create render pass of swapchain.
    //--- prepare attachment references data.
    std::vector<AttachmentDescription> att_descs;
    AttachmentDescription att_desc;
    att_desc.m_format = TextureFormat_Vulkan::Reverse(m_final_sur_format.format);
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- prepare attachment references data.
    std::vector<SubpassDescription> sp_descs;
    SubpassDescription sp_desc;
    AttachmentReference atta_ref;
    //------ sp0 Compose
    sp_desc = SubpassDescription();
    sp_desc.m_name = "Compose";
    sp_desc.m_bind_point = PipelineBindPoint_GRAPHICS;
    //------ color attachment reference.
    atta_ref.m_attachment_ID = 0;
    atta_ref.m_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    sp_desc.m_color_attachment_refs.push_back(atta_ref);
    sp_descs.push_back(sp_desc);
    //--- SubpassDependency.
    std::vector<SubpassDependency> sp_denps;
    SubpassDependency sp_denp;
    //------ sp external to sp0
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_dst_spID = 0;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);
    //------ sp0 and sp external
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = 0;
    sp_denp.m_dst_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);

    RenderPassStrongReferenceObject rp = new RenderPass(sRenderPass_Composing);
    SD_SREF(rp).AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    SD_SREF(rp).Initialize();
    RegisterRenderPass(rp);
}

void VulkanManager::InitializeSettings()
{
    for (TextureFormatEnum &fmt : m_graphics_config.m_desired_depth_buffer_formats) {
        VkFormat vk_fmt = TextureFormat_Vulkan::Convert(fmt);
        VkFormatProperties fmt_props;
        vkGetPhysicalDeviceFormatProperties(m_phy_device, vk_fmt, &fmt_props);
        if ((fmt_props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) ==
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            m_supported_depth_buffer_formats.push_back(fmt);
        }
    }

    if (m_supported_depth_buffer_formats.size() > 0) {
        SDLOG("Default depth buffer format is (%d).", m_supported_depth_buffer_formats[0]);
    }
    else {
        SDLOGE("There is no supported depth format in this device.");
    }

    //-------- Collect color format.
    for (TextureFormatEnum &fmt : m_graphics_config.m_desired_color_buffer_formats) {
        VkFormat vk_fmt = TextureFormat_Vulkan::Convert(fmt);
        VkFormatProperties fmt_props;
        vkGetPhysicalDeviceFormatProperties(m_phy_device, vk_fmt, &fmt_props);
        if ((fmt_props.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) ==
            VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) {
            m_supported_color_buffer_formats.push_back(fmt);
        }
    }

    if (m_supported_color_buffer_formats.size() > 0) {
        SDLOG("Default color buffer format is (%d).", m_supported_color_buffer_formats[0]);
    }
    else {
        SDLOGE("There is no supported color format in this device.");
    }
}

void VulkanManager::InitializeGraphicsQueues()
{
    QueueAbility q_ability;
    q_ability.Enable({ QueueAbility_GRAPHICS, QueueAbility_COMPUTE });
    if (m_queue_family.RequestQueue(0) == true) {
        m_present_queue = new GraphicsQueue("PresentQueue", q_ability, SD_COMP_HANDE_CAST(m_phy_device), SD_COMP_HANDE_CAST(m_device), m_queue_family.GetQueueFamilyID(), 0);
        SD_SREF(m_present_queue).Initialize();
    }

    if (m_queue_family.RequestQueue(1) == true) {
        m_graphics_queue = new GraphicsQueue("GraphicsQueue", q_ability, SD_COMP_HANDE_CAST(m_phy_device), SD_COMP_HANDE_CAST(m_device), m_queue_family.GetQueueFamilyID(), 1);
        SD_SREF(m_graphics_queue).Initialize();
    }

    if (m_queue_family.RequestQueue(2) == true) {
        m_loading_queue = new GraphicsQueue("LoadingQueue", q_ability, SD_COMP_HANDE_CAST(m_phy_device), SD_COMP_HANDE_CAST(m_device), m_queue_family.GetQueueFamilyID(), 2);
        SD_SREF(m_loading_queue).Initialize();
    }

    m_loading_cmd_pool = new CommandPool("LoadingCommandPool");
    SD_SREF(m_loading_cmd_pool).Initialize();
    m_loading_cmd_buffer = SD_WREF(m_loading_cmd_pool).AllocateCommandBuffer("LoadingCommandBuffer");
}

void VulkanManager::InitializeSwapChain()
{
    InitializeVulkanSurface();
    m_swapchain = new GraphicsSwapchain("VulkanSwapchain", m_present_queue);
    SD_SREF(m_swapchain).Initialize();
}

bool VulkanManager::IsContainerNecessaryQueueFlags(VkQueueFlags i_flags)
{
    bool inconsistent = false;
    
    for (VkQueueFlags flag : m_desired_queue_abilities) {
        if ((i_flags & flag) != flag) {
            inconsistent = true;
            break;
        }
    }

    return (inconsistent == false);
}

int32_t VulkanManager::GetSuitableMemoryTypeID(const VkMemoryRequirements &i_mem_req, VkFlags i_mem_prop_flags)
{
    for (uint32_t mem_type_ID = 0; mem_type_ID < m_phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type =
            i_mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported = 
            ((m_phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & i_mem_prop_flags) ==
                i_mem_prop_flags);
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            return mem_type_ID;
        }
    }

    return -1;
}

______________SD_END_GRAPHICS_NAMESPACE______________