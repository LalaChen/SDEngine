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
    VkRenderPassCreateInfo rp_c_info = {};
    rp_c_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_c_info.flags = 0;
    rp_c_info.pNext = nullptr;
    rp_c_info.attachmentCount = static_cast<uint32_t>(i_vk_att_descs.size());
    rp_c_info.pAttachments = i_vk_att_descs.data();
    rp_c_info.dependencyCount = static_cast<uint32_t>(i_vk_sp_dependencies.size());
    rp_c_info.pDependencies = i_vk_sp_dependencies.data();
    rp_c_info.subpassCount = static_cast<uint32_t>(i_vk_sps.size());
    rp_c_info.pSubpasses = i_vk_sps.data();
    return rp_c_info;
}

VkRenderPassMultiviewCreateInfo InitializeVkRenderPassMultiviewCreateInfo(
    const std::vector<uint32_t> &i_view_masks,
    const std::vector<int32_t> &i_view_offsets,
    const std::vector<uint32_t> &i_correlation_masks
)
{
    VkRenderPassMultiviewCreateInfo rpm_c_info = {};
    rpm_c_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
    rpm_c_info.pNext = nullptr;
    rpm_c_info.subpassCount = static_cast<uint32_t>(i_view_masks.size());
    rpm_c_info.pViewMasks = i_view_masks.data();
    rpm_c_info.dependencyCount = static_cast<uint32_t>(i_view_offsets.size());
    rpm_c_info.pViewOffsets = i_view_offsets.data();
    rpm_c_info.correlationMaskCount = static_cast<uint32_t>(i_correlation_masks.size());
    rpm_c_info.pCorrelationMasks = i_correlation_masks.data();
    return rpm_c_info;
}

______________SD_END_GRAPHICS_NAMESPACE______________