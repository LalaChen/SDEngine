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

//--------------------------- start of namespace SDE ------------------------------
namespace SDE
{
//------------------------- start of namespace Graphics ---------------------------
namespace Graphics
{

//----------- Vertex Buffer Function ------------
VkResult VulkanManager::CreateVkBuffer(
    VkBuffer &io_buffer_handle,
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

    return vkCreateBuffer(m_VK_device, &vec_buf_c_info, nullptr, &io_buffer_handle);
}

VkResult VulkanManager::RefreshDataToHostVisibleVKDeviceMemory(VkDeviceMemory i_memory_handle, VkDeviceSize i_mem_allocated_size, VoidPtr i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    void *local_ptr = nullptr;
    result = MapVkDeviceMemory(i_memory_handle, i_mem_allocated_size, local_ptr);
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
    VkBuffer i_src_buffer_handle,
    VkAccessFlags i_src_access_flags,
    VkPipelineStageFlags i_src_pipe_stage_flags,
    VkBuffer i_dst_buffer_handle,
    VkAccessFlags i_dst_access_flags,
    VkPipelineStageFlags i_dst_pipe_stage_flags,
    VkDeviceSize i_data_size)
{
    VkResult result = VK_SUCCESS;
    //1. begin command buffer.
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(m_VK_main_cmd_buffer, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_VK_main_cmd_buffer);
        return result;
    }
    //--- i. set buffer memory barrier (block transfer).
    VkBufferMemoryBarrier beg_mem_barrier = {};
    beg_mem_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    beg_mem_barrier.pNext = nullptr;
    beg_mem_barrier.srcAccessMask = i_src_access_flags; //The buffer doesn't have access state.
    beg_mem_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    beg_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    beg_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    beg_mem_barrier.buffer = i_dst_buffer_handle;
    beg_mem_barrier.offset = 0;
    beg_mem_barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(m_VK_main_cmd_buffer, i_src_pipe_stage_flags, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr, //memory barrier
        1, &beg_mem_barrier, //buffer memory barrier
        0, nullptr //image memory barrier
    );

    //--- ii. copy buffer.
    VkBufferCopy buf_cpy_info = {};
    buf_cpy_info.size = i_data_size;
    buf_cpy_info.srcOffset = 0;
    buf_cpy_info.dstOffset = 0;
    vkCmdCopyBuffer(m_VK_main_cmd_buffer, i_src_buffer_handle, i_dst_buffer_handle, 1, &buf_cpy_info);

    //--- iii. set buffer memory barrier (block transfer).
    VkBufferMemoryBarrier end_mem_barrier = {};
    end_mem_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    end_mem_barrier.pNext = nullptr;
    end_mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    end_mem_barrier.dstAccessMask = i_dst_access_flags;
    end_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    end_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    end_mem_barrier.buffer = i_dst_buffer_handle;
    end_mem_barrier.offset = 0;
    end_mem_barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, i_dst_pipe_stage_flags,
        0,
        0, nullptr, //memory barrier
        1, &end_mem_barrier, //buffer memory barrier
        0, nullptr //image memory barrier
    );
    //--- iv. end command buffer.
    result = vkEndCommandBuffer(m_VK_main_cmd_buffer);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_VK_main_cmd_buffer);
        return result;
    }

    //3. Submit command.
    VkPipelineStageFlags submit_wait_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr;
    submit_info.pWaitDstStageMask = &submit_wait_flag;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_VK_main_cmd_buffer;

    result = vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure!!!");
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
        SDLOGW("reset main command buffer fence failure!!!");
        return result;
    }

    result = vkResetCommandBuffer(m_VK_main_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer failure!!!");
        return result;
    }

    return result;
}

void VulkanManager::DestroyVkBuffer(VkBuffer &io_buffer_handle)
{
    if (io_buffer_handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_VK_device, io_buffer_handle, nullptr);
    }
    io_buffer_handle = VK_NULL_HANDLE;
}

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}