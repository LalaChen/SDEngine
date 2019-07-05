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
void VulkanManager::CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type)
{
    //1. create buffer.
    VkResult result = VK_SUCCESS;
    VkBufferCreateInfo vbuf_c_info = {};
    vbuf_c_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vbuf_c_info.pNext = nullptr;
    vbuf_c_info.flags = 0;
    vbuf_c_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vbuf_c_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vbuf_c_info.queueFamilyIndexCount = 0;
    vbuf_c_info.pQueueFamilyIndices = nullptr;
    result = vkCreateBuffer(m_VK_device, &vbuf_c_info, nullptr, reinterpret_cast<VkBuffer*>(&io_identity.m_buffer_handle));
    if (result != VK_SUCCESS) {
        SDLOGE("VK Error: Create buffer error. code=%d", result);
        return;
    }
    //2. create device memory.
    //--- i. Pick up memory properties 
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);
    //--- ii. Get memory requirements.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_VK_device, reinterpret_cast<VkBuffer>(io_identity.m_buffer_handle), &mem_req);
    //--- iii. decide memory property type.
    VkFlags memo_prop_flags;
    if (i_memory_type == VertexBufferMemoryType_STATIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    else if (i_memory_type == VertexBufferMemoryType_DYNAMIC){
        memo_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }
    else {
        SDLOGE("VKError : Wrong memory type. type=%d", i_memory_type);
        if (io_identity.m_buffer_handle != SD_NULL_HANDLE) {
            vkDestroyBuffer(m_VK_device, reinterpret_cast<VkBuffer>(io_identity.m_buffer_handle), nullptr);
            io_identity.m_buffer_handle = SD_NULL_HANDLE;
        }
        return;
    }
    //--- iv. find suitable type and then allocate memory.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported =
            ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & memo_prop_flags) == memo_prop_flags);

        //------ Check this mem type.
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            //------ find suitable type.
            VkMemoryAllocateInfo mem_a_info = {};
            mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            mem_a_info.pNext = nullptr;
            mem_a_info.allocationSize = mem_req.size;
            mem_a_info.memoryTypeIndex = mem_type_ID;
            result = vkAllocateMemory(m_VK_device, &mem_a_info, nullptr, reinterpret_cast<VkDeviceMemory*>(&io_identity.m_memory_handle));
            if (result != VK_SUCCESS) {
                SDLOGE("VKError : Allocate memory failure. code=%d", result);
            }
            break;
        }
    }
    //3. bind memory and buffer together.
    vkBindBufferMemory(m_VK_device, reinterpret_cast<VkBuffer>(io_identity.m_buffer_handle), reinterpret_cast<VkDeviceMemory>(io_identity.m_memory_handle), 0);
}

void VulkanManager::RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, Size_ui64 i_data_size)
{
    VkResult result;
    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    //1. create staging buffer in host and then refresh data into the one.
    //--- i. create staging buffer(host memory).
    result = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, i_data_size, staging_buffer);
    if (result != VK_SUCCESS) {
        SDLOGW("Create staging buffer failure.");
        return result;
    }

    result = AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 0u, staging_buffer, staging_memory);
    if (result != VK_SUCCESS) {
        SDLOGW("Allocate staging memory failure.");
        return result;
    }

    //--- ii. map memory.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_VK_device, staging_buffer, &mem_req);
    void *local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, staging_memory, 0, mem_req.size, 0, (void**)&local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
        return result;
    }
    //--- iii. memory copy.
    std::memcpy(local_ptr, i_data_ptr, i_data_size);

    //--- iv. flush 
    ///*
    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = staging_memory;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }
    //*/
    //--- iv. unmap memory.
    vkUnmapMemory(m_VK_device, staging_memory);

    //2. push copy command.
    //--- i. begin command buffer.
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
    beg_mem_barrier.srcAccessMask = 0; //The buffer doesn't have access state.
    beg_mem_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    beg_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    beg_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    beg_mem_barrier.buffer = i_VK_buffer;
    beg_mem_barrier.offset = 0;
    beg_mem_barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
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
    vkCmdCopyBuffer(m_VK_main_cmd_buffer, staging_buffer, i_VK_buffer, 1, &buf_cpy_info);

    //--- iv. set buffer memory barrier (block transfer).
    VkBufferMemoryBarrier end_mem_barrier = {};
    end_mem_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    end_mem_barrier.pNext = nullptr;
    end_mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    end_mem_barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    end_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    end_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    end_mem_barrier.buffer = i_VK_buffer;
    end_mem_barrier.offset = 0;
    end_mem_barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
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

    ReleaseMemory(staging_memory);
    DestroyBuffer(staging_buffer);

    result = vkResetCommandBuffer(m_VK_main_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer failure!!!");
        return result;
    }

    return VK_SUCCESS;
}

void VulkanManager::RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, Size_ui64 i_data_size)
{
}

void VulkanManager::DeleteVertexBuffer(const VertexBufferIdentity &i_identity)
{
}

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}