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

#include "IndexBufferFormat_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkFormat IndexBufferFormat_Vulkan::BufferFormats[IndexBufferFormat_MAX_DEFINE_VALUE] =
{
    VK_FORMAT_R16_UINT,
    VK_FORMAT_R32_UINT
};

VkIndexType IndexBufferFormat_Vulkan::IndexTypes[IndexBufferFormat_MAX_DEFINE_VALUE] =
{
    VK_INDEX_TYPE_UINT16,
    VK_INDEX_TYPE_UINT32
};

VkFormat IndexBufferFormat_Vulkan::Convert(const IndexBufferFormatEnum &i_src)
{
    if (i_src != IndexBufferFormat_MAX_DEFINE_VALUE) {
        return BufferFormats[SD_ENUM_TO_UINT(i_src)];
    }
    else {
        return VK_FORMAT_MAX_ENUM;
    }
}

VkIndexType IndexBufferFormat_Vulkan::ConvertIndexType(const IndexBufferFormatEnum &i_src)
{
    if (i_src != IndexBufferFormat_MAX_DEFINE_VALUE) {
        return IndexTypes[SD_ENUM_TO_UINT(i_src)];
    }
    else {
        return VK_INDEX_TYPE_MAX_ENUM;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________