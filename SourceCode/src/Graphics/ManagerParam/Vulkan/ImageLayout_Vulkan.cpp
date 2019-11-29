#include "ImageLayout_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkImageLayout ImageLayout_Vulkan::ImageLayouts[ImageLayout_MAX_DEFINE_VALUE] =
{
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    VK_IMAGE_LAYOUT_PREINITIALIZED
};

VkImageLayout ImageLayout_Vulkan::Convert(const ImageLayoutEnum &i_src)
{
    if (i_src != ImageLayout_MAX_DEFINE_VALUE) {
        return ImageLayouts[static_cast<uint32_t>(i_src)];
    }
    else {
        return VK_IMAGE_LAYOUT_MAX_ENUM;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________