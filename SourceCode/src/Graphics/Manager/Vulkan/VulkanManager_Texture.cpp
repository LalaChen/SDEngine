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
#include "ManagerParam.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateTextureImage(TextureIdentity &io_tex_identity, SamplerIdentity &io_sampler_identity, VoidPtr i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    VkImage &image_handle = reinterpret_cast<VkImage&>(io_tex_identity.m_image_handle);
    VkDeviceMemory &memory_handle = reinterpret_cast<VkDeviceMemory&>(io_tex_identity.m_memory_handle);
    VkDeviceSize &allocated_size = reinterpret_cast<VkDeviceSize&>(io_tex_identity.m_allocated_size);
    VkImageType image_type = TextureType_Vulkan::Convert(io_tex_identity.m_texture_type);
    VkFormat image_format = TextureFormat_Vulkan::Convert(io_tex_identity.m_texture_format);
    VkExtent3D image_size = { 
        io_tex_identity.m_image_size.m_width,
        io_tex_identity.m_image_size.m_height,
        io_tex_identity.m_image_size.m_length
    };

    //1. Create image.
    result = CreateVkImage(
        image_handle,
        image_type, image_format, image_size,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        io_tex_identity.m_mipmap_levels, io_tex_identity.m_array_layers,
        VK_IMAGE_TILING_OPTIMAL,
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

    //3. Refresh texture image.
    RefreshTextureImage(io_tex_identity, i_data_ptr, ImageOffset(), io_tex_identity.m_image_size, i_data_size);

    //4. Create sampler for this texture.
    float max_lod = static_cast<float>(io_tex_identity.m_mipmap_levels);
    VkSampler &sampler_handle = reinterpret_cast<VkSampler&>(io_sampler_identity.m_sampler);
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
}

void VulkanManager::RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size)
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
        CopyVkBufferToVkImage(
            staging_buffer_handle, i_data_size,
            image_handle, image_offset, image_size,
            0, VK_ACCESS_SHADER_READ_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT
        );
        //4. destroy host buffer.
        FreeVkDeviceMemory(staging_memory);

        DestroyVkBuffer(staging_buffer_handle);
    }
}

void VulkanManager::DeleteTextureImage(TextureIdentity &io_identity)
{
    //destroy image handle.
    VkImage &image_handle = reinterpret_cast<VkImage&>(io_identity.m_image_handle);
    DestroyVkImage(image_handle);    
}

______________SD_END_GRAPHICS_NAMESPACE______________