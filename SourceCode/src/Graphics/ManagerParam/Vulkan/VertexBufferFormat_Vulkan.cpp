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

#include <vulkan/vulkan.h>

#include "VertexBufferFormat_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkFormat VertexBufferFormat_Vulkan::VertexBufferFormatTypes[VertexBufferFormat_MAX_DEFINE_VALUE] = {
//------ unsigned byte.
    VK_FORMAT_R8_UINT,
    VK_FORMAT_R8G8_UINT,
    VK_FORMAT_R8G8B8_UINT,
    VK_FORMAT_R8G8B8A8_UINT,
//------ signed byte.
    VK_FORMAT_R8_SINT,
    VK_FORMAT_R8G8_SINT,
    VK_FORMAT_R8G8B8_SINT,
    VK_FORMAT_R8G8B8A8_SINT,
//------ unsigned short.
    VK_FORMAT_R16_UINT,
    VK_FORMAT_R16G16_UINT,
    VK_FORMAT_R16G16B16_UINT,
    VK_FORMAT_R16G16B16A16_UINT,
//------ signed short.
    VK_FORMAT_R16_SINT,
    VK_FORMAT_R16G16_SINT,
    VK_FORMAT_R16G16B16_SINT,
    VK_FORMAT_R16G16B16A16_SINT,
//------ unsigned int32.
    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32A32_UINT,
//------ signed int32.
    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32A32_SINT,
//------ signed float.
    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT,
//------ unsigned int64.
    VK_FORMAT_R64_UINT,
    VK_FORMAT_R64G64_UINT,
    VK_FORMAT_R64G64B64_UINT,
    VK_FORMAT_R64G64B64A64_UINT,
//------ signed int64.
    VK_FORMAT_R64_SINT,
    VK_FORMAT_R64G64_SINT,
    VK_FORMAT_R64G64B64_SINT,
    VK_FORMAT_R64G64B64A64_SINT,
//------ double.
    VK_FORMAT_R64_SFLOAT,
    VK_FORMAT_R64G64_SFLOAT,
    VK_FORMAT_R64G64B64_SFLOAT,
    VK_FORMAT_R64G64B64A64_SFLOAT
};

VkFormat VertexBufferFormat_Vulkan::Convert(const VertexBufferFormatEnum &i_src)
{
    if (i_src != VertexBufferFormat_MAX_DEFINE_VALUE) {
        return VertexBufferFormatTypes[static_cast<uint32_t>(i_src)];
    }
    else {
        return VK_FORMAT_UNDEFINED;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________