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
    VkResult result = VK_SUCCESS;
    VkBuffer &vk_buffer_handle = reinterpret_cast<VkBuffer&>(io_identity.m_buffer_handle);
    VkDeviceMemory &vk_memory_handle = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory_handle);
    VkFlags memo_prop_flags;
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

    //2. create buffer.   
    VkBufferUsageFlags buf_usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    result = CreateVkBuffer(buf_usage_flags, VK_SHARING_MODE_EXCLUSIVE, i_data_size, vk_buffer_handle);
    if (result != VK_SUCCESS) {
        SDLOGE("VKError : Create buffer error. code=%d", result);
        if (vk_buffer_handle != VK_NULL_HANDLE) {
            DestroyVkBuffer(vk_buffer_handle);
            vk_buffer_handle = VK_NULL_HANDLE;
        }
        return;
    }

    //3. create device memory.
    result = AllocatVkDeviceMemoryForBuffer(memo_prop_flags, 0, vk_buffer_handle, vk_memory_handle);
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
    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    //1. create staging host visible buffer and then refresh data into the one.
    //--- i. create staging buffer(host memory).
    result = CreateVkBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, i_data_size, staging_buffer);
    if (result != VK_SUCCESS) {
        SDLOGW("Create staging buffer failure.");
        return;
    }

    result = AllocatVkDeviceMemoryForBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0u, staging_buffer, staging_memory);
    if (result != VK_SUCCESS) {
        SDLOGW("Allocate staging memory failure.");
        return;
    }

    //--- ii. refresh data to temporary buffer.
    result = RefreshDataInHostVisibleVkBuffer(staging_buffer, staging_memory, i_data_ptr, i_data_size);
    if (result != VK_SUCCESS) {
        SDLOGW("Refresh host visible buffer failure.");
        return;
    }

    //2. Copy temporary host visible buffer to target static buffer.
    CopyDataToStaticVkBuffer(
        staging_buffer, 0, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        vk_buffer_handle, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        i_data_size);

    //3. free temporary memory and buffer.
    FreeVkDeviceMemory(staging_memory);
    DestroyVkBuffer(staging_buffer);

    result = vkResetCommandBuffer(m_VK_main_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer failure!!!");
        return;
    }
}

void VulkanManager::RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
    VkResult result;
    const VkBuffer &vk_buffer_handle = reinterpret_cast<const VkBuffer&>(i_identity.m_buffer_handle);
    const VkDeviceMemory &vk_memory_handle = reinterpret_cast<const VkDeviceMemory&>(i_identity.m_memory_handle);
    result = RefreshDataInHostVisibleVkBuffer(vk_buffer_handle, vk_memory_handle, i_data_ptr, i_data_size);
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

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}