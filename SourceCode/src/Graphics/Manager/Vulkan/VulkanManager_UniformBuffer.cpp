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

void VulkanManager::CreateUniformBuffer(UniformBufferIdentity &io_identity)
{
    VkResult result = VK_SUCCESS;
    VkBuffer& buffer_handle = reinterpret_cast<VkBuffer&>(io_identity.m_buffer_handle);
    VkDeviceMemory& memory_handle = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory_handle);
    VkDeviceSize& allocated_size = reinterpret_cast<VkDeviceSize&>(io_identity.m_memory_size);
    VkFlags memo_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; //Create uniform buffer with host memory.

    //1. Set buffer size and memory property flags.
    VkBufferUsageFlags buf_usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    
    //2. Create buffer.   
    result = CreateVkBuffer(buffer_handle, io_identity.m_data_size, buf_usage_flags, VK_SHARING_MODE_EXCLUSIVE);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError : Create buffer error. code=%d", result);
        if (buffer_handle != VK_NULL_HANDLE) {
            DestroyVkBuffer(buffer_handle);
            buffer_handle = VK_NULL_HANDLE;
        }
        return;
    }

    //3. Create device memory.
    result = AllocatVkDeviceMemoryForVkBuffer(memory_handle, allocated_size, buffer_handle, 0, memo_prop_flags);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError: Create memory error. code=%d", result);
        if (memory_handle != VK_NULL_HANDLE) {
            FreeVkDeviceMemory(memory_handle);
            memory_handle = VK_NULL_HANDLE;
        }
        return;
    }
}

void VulkanManager::DeleteUnifromBuffer(UniformBufferIdentity &io_identity)
{
    VkBuffer &buffer_handle = reinterpret_cast<VkBuffer&>(io_identity.m_buffer_handle);
    VkDeviceMemory &memory_handle = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory_handle);
    FreeVkDeviceMemory(memory_handle);
    io_identity.m_memory_handle = SD_NULL_HANDLE;
    DestroyVkBuffer(buffer_handle);
    io_identity.m_buffer_handle = SD_NULL_HANDLE;
}


void VulkanManager::MapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub_wref, VoidPtr &io_buffer_handle)
{
    const UniformBufferIdentity &identity = GetIdentity(i_ub_wref);
    VkDeviceMemory memory_handle = reinterpret_cast<VkDeviceMemory>(identity.m_memory_handle);
    MapVkDeviceMemory(memory_handle, identity.m_memory_size, io_buffer_handle);
}

void VulkanManager::UnmapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub_wref)
{
    const UniformBufferIdentity &identity = GetIdentity(i_ub_wref);
    VkDeviceMemory memory_handle = reinterpret_cast<VkDeviceMemory>(identity.m_memory_handle);
    UnmapVkDeviceMemory(memory_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________