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

void VulkanManager::SwitchVKImageLayout(
    VkCommandBuffer i_cmd_buffer,
    VkImage i_image_handle,
    uint32_t i_sub_src_range_aspect_mask,
    uint32_t i_sub_src_range_base_mip_lv,
    uint32_t i_sub_src_range_mip_lv_count,
    uint32_t i_sub_src_range_base_layer_lv,
    uint32_t i_sub_src_range_layer_lv_count,
    VkImageLayout i_old_layout,
    VkImageLayout i_new_layout,
    VkPipelineStageFlags i_src_stage,
    VkPipelineStageFlags i_dst_stage,
    VkAccessFlags i_src_access_mask,
    VkAccessFlags i_dst_access_mask,
    uint32_t i_src_queue_family_id,
    uint32_t i_dst_queue_family_id)
{
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.image = i_image_handle;
    barrier.oldLayout = i_old_layout;
    barrier.newLayout = i_new_layout;
    barrier.subresourceRange.aspectMask = i_sub_src_range_aspect_mask;
    barrier.subresourceRange.baseMipLevel = i_sub_src_range_base_mip_lv;
    barrier.subresourceRange.levelCount = i_sub_src_range_mip_lv_count;
    barrier.subresourceRange.baseArrayLayer = i_sub_src_range_base_layer_lv;
    barrier.subresourceRange.layerCount = i_sub_src_range_layer_lv_count;
    barrier.srcAccessMask = i_src_access_mask;
    barrier.dstAccessMask = i_dst_access_mask;
    barrier.srcQueueFamilyIndex = i_src_queue_family_id;
    barrier.dstQueueFamilyIndex = i_dst_queue_family_id;

    vkCmdPipelineBarrier(i_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        0,
        0, nullptr, //memory barrier
        0, nullptr, //buffer memory barrier
        1, &barrier //image memory barrier
    );
}

void VulkanManager::SwitchVKBufferLayout(
    VkCommandBuffer i_cmd_buffer,
    VkBuffer i_buffer_handle,
    VkDeviceSize i_offset,
    VkDeviceSize i_size,
    VkPipelineStageFlags i_src_pipeline_stage,
    VkPipelineStageFlags i_dst_pipeline_stage,
    VkAccessFlags i_src_access_mask,
    VkAccessFlags i_dst_access_mask,
    uint32_t i_src_queue_family_id,
    uint32_t i_dst_queue_family_id)
{
    VkBufferMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.buffer = i_buffer_handle;
    barrier.offset = i_offset;
    barrier.size = VK_WHOLE_SIZE;
    barrier.srcAccessMask = i_src_access_mask; //The buffer doesn't have access state.
    barrier.dstAccessMask = i_dst_access_mask;
    barrier.srcQueueFamilyIndex = i_src_queue_family_id;
    barrier.dstQueueFamilyIndex = i_dst_queue_family_id;

    vkCmdPipelineBarrier(i_cmd_buffer, i_src_pipeline_stage, i_dst_pipeline_stage,
        0,
        0, nullptr, //memory barrier
        1, &barrier, //buffer memory barrier
        0, nullptr //image memory barrier
    );
}

______________SD_END_GRAPHICS_NAMESPACE______________