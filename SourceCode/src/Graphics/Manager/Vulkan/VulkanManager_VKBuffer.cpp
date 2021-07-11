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

//----------- Vertex Buffer Function ------------
VkResult VulkanManager::CreateVkBuffer(
    VkBuffer &io_handle,
    VkDeviceSize i_size,
    VkBufferUsageFlags i_buffer_usage,
    VkSharingMode i_sharing_mode)
{
    //1. create buffer information.
    VkBufferCreateInfo vec_buf_c_info = {};
    vec_buf_c_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vec_buf_c_info.pNext = nullptr;
    vec_buf_c_info.flags = 0;
    vec_buf_c_info.usage = i_buffer_usage;
    vec_buf_c_info.size = i_size;
    vec_buf_c_info.sharingMode = i_sharing_mode;
    if (i_sharing_mode == VK_SHARING_MODE_EXCLUSIVE) {
        vec_buf_c_info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
        vec_buf_c_info.pQueueFamilyIndices = nullptr;
    }
    else {
        SDLOGW("Not support concurrent mode (I haven't design for concurrent res now)!!");
    }

    return vkCreateBuffer(m_device_handle, &vec_buf_c_info, nullptr, &io_handle);
}

VkResult VulkanManager::RefreshDataToHostVisibleVKDeviceMemory(VkDeviceMemory i_memory_handle, VkDeviceSize i_allocated_size, const void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    void *local_ptr = nullptr;
    result = MapVkDeviceMemory(i_memory_handle, i_allocated_size, local_ptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    //--- i. memory copy.
    std::memcpy(local_ptr, i_data_ptr, i_data_size);

    //--- ii. flush memory.
    result = FlushMappedVkDeviceMemoryRanges(i_memory_handle);
    if (result != VK_SUCCESS) {
        return result;
    }
    //--- iii. unmap memory.
    UnmapVkDeviceMemory(i_memory_handle);
    return result;
}

VkResult VulkanManager::CopyVkBuffer(
    VkCommandBuffer i_cb_handle,
    VkBuffer i_sb_handle,
    VkDeviceSize i_data_size,
    VkBuffer i_db_handle,
    VkAccessFlags i_src_access_flags,
    VkAccessFlags i_dst_access_flags,
    VkPipelineStageFlags i_src_pipe_stage_flags,
    VkPipelineStageFlags i_dst_pipe_stage_flags)
{
    VkResult result = VK_SUCCESS;
    //1. begin command buffer.
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(i_cb_handle, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%p)(e:%d)!!!", i_cb_handle, result);
        return result;
    }
    //--- i. set buffer memory barrier (block transfer).
    SwitchVKBufferLayout(i_cb_handle,
        i_db_handle, 0, VK_WHOLE_SIZE,
        i_src_pipe_stage_flags, 
        VK_PIPELINE_STAGE_TRANSFER_BIT);

    //--- ii. copy buffer.
    VkBufferCopy buf_cpy_info = {};
    buf_cpy_info.size = i_data_size;
    buf_cpy_info.srcOffset = 0;
    buf_cpy_info.dstOffset = 0;
    vkCmdCopyBuffer(i_cb_handle, i_sb_handle, i_db_handle, 1, &buf_cpy_info);

    //--- iii. set buffer memory barrier (block transfer).
    SwitchVKBufferLayout(i_cb_handle,
        i_db_handle, 0, VK_WHOLE_SIZE,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        i_dst_pipe_stage_flags);

    //--- iv. end command buffer.
    result = vkEndCommandBuffer(i_cb_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", i_cb_handle);
        return result;
    }

    //3. Submit command.
    std::vector<VkCommandBuffer> cb_handles = {i_cb_handle};
    return SubmitVkCommandBuffers(cb_handles);
}

void VulkanManager::BindVkVertexBuffer(
    VkCommandBuffer i_cb_handle,
    VkBuffer i_vb_handle,
    uint32_t i_binding_id,
    VkDeviceSize i_offset)
{
    vkCmdBindVertexBuffers(i_cb_handle, i_binding_id, 1, &i_vb_handle, &i_offset);
}

void VulkanManager::BindVkIndexBuffer(
    VkCommandBuffer i_cb_handle,
    VkBuffer i_ib_handle,
    VkDeviceSize i_offset,
    VkIndexType i_type)
{
    vkCmdBindIndexBuffer(i_cb_handle, i_ib_handle, i_offset, i_type);
}

void VulkanManager::DestroyVkBuffer(VkBuffer &io_buffer_handle)
{
    if (io_buffer_handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device_handle, io_buffer_handle, nullptr);
    }
    io_buffer_handle = VK_NULL_HANDLE;
}

______________SD_END_GRAPHICS_NAMESPACE______________