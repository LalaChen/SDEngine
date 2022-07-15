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

VkResult VulkanManager::CreateVkImage(
    VkImage &io_image,
    VkDevice i_device,
    const VkImageCreateInfo &i_info)
{
    return vkCreateImage(i_device, &i_info, nullptr, &io_image);
}

VkResult VulkanManager::PrepareCopyVkBufferToVkImage(
    VkCommandBuffer i_cmd_buffer,
    VkBuffer i_src_buffer,
    VkDeviceSize i_data_size,
    VkImage i_dst_image,
    VkOffset3D i_image_offset,
    VkExtent3D i_image_size,
    VkAccessFlags i_src_access_flags,
    VkAccessFlags i_dst_access_flags,
    VkImageLayout i_dst_image_original_layout,
    VkImageLayout i_dst_image_final_layout,
    VkPipelineStageFlags i_src_pipe_stage_flags,
    VkPipelineStageFlags i_dst_pipe_stage_flags)
{
    VkResult result = VK_NOT_READY;
    //1. begin command buffer.
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(i_cmd_buffer, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer failure(%d) while copying buffer to image(%x)!!!", result, i_cmd_buffer);
        return result;
    }

    //2. set buffer memory barrier (block when transfer).
    SwitchVKImageLayout(i_cmd_buffer, i_dst_image,
        VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1,
        i_dst_image_original_layout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        i_src_pipe_stage_flags, VK_PIPELINE_STAGE_TRANSFER_BIT);

    //3. copy buffer.
    VkBufferImageCopy buf_to_img_cpy_info = {};
    buf_to_img_cpy_info.bufferOffset = 0;
    buf_to_img_cpy_info.bufferRowLength = 0;
    buf_to_img_cpy_info.bufferImageHeight = 0;
    buf_to_img_cpy_info.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    buf_to_img_cpy_info.imageSubresource.mipLevel = 0;
    buf_to_img_cpy_info.imageSubresource.baseArrayLayer = 0;
    buf_to_img_cpy_info.imageSubresource.layerCount = 1;
    buf_to_img_cpy_info.imageOffset = i_image_offset;
    buf_to_img_cpy_info.imageExtent = i_image_size;

    vkCmdCopyBufferToImage(i_cmd_buffer, i_src_buffer, i_dst_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buf_to_img_cpy_info);

    //4. set buffer memory barrier (block transfer).
    SwitchVKImageLayout(i_cmd_buffer, i_dst_image,
        VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, i_dst_image_final_layout,
        VK_PIPELINE_STAGE_TRANSFER_BIT, i_dst_pipe_stage_flags);

    vkEndCommandBuffer(i_cmd_buffer);
 
    return VK_SUCCESS;
}

void VulkanManager::DestroyVkImage(VkImage &io_image, VkDevice i_device)
{
    if (io_image != VK_NULL_HANDLE) {
        vkDestroyImage(i_device, io_image, nullptr);
    }
    io_image = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________