#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample1_DrawTriangle.h"

using namespace::SDE::Basic;
using namespace::SDE::Graphics;

Sample1_DrawTriangle::Sample1_DrawTriangle(VulkanAPITestManager *i_mgr)
: m_mgr(i_mgr)
, m_VK_ver_color_buffer(VK_NULL_HANDLE)
, m_VK_ver_color_buf_memory(VK_NULL_HANDLE)
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
        m_VK_vbuf_memory = VK_NULL_HANDLE;
        m_VK_vertice_buffer = VK_NULL_HANDLE;

        m_mgr->ReleaseMemory(m_VK_ver_color_buf_memory);
        m_mgr->DestroyBuffer(m_VK_ver_color_buffer);
        m_VK_ver_color_buf_memory = VK_NULL_HANDLE;
        m_VK_ver_color_buffer = VK_NULL_HANDLE;

        m_mgr->ReleaseMemory(m_VK_ibuf_memory);
        m_mgr->DestroyBuffer(m_VK_indices_buffer);
        m_VK_ibuf_memory = VK_NULL_HANDLE;
        m_VK_indices_buffer = VK_NULL_HANDLE;

        for (uint32_t i = 0; i < m_VK_main_shader_set_layouts.size(); i++) {
            m_mgr->DestroyDesciptorSetLayout(m_VK_main_shader_set_layouts[i]);
            m_VK_main_shader_set_layouts[i] = VK_NULL_HANDLE;
        }
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

    std::vector<Color4f> quad_colors = {
        Color4f(1.0f, 0.0f, 0.0f, 1.0f),
        Color4f(0.0f, 1.0f, 0.0f, 1.0f),
        Color4f(0.0f, 0.0f, 1.0f, 1.0f),
        Color4f(1.0f, 1.0f, 1.0f, 1.0f)
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

    //2. create color buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(Color4f)*quad_colors.size()), m_VK_ver_color_buffer);

    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_ver_color_buffer, m_VK_ver_color_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex color buffer failure!!!");
        return;
    }

    //--- iii. get memory.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_ver_color_buffer, quad_colors.data(),
        static_cast<VkDeviceSize>(sizeof(Color4f)*quad_colors.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex color buffer failure!!!");
        return;
    }

    //3. create indice buffer.
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
    //1. Create VkDescriptorSetLayout for main shader.
    //--- i. Create basic block decriptor set layout.
    //VkDescriptorSetLayoutBinding --- BasicUnifromBuffer
    VkDescriptorSetLayoutBinding var_basic_uniform_buffer;
    var_basic_uniform_buffer.binding = 0;
    var_basic_uniform_buffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    var_basic_uniform_buffer.descriptorCount = 1; //Only one block.
    var_basic_uniform_buffer.pImmutableSamplers = nullptr;
    var_basic_uniform_buffer.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;

    //--- ii. Write create descriptor set layout info.
    std::vector<VkDescriptorSetLayoutBinding> uniform_var_location_set0;
    uniform_var_location_set0.resize(1);
    uniform_var_location_set0.push_back(var_basic_uniform_buffer);

    std::vector<VkDescriptorSetLayoutCreateInfo> desc_set_c_infos;
    desc_set_c_infos.resize(1);
    desc_set_c_infos[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    desc_set_c_infos[0].pNext = nullptr;
    desc_set_c_infos[0].flags = 0;
    desc_set_c_infos[0].pBindings = uniform_var_location_set0.data();//set = 0
    desc_set_c_infos[0].bindingCount = static_cast<uint32_t>(uniform_var_location_set0.size());

    //--- iii. Create descriptor set layouts.
    m_mgr->CreateDescriptorSetLayout(desc_set_c_infos, m_VK_main_shader_set_layouts);
}