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
#include "ShaderKind_Vulkan.h"
#include "UniformBindingType_Vulkan.h"
#include "UniformImages.h"
#include "UniformBuffer.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity, const std::vector<UniformVariableDescriptorWeakReferenceObject> &i_uvd_wrefs)
{
    VkDescriptorSetLayout &dsl_handle = reinterpret_cast<VkDescriptorSetLayout&>(io_identity.m_handle);
    std::vector<VkDescriptorSetLayoutBinding> vk_dsl_bindings;
    for (const UniformVariableDescriptorWeakReferenceObject &uvd_wref : i_uvd_wrefs) {
        UniformBinding ub = uvd_wref.GetConstRef().CreateUniformBinding();
        VkDescriptorSetLayoutBinding vk_dsl_binding = {};
        vk_dsl_binding.binding = ub.m_binding_id;
        vk_dsl_binding.descriptorCount = ub.m_element_number;
        vk_dsl_binding.descriptorType = UniformBindingType_Vulkan::Convert(ub.m_binding_type);
        vk_dsl_binding.stageFlags = ShaderStage_Vulkan::Convert(ub.m_target_stages);
        vk_dsl_binding.pImmutableSamplers = VK_NULL_HANDLE;
        vk_dsl_bindings.push_back(vk_dsl_binding);
    }
    VkDescriptorSetLayoutCreateInfo dsl_c_info = InitializeVkDescriptorSetLayoutCreateInfo();
    dsl_c_info.bindingCount = static_cast<uint32_t>(vk_dsl_bindings.size());
    dsl_c_info.pBindings = vk_dsl_bindings.data();
    CreateVKDescriptorSetLayout(dsl_handle, dsl_c_info);
}

void VulkanManager::DestroyDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity)
{
    VkDescriptorSetLayout &dsl_handle = reinterpret_cast<VkDescriptorSetLayout&>(io_identity.m_handle);
    DestroyVkDescriptorSetLayout(dsl_handle);
}

void VulkanManager::CreateDescriptorPool(DescriptorPoolIdentity &io_identity)
{
    VkDescriptorPool &dp_handle = reinterpret_cast<VkDescriptorPool&>(io_identity.m_handle);
    VkDescriptorPoolCreateInfo c_info = InitializeVkDescriptorPoolCreateInfo();
    if (io_identity.m_individual_op_flag == true) {
        c_info.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    }
    c_info.maxSets = io_identity.m_max_set;

    std::vector<VkDescriptorPoolSize> type_sizes;
    type_sizes.resize(UniformBindingType_MAX_DEFINE_VALUE);

    for (uint32_t type_id = 0; type_id < UniformBindingType_MAX_DEFINE_VALUE; ++type_id) {
        type_sizes[type_id] = {};
        type_sizes[type_id].type = UniformBindingType_Vulkan::Convert(static_cast<UniformBindingTypeEnum>(type_id));
        type_sizes[type_id].descriptorCount = io_identity.m_descriptor_counts[type_id];
    }

    c_info.pPoolSizes = type_sizes.data();
    c_info.poolSizeCount = static_cast<uint32_t>(type_sizes.size());
    CreateVkDescriptorPool(dp_handle, c_info);
}

void VulkanManager::DestroyDescriptorPool(DescriptorPoolIdentity &io_identity)
{
    VkDescriptorPool &dp_handle = reinterpret_cast<VkDescriptorPool&>(io_identity.m_handle);
    DestroyVkDescriptorPool(dp_handle);
}


void VulkanManager::AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref, const DescriptorSetLayoutWeakReferenceObject &i_layout_wref)
{
    VkDescriptorSet &ds_handle = reinterpret_cast<VkDescriptorSet&>(io_identity.m_handle);
    const DescriptorPoolIdentity &pool_identity = GetIdentity(i_pool_wref);
    const DescriptorSetLayoutIdentity &layout_identity = GetIdentity(i_layout_wref);
    VkDescriptorSetAllocateInfo a_info = InitializeVkDescriptorSetAllocateInfo();
    a_info.descriptorPool = reinterpret_cast<VkDescriptorPool>(pool_identity.m_handle);
    a_info.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>(&layout_identity.m_handle);
    a_info.descriptorSetCount = 1;
    AllocateVkDescriptorSet(ds_handle, a_info);
}

void VulkanManager::WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uv_wrefs)
{
    std::vector<VkWriteDescriptorSet> write_infos;
    std::vector<VkCopyDescriptorSet> copy_infos;
    std::list<std::vector<VkDescriptorImageInfo>> decriptor_img_infos;
    std::list<VkDescriptorBufferInfo> decriptor_buffer_infos;

    for (const UniformVariableWeakReferenceObject &uv_wref : i_uv_wrefs) {
        if (uv_wref.IsNull() == false) {
            UniformBindingTypeEnum binding_type = uv_wref.GetRef().GetType();
            uint32_t binding_id = uv_wref.GetRef().GetBindingID();

            if (binding_type == UniformBindingType_UNIFORM_BUFFER) {
                UniformBufferWeakReferenceObject buffer_wref = uv_wref.DynamicCastTo<UniformBuffer>();
                UniformBufferIdentity ub_identity = GetIdentity(buffer_wref);

                VkDescriptorBufferInfo basic_uniform_b_info = {};
                basic_uniform_b_info.buffer = reinterpret_cast<VkBuffer>(ub_identity.m_buffer_handle);
                basic_uniform_b_info.offset = 0;
                basic_uniform_b_info.range = ub_identity.m_data_size;
                decriptor_buffer_infos.push_back(basic_uniform_b_info);

                VkWriteDescriptorSet buffer_set_info = InitializeVkWriteDescriptorSetInfo();
                buffer_set_info.dstSet = reinterpret_cast<VkDescriptorSet>(i_identity.m_handle);
                buffer_set_info.dstBinding = binding_id;
                buffer_set_info.descriptorCount = 1;
                buffer_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                buffer_set_info.pBufferInfo = &(decriptor_buffer_infos.back());

                write_infos.push_back(buffer_set_info);
            }
            else if (binding_type == UniformBindingType_COMBINED_IMAGE_SAMPLER) {
                UniformImagesWeakReferenceObject image_wref = uv_wref.DynamicCastTo<UniformImages>();
                const std::vector<TextureWeakReferenceObject> tex_wrefs = image_wref.GetRef().GetTextures();
                decriptor_img_infos.push_back(std::vector<VkDescriptorImageInfo>());
                std::vector<VkDescriptorImageInfo>& uv_image_infos = decriptor_img_infos.back();

                for (const TextureWeakReferenceObject &tex_wref : tex_wrefs) {
                    TextureIdentity tex_identity = GetIdentity(tex_wref);
                    SamplerIdentity sampler_identity = GetIdentityFromTexture(tex_wref);
                    VkDescriptorImageInfo tex_i_info = {};
                    tex_i_info.sampler = reinterpret_cast<VkSampler>(sampler_identity.m_handle);
                    tex_i_info.imageView = reinterpret_cast<VkImageView>(tex_identity.m_view_handle);
                    tex_i_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    uv_image_infos.push_back(tex_i_info);
                }

                VkWriteDescriptorSet texs_set_info = InitializeVkWriteDescriptorSetInfo();
                texs_set_info.dstSet = reinterpret_cast<VkDescriptorSet>(i_identity.m_handle);
                texs_set_info.dstBinding = binding_id;
                texs_set_info.descriptorCount = static_cast<uint32_t>(uv_image_infos.size());
                texs_set_info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                texs_set_info.pImageInfo = uv_image_infos.data();
                write_infos.push_back(texs_set_info);
            }
        }
    }

    UpdateVkDescriptorSet(write_infos, copy_infos);
}

void VulkanManager::FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref)
{
    VkDescriptorSet &ds_handle = reinterpret_cast<VkDescriptorSet&>(io_identity.m_handle);
    const DescriptorPoolIdentity &pool_identity = GetIdentity(i_pool_wref);
    VkDescriptorPool dp_handle = reinterpret_cast<VkDescriptorPool>(pool_identity.m_handle);
    FreeVkDescriptorSet(ds_handle, dp_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________ 
