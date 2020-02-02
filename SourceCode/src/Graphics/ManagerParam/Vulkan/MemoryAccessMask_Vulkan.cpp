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

#include "MemoryAccessMask_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkAccessFlagBits MemoryAccess_Vulkan::MemoryAccessMasks[MemoryAccessMask_MAX_DEFINE_VALUE] =
{
    VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
    VK_ACCESS_INDEX_READ_BIT,
    VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    VK_ACCESS_UNIFORM_READ_BIT,
    VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    VK_ACCESS_SHADER_READ_BIT,
    VK_ACCESS_SHADER_WRITE_BIT,
    VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_TRANSFER_READ_BIT,
    VK_ACCESS_TRANSFER_WRITE_BIT,
    VK_ACCESS_HOST_READ_BIT,
    VK_ACCESS_HOST_WRITE_BIT,
    VK_ACCESS_MEMORY_READ_BIT,
    VK_ACCESS_MEMORY_WRITE_BIT
};

VkAccessFlags MemoryAccess_Vulkan::Convert(const std::vector<MemoryAccessMaskEnum> &i_srcs)
{
    VkAccessFlags result = 0;
    for (MemoryAccessMaskEnum access_mask : i_srcs) {
        if (SD_ENUM_TO_INT(access_mask) < MemoryAccessMask_MAX_DEFINE_VALUE) {
            result |= MemoryAccessMasks[SD_ENUM_TO_INT(access_mask)];
        }
    }
    return result;
}

VkAccessFlags MemoryAccess_Vulkan::Convert(MemoryAccessMaskEnum i_src)
{
    VkAccessFlags result = 0;
    if (SD_ENUM_TO_INT(i_src) < MemoryAccessMask_MAX_DEFINE_VALUE) {
        result |= MemoryAccessMasks[SD_ENUM_TO_INT(i_src)];
    }
    return result;
}

______________SD_END_GRAPHICS_NAMESPACE______________