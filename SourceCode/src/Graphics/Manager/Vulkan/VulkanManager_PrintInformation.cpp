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

#include "VulkanManager.h"

#include "VulkanWrapper.h"
#include "TextureFormat_Vulkan.h"
#include "LogManager.h"

#define SD_VK_IS_FEATURE( var, feature_bit ) ((var & feature_bit) == feature_bit)

#define SD_VK_SHOW_LIMIT_INT32(limitProp) SDLOG(#limitProp" : %d.", picked_dev_props.limits.limitProp);
#define SD_VK_SHOW_LIMIT_UINT32(limitProp) SDLOG(#limitProp" : %u.", picked_dev_props.limits.limitProp);
#define SD_VK_SHOW_LIMIT_DEV_SIZE(limitProp) SDLOG(#limitProp" : %llu.", picked_dev_props.limits.limitProp);
#define SD_VK_SHOW_LIMIT_UINT32_2(limitProp) SDLOG(#limitProp" : (%d, %d).", \
    picked_dev_props.limits.limitProp[0], picked_dev_props.limits.limitProp[1]);
#define SD_VK_SHOW_LIMIT_UINT32_3(limitProp) SDLOG(#limitProp" : (%d, %d, %d).", \
    picked_dev_props.limits.limitProp[0], picked_dev_props.limits.limitProp[1], picked_dev_props.limits.limitProp[2]);
#define SD_VK_SHOW_LIMIT_FLOAT(limitProp) SDLOG(#limitProp" : %lf.", picked_dev_props.limits.limitProp);
#define SD_VK_SHOW_LIMIT_FLOAT_2(limitProp) SDLOG(#limitProp" : (%lf, %lf).", \
    picked_dev_props.limits.limitProp[0], picked_dev_props.limits.limitProp[1]);

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::PrintSystemInformation()
{
    SDLOG("----------------- Vulkan Graphics Information -----------------");
    SDLOG("--- Memory Information ---");
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    //1. Get memory property.
    vkGetPhysicalDeviceMemoryProperties(m_phy_device, &phy_dev_memory_props);
    for (uint32_t type = 0; type < phy_dev_memory_props.memoryTypeCount; ++type) {
        SDLOG("Type[%d] : Flags(%u) HeapID(%u)", type,
            phy_dev_memory_props.memoryTypes[type].propertyFlags,
            phy_dev_memory_props.memoryTypes[type].heapIndex);
    }

    //2. Get memory info. We will get memory size of graphics card and main memory.
    for (uint32_t heap_ID = 0; heap_ID < phy_dev_memory_props.memoryHeapCount; ++heap_ID) {
        SDLOG("Heap[%d] : Flag(%u) Size(%llu)", heap_ID,
            phy_dev_memory_props.memoryHeaps[heap_ID].flags,
            phy_dev_memory_props.memoryHeaps[heap_ID].size);
    }

    SDLOG("Support Texture Image Information :");
    for (uint32_t format_ID = 0; format_ID < TextureFormat_MAX_DEFINE_VALUE; ++format_ID) {
        VkFormatProperties format_prop;
        TextureFormatEnum format_enum = static_cast<TextureFormatEnum>(format_ID);
        VkFormat format = static_cast<VkFormat>(TextureFormat_Vulkan::Convert(format_enum));
        vkGetPhysicalDeviceFormatProperties(m_phy_device, format, &format_prop);
        SDLOG("Format[%d](%d)(%s) :"
              " features(%08x)(StorageImage:%d),"
              " linearTile(%08x)(Sampler:%d)(CA:%d)(DA:%d)(BlitSRC:%d)(BlitDST:%d),"
              " optinalTile(%08x)(Sampler:%d)(CA:%d)(DA:%d)(BlitSRC:%d)(BlitDST:%d)",
            format_ID, format, TextureFormat_Vulkan::GetTextureFormatName(format_enum),
            format_prop.bufferFeatures,
            SD_VK_IS_FEATURE(format_prop.bufferFeatures, VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT),
            format_prop.linearTilingFeatures,
            SD_VK_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT),
            SD_VK_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT),
            SD_VK_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
            SD_VK_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_BLIT_SRC_BIT),
            SD_VK_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_BLIT_DST_BIT),
            format_prop.optimalTilingFeatures,
            SD_VK_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT),
            SD_VK_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT),
            SD_VK_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
            SD_VK_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_BLIT_SRC_BIT),
            SD_VK_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_BLIT_DST_BIT)
            );
    }

    VkPhysicalDeviceProperties picked_dev_props;
    vkGetPhysicalDeviceProperties(m_phy_device, &picked_dev_props);
    SDLOG("Device Name : %s(%d:%d:%d)",
        picked_dev_props.deviceName,
        picked_dev_props.vendorID, picked_dev_props.deviceID, picked_dev_props.deviceType);
    SDLOG("Driver Version : %d.%d.%d",
        VK_VERSION_MAJOR(picked_dev_props.driverVersion), VK_VERSION_MINOR(picked_dev_props.driverVersion), VK_VERSION_PATCH(picked_dev_props.driverVersion));
    SDLOG("API version(%d.%d.%d",
        VK_VERSION_MAJOR(picked_dev_props.apiVersion), VK_VERSION_MINOR(picked_dev_props.apiVersion), VK_VERSION_PATCH(picked_dev_props.apiVersion));
    SDLOG("--- Limits ---");
    SD_VK_SHOW_LIMIT_UINT32(maxImageDimension1D);
    SD_VK_SHOW_LIMIT_UINT32(maxImageDimension2D);
    SD_VK_SHOW_LIMIT_UINT32(maxImageDimension3D);
    SD_VK_SHOW_LIMIT_UINT32(maxImageDimensionCube);
    SD_VK_SHOW_LIMIT_UINT32(maxImageArrayLayers);
    SD_VK_SHOW_LIMIT_UINT32(maxTexelBufferElements);
    SD_VK_SHOW_LIMIT_UINT32(maxUniformBufferRange);
    SD_VK_SHOW_LIMIT_UINT32(maxStorageBufferRange);
    SD_VK_SHOW_LIMIT_UINT32(maxPushConstantsSize);
    SD_VK_SHOW_LIMIT_UINT32(maxMemoryAllocationCount);
    SD_VK_SHOW_LIMIT_UINT32(maxSamplerAllocationCount);
    SD_VK_SHOW_LIMIT_DEV_SIZE(bufferImageGranularity);
    SD_VK_SHOW_LIMIT_DEV_SIZE(sparseAddressSpaceSize);
    SD_VK_SHOW_LIMIT_UINT32(maxBoundDescriptorSets);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageDescriptorSamplers);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageDescriptorUniformBuffers);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageDescriptorStorageBuffers);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageDescriptorSampledImages);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageDescriptorStorageImages);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageDescriptorStorageBuffers);
    SD_VK_SHOW_LIMIT_UINT32(maxPerStageResources);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetSamplers);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetUniformBuffers);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetUniformBuffersDynamic);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetStorageBuffers);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetStorageBuffersDynamic);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetSampledImages);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetStorageImages);
    SD_VK_SHOW_LIMIT_UINT32(maxDescriptorSetInputAttachments);
    SD_VK_SHOW_LIMIT_UINT32(maxVertexInputAttributes);
    SD_VK_SHOW_LIMIT_UINT32(maxVertexInputBindings);
    SD_VK_SHOW_LIMIT_UINT32(maxVertexInputAttributeOffset);
    SD_VK_SHOW_LIMIT_UINT32(maxVertexInputBindingStride);
    SD_VK_SHOW_LIMIT_UINT32(maxVertexOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationGenerationLevel);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationPatchSize);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationControlPerVertexInputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationControlPerVertexOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationControlPerPatchOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationControlTotalOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationEvaluationInputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxTessellationEvaluationOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxGeometryShaderInvocations);
    SD_VK_SHOW_LIMIT_UINT32(maxGeometryInputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxGeometryOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxGeometryOutputVertices);
    SD_VK_SHOW_LIMIT_UINT32(maxGeometryTotalOutputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxGeometryShaderInvocations);
    SD_VK_SHOW_LIMIT_UINT32(maxFragmentInputComponents);
    SD_VK_SHOW_LIMIT_UINT32(maxFragmentOutputAttachments);
    SD_VK_SHOW_LIMIT_UINT32(maxFragmentDualSrcAttachments);
    SD_VK_SHOW_LIMIT_UINT32(maxFragmentCombinedOutputResources);
    SD_VK_SHOW_LIMIT_UINT32(maxComputeSharedMemorySize);
    SD_VK_SHOW_LIMIT_UINT32_3(maxComputeWorkGroupCount);
    SD_VK_SHOW_LIMIT_UINT32(maxComputeWorkGroupInvocations);
    SD_VK_SHOW_LIMIT_UINT32_3(maxComputeWorkGroupSize);
    SD_VK_SHOW_LIMIT_UINT32(subPixelPrecisionBits);
    SD_VK_SHOW_LIMIT_UINT32(subTexelPrecisionBits);
    SD_VK_SHOW_LIMIT_UINT32(mipmapPrecisionBits);
    SD_VK_SHOW_LIMIT_UINT32(maxDrawIndexedIndexValue);
    SD_VK_SHOW_LIMIT_UINT32(maxDrawIndirectCount);
    SD_VK_SHOW_LIMIT_FLOAT(maxSamplerLodBias);
    SD_VK_SHOW_LIMIT_FLOAT(maxSamplerAnisotropy);
    SD_VK_SHOW_LIMIT_UINT32(maxViewports);
    SD_VK_SHOW_LIMIT_UINT32_2(maxViewportDimensions);
    SD_VK_SHOW_LIMIT_FLOAT_2(viewportBoundsRange);
    SD_VK_SHOW_LIMIT_UINT32(viewportSubPixelBits);
    SD_VK_SHOW_LIMIT_DEV_SIZE(minMemoryMapAlignment);
    SD_VK_SHOW_LIMIT_DEV_SIZE(minTexelBufferOffsetAlignment);
    SD_VK_SHOW_LIMIT_DEV_SIZE(minUniformBufferOffsetAlignment);
    SD_VK_SHOW_LIMIT_DEV_SIZE(minStorageBufferOffsetAlignment);
    SD_VK_SHOW_LIMIT_INT32(minTexelOffset);
    SD_VK_SHOW_LIMIT_UINT32(maxTexelOffset);
    SD_VK_SHOW_LIMIT_INT32(minTexelGatherOffset);
    SD_VK_SHOW_LIMIT_UINT32(maxTexelGatherOffset);
    SD_VK_SHOW_LIMIT_FLOAT(minInterpolationOffset);
    SD_VK_SHOW_LIMIT_FLOAT(maxInterpolationOffset);
    SD_VK_SHOW_LIMIT_UINT32(subPixelInterpolationOffsetBits);
    SD_VK_SHOW_LIMIT_UINT32(maxFramebufferWidth);
    SD_VK_SHOW_LIMIT_UINT32(maxFramebufferHeight);
    SD_VK_SHOW_LIMIT_UINT32(maxFramebufferLayers);
    SD_VK_SHOW_LIMIT_UINT32(framebufferColorSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(framebufferDepthSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(framebufferStencilSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(framebufferNoAttachmentsSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(maxColorAttachments);
    SD_VK_SHOW_LIMIT_UINT32(sampledImageColorSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(sampledImageIntegerSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(sampledImageDepthSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(sampledImageStencilSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(storageImageSampleCounts);
    SD_VK_SHOW_LIMIT_UINT32(maxSampleMaskWords);
    SD_VK_SHOW_LIMIT_UINT32(timestampComputeAndGraphics);
    SD_VK_SHOW_LIMIT_FLOAT(timestampPeriod);
    SD_VK_SHOW_LIMIT_UINT32(maxClipDistances);
    SD_VK_SHOW_LIMIT_UINT32(maxCullDistances);
    SD_VK_SHOW_LIMIT_UINT32(maxCombinedClipAndCullDistances);
    SD_VK_SHOW_LIMIT_UINT32(discreteQueuePriorities);
    SD_VK_SHOW_LIMIT_FLOAT_2(pointSizeRange);
    SD_VK_SHOW_LIMIT_FLOAT_2(lineWidthRange);
    SD_VK_SHOW_LIMIT_FLOAT(pointSizeGranularity);
    SD_VK_SHOW_LIMIT_FLOAT(lineWidthGranularity);
    SD_VK_SHOW_LIMIT_UINT32(strictLines);
    SD_VK_SHOW_LIMIT_UINT32(standardSampleLocations);
    SD_VK_SHOW_LIMIT_DEV_SIZE(optimalBufferCopyOffsetAlignment);
    SD_VK_SHOW_LIMIT_DEV_SIZE(optimalBufferCopyRowPitchAlignment);
    SD_VK_SHOW_LIMIT_DEV_SIZE(nonCoherentAtomSize);
    
    //----------------- surface information.
    if (m_surface != VK_NULL_HANDLE) {
        SDLOG("--- Surface Capabilities ---");
        VkSurfaceCapabilitiesKHR sur_caps;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_phy_device, m_surface, &sur_caps);
        SDLOG("ImageCount Min:%d Max:%d", sur_caps.minImageCount, sur_caps.maxImageCount);
        SDLOG("ImageExtents Min:(%d,%d) Cur:(%d,%d) Max:(%d,%d)",
            sur_caps.minImageExtent.width, sur_caps.minImageExtent.height,
            sur_caps.currentExtent.width, sur_caps.currentExtent.height,
            sur_caps.maxImageExtent.width, sur_caps.maxImageExtent.height);
        SDLOG("maxImageArrayLayers:%d", sur_caps.maxImageArrayLayers);
        SDLOG("supportedTransforms:%d", sur_caps.supportedTransforms);
        SDLOG("currentTransform:%x (VkSurfaceTransformFlagBitsKHR)", sur_caps.currentTransform);
        SDLOG("supportedCompositeAlpha:%d", sur_caps.supportedCompositeAlpha);
        SDLOG("supportedUsageFlags:%d", sur_caps.supportedUsageFlags);
    }

    SDLOG("----------------- Vulkan Graphics Information -----------------");
}

______________SD_END_GRAPHICS_NAMESPACE______________