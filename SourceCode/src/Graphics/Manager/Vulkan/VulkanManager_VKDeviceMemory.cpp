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

VkResult VulkanManager::AllocatVkDeviceMemoryForVkBuffer(
    VkDeviceMemory & io_memory,
    VkDeviceSize &io_allocated_size,
    VkDevice i_device,
    VkBuffer i_buffer,
    VkDeviceSize i_mem_offset,
    VkFlags i_memo_prop_flags)
{
    //1. Get device info.
    VkResult result;
    //--- i. Pick up memory properties 
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_phy_device, &phy_dev_memory_props);
    //--- ii. Get memory requirements.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(i_device, i_buffer, &mem_req);
    io_allocated_size = mem_req.size;
    //--- iii. find suitable type and then allocate memory.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported = ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & i_memo_prop_flags) == i_memo_prop_flags);

        //--- i. Check this mem type.
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            //------ find suitable type.
            VkMemoryAllocateInfo mem_a_info = {};
            mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            mem_a_info.pNext = nullptr;
            mem_a_info.allocationSize = mem_req.size;
            mem_a_info.memoryTypeIndex = mem_type_ID;
            result = vkAllocateMemory(m_device, &mem_a_info, nullptr, &io_memory);
            if (result != VK_SUCCESS) {
                SDLOGE("VKError : Allocate memory failure. code=%d", result);
                return result;
            }
            break;
        }
    }
    //2. bind memory and buffer together.
    result = vkBindBufferMemory(m_device, i_buffer, io_memory, i_mem_offset);
    return result;
}

void VulkanManager::GetVkImageMemoryRequirement(
    VkMemoryRequirements &io_mem_req,
    VkDevice i_device,
    VkImage i_image)
{
    vkGetImageMemoryRequirements(i_device, i_image, &io_mem_req);
}

VkResult VulkanManager::AllocateVkDeviceMemoryForVkImage(
    VkDeviceMemory &io_memory,
    VkDevice i_device,
    VkImage i_image,
    VkDeviceSize i_mem_offset,
    const VkMemoryAllocateInfo &i_info)
{
    VkResult result;
    result = vkAllocateMemory(i_device, &i_info, nullptr, &io_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate memory failure(%d).", result);
        return result;
    }

    return vkBindImageMemory(i_device, i_image, io_memory, i_mem_offset);
}

VkResult VulkanManager::MapVkDeviceMemory(
    VkDeviceMemory i_memory,
    VkDevice i_device,
    VkDeviceSize i_allocated_size,
    VoidPtr &i_local_ptr)
{
    VkResult result = VK_SUCCESS;
    i_local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(i_device, i_memory, 0, i_allocated_size, 0, (void**)&i_local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
    }
    return result;
}

VkResult VulkanManager::FlushMappedVkDeviceMemoryRanges(VkDeviceMemory i_memory, VkDevice i_device)
{
    VkResult result = VK_SUCCESS;
    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = i_memory;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(i_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
    }
    return result;
}

void VulkanManager::UnmapVkDeviceMemory(VkDeviceMemory i_memory, VkDevice i_device)
{
    vkUnmapMemory(m_device, i_memory);
}

void VulkanManager::FreeVkDeviceMemory(
    VkDeviceMemory &io_memory,
    VkDevice i_device)
{
    if (io_memory != VK_NULL_HANDLE) {
        vkFreeMemory(i_device, io_memory, nullptr);
    }
    io_memory = VK_NULL_HANDLE;
}


______________SD_END_GRAPHICS_NAMESPACE______________