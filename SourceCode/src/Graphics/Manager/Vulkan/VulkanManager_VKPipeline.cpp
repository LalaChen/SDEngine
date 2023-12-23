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

#include "VulkanManager.h"
#include "VulkanWrapper.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkResult VulkanManager::CreateVKShaderModule(
    VkShaderModule &io_module,
    VkDevice i_device,
    const UByte *i_binary_ptr,
    const Size_ui64 i_binary_size)
{
    VkResult result = VK_SUCCESS;

    VkShaderModuleCreateInfo c_info = {};
    c_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    c_info.pNext = nullptr;
    c_info.flags = 0;
    c_info.codeSize = i_binary_size;
    c_info.pCode = reinterpret_cast<const uint32_t*>(i_binary_ptr);

    result = vkCreateShaderModule(i_device, &c_info, nullptr, &io_module);
    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create shader module! Result = %x.", result);
    }

    return result;
}

void VulkanManager::DestroyVKShaderModule(
    VkShaderModule &io_module,
    VkDevice i_device)
{
    if (io_module != VK_NULL_HANDLE) {
        vkDestroyShaderModule(i_device, io_module, nullptr);
    }
    io_module = VK_NULL_HANDLE;
}


VkResult VulkanManager::CreateVKDescriptorSetLayout(
    VkDescriptorSetLayout &io_layout,
    VkDevice i_device,
    const VkDescriptorSetLayoutCreateInfo& i_info)
{
    VkResult result = VK_SUCCESS;
    result = vkCreateDescriptorSetLayout(i_device, &i_info, nullptr, &io_layout);

    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create descriptor set. Result = %x.", result);
    }

    return result;
}

void VulkanManager::DestroyVKDescriptorSetLayout(
    VkDescriptorSetLayout &io_layout,
    VkDevice i_device)
{
    if (io_layout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(i_device, io_layout, nullptr);
    }
    io_layout = VK_NULL_HANDLE;
}

VkResult VulkanManager::CreateVKPipelineLayout(
    VkPipelineLayout &io_layout,
    VkDevice i_device,
    const VkPipelineLayoutCreateInfo & i_info)
{
    VkResult result = VK_SUCCESS;
    result = vkCreatePipelineLayout(i_device, &i_info, nullptr, &io_layout);

    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create pipeline layout. Result = %x.", result);
    }

    return result;
}

void VulkanManager::DestroyVKPipelineLayout(
    VkPipelineLayout &io_layout,
    VkDevice i_device)
{
    if (io_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(i_device, io_layout, nullptr);
    }
    io_layout = VK_NULL_HANDLE;
}

VkResult VulkanManager::CreateVKPipeline(
    VkPipeline &io_pipeline,
    VkDevice i_device,
    const VkGraphicsPipelineCreateInfo &i_info)
{
    VkResult result = VK_SUCCESS;
    result = vkCreateGraphicsPipelines(i_device, VK_NULL_HANDLE, 1, &i_info, nullptr, &io_pipeline);

    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create pipeline set. Result = %x.", result);
    }

    return result;
}

void VulkanManager::BindVkPipeline(
    VkCommandBuffer i_cmd_buffer,
    VkPipeline i_pipeline,
    VkPipelineBindPoint i_pipe_point)
{
    vkCmdBindPipeline(i_cmd_buffer, i_pipe_point, i_pipeline);
}

void VulkanManager::DestroyVKPipeline(
    VkPipeline &io_pipeline,
    VkDevice i_device)
{
    if (io_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(i_device, io_pipeline, nullptr);
    }
    io_pipeline = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________