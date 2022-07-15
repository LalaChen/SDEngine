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


/*! \file      VulkanStructureInitializer.h
 *  \brief     Introduce of function VulkanStructureInitializer.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/01
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "VulkanWrapper.h"
#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkImageCreateInfo InitializeVkImageCreateInfo(
    VkImageType i_type,
    VkFormat i_image_format,
    VkExtent3D i_image_size,
    VkImageUsageFlags i_usage_flags,
    VkSampleCountFlagBits i_sample_count = VK_SAMPLE_COUNT_1_BIT,
    VkImageLayout i_image_layout = VK_IMAGE_LAYOUT_UNDEFINED,
    uint32_t i_mipmap_levels = 1,
    uint32_t i_array_layers = 1,
    VkImageTiling i_tiling_mode = VK_IMAGE_TILING_OPTIMAL,
    VkSharingMode i_sharing_mode = VK_SHARING_MODE_EXCLUSIVE);

VkImageViewCreateInfo InitializeVkImageViewCreateInfo(
    VkImage i_image,
    VkImageViewType i_view_type,
    VkFormat i_img_format,
    VkComponentMapping i_comp_swizzle,
    VkImageSubresourceRange i_sub_src_range);

VkBufferCreateInfo InitializeVkBufferCreateInfo(
    VkDeviceSize i_size,
    VkBufferUsageFlags i_buffer_usage,
    VkSharingMode i_sharing_mode = VK_SHARING_MODE_EXCLUSIVE);

VkMemoryAllocateInfo InitializeVkMemoryAllocateInfo(
    VkDeviceSize i_size,
    int32_t i_mem_type_ID);

VkSwapchainCreateInfoKHR InitializeVkSwapchainCreateInfoKHR(
    VkSurfaceKHR i_surface,
    VkSurfaceFormatKHR i_surface_fmt,
    uint32_t i_min_img_count,
    uint32_t i_width, uint32_t i_height,
    uint32_t i_layer_number,
    const std::vector<uint32_t> &i_queue_fam_ids,
    VkPresentModeKHR i_present_mode);

VkCommandBufferBeginInfo InitializeVKCommandBufferBeginInfo();

VkCommandBufferInheritanceInfo InitializeVkCommandBufferInheritanceInfo();

VkDescriptorSetLayoutCreateInfo InitializeVkDescriptorSetLayoutCreateInfo();

VkDescriptorPoolCreateInfo InitializeVkDescriptorPoolCreateInfo();

VkDescriptorSetAllocateInfo InitializeVkDescriptorSetAllocateInfo();

VkWriteDescriptorSet InitializeVkWriteDescriptorSetInfo();

VkRenderPassCreateInfo InitializeVkRenderPassCreateInfo(
    const std::vector<VkAttachmentDescription> &i_vk_att_descs,
    const std::vector<VkSubpassDescription> &i_vk_sps,
    const std::vector<VkSubpassDependency> &i_vk_sp_dependencies);

VkRenderPassMultiviewCreateInfo InitializeVkRenderPassMultiviewCreateInfo(
    const std::vector<uint32_t> &i_view_masks,
    const std::vector<int32_t> &i_view_offsets,
    const std::vector<uint32_t> &i_correlation_masks);

/*! \fn VkSamplerCreateInfo InitializeVkSamplerCreateInfo(VkFilter i_mag_filter_type, VkFilter i_min_filter_type, VkSamplerMipmapMode i_mipmap_mode, VkSamplerAddressMode i_wrap_mode_s, VkSamplerAddressMode i_wrap_mode_t, VkSamplerAddressMode i_wrap_mode_r, float i_mip_lod_bias, VkBool32 i_enable_anisotropy, float i_max_anisotropy, VkBool32 i_enable_compare, VkCompareOp i_compare_op, float i_min_lod, float i_max_lod, VkBorderColor i_border_color, VkBool32 i_unnormalize_coord);
 *  \param [in] i_mag_filter_type Mag filter type. Default type is nearset.
 *  \param [in] i_min_filter_type Min filter type. Default type is nearest.
 *  \param [in] i_mipmap_mode Mipmap type. Default is nearest.
 *  \param [in] i_wrap_mode_s Texture coordinate wrap mode. Default is clamp tp edge.
 *  \param [in] i_wrap_mode_t Texture coordinate wrap mode. Default is clamp tp edge.
 *  \param [in] i_wrap_mode_r Texture coordinate wrap mode. Default is clamp tp edge.
 *  \param [in] i_mip_lod_bias Mipmap bias. Default is 0.
 *  \param [in] i_enable_anisotropy Default false.
 *  \param [in] i_max_anisotropy Default 1.0f.
 *  \param [in] i_enable_compare Default false.
 *  \param [in] i_compare_op Default always.
 *  \param [in] i_min_lod Default 0.0f.
 *  \param [in] i_max_lod Default 1.0f.
 *  \param [in] i_border_color Default float opaque black.
 *  \param [in] i_unnormalize_coord Default false.(Normalize coord.)
 *  \brief Initialize sampler info.
 */
VkSamplerCreateInfo InitializeVkSamplerCreateInfo(
    VkFilter i_mag_filter_type = VK_FILTER_NEAREST,
    VkFilter i_min_filter_type = VK_FILTER_NEAREST,
    VkSamplerMipmapMode i_mipmap_mode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
    VkSamplerAddressMode i_wrap_mode_s = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VkSamplerAddressMode i_wrap_mode_t = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VkSamplerAddressMode i_wrap_mode_r = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    float i_mip_lod_bias = 0.0f,
    VkBool32 i_enable_anisotropy = VK_FALSE,
    float i_max_anisotropy = 1.0f,
    VkBool32 i_enable_compare = VK_FALSE,
    VkCompareOp i_compare_op = VK_COMPARE_OP_ALWAYS,
    float i_min_lod = 0.0f,
    float i_max_lod = 1.0f,
    VkBorderColor i_border_color = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
    VkBool32 i_unnormalize_coord = VK_FALSE);

VkFenceCreateInfo InitializeVkFenceCreateInfo();

VkSemaphoreCreateInfo InitializeVkSemaphoreCreateInfo();

VkPresentInfoKHR InitializeVkPresentInfoKHR(
    const std::vector<VkSemaphore> &i_semas,
    const std::vector<VkSwapchainKHR> &i_sws,
    const std::vector<uint32_t> &i_img_idices);

______________SD_END_GRAPHICS_NAMESPACE______________

