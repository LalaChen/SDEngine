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

#include "LogManager.h"
#include "VulkanManager.h"
#include "ShaderKind_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content)
{
    VkShaderModule &shader_module = reinterpret_cast<VkShaderModule&>(io_identity.m_prog_handle);
    VkShaderStageFlagBits shader_stage = ShaderKind_Vulkan::Convert(io_identity.m_shader_kind);
    CreateVKShaderModule(shader_module, shader_stage, i_content.data(), i_content.size());
}

void VulkanManager::DeleteShaderModule(ShaderModuleIdentity &io_identity)
{
    VkShaderModule &shader_module = reinterpret_cast<VkShaderModule&>(io_identity.m_prog_handle);
    DestroyVKShaderModule(shader_module);
}

______________SD_END_GRAPHICS_NAMESPACE______________