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

#include "ImageAspect_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkImageAspectFlagBits ImageAspect_Vulkan::ImageAspects[ImageAspect_MAX_DEFINE_VALUE] =
{
    VK_IMAGE_ASPECT_COLOR_BIT,
    VK_IMAGE_ASPECT_DEPTH_BIT,
    VK_IMAGE_ASPECT_STENCIL_BIT,
    VK_IMAGE_ASPECT_METADATA_BIT,
    VK_IMAGE_ASPECT_PLANE_0_BIT,
    VK_IMAGE_ASPECT_PLANE_1_BIT,
    VK_IMAGE_ASPECT_PLANE_2_BIT
};

VkImageAspectFlags ImageAspect_Vulkan::Convert(const std::vector<ImageAspectEnum> &i_srcs)
{
    VkImageAspectFlags result = 0;
    for (ImageAspectEnum ia : i_srcs) {
        if (ia != ImageAspect_MAX_DEFINE_VALUE) {
            result |= ImageAspects[SD_ENUM_TO_UINT(ia)];
        }
    }
    return result;
}

VkImageAspectFlags ImageAspect_Vulkan::Convert(const ImageAspectEnum &i_src)
{
    VkImageAspectFlags result = 0;
    if (i_src != ImageAspect_MAX_DEFINE_VALUE) {
        result |= ImageAspects[SD_ENUM_TO_UINT(i_src)];
    }
    return result;
}

______________SD_END_GRAPHICS_NAMESPACE______________