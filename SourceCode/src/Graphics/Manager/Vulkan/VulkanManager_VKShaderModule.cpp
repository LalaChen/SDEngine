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

VkResult VulkanManager::CreateVKShaderModule(
    VkShaderModule &io_shader_module_handle,
    VkShaderStageFlagBits i_stage,
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

    result = vkCreateShaderModule(m_VK_device, &c_info, nullptr, &io_shader_module_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create shader module! Result = %x.");
    }

    return result;
}

void VulkanManager::DestroyVKShaderModule(VkShaderModule &io_shader_module_handle)
{
    if (io_shader_module_handle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_VK_device, io_shader_module_handle, nullptr);
    }
    io_shader_module_handle = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________