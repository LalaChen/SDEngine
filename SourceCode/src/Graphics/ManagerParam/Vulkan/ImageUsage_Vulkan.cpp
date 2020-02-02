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

#include "ImageUsage_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkImageUsageFlagBits ImageUsage_Vulkan::ImageUsages[ImageUsage_MAX_DEFINE_VALUE] =
{
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_USAGE_STORAGE_BIT,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
};

VkImageUsageFlags ImageUsage_Vulkan::Convert(const std::vector<ImageUsageEnum> &i_srcs)
{
    VkImageUsageFlags result = 0;
    for (ImageUsageEnum iu : i_srcs) {
        if (iu != ImageUsage_MAX_DEFINE_VALUE) {
            result |= ImageUsages[SD_ENUM_TO_UINT(iu)];
        }
    }
    return result;
}

VkImageUsageFlags ImageUsage_Vulkan::Convert(ImageUsageEnum i_src)
{
    VkImageUsageFlags result = 0;
    if (i_src != ImageUsage_MAX_DEFINE_VALUE) {
        result |= ImageUsages[SD_ENUM_TO_UINT(i_src)];
    }
    return result;
}

______________SD_END_GRAPHICS_NAMESPACE______________