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
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkResult VulkanManager::CreateVkSampler(
    VkSampler & io_sampler_handle,
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
    VkSamplerCreateInfo sampler_c_info = {};
    sampler_c_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_c_info.pNext = nullptr;
    sampler_c_info.flags = 0; //
    sampler_c_info.minFilter = i_min_filter_type;
    sampler_c_info.magFilter = i_mag_filter_type;
    sampler_c_info.mipmapMode = i_mipmap_mode;
    sampler_c_info.addressModeU = i_wrap_mode_s;
    sampler_c_info.addressModeV = i_wrap_mode_t;
    sampler_c_info.addressModeW = i_wrap_mode_r;
    sampler_c_info.mipLodBias = i_mip_lod_bias;
    sampler_c_info.anisotropyEnable = i_enable_anisotropy; //We need to check physical device have this feature or not.
    sampler_c_info.maxAnisotropy = i_max_anisotropy; //We need to check this value in this physical device.
    sampler_c_info.compareEnable = i_enable_compare;
    sampler_c_info.compareOp = i_compare_op;
    sampler_c_info.minLod = i_min_lod;
    sampler_c_info.maxLod = i_max_lod;
    sampler_c_info.borderColor = i_border_color;
    sampler_c_info.unnormalizedCoordinates = i_unnormalize_coord;

    return vkCreateSampler(m_VK_device, &sampler_c_info, nullptr, &io_sampler_handle);
}

void VulkanManager::FreeVkSampler(SamplerIdentity &io_identity)
{
    if (io_identity.m_sampler_handle != SD_NULL_HANDLE) {
        VkSampler &sampler_handle = reinterpret_cast<VkSampler&>(io_identity.m_sampler_handle);
        vkDestroySampler(m_VK_device, sampler_handle, nullptr);
    }
    io_identity = SamplerIdentity();
}

______________SD_END_GRAPHICS_NAMESPACE______________