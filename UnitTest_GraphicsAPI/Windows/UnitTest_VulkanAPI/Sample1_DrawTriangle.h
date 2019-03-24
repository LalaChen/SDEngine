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
    VkBuffer m_VK_vertice_buffer;
    VkDeviceMemory m_VK_vbuf_memory;
};