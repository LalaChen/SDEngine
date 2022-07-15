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

#include "IndexBufferFormat_Vulkan.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

//------------------------------------------------ Vertex Buffer ------------------------------------------------
void VulkanManager::CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    VkBuffer &buffer = reinterpret_cast<VkBuffer&>(io_identity.m_handle);
    VkDevice &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkDeviceMemory &memory = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory);
    VkDeviceSize &allocated_size = reinterpret_cast<VkDeviceSize&>(io_identity.m_memory_size);
    VkFlags memo_prop_flags;
    //1. Set buffer size and memory property flags.
    device = m_device;
    io_identity.m_data_size = i_data_size;

    if (io_identity.m_memory_type == MemoryType_STATIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    else if (io_identity.m_memory_type == MemoryType_DYNAMIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        SDLOGE("Wrong memory type. type=%d", io_identity.m_memory_type);
        return;
    }

    //2. Create buffer.   
    VkBufferUsageFlags buf_usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    VkBufferCreateInfo info = InitializeVkBufferCreateInfo(io_identity.m_data_size, buf_usage_flags);
    result = CreateVkBuffer(buffer, device, info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create buffer failure(%d)", result);
        if (buffer != VK_NULL_HANDLE) {
            DestroyVkBuffer(buffer, device);
            buffer = VK_NULL_HANDLE;
        }
        return;
    }

    //3. Create device memory.
    result = AllocatVkDeviceMemoryForVkBuffer(memory, allocated_size, device, buffer, 0, memo_prop_flags);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError: Create memory error. code=%d", result);
        if (memory != VK_NULL_HANDLE) {
            FreeVkDeviceMemory(memory, device);
            memory = VK_NULL_HANDLE;
        }
        return;
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size)
{
    std::lock_guard<std::mutex> lock(m_loading_buffer_lock);

    VkResult result = VK_SUCCESS;
    const VkBuffer &buffer = reinterpret_cast<const VkBuffer&>(i_identity.m_handle);
    const VkDevice &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);

    const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(m_loading_cmd_buffer);
    const VkCommandBuffer       &cmd_buffer  = reinterpret_cast<const VkCommandBuffer&>(cb_identity.m_handle);

    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    VkDeviceSize staging_memory_size = 0;

    //1. create staging host visible buffer and then refresh data into the one.
    //--- i. create staging buffer(host memory).
    VkBufferCreateInfo info = InitializeVkBufferCreateInfo(i_data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    result = CreateVkBuffer(staging_buffer, device, info);
    if (result != VK_SUCCESS) {
        SDLOGW("Create staging buffer for copying data to buffer failure(%d).", result);
        return;
    }

    result = AllocatVkDeviceMemoryForVkBuffer(
        staging_memory, staging_memory_size,
        device, staging_buffer, 0u, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (result != VK_SUCCESS) {
        SDLOGW("Allocate staging memory failure(%d).", result);
        return;
    }

    //--- ii. refresh data to temporary buffer.
    result = RefreshDataToHostVisibleVKDeviceMemory(staging_memory, device, staging_memory_size, i_data_ptr, i_data_size);
    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure(%d).", result);
        return;
    }

    //2. Copy temporary host visible buffer to target static buffer.
    PrepareCopyVkBufferCommand(
        cmd_buffer,
        staging_buffer, i_data_size,
        buffer,
        0, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);

    SD_SREF(m_loading_queue).SubmitCommandBuffer(m_loading_cmd_buffer);

    //3. free temporary memory and buffer.
    FreeVkDeviceMemory(staging_memory, device);

    DestroyVkBuffer(staging_buffer, device);
}

void VulkanManager::RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result;
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    result = RefreshDataToHostVisibleVKDeviceMemory(memory, device, i_identity.m_memory_size, i_data_ptr, i_data_size);
    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure(%d).", result);
        return;
    }
}

void VulkanManager::DeleteVertexBuffer(VertexBufferIdentity &io_identity)
{
    VkBuffer       &buffer = reinterpret_cast<VkBuffer&>(io_identity.m_handle);
    VkDeviceMemory &memory = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory);
    VkDevice       &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    FreeVkDeviceMemory(memory, device);
    io_identity.m_memory = SD_NULL_HANDLE;
    DestroyVkBuffer(buffer, device);
    io_identity.m_handle = SD_NULL_HANDLE;
    io_identity.SetInvalid();
}

void VulkanManager::MapVertexBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle)
{
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    MapVkDeviceMemory(memory, device, i_identity.m_memory_size, io_buffer_handle);
}

void VulkanManager::UnmapVertexBuffer(const VertexBufferIdentity &i_identity)
{
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    UnmapVkDeviceMemory(memory, device);
}

void VulkanManager::BindVertexBuffer(const VertexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, uint32_t i_binding_id, Size_ui64 i_offset)
{
    const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
    VkCommandBuffer cmd_buffer    = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    VkBuffer        vertex_buffer = reinterpret_cast<VkBuffer>(i_identity.m_handle);
    BindVkVertexBuffer(cmd_buffer, vertex_buffer, i_binding_id, static_cast<VkDeviceSize>(i_offset));
}

//------------------------------------------------ Indice Buffer ------------------------------------------------
void VulkanManager::CreateIndexBuffer(IndexBufferIdentity &io_identity, Size_ui64 i_data_size)
{
    VkResult result = VK_SUCCESS;
    VkBuffer       &buffer         = reinterpret_cast<VkBuffer&>(io_identity.m_buffer);
    VkDeviceMemory &memory         = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory);
    VkDeviceSize   &allocated_size = reinterpret_cast<VkDeviceSize&>(io_identity.m_memory_size);
    VkDevice       &device         = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkFlags memo_prop_flags;
    //1. Set buffer size and memory property flags.
    io_identity.m_data_size = i_data_size;
    device = m_device;

    if (io_identity.m_memory_type == MemoryType_STATIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    else if (io_identity.m_memory_type == MemoryType_DYNAMIC) {
        memo_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        SDLOGE("Wrong memory type failure(%d).", io_identity.m_memory_type);
        return;
    }

    //2. Create buffer.
    VkBufferUsageFlags buf_usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    VkBufferCreateInfo info = InitializeVkBufferCreateInfo(io_identity.m_data_size, buf_usage_flags);
    result = CreateVkBuffer(buffer, device, info);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError : Create buffer error. code=%d", result);
        if (buffer != VK_NULL_HANDLE) {
            DestroyVkBuffer(buffer, device);
            buffer = VK_NULL_HANDLE;
        }
        return;
    }

    //3. Create device memory.
    result = AllocatVkDeviceMemoryForVkBuffer(memory, allocated_size, device, buffer, 0, memo_prop_flags);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError: Create memory error. code=%d", result);
        if (memory != VK_NULL_HANDLE) {
            FreeVkDeviceMemory(memory, device);
            memory = VK_NULL_HANDLE;
        }
        return;
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::RefreshStaticIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size)
{
    std::lock_guard<std::mutex> lock(m_loading_buffer_lock);
    VkResult result = VK_SUCCESS;
    const VkBuffer &buffer = reinterpret_cast<const VkBuffer&>(i_identity.m_buffer);
    const VkDevice &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    VkBuffer       staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    VkDeviceSize   staging_memory_size = 0;

    const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(m_loading_cmd_buffer);
    const VkCommandBuffer       &cmd_buffer  = reinterpret_cast<const VkCommandBuffer&>(cb_identity.m_handle);
    //1. create staging host visible buffer and then refresh data into the one.
    //--- i. create staging buffer(host memory).
    VkBufferCreateInfo info = InitializeVkBufferCreateInfo(i_data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    result = CreateVkBuffer(staging_buffer, device, info);
    if (result != VK_SUCCESS) {
        SDLOGW("Create staging buffer for copying data to buffer failure(%d).", result);
        return;
    }

    result = AllocatVkDeviceMemoryForVkBuffer(
        staging_memory, staging_memory_size, device, staging_buffer, 0u, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("Allocate staging memory failure(%d).", result);
        return;
    }

    //--- ii. refresh data to temporary buffer.
    result = RefreshDataToHostVisibleVKDeviceMemory(
        staging_memory, device, staging_memory_size, i_data_ptr, i_data_size);

    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure(%d).", result);
    }

    //2. Copy temporary host visible buffer to target static buffer.
    PrepareCopyVkBufferCommand(
        cmd_buffer,
        staging_buffer,
        i_data_size,
        buffer,
        0, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);

    SD_SREF(m_loading_queue).SubmitCommandBuffer(m_loading_cmd_buffer);

    //3. free temporary memory and buffer.
    FreeVkDeviceMemory(staging_memory, device);

    DestroyVkBuffer(staging_buffer, device);
}

void VulkanManager::RefreshDynamicIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result;
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    result = RefreshDataToHostVisibleVKDeviceMemory(memory, device, i_identity.m_memory_size, i_data_ptr, i_data_size);
    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure(%d).", result);
    }
}

void VulkanManager::DeleteIndexBuffer(IndexBufferIdentity &io_identity)
{
    VkBuffer       &buffer = reinterpret_cast<VkBuffer&>(io_identity.m_buffer);
    VkDevice       &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkDeviceMemory &memory = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory);
    FreeVkDeviceMemory(memory, device);
    io_identity.m_memory = SD_NULL_HANDLE;
    DestroyVkBuffer(buffer, device);
    io_identity.m_buffer = SD_NULL_HANDLE;
    io_identity.SetInvalid();
}

void VulkanManager::MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr &io_buffer)
{
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    MapVkDeviceMemory(memory, device, i_identity.m_memory_size, io_buffer);
}

void VulkanManager::UnmapIndexBuffer(const IndexBufferIdentity &i_identity)
{
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    UnmapVkDeviceMemory(memory, device);
}

void VulkanManager::BindIndexBuffer(const IndexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset)
{
    const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
    VkCommandBuffer cmd_buffer = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    VkBuffer        buffer     = reinterpret_cast<VkBuffer>(i_identity.m_buffer);
    VkIndexType     index_type = IndexBufferFormat_Vulkan::ConvertIndexType(i_identity.m_format);
    BindVkIndexBuffer(cmd_buffer, buffer, static_cast<VkDeviceSize>(i_offset), index_type);
}

______________SD_END_GRAPHICS_NAMESPACE______________