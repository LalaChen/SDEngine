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

VkResult VulkanManager::AllocatVkDeviceMemoryForVkBuffer(
    VkDeviceMemory &io_memory_handle,
    VkDeviceSize &io_allocated_size,
    VkBuffer i_buffer_handle,
    VkDeviceSize i_mem_offset,
    VkFlags i_memo_prop_flags)
{
    //1. Get device info.
    VkResult result;
    //--- i. Pick up memory properties 
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);
    //--- ii. Get memory requirements.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_VK_device, i_buffer_handle, &mem_req);
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
            result = vkAllocateMemory(m_VK_device, &mem_a_info, nullptr, &io_memory_handle);
            if (result != VK_SUCCESS) {
                SDLOGE("VKError : Allocate memory failure. code=%d", result);
                return result;
            }
            break;
        }
    }
    //2. bind memory and buffer together.
    result = vkBindBufferMemory(m_VK_device, i_buffer_handle, io_memory_handle, i_mem_offset);
    return result;
}

VkResult VulkanManager::AllocateVkDeviceMemortForVkImage(
    VkDeviceMemory &io_memory_handle,
    VkDeviceSize &io_allocated_size,
    VkImage &i_image_handle,
    VkDeviceSize i_mem_offset,
    VkFlags i_memo_prop_flags)
{
    //1. Get device info.
    VkResult result;
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);

    //2. Get requirement info of image.
    VkMemoryRequirements mem_req;
    vkGetImageMemoryRequirements(m_VK_device, i_image_handle, &mem_req);
    io_allocated_size = mem_req.size;

    //3. Allocate memory space following memory type and prop flag.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported = ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & i_memo_prop_flags) == i_memo_prop_flags);

        //--- i. Check this mem type.
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            //------ find suitable type.
            VkMemoryAllocateInfo buffer_mem_a_info = {};
            buffer_mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            buffer_mem_a_info.pNext = nullptr;
            buffer_mem_a_info.allocationSize = io_allocated_size;
            buffer_mem_a_info.memoryTypeIndex = mem_type_ID;
            result = vkAllocateMemory(m_VK_device, &buffer_mem_a_info, nullptr, &io_memory_handle);
            if (result != VK_SUCCESS) {
                SDLOGE("Allocate memory failure.");
                return result;
            }
            else {
                break;
            }
        }
    }
    //4. Bind memory to image.
    return vkBindImageMemory(m_VK_device, i_image_handle, io_memory_handle, i_mem_offset);
}

VkResult VulkanManager::MapVkDeviceMemory(
    VkDeviceMemory i_memory_handle,
    VkDeviceSize i_allocated_size,
    VoidPtr &i_local_ptr)
{
    VkResult result = VK_SUCCESS;
    i_local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, i_memory_handle, 0, i_allocated_size, 0, (void**)&i_local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
    }
    return result;
}

VkResult VulkanManager::FlushMappedVkDeviceMemoryRanges(VkDeviceMemory i_memory_handle)
{
    VkResult result = VK_SUCCESS;
    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = i_memory_handle;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
    }
    return result;
}

void VulkanManager::UnmapVkDeviceMemory(VkDeviceMemory i_memory_handle)
{
    vkUnmapMemory(m_VK_device, i_memory_handle);
}

void VulkanManager::FreeVkDeviceMemory(VkDeviceMemory &io_memory_handle)
{
    if (io_memory_handle != VK_NULL_HANDLE) {
        vkFreeMemory(m_VK_device, io_memory_handle, nullptr);
    }
    io_memory_handle = VK_NULL_HANDLE;
}


______________SD_END_GRAPHICS_NAMESPACE______________