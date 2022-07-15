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

#include "VulkanStructureInitializer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkImageCreateInfo InitializeVkImageCreateInfo(
    VkImageType i_type,
    VkFormat i_image_format,
    VkExtent3D i_image_size,
    VkImageUsageFlags i_usage_flags,
    VkSampleCountFlagBits i_sample_count,
    VkImageLayout i_image_layout,
    uint32_t i_mipmap_levels,
    uint32_t i_array_layers,
    VkImageTiling i_tiling_mode,
    VkSharingMode i_sharing_mode)
{
    //1. Write image creating information.
    VkImageCreateInfo info = {};
    info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext         = nullptr;
    info.flags         = 0; //We need to set flag when we want to use sparse texture data or sth.
    info.imageType     = i_type;
    info.mipLevels     = i_mipmap_levels;// mipmap levels. We don't want to create mip map in here.
    info.arrayLayers   = i_array_layers; //Use it when data is 2d texture array.
    info.tiling        = i_tiling_mode; //Set texture tiling mode. If the image is linked data in system memory, we need to use VK_IMAGE_TILING_LINEAR.
    info.initialLayout = i_image_layout;
    info.usage         = i_usage_flags; //We will copy data to this image(trasnfer dst) and use it in shader(sampled).
    info.samples       = i_sample_count;
    info.extent        = i_image_size;
    info.format        = i_image_format;
    info.sharingMode   = i_sharing_mode;

    if (i_sharing_mode == VK_SHARING_MODE_EXCLUSIVE) {
        info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
        info.pQueueFamilyIndices = nullptr;
    }
    return info;
}

VkImageViewCreateInfo InitializeVkImageViewCreateInfo(
    VkImage i_image,
    VkImageViewType i_view_type,
    VkFormat i_img_format,
    VkComponentMapping i_comp_swizzle,
    VkImageSubresourceRange i_sub_src_range)
{
    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.viewType = i_view_type;
    info.image = i_image;
    info.format = i_img_format;
    info.components = i_comp_swizzle;
    info.subresourceRange = i_sub_src_range;
    return info;
}

VkBufferCreateInfo InitializeVkBufferCreateInfo(
    VkDeviceSize i_size,
    VkBufferUsageFlags i_buffer_usage,
    VkSharingMode i_sharing_mode)
{
    //1. create buffer information.
    VkBufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.usage = i_buffer_usage;
    info.size = i_size;
    info.sharingMode = i_sharing_mode;

    if (i_sharing_mode == VK_SHARING_MODE_EXCLUSIVE) {
        info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
        info.pQueueFamilyIndices = nullptr;
    }
    return info;
}

VkMemoryAllocateInfo InitializeVkMemoryAllocateInfo(
    VkDeviceSize i_size,
    int32_t i_mem_type_ID)
{
    VkMemoryAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = i_size;
    info.memoryTypeIndex = i_mem_type_ID;
    return info;
}

VkSwapchainCreateInfoKHR InitializeVkSwapchainCreateInfoKHR(
    VkSurfaceKHR i_surface,
    VkSurfaceFormatKHR i_surface_fmt,
    uint32_t i_min_img_count,
    uint32_t i_width, uint32_t i_height,
    uint32_t i_layer_number,
    const std::vector<uint32_t> &i_queue_fam_ids,
    VkPresentModeKHR i_present_mode)
{
    VkSwapchainCreateInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.surface = i_surface;
    info.minImageCount = i_min_img_count;
    info.imageFormat = i_surface_fmt.format;
    info.imageColorSpace = i_surface_fmt.colorSpace;
    info.imageExtent = { i_width, i_height};
    info.imageArrayLayers = i_layer_number;
    info.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = static_cast<uint32_t>(i_queue_fam_ids.size());
    info.pQueueFamilyIndices = i_queue_fam_ids.data();
    info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; //Use this for android.(If use currentTransform as presentTransform, we will see double rotation)
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = i_present_mode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = VK_NULL_HANDLE;
    return info;
}

VkCommandBufferBeginInfo InitializeVKCommandBufferBeginInfo()
{
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.pInheritanceInfo = nullptr;
    return info;
}

VkCommandBufferInheritanceInfo InitializeVkCommandBufferInheritanceInfo()
{
    VkCommandBufferInheritanceInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    info.pNext = nullptr;
    info.framebuffer = VK_NULL_HANDLE;
    info.renderPass = VK_NULL_HANDLE;
    info.subpass = 0;
    info.queryFlags = 0;
    info.occlusionQueryEnable = VK_FALSE;
    info.pipelineStatistics = 0;
    return info;
}

VkDescriptorSetLayoutCreateInfo InitializeVkDescriptorSetLayoutCreateInfo()
{
    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.bindingCount = 0;
    info.pBindings = nullptr;
    return info;
}

VkDescriptorPoolCreateInfo InitializeVkDescriptorPoolCreateInfo()
{
    VkDescriptorPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.flags = 0;
    info.pNext = nullptr;
    info.pPoolSizes = nullptr;
    info.poolSizeCount = 0;
    info.maxSets = 0;
    return info;
}

VkDescriptorSetAllocateInfo InitializeVkDescriptorSetAllocateInfo()
{
    VkDescriptorSetAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.descriptorPool = VK_NULL_HANDLE;
    info.descriptorSetCount = 0;
    info.pSetLayouts = nullptr;
    return info;
}

VkWriteDescriptorSet InitializeVkWriteDescriptorSetInfo()
{
    VkWriteDescriptorSet info = {};
    info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    info.pNext = nullptr;
    info.dstSet = VK_NULL_HANDLE;
    info.dstBinding = 0; //binding 0, set 0
    info.descriptorCount = 0;
    info.pBufferInfo = nullptr;
    info.pImageInfo = nullptr;
    info.pTexelBufferView = nullptr;
    info.dstArrayElement = 0;
    return info;
}

VkRenderPassCreateInfo InitializeVkRenderPassCreateInfo(
    const std::vector<VkAttachmentDescription>& i_vk_att_descs,
    const std::vector<VkSubpassDescription>& i_vk_sps,
    const std::vector<VkSubpassDependency>& i_vk_sp_dependencies
)
{
    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.flags = 0;
    info.pNext = nullptr;
    info.attachmentCount = static_cast<uint32_t>(i_vk_att_descs.size());
    info.pAttachments = i_vk_att_descs.data();
    info.dependencyCount = static_cast<uint32_t>(i_vk_sp_dependencies.size());
    info.pDependencies = i_vk_sp_dependencies.data();
    info.subpassCount = static_cast<uint32_t>(i_vk_sps.size());
    info.pSubpasses = i_vk_sps.data();
    return info;
}

VkRenderPassMultiviewCreateInfo InitializeVkRenderPassMultiviewCreateInfo(
    const std::vector<uint32_t> &i_view_masks,
    const std::vector<int32_t> &i_view_offsets,
    const std::vector<uint32_t> &i_correlation_masks)
{
    VkRenderPassMultiviewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
    info.pNext = nullptr;
    info.subpassCount = static_cast<uint32_t>(i_view_masks.size());
    info.pViewMasks = i_view_masks.data();
    info.dependencyCount = static_cast<uint32_t>(i_view_offsets.size());
    info.pViewOffsets = i_view_offsets.data();
    info.correlationMaskCount = static_cast<uint32_t>(i_correlation_masks.size());
    info.pCorrelationMasks = i_correlation_masks.data();
    return info;
}

VkSamplerCreateInfo InitializeVkSamplerCreateInfo(
    VkFilter i_mag_filter_type,
    VkFilter i_min_filter_type,
    VkSamplerMipmapMode i_mipmap_mode,
    VkSamplerAddressMode i_wrap_mode_s,
    VkSamplerAddressMode i_wrap_mode_t,
    VkSamplerAddressMode i_wrap_mode_r,
    float i_mip_lod_bias,
    VkBool32 i_enable_anisotropy,
    float i_max_anisotropy,
    VkBool32 i_enable_compare,
    VkCompareOp i_compare_op,
    float i_min_lod,
    float i_max_lod,
    VkBorderColor i_border_color,
    VkBool32 i_unnormalize_coord)
{
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0; //
    info.minFilter = i_min_filter_type;
    info.magFilter = i_mag_filter_type;
    info.mipmapMode = i_mipmap_mode;
    info.addressModeU = i_wrap_mode_s;
    info.addressModeV = i_wrap_mode_t;
    info.addressModeW = i_wrap_mode_r;
    info.mipLodBias = i_mip_lod_bias;
    info.anisotropyEnable = i_enable_anisotropy; //We need to check physical device have this feature or not.
    info.maxAnisotropy = i_max_anisotropy; //We need to check this value in this physical device.
    info.compareEnable = i_enable_compare;
    info.compareOp = i_compare_op;
    info.minLod = i_min_lod;
    info.maxLod = i_max_lod;
    info.borderColor = i_border_color;
    info.unnormalizedCoordinates = i_unnormalize_coord;
    return info;
}

VkFenceCreateInfo InitializeVkFenceCreateInfo()
{
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    return info;
}

VkSemaphoreCreateInfo InitializeVkSemaphoreCreateInfo()
{
    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    return info;
}

VkPresentInfoKHR InitializeVkPresentInfoKHR(
    const std::vector<VkSemaphore> &i_semas,
    const std::vector<VkSwapchainKHR> &i_sws,
    const std::vector<uint32_t> &i_img_idices)
{
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.waitSemaphoreCount = i_semas.size();
    info.pWaitSemaphores = i_semas.data();
    info.swapchainCount = i_sws.size();
    info.pSwapchains = i_sws.data();
    info.pImageIndices = i_img_idices.data();
    return info;
}

______________SD_END_GRAPHICS_NAMESPACE______________