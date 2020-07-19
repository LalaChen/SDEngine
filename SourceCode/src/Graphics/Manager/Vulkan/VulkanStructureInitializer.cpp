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

VkDescriptorBufferInfo InitializeVkDescriptorBufferInfo()
{
    VkDescriptorBufferInfo info = {};
    info.buffer = VK_NULL_HANDLE;
    info.offset = 0;
    info.range = 0;
    return info;
}

VkDescriptorImageInfo InitializeVkDescriptorImageInfo()
{
    VkDescriptorImageInfo info = {};
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

______________SD_END_GRAPHICS_NAMESPACE______________