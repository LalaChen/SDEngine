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

#include "TextureType_Vulkan.h"
#include "TextureFormat_Vulkan.h"
#include "SamplerMipmapMode_Vulkan.h"
#include "SamplerFilterType_Vulkan.h"
#include "SamplerBorderColorType_Vulkan.h"
#include "SamplerWrapMode_Vulkan.h"
#include "CompareOp_Vulkan.h"
#include "ImageLayout_Vulkan.h"
#include "ImageUsage_Vulkan.h"
#include "ImageTiling_Vulkan.h"
#include "SampleCount_Vulkan.h"
#include "ManagerParam.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateTextureImage(TextureIdentity &io_tex_identity, SamplerIdentity &io_sampler_identity)
{
    VkResult result = VK_SUCCESS;
    VkImage &image_handle = reinterpret_cast<VkImage&>(io_tex_identity.m_image_handle);
    VkDeviceMemory &memory_handle = reinterpret_cast<VkDeviceMemory&>(io_tex_identity.m_memory_handle);
    VkImageView &view_handle = reinterpret_cast<VkImageView&>(io_tex_identity.m_view_handle);
    VkDeviceSize &allocated_size = reinterpret_cast<VkDeviceSize&>(io_tex_identity.m_allocated_size);
    VkImageType image_type = TextureType_Vulkan::Convert(io_tex_identity.m_texture_type);
    VkImageViewType view_type = TextureViewType_Vulkan::Convert(io_tex_identity.m_texture_view_type);
    VkFormat image_format = TextureFormat_Vulkan::Convert(io_tex_identity.m_texture_format);
    VkImageLayout init_layout = ImageLayout_Vulkan::Convert(io_tex_identity.m_init_layout);
    VkImageUsageFlags init_usages = ImageUsage_Vulkan::Convert(io_tex_identity.m_image_usages);
    VkImageTiling image_tiling = ImageTiling_Vulkan::Convert(io_tex_identity.m_tiling);
    VkSampleCountFlagBits sample_count = SampleCount_Vulkan::Convert(io_tex_identity.m_sample_count);

    VkExtent3D image_size = { 
        io_tex_identity.m_image_size.m_width,
        io_tex_identity.m_image_size.m_height,
        io_tex_identity.m_image_size.m_length
    };

    //1. Create image.
    result = CreateVkImage(
        image_handle,
        image_type, image_format, image_size,
        init_usages,
        sample_count,
        init_layout,
        io_tex_identity.m_mipmap_levels, io_tex_identity.m_array_layers,
        image_tiling,
        VK_SHARING_MODE_EXCLUSIVE);

    if (result != VK_SUCCESS) {
        SDLOGE("Create image failure(%x)!!!", result);
        return;
    }

    //2. Allocate device image for this image.
    result = AllocateVkDeviceMemoryForVkImage(memory_handle, allocated_size, image_handle, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    if (result != VK_SUCCESS) {
        SDLOGE("Allocated device local memory for this image failure(%x)!!!", result);
        return;
    }

    //3. Create sampler for this texture.
    float max_lod = static_cast<float>(io_tex_identity.m_mipmap_levels);
    VkSampler &sampler_handle = reinterpret_cast<VkSampler&>(io_sampler_identity.m_handle);
    result = CreateVkSampler(
        sampler_handle,
        SamplerFilterType_Vulkan::Convert(io_sampler_identity.m_min_filter_type),
        SamplerFilterType_Vulkan::Convert(io_sampler_identity.m_mag_filter_type),
        SamplerMipmapMode_Vulkan::Convert(io_sampler_identity.m_mipmap_mode),
        SamplerWrapMode_Vulkan::Convert(io_sampler_identity.m_wrap_mode_s),
        SamplerWrapMode_Vulkan::Convert(io_sampler_identity.m_wrap_mode_t),
        SamplerWrapMode_Vulkan::Convert(io_sampler_identity.m_wrap_mode_r),
        0.0f,//Mip Bias
        io_sampler_identity.m_use_anisotropy,
        io_sampler_identity.m_max_anisotropy,
        io_sampler_identity.m_use_compare,
        CompareOp_Vulkan::Convert(io_sampler_identity.m_compare_op),
        0.0f, max_lod,
        BorderColorType_Vulkan::Convert(io_sampler_identity.m_sampler_b_color_type),
        VK_FALSE //need to normalize texture.
    );

    if (result != VK_SUCCESS) {
        SDLOGE("Allocated sampler for this image failure(%x)!!!", result);
        return;
    }

    //4. create image vie for this texture.
    VkComponentMapping swizzle = {
        VK_COMPONENT_SWIZZLE_IDENTITY, 
        VK_COMPONENT_SWIZZLE_IDENTITY, 
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY };

    VkImageSubresourceRange subres = {};
    subres.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subres.baseMipLevel = 0;
    subres.levelCount = io_tex_identity.m_mipmap_levels;
    subres.baseArrayLayer = 0;
    subres.layerCount = io_tex_identity.m_array_layers;

    if (image_format == VK_FORMAT_D16_UNORM ||
        image_format == VK_FORMAT_D16_UNORM_S8_UINT ||
        image_format == VK_FORMAT_D24_UNORM_S8_UINT ||
        image_format == VK_FORMAT_D32_SFLOAT ||
        image_format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
        subres.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }

    result = CreateVkImageView(view_handle, image_handle, view_type, image_format, swizzle, subres);
    if (result != VK_SUCCESS) {
        SDLOGE("Create image view for this image failure(%x)!!!", result);
        return;
    }
}

void VulkanManager::RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum &i_dst_layout)
{
    if (i_identity.m_image_handle != VK_NULL_HANDLE && 
        i_data_size <= i_identity.m_allocated_size && 
        i_size.m_width <= i_identity.m_image_size.m_width &&
        i_size.m_height <= i_identity.m_image_size.m_height &&
        i_size.m_length <= i_identity.m_image_size.m_length) {

        VkResult result = VK_SUCCESS;
        const VkImage &image_handle = reinterpret_cast<const VkImage&>(i_identity.m_image_handle);
        //1. Create staging host buffer.
        VkBuffer staging_buffer_handle = VK_NULL_HANDLE;
        VkDeviceMemory staging_memory = VK_NULL_HANDLE;
        VkDeviceSize staging_memory_size = 0;
        VkOffset3D image_offset = { i_offset.m_x, i_offset.m_y, i_offset.m_z };
        VkExtent3D image_size = { i_size.m_width, i_size.m_height, i_size.m_length };
        //2. Create host memory for staging to store this data temporary.
        //--- i. create staging buffer(host memory).
        result = CreateVkBuffer(staging_buffer_handle, i_data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
        if (result != VK_SUCCESS) {
            SDLOGW("Create staging buffer for copying data to image failure.");
            return;
        }

        result = AllocatVkDeviceMemoryForVkBuffer(staging_memory, staging_memory_size, staging_buffer_handle, 0u, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if (result != VK_SUCCESS) {
            SDLOGW("Allocate staging memory failure.");
            return;
        }
        //--- ii. refresh data to temporary buffer.
        result = RefreshDataToHostVisibleVKDeviceMemory(staging_memory, staging_memory_size, i_data_ptr, i_data_size);
        if (result != VK_SUCCESS) {
            SDLOGW("Refresh host visible buffer failure.");
            return;
        }

        //3. Copy host buffer to image.
        VkImageLayout init_layout = ImageLayout_Vulkan::Convert(i_identity.m_init_layout);
        VkImageLayout dst_layout = init_layout;
        if (i_dst_layout != ImageLayout_MAX_DEFINE_VALUE) {
            dst_layout = ImageLayout_Vulkan::Convert(i_dst_layout);
        }
        CopyVkBufferToVkImage(
            m_main_cb_handle,
            staging_buffer_handle, i_data_size,
            image_handle, image_offset, image_size,
            0, 0,
            init_layout, dst_layout,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
        );
        //4. destroy host buffer.
        FreeVkDeviceMemory(staging_memory);

        DestroyVkBuffer(staging_buffer_handle);
    }
}

void VulkanManager::DeleteTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity)
{
    //destroy image handle.
    VkImage &image_handle = reinterpret_cast<VkImage&>(io_identity.m_image_handle);
    DestroyVkImage(image_handle);
    VkSampler &sample_handle = reinterpret_cast<VkSampler&>(io_sampler_identity.m_handle);
    DestroyVkSampler(sample_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________