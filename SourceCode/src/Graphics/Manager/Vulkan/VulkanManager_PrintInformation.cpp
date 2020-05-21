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

#include "VulkanWrapper.h"
#include "TextureFormat_Vulkan.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::PrintSystemInformation()
{
    SDLOG("----------------- Vulkan Graphics Information -----------------");
    SDLOG("Memory Information :");
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    //1. Get memory property.
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);
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
        vkGetPhysicalDeviceFormatProperties(m_VK_physical_device, format, &format_prop);
        SDLOG("Format[%d](%d)(%s) :"
              " features(%08x)(StorageImage:%d),"
              " linearTile(%08x)(Sampler:%d)(CA:%d)(DA:%d)(BlitSRC:%d)(BlitDST:%d),"
              " optinalTile(%08x)(Sampler:%d)(CA:%d)(DA:%d)(BlitSRC:%d)(BlitDST:%d)",
            format_ID, format, TextureFormat_Vulkan::GetTextureFormatName(format_enum),
            format_prop.bufferFeatures,
            SD_IS_FEATURE(format_prop.bufferFeatures, VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT),
            format_prop.linearTilingFeatures,
            SD_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT),
            SD_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT),
            SD_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
            SD_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_BLIT_SRC_BIT),
            SD_IS_FEATURE(format_prop.linearTilingFeatures, VK_FORMAT_FEATURE_BLIT_DST_BIT),
            format_prop.optimalTilingFeatures,
            SD_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT),
            SD_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT),
            SD_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
            SD_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_BLIT_SRC_BIT),
            SD_IS_FEATURE(format_prop.optimalTilingFeatures, VK_FORMAT_FEATURE_BLIT_DST_BIT)
            );
    }

    VkPhysicalDeviceProperties picked_dev_props;
    vkGetPhysicalDeviceProperties(m_VK_physical_device, &picked_dev_props);
    SDLOG("Limits:");
    SDLOG("maxColorAttachments : %d.", picked_dev_props.limits.maxColorAttachments);
    SDLOG("maxBoundDescriptorSets : %d.", picked_dev_props.limits.maxBoundDescriptorSets);
    SDLOG("maxDescriptorSetSamplers : %d.", picked_dev_props.limits.maxDescriptorSetSamplers);
    SDLOG("maxDescriptorSetUniformBuffers : %d.", picked_dev_props.limits.maxDescriptorSetUniformBuffers);
    SDLOG("maxDescriptorSetUniformBuffersDynamic : %d.", picked_dev_props.limits.maxDescriptorSetUniformBuffersDynamic);
    SDLOG("maxDescriptorSetStorageBuffers : %d.", picked_dev_props.limits.maxDescriptorSetStorageBuffers);
    SDLOG("maxDescriptorSetStorageBuffersDynamic : %d.", picked_dev_props.limits.maxDescriptorSetStorageBuffersDynamic);
    SDLOG("maxDescriptorSetSampledImages : %d.", picked_dev_props.limits.maxDescriptorSetSampledImages);
    SDLOG("maxDescriptorSetStorageImages : %d.", picked_dev_props.limits.maxDescriptorSetStorageImages);
    SDLOG("maxDescriptorSetInputAttachments : %d.", picked_dev_props.limits.maxDescriptorSetInputAttachments);
    SDLOG("maxPushConstantsSize : %d.", picked_dev_props.limits.maxPushConstantsSize);
    SDLOG("maxVertexInputAttributes : %d.", picked_dev_props.limits.maxVertexInputAttributes);
    SDLOG("maxVertexInputBindings : %d.", picked_dev_props.limits.maxVertexInputBindings);
    SDLOG("maxVertexInputAttributeOffset : %d.", picked_dev_props.limits.maxVertexInputAttributeOffset);
    SDLOG("maxVertexInputBindingStride : %d.", picked_dev_props.limits.maxVertexInputBindingStride);
}

______________SD_END_GRAPHICS_NAMESPACE______________