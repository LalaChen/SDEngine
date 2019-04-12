#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample1_DrawTriangle.h"

using namespace::SDE::Basic;
using namespace::SDE::Graphics;

Sample1_DrawTriangle::Sample1_DrawTriangle(VulkanAPITestManager *i_mgr)
: m_mgr(i_mgr)
, m_VK_vertice_buffer(VK_NULL_HANDLE)
, m_VK_vbuf_memory(VK_NULL_HANDLE)
, m_VK_indices_buffer(VK_NULL_HANDLE)
, m_VK_ibuf_memory(VK_NULL_HANDLE)
{
}

Sample1_DrawTriangle::~Sample1_DrawTriangle()
{

}

void Sample1_DrawTriangle::Initialize()
{
    CreateBuffers();
}

void Sample1_DrawTriangle::Render()
{
}

void Sample1_DrawTriangle::Destroy()
{
    if (m_mgr != nullptr) {
        m_mgr->ReleaseMemory(m_VK_vbuf_memory);
        m_mgr->DestroyBuffer(m_VK_vertice_buffer);
        m_mgr->ReleaseMemory(m_VK_ibuf_memory);
        m_mgr->DestroyBuffer(m_VK_indices_buffer);
    }
}

void Sample1_DrawTriangle::CreateBuffers()
{
    std::vector<vec3> quad_vecs = {
    vec3(0.5f,  0.5f, -0.1f),
    vec3(-0.5f,  0.5f, -0.1f),
    vec3(-0.5f, -0.5f, -0.1f),
    vec3(0.5f, -0.5f, -0.1f)
    };

    std::vector<uint16_t> quad_indices = {
        0,1,2,
        0,2,3
    };

    VkResult result = VK_SUCCESS;

    //1. create vertex buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(vec3)*quad_vecs.size()), m_VK_vertice_buffer);

    if (result != VK_SUCCESS) {
        SDLOGE("Create vertice buffer failure");
        return;
    }
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_vertice_buffer, m_VK_vbuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex buffer failure!!!");
        return;
    }
    //--- iii. get memory.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_vertice_buffer, quad_vecs.data(),
        static_cast<VkDeviceSize>(sizeof(vec3)*quad_vecs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex buffer failure!!!");
        return;
    }
    //2. create indice buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(uint16_t)*quad_indices.size()), m_VK_indices_buffer);

    if (result != VK_SUCCESS) {
        SDLOGE("Create indice buffer failure");
        return;
    }
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_indices_buffer, m_VK_ibuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate indice buffer failure!!!");
        return;
    }
    //--- iii. get memory.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_indices_buffer, quad_indices.data(),
        static_cast<VkDeviceSize>(sizeof(uint16_t)*quad_indices.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh indice buffer failure!!!");
        return;
    }
}

void Sample1_DrawTriangle::CreateShaders()
{
    //
}