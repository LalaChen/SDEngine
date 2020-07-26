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
#include "UniformBindingType_Vulkan.h"
#include "UniformImages.h"
#include "UniformBuffer.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

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


void VulkanManager::AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref, const GraphicsPipelineWeakReferenceObject &i_pipe_wref)
{
    VkDescriptorSet &ds_handle = reinterpret_cast<VkDescriptorSet&>(io_identity.m_handle);
    const DescriptorPoolIdentity &pool_identity = GetIdentity(i_pool_wref);
    const GraphicsPipelineIdentity &pipe_identity = GetIdentity(i_pipe_wref);
    VkDescriptorSetAllocateInfo a_info = InitializeVkDescriptorSetAllocateInfo();
    a_info.descriptorPool = reinterpret_cast<VkDescriptorPool>(pool_identity.m_handle);
    a_info.pSetLayouts = reinterpret_cast<const VkDescriptorSetLayout*>(&pipe_identity.m_descriptor_layout_handle);
    a_info.descriptorSetCount = 1;
}

void VulkanManager::WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uv_wrefs)
{
    std::vector<VkWriteDescriptorSet> write_infos;
    std::vector<VkCopyDescriptorSet> copy_infos;
    std::vector<VkDescriptorImageInfo> decriptor_img_infos;
    std::vector<VkDescriptorBufferInfo> decriptor_buffer_infos;

    for (const UniformVariableWeakReferenceObject &uv_wref : i_uv_wrefs) {
        if (uv_wref.IsNull() == false) {
            UniformBindingTypeEnum binding_type = uv_wref.GetRef().GetType();
            uint32_t binding_id = uv_wref.GetRef().GetBindingID();

            if (binding_type == UniformBindingType_UNIFORM_BUFFER) {
                UniformBufferWeakReferenceObject buffer_wref = uv_wref.DynamicCastTo<UniformBuffer>();
                UniformBufferIdentity ub_identity = GetIdentity(buffer_wref);

                VkDescriptorBufferInfo basic_uniform_b_info = InitializeVkDescriptorBufferInfo();
                basic_uniform_b_info.buffer = reinterpret_cast<VkBuffer>(ub_identity.m_buffer_handle);
                basic_uniform_b_info.offset = 0;
                basic_uniform_b_info.range = ub_identity.m_data_size;
                decriptor_buffer_infos.push_back(basic_uniform_b_info);

                VkWriteDescriptorSet buffer_set_info = InitializeVkWriteDescriptorSetInfo();
                buffer_set_info.dstSet = reinterpret_cast<VkDescriptorSet>(i_identity.m_handle);
                buffer_set_info.dstBinding = binding_id;
                buffer_set_info.descriptorCount = 1;
                buffer_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                buffer_set_info.pBufferInfo = &decriptor_buffer_infos[decriptor_buffer_infos.size() - 1];

                write_infos.push_back(buffer_set_info);
            }
            else if (binding_type == UniformBindingType_COMBINED_IMAGE_SAMPLER) {
                UniformImagesWeakReferenceObject image_wref = uv_wref.DynamicCastTo<UniformImages>();
                const std::vector<TextureWeakReferenceObject> tex_wrefs = image_wref.GetRef().GetTextures();
                uint32_t tex_start_id = decriptor_img_infos.size();
                uint32_t tex_amount = 0;

                for (TextureWeakReferenceObject tex_wref : tex_wrefs) {
                    TextureIdentity tex_identity = GetIdentity(tex_wref);
                    SamplerIdentity sampler_identity = GetIdentityFromTexture(tex_wref);
                    VkDescriptorImageInfo tex_i_info = InitializeVkDescriptorImageInfo();
                    tex_i_info.sampler = reinterpret_cast<VkSampler>(sampler_identity.m_handle);
                    tex_i_info.imageView = reinterpret_cast<VkImageView>(tex_identity.m_view_handle);
                    tex_i_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    decriptor_img_infos.push_back(tex_i_info);
                    ++tex_amount;
                }

                VkWriteDescriptorSet texs_set_info = InitializeVkWriteDescriptorSetInfo();
                texs_set_info.dstSet = reinterpret_cast<VkDescriptorSet>(i_identity.m_handle);
                texs_set_info.dstBinding = binding_id;
                texs_set_info.descriptorCount = tex_amount;
                texs_set_info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                texs_set_info.pImageInfo = &decriptor_img_infos[tex_start_id];
            }
        }
    }

    UpdateVkDescriptorSet(write_infos, copy_infos);
}

void VulkanManager::BindDescriptorSet(const DescriptorSetIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref, const GraphicsPipelineWeakReferenceObject &i_pipe_wref)
{
    const CommandBufferIdentity &cb_identity = GetIdentity(i_cb_wref);
    const GraphicsPipelineIdentity &pipe_identity = GetIdentity(i_pipe_wref);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    VkDescriptorSet ds_handle = reinterpret_cast<VkDescriptorSet>(i_identity.m_handle);
    VkPipelineLayout pipe_layout_handle = reinterpret_cast<VkPipelineLayout>(pipe_identity.m_pipeline_layout_handle);
    BindVkDescriptorSet(
        cb_handle, VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipe_layout_handle, ds_handle);
}

void VulkanManager::FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref)
{
    VkDescriptorSet &ds_handle = reinterpret_cast<VkDescriptorSet&>(io_identity.m_handle);
    const DescriptorPoolIdentity &pool_identity = GetIdentity(i_pool_wref);
    VkDescriptorPool dp_handle = reinterpret_cast<VkDescriptorPool>(pool_identity.m_handle);
    FreeVkDescriptorSet(ds_handle, dp_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________ 
