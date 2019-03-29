#pragma once

#include <vulkan/vulkan.h>

class Sample1_DrawTriangle
{
public:
    Sample1_DrawTriangle();
    ~Sample1_DrawTriangle();
public:
    void Initialize(VkPhysicalDevice i_VK_phy_device, VkDevice i_VK_device);
    void Render();
protected:
    //Device Local memory buffer. (Memory type VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT can't use map and unmap)
    VkBuffer m_VK_vertice_buffer;
    VkDeviceMemory m_VK_vbuf_memory;
    VkBuffer m_VK_indices_buffer;
    VkDeviceMemory m_VK_ibuf_memory;
    //Staging buffer. VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT.
protected:
    VkPhysicalDevice m_VK_phy_device;
    VkDevice m_VK_device;
};