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
#include "ShaderKind_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkShaderStageFlagBits ShaderKind_Vulkan::ShaderKinds[ShaderKind_MAX_DEFINE_VALUE] =
{
    VK_SHADER_STAGE_VERTEX_BIT,
    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    VK_SHADER_STAGE_GEOMETRY_BIT,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    VK_SHADER_STAGE_COMPUTE_BIT
};

VkShaderStageFlagBits ShaderKind_Vulkan::Convert(const ShaderKindEnum &i_src)
{
    if (i_src != ShaderKind_MAX_DEFINE_VALUE) {
        return ShaderKinds[static_cast<uint32_t>(i_src)];
    }
    else {
        return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________