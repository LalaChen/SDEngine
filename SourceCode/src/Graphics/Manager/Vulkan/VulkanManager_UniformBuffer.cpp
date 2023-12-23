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

#include "VulkanManager.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateUniformBuffer(UniformBufferIdentity &io_identity)
{
    VkResult result = VK_SUCCESS;
    VkBuffer       &buffer         = reinterpret_cast<VkBuffer&>(io_identity.m_buffer);
    VkDevice       &device         = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkDeviceMemory &memory         = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory);
    VkDeviceSize   &allocated_size = reinterpret_cast<VkDeviceSize&>(io_identity.m_memory_size);
    VkFlags memo_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; //Create uniform buffer with host memory.

    //1. Set buffer size and memory property flags.
    VkBufferUsageFlags buf_usage_flags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    
    device = m_device;
    //2. Create buffer.
    VkBufferCreateInfo info = InitializeVkBufferCreateInfo(io_identity.m_data_size, buf_usage_flags);

    result = CreateVkBuffer(buffer, device, info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create buffer for uniform failure(%d)", result);
        if (buffer != VK_NULL_HANDLE) {
            DestroyVkBuffer(buffer, device);
            buffer = VK_NULL_HANDLE;
        }
        return;
    }

    //3. Create device memory.
    result = AllocatVkDeviceMemoryForVkBuffer(memory, allocated_size, device, buffer, 0, memo_prop_flags);
    if (result != VK_SUCCESS) {
        SDLOGE("Create memory for uniform failure(%d)", result);
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

void VulkanManager::DeleteUnifromBuffer(UniformBufferIdentity &io_identity)
{
    VkBuffer       &buffer = reinterpret_cast<VkBuffer&>(io_identity.m_buffer);
    VkDeviceMemory &memory = reinterpret_cast<VkDeviceMemory&>(io_identity.m_memory);
    VkDevice       &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    FreeVkDeviceMemory(memory, device);
    io_identity.m_memory = SD_NULL_HANDLE;
    DestroyVkBuffer(buffer, device);
    io_identity.m_buffer = SD_NULL_HANDLE;
    io_identity.SetInvalid();
    io_identity = UniformBufferIdentity();
}


void VulkanManager::MapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub, VoidPtr &io_buffer_handle)
{
    const UniformBufferIdentity &identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_ub);
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(identity.m_device);
    MapVkDeviceMemory(memory, device, identity.m_memory_size, io_buffer_handle);
}

void VulkanManager::UnmapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub)
{
    const UniformBufferIdentity &identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_ub);
    const VkDeviceMemory &memory = reinterpret_cast<const VkDeviceMemory&>(identity.m_memory);
    const VkDevice       &device = reinterpret_cast<const VkDevice&>(identity.m_device);
    UnmapVkDeviceMemory(memory, device);
}

______________SD_END_GRAPHICS_NAMESPACE______________