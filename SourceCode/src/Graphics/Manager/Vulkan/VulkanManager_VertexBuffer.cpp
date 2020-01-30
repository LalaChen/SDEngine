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

//----------- Vertex Buffer Function ------------
void VulkanManager::CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type)
{
    VkResult result = VK_SUCCESS;
    VkBuffer &vk_buffer_handle = reinterpret_cast<VkBuffer&>(io_identity.m_buffer_handle);
    VkDeviceMemory &vk_memory_handle = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory_handle);
    VkDeviceSize &vk_allocated_size = reinterpret_cast<VkDeviceSize&>(io_identity.m_memory_size);
    VkFlags memo_prop_flags;
    //1. Set buffer size and memory property flags.
    io_identity.m_data_size = i_data_size;

    if (i_memory_type == VertexBufferMemoryType_STATIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    else if (i_memory_type == VertexBufferMemoryType_DYNAMIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        SDLOGE("VKError : Wrong memory type. type=%d", i_memory_type);
        return;
    }

    //2. Create buffer.   
    VkBufferUsageFlags buf_usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    result = CreateVkBuffer(vk_buffer_handle, io_identity.m_data_size, buf_usage_flags, VK_SHARING_MODE_EXCLUSIVE);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError : Create buffer error. code=%d", result);
        if (vk_buffer_handle != VK_NULL_HANDLE) {
            DestroyVkBuffer(vk_buffer_handle);
            vk_buffer_handle = VK_NULL_HANDLE;
        }
        return;
    }

    //3. Create device memory.
    result = AllocatVkDeviceMemoryForVkBuffer(vk_memory_handle, vk_allocated_size, vk_buffer_handle, 0, memo_prop_flags);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError: Create memory error. code=%d", result);
        if (vk_memory_handle != VK_NULL_HANDLE) {
            FreeVkDeviceMemory(vk_memory_handle);
            vk_memory_handle = VK_NULL_HANDLE;
        }
        return;
    }
}

void VulkanManager::RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    const VkBuffer &vk_buffer_handle = reinterpret_cast<const VkBuffer&>(i_identity.m_buffer_handle);
    VkBuffer staging_buffer_handle = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    VkDeviceSize staging_memory_size = 0;

    //1. create staging host visible buffer and then refresh data into the one.
    //--- i. create staging buffer(host memory).
    result = CreateVkBuffer(staging_buffer_handle, i_data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
    if (result != VK_SUCCESS) {
        SDLOGW("Create staging buffer for copying data to buffer failure.");
        return;
    }

    result = AllocatVkDeviceMemoryForVkBuffer(staging_memory, staging_memory_size, staging_buffer_handle, 0u, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("Allocate staging memory failure.");
        return;
    }

    //--- ii. refresh data to temporary buffer.
    result = RefreshDataToHostVisibleVKDeviceMemory(staging_memory, staging_memory_size, i_data_ptr, i_data_size);
    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure.");
        return;
    }

    //2. Copy temporary host visible buffer to target static buffer.
    CopyVkBuffer(
        staging_buffer_handle, i_data_size,
        vk_buffer_handle,
        0, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);

    //3. free temporary memory and buffer.
    FreeVkDeviceMemory(staging_memory);

    DestroyVkBuffer(staging_buffer_handle);
}

void VulkanManager::RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result;
    const VkDeviceMemory &vk_memory_handle = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory_handle);
    result = RefreshDataToHostVisibleVKDeviceMemory(vk_memory_handle, i_identity.m_memory_size, i_data_ptr, i_data_size);
    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure.");
        return;
    }
}

void VulkanManager::DeleteVertexBuffer(VertexBufferIdentity &io_identity)
{
    VkBuffer &vk_buffer_handle = reinterpret_cast<VkBuffer&>(io_identity.m_buffer_handle);
    VkDeviceMemory &vk_memory_handle = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory_handle);
    FreeVkDeviceMemory(vk_memory_handle);
    io_identity.m_memory_handle = SD_NULL_HANDLE;
    DestroyVkBuffer(vk_buffer_handle);
    io_identity.m_buffer_handle = SD_NULL_HANDLE;
}

void VulkanManager::MapBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle)
{
    const VkDeviceMemory &vk_memory_handle = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory_handle);
    MapVkDeviceMemory(vk_memory_handle, i_identity.m_memory_size, io_buffer_handle);
}

void VulkanManager::UnmapBuffer(const VertexBufferIdentity &i_identity)
{
    const VkDeviceMemory &vk_memory_handle = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory_handle);
    UnmapVkDeviceMemory(vk_memory_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________