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

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkResult VulkanManager::CreateVkImage(
    VkImage &io_image_handle,
    VkImageType i_type,
    VkFormat i_image_format,
    VkExtent3D i_image_size,
    VkImageUsageFlags i_usage_flags,
    VkImageLayout i_image_layout,
    uint32_t i_mipmap_levels,
    uint32_t i_array_layers,
    VkImageTiling i_tiling_mode,
    VkSharingMode i_sharing_mode)
{
    //1. Write image creating information.
    VkImageCreateInfo img_c_info = {};
    img_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    img_c_info.pNext = nullptr;
    img_c_info.flags = 0; //We need to set flag when we want to use sparse texture data or sth.
    img_c_info.imageType = i_type;
    img_c_info.mipLevels = i_mipmap_levels;// mipmap levels. We don't want to create mip map in here.
    img_c_info.arrayLayers = i_array_layers; //Use it when data is 2d texture array.
    img_c_info.tiling = i_tiling_mode; //Set texture tiling mode. If the image is linked data in system memory, we need to use VK_IMAGE_TILING_LINEAR.
    img_c_info.initialLayout = i_image_layout;
    img_c_info.usage = i_usage_flags; //We will copy data to this image(trasnfer dst) and use it in shader(sampled).
    img_c_info.samples = VK_SAMPLE_COUNT_1_BIT;
    img_c_info.extent = i_image_size;
    img_c_info.format = i_image_format;
    img_c_info.sharingMode = i_sharing_mode;

    if (i_sharing_mode == VK_SHARING_MODE_EXCLUSIVE) {
        img_c_info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
        img_c_info.pQueueFamilyIndices = nullptr;
    }
    else {
        SDLOGW("Not support concurrent mode (I haven't design for concurrent res now)!!");
    }

    return vkCreateImage(m_VK_device, &img_c_info, nullptr, &io_image_handle);
}

VkResult VulkanManager::CopyVkBufferToVkImage(
    VkBuffer i_src_buffer_handle,
    VkDeviceSize i_data_size,
    VkImage i_dst_image_handle,
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

    result = vkBeginCommandBuffer(m_VK_main_cmd_buffer, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer at copy buffer to image(%x)!!!", m_VK_main_cmd_buffer);
        return result;
    }
    //2. set buffer memory barrier (block when transfer).
    SwitchVKImageLayout(m_VK_main_cmd_buffer,
        i_dst_image_handle,
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

    vkCmdCopyBufferToImage(m_VK_main_cmd_buffer, i_src_buffer_handle, i_dst_image_handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buf_to_img_cpy_info);

    //4. set buffer memory barrier (block transfer).
    SwitchVKImageLayout(m_VK_main_cmd_buffer,
        i_dst_image_handle,
        VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, i_dst_image_final_layout,
        VK_PIPELINE_STAGE_TRANSFER_BIT, i_dst_pipe_stage_flags);

    //5. Submit command.
    VkPipelineStageFlags submit_wait_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    submit_info.pWaitDstStageMask = nullptr;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_VK_main_cmd_buffer;

    result = vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure when copying buffer to image !!!");
        return result;
    }

    do {
        result = vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_TRUE, MaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure when copying buffer to image !!!");
        return result;
    }

    result = vkResetCommandBuffer(m_VK_main_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer failure!!!");
        return result;
    }

    return VK_SUCCESS;
}

void VulkanManager::DestroyVkImage(VkImage &io_image_handle)
{
    if (io_image_handle != VK_NULL_HANDLE) {
        vkDestroyImage(m_VK_device, io_image_handle, nullptr);
    }
    io_image_handle = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________