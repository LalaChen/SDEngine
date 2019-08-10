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

VkResult VulkanManager::MapBufferMemory(VkDeviceMemory i_memory_handle, VkDeviceSize i_allocated_size, VoidPtr &i_local_ptr)
{
    VkResult result = VK_SUCCESS;
    i_local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, i_memory_handle, 0, i_allocated_size, 0, (void**)&i_local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
    }
    return result;
}

VkResult VulkanManager::FlushMappedMemoryRanges(VkDeviceMemory i_memory_handle)
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

void VulkanManager::UnmapBufferMemory(VkDeviceMemory i_memory_handle)
{
    vkUnmapMemory(m_VK_device, i_memory_handle);
}

void VulkanManager::FreeVkDeviceMemory(VkDeviceMemory i_memory_handle)
{
    if (i_memory_handle != VK_NULL_HANDLE) {
        vkFreeMemory(m_VK_device, i_memory_handle, nullptr);
    }
}


//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}