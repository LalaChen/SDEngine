#pragma once

#include <vulkan/vulkan.h>

class VulkanAPITestManager;

class Sample1_DrawTriangle
{
public:
    explicit Sample1_DrawTriangle(VulkanAPITestManager *i_mgr);
    virtual ~Sample1_DrawTriangle();
public:
    void Initialize();
    void Render();
    void Destroy();
private:
    void CreateBuffers();
    void CreateShaders();
protected:
    VulkanAPITestManager *m_mgr;
protected:
    //Device Local memory buffer. (Memory type VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT can't use map and unmap)
    VkBuffer m_VK_vertice_buffer;
    VkDeviceMemory m_VK_vbuf_memory;
    VkBuffer m_VK_ver_color_buffer;
    VkDeviceMemory m_VK_ver_color_buf_memory;
    VkBuffer m_VK_indices_buffer;
    VkDeviceMemory m_VK_ibuf_memory;
protected:
    std::vector<VkDescriptorSetLayout> m_VK_main_shader_set_layouts;
};