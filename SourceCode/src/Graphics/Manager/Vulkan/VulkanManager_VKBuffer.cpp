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

//---------------------------- start of namespace SDE -----------------------------
namespace SDE
{
//------------------------- start of namespace Graphics ---------------------------
namespace Graphics
{

//----------- Vertex Buffer Function ------------
VkResult VulkanManager::CreateVkBuffer(VkBufferUsageFlags i_buffer_usage, VkSharingMode i_sharing_mode, VkDeviceSize i_size, VkBuffer &io_buffer_handle)
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

VkResult VulkanManager::AllocatVkDeviceMemoryForBuffer(VkFlags i_memo_prop_flags, VkDeviceSize i_mem_offset, VkBuffer i_buffer_handle, VkDeviceMemory &io_memory_handle)
{
    //1. Get device info.
    VkResult result;
    //--- i. Pick up memory properties 
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);
    //--- ii. Get memory requirements.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_VK_device, i_buffer_handle, &mem_req);
    //--- iii. find suitable type and then allocate memory.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported = ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & i_memo_prop_flags) == i_memo_prop_flags);

        //------ Check this mem type.
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            //------ find suitable type.
            VkMemoryAllocateInfo mem_a_info = {};
            mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            mem_a_info.pNext = nullptr;
            mem_a_info.allocationSize = mem_req.size;
            mem_a_info.memoryTypeIndex = mem_type_ID;
            result = vkAllocateMemory(m_VK_device, &mem_a_info, nullptr, &io_memory_handle);
            if (result != VK_SUCCESS) {
                SDLOGE("VKError : Allocate memory failure. code=%d", result);
                return result;
            }
            break;
        }
    }
    //3. bind memory and buffer together.
    result = vkBindBufferMemory(m_VK_device, i_buffer_handle, io_memory_handle, i_mem_offset);
    return result;
}

VkResult VulkanManager::RefreshDataInHostVisibleVkBuffer(VkBuffer i_buffer_handle, VkDeviceMemory i_memory_handle, void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_VK_device, i_buffer_handle, &mem_req);
    void *local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, i_memory_handle, 0, mem_req.size, 0, (void**)&local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
        return result;
    }
    //--- iii. memory copy.
    std::memcpy(local_ptr, i_data_ptr, i_data_size);

    //--- iv. flush memory.
    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = i_memory_handle;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }

    //--- iv. unmap memory.
    vkUnmapMemory(m_VK_device, i_memory_handle);
    return result;
}

VkResult VulkanManager::CopyDataToStaticVkBuffer(
    VkBuffer i_src_buffer_handle, VkAccessFlags i_src_access_flags, VkPipelineStageFlags i_src_pipe_stage_flags,
    VkBuffer i_dst_buffer_handle, VkAccessFlags i_dst_access_flags, VkPipelineStageFlags i_dst_pipe_stage_flags,
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
    //--- ii. set buffer memory barrier (block transfer).
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

    //--- iii. copy buffer.
    VkBufferCopy buf_cpy_info = {};
    buf_cpy_info.size = i_data_size;
    buf_cpy_info.srcOffset = 0;
    buf_cpy_info.dstOffset = 0;
    vkCmdCopyBuffer(m_VK_main_cmd_buffer, i_src_buffer_handle, i_dst_buffer_handle, 1, &buf_cpy_info);

    //--- iv. set buffer memory barrier (block transfer).
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
    //--- v. end command buffer.
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

    result = vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_FALSE, MaxFenceWaitTime);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait copy sync failure!!!");
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
        return result;
    }

    return result;
}

void VulkanManager::FreeVkDeviceMemory(VkDeviceMemory i_memory_handle)
{
    if (i_memory_handle != VK_NULL_HANDLE) {
        vkFreeMemory(m_VK_device, i_memory_handle, nullptr);
    }
}

void VulkanManager::DestroyVkBuffer(VkBuffer i_buffer_handle)
{
    if (i_buffer_handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_VK_device, i_buffer_handle, nullptr);
    }
}

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}