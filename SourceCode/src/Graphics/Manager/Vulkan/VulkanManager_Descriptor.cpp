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

void VulkanManager::CreateDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity, const std::vector<UniformVariableDescriptorWeakReferenceObject> &i_uvds)
{
    VkDescriptorSetLayout &ds_layout = reinterpret_cast<VkDescriptorSetLayout&>(io_identity.m_handle);
    VkDevice              &device    = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;
    std::vector<VkDescriptorSetLayoutBinding> vk_dsl_bindings;
    for (const UniformVariableDescriptorWeakReferenceObject &uvd : i_uvds) {
        UniformBinding ub = SD_WREF(uvd).CreateUniformBinding();
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
    VkResult result = CreateVKDescriptorSetLayout(ds_layout, device, dsl_c_info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create fence failure(%d) !!!", result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::DestroyDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity)
{
    VkDescriptorSetLayout &ds_layout = reinterpret_cast<VkDescriptorSetLayout&>(io_identity.m_handle);
    VkDevice              &device    = reinterpret_cast<VkDevice&>(io_identity.m_device);

    DestroyVkDescriptorSetLayout(ds_layout, device);
    io_identity.SetInvalid();
    io_identity = DescriptorSetLayoutIdentity();
}

void VulkanManager::CreateDescriptorPool(DescriptorPoolIdentity &io_identity)
{
    VkDescriptorPool &des_pool = reinterpret_cast<VkDescriptorPool&>(io_identity.m_handle);
    VkDevice         &device   = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkDescriptorPoolCreateInfo c_info = InitializeVkDescriptorPoolCreateInfo();

    device = m_device;

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
    VkResult result = CreateVkDescriptorPool(des_pool, device, c_info);
    if (result != VK_SUCCESS) {
        //
    } else {
        io_identity.SetValid();
    } 
}

void VulkanManager::DestroyDescriptorPool(DescriptorPoolIdentity &io_identity)
{
    VkDescriptorPool &pool   = reinterpret_cast<VkDescriptorPool&>(io_identity.m_handle);
    VkDevice         &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    DestroyVkDescriptorPool(pool, device);
    io_identity.SetInvalid();
    io_identity = DescriptorPoolIdentity();
}


void VulkanManager::AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool, const DescriptorSetLayoutWeakReferenceObject &i_layout)
{
    VkDescriptorSet &set    = reinterpret_cast<VkDescriptorSet&>(io_identity.m_handle);
    VkDevice        &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;

    const DescriptorPoolIdentity &pool_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_pool);
    const DescriptorSetLayoutIdentity &layout_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_layout);
    VkDescriptorSetAllocateInfo info = InitializeVkDescriptorSetAllocateInfo();
    info.descriptorPool = reinterpret_cast<VkDescriptorPool>(pool_identity.m_handle);
    info.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>(&layout_identity.m_handle);
    info.descriptorSetCount = 1;
    VkResult result = AllocateVkDescriptorSet(set, device, info);
    if (result != VK_SUCCESS) {
        SDLOGE("result = %d. PoolHandle : %p(%d, %d, %d), layoutHandle : %p, SetHandle : %p ", 
            result,
            pool_identity.m_handle, pool_identity.m_descriptor_counts[0], pool_identity.m_descriptor_counts[1], pool_identity.m_max_set,
            layout_identity.m_handle,
            io_identity.m_handle);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uvs)
{
    if (i_identity.m_handle == SD_NULL_HANDLE) {
        SDLOGE("Try to write uniform veriable into null descriptor set.");
        return;
    }

    if (i_uvs.size() == 0) {
        SDLOGE("no uniform variable need to write into this descriptor set.");
        return;
    }

    const VkDevice &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);

    std::vector<VkWriteDescriptorSet> write_infos;
    std::vector<VkCopyDescriptorSet> copy_infos;
    std::list<std::vector<VkDescriptorImageInfo>> decriptor_img_infos;
    std::list<VkDescriptorBufferInfo> decriptor_buffer_infos;

    for (const UniformVariableWeakReferenceObject &uv : i_uvs) {
        if (uv.IsNull() == false) {
            UniformBindingTypeEnum binding_type = SD_WREF(uv).GetType();
            uint32_t binding_id = SD_WREF(uv).GetBindingID();

            if (binding_type == UniformBindingType_UNIFORM_BUFFER) {
                UniformBufferWeakReferenceObject buffer = uv.DynamicCastTo<UniformBuffer>();
                UniformBufferIdentity ub_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(buffer);

                VkDescriptorBufferInfo basic_uniform_b_info = {};
                basic_uniform_b_info.buffer = reinterpret_cast<VkBuffer>(ub_identity.m_buffer);
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
                UniformImagesWeakReferenceObject image = uv.DynamicCastTo<UniformImages>();
                const std::vector<TextureWeakReferenceObject> texs = SD_WREF(image).GetTextures();
                decriptor_img_infos.push_back(std::vector<VkDescriptorImageInfo>());
                std::vector<VkDescriptorImageInfo>& uv_image_infos = decriptor_img_infos.back();

                for (const TextureWeakReferenceObject &tex : texs) {
                    if (tex.IsNull() == false) {
                        TextureIdentity tex_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(tex);
                        SamplerIdentity sampler_identity = SD_SREF(m_graphics_identity_getter).GetIdentityFromTexture(tex);
                        VkDescriptorImageInfo tex_i_info = {};
                        tex_i_info.sampler = reinterpret_cast<VkSampler>(sampler_identity.m_handle);
                        tex_i_info.imageView = reinterpret_cast<VkImageView>(tex_identity.m_image_view);
                        tex_i_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        uv_image_infos.push_back(tex_i_info);
                    }
                }

                if (uv_image_infos.size() > 0) {
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
    }

    UpdateVkDescriptorSet(device, write_infos, copy_infos);
}

void VulkanManager::FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool)
{
    VkDescriptorSet &set    = reinterpret_cast<VkDescriptorSet&>(io_identity.m_handle);
    VkDevice        &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    const DescriptorPoolIdentity &pool_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_pool);
    VkDescriptorPool pool = reinterpret_cast<VkDescriptorPool>(pool_identity.m_handle);
    FreeVkDescriptorSet(set, device, pool);
    io_identity.SetInvalid();
    io_identity = DescriptorSetIdentity();
}

______________SD_END_GRAPHICS_NAMESPACE______________ 
