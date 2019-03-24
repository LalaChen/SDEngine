#include "Sample1_DrawTriangle.h"

Sample1_DrawTriangle::Sample1_DrawTriangle()
: m_VK_vertice_buffer(VK_NULL_HANDLE)
, m_VK_vbuf_memory(VK_NULL_HANDLE)
{
}

Sample1_DrawTriangle::~Sample1_DrawTriangle()
{
}

void Sample1_DrawTriangle::Initialize(VkPhysicalDevice i_VK_phy_device, VkDevice i_VK_device)
{
    VkPhysicalDeviceMemoryProperties phy_dev_mem_props;
    vkGetPhysicalDeviceMemoryProperties(i_VK_phy_device, &phy_dev_mem_props);
    //1.
    for (uint32_t type = 0; type < phy_dev_mem_props.memoryTypeCount; ++type) {

    }
}

void Sample1_DrawTriangle::Render()
{
}