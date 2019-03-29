#include "SDEngine.h"
#include "Sample1_DrawTriangle.h"

using namespace::SDE::Basic;
using namespace::SDE::Graphics;

Sample1_DrawTriangle::Sample1_DrawTriangle()
: m_VK_vertice_buffer(VK_NULL_HANDLE)
, m_VK_vbuf_memory(VK_NULL_HANDLE)
, m_VK_indices_buffer(VK_NULL_HANDLE)
, m_VK_ibuf_memory(VK_NULL_HANDLE)
, m_VK_phy_device(VK_NULL_HANDLE)
, m_VK_device(VK_NULL_HANDLE)
{
}

Sample1_DrawTriangle::~Sample1_DrawTriangle()
{
    if (m_VK_device != VK_NULL_HANDLE) {
        if (m_VK_vbuf_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_VK_device, m_VK_vbuf_memory, nullptr);
        }
        if (m_VK_vertice_buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_VK_device, m_VK_vertice_buffer, nullptr);
        }
        if (m_VK_ibuf_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_VK_device, m_VK_ibuf_memory, nullptr);
        }
        if (m_VK_indices_buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_VK_device, m_VK_indices_buffer, nullptr);
        }
    }
}

void Sample1_DrawTriangle::Initialize(VkPhysicalDevice i_VK_phy_device, VkDevice i_VK_device)
{
    VkMemoryPropertyFlagBits mem_prop_flag = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;//Storage at graphics card.

    std::vector<vec3> quad_vecs = {
        vec3( 0.5f,  0.5f, -0.1f),
        vec3(-0.5f,  0.5f, -0.1f),
        vec3(-0.5f, -0.5f, -0.1f),
        vec3( 0.5f, -0.5f, -0.1f)
    };

    std::vector<uint16_t> quad_indices = {
        0,1,2,
        0,2,3
    };

    VkResult result = VK_SUCCESS;

    //1. create vertex buffer.
    //--- i. create buffer information.
    VkBufferCreateInfo vec_buf_c_info = {};
    vec_buf_c_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vec_buf_c_info.pNext = nullptr;
    vec_buf_c_info.flags = 0;
    vec_buf_c_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vec_buf_c_info.size = static_cast<uint64_t>(sizeof(vec3) * quad_vecs.size());
    vec_buf_c_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vec_buf_c_info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
    vec_buf_c_info.pQueueFamilyIndices = nullptr;

    result = vkCreateBuffer(i_VK_device, &vec_buf_c_info, nullptr, &m_VK_vertice_buffer);

    if (result != VK_SUCCESS) {
        SDLOGE("Create vertice buffer failure");
        return;
    }

    //--- ii. Get device info.
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(i_VK_phy_device, &phy_dev_memory_props);
    for (uint32_t type = 0; type < phy_dev_memory_props.memoryTypeCount; ++type) {
        SDLOGD("Type[%d] : Flags(%u) HeadID(%llu)", type,
            phy_dev_memory_props.memoryTypes[type].propertyFlags,
            phy_dev_memory_props.memoryTypes[type].heapIndex);
    }

    //--- iii. Get memory info. We will get memory size of graphics card and main memory.
    for (uint32_t heap_ID = 0; heap_ID < phy_dev_memory_props.memoryHeapCount; ++heap_ID) {
        SDLOGD("Heap[%d] : Flag(%u) Size(%llu)", heap_ID,
            phy_dev_memory_props.memoryHeaps[heap_ID].flags,
            phy_dev_memory_props.memoryHeaps[heap_ID].size);
    }

    //--- iv. get requirement info of vertices buffer.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(i_VK_device, m_VK_vertice_buffer, &mem_req);
    SDLOGD("Req info : Size(%llu) Alignment(%llu) MemType(%u)",
        mem_req.size, mem_req.alignment, mem_req.memoryTypeBits);

    //--- v. allocate memory space following memory type and prop flag.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported = 
            ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & mem_prop_flag) == mem_prop_flag);

        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            VkMemoryAllocateInfo buffer_mem_a_info = {};
            buffer_mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            buffer_mem_a_info.pNext = nullptr;
            buffer_mem_a_info.allocationSize = mem_req.size;
            buffer_mem_a_info.memoryTypeIndex = mem_type_ID;

            result = vkAllocateMemory(i_VK_device, &buffer_mem_a_info, nullptr, &m_VK_vbuf_memory);
            if (result != VK_SUCCESS) {
                SDLOGE("Allocate vertex buffer failure!!!");
                return;
            }
        }
    }

    result = vkBindBufferMemory(i_VK_device, m_VK_vertice_buffer, m_VK_vbuf_memory, 0);
    if (result != VK_SUCCESS) {
        SDLOGE("Bind vertex buffer memory failure!!!");
        return;
    }
    //--- vi. Map memory.
    void *local_ptr = nullptr;
    result = vkMapMemory(i_VK_device, m_VK_vbuf_memory, 0, static_cast<uint64_t>(sizeof(vec3) * quad_vecs.size()), 0, &local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map vertex buffer memory failure!!!");
        return;
    }

    std::memcpy(local_ptr, quad_vecs.data(), static_cast<size_t>(sizeof(vec3) * quad_vecs.size()));

    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = m_VK_vbuf_memory;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;

    vkFlushMappedMemoryRanges(i_VK_device, 1, &mem_ranges);

    vkUnmapMemory(i_VK_device, m_VK_vbuf_memory);

    //2. create indice buffer.
    //--- i. create buffer information.
    VkBufferCreateInfo ind_buf_c_info = {};
    ind_buf_c_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    ind_buf_c_info.pNext = nullptr;
    ind_buf_c_info.flags = 0;
    ind_buf_c_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    ind_buf_c_info.size = static_cast<uint64_t>(sizeof(uint16_t) * quad_indices.size());
    ind_buf_c_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ind_buf_c_info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
    ind_buf_c_info.pQueueFamilyIndices = nullptr;

    result = vkCreateBuffer(i_VK_device, &ind_buf_c_info, nullptr, &m_VK_indices_buffer);

    if (result != VK_SUCCESS) {
        SDLOGE("Create vertice buffer failure");
        return;
    }

    //--- ii. Get device info.
    vkGetPhysicalDeviceMemoryProperties(i_VK_phy_device, &phy_dev_memory_props);
    for (uint32_t type = 0; type < phy_dev_memory_props.memoryTypeCount; ++type) {
        SDLOGD("Type[%d] : Flags(%u) HeadID(%llu)", type,
            phy_dev_memory_props.memoryTypes[type].propertyFlags,
            phy_dev_memory_props.memoryTypes[type].heapIndex);
    }

    //--- iii. Get memory info. We will get memory size of graphics card and main memory.
    for (uint32_t heap_ID = 0; heap_ID < phy_dev_memory_props.memoryHeapCount; ++heap_ID) {
        SDLOGD("Heap[%d] : Flag(%u) Size(%llu)", heap_ID,
            phy_dev_memory_props.memoryHeaps[heap_ID].flags,
            phy_dev_memory_props.memoryHeaps[heap_ID].size);
    }

    //--- iv. get requirement info of vertices buffer.
    vkGetBufferMemoryRequirements(i_VK_device, m_VK_vertice_buffer, &mem_req);
    SDLOGD("Req info : Size(%llu) Alignment(%llu) MemType(%u)",
        mem_req.size, mem_req.alignment, mem_req.memoryTypeBits);

    //--- v. allocate memory space following memory type and prop flag.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported =
            ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & mem_prop_flag) == mem_prop_flag);

        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            VkMemoryAllocateInfo buffer_mem_a_info = {};
            buffer_mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            buffer_mem_a_info.pNext = nullptr;
            buffer_mem_a_info.allocationSize = mem_req.size;
            buffer_mem_a_info.memoryTypeIndex = mem_type_ID;

            result = vkAllocateMemory(i_VK_device, &buffer_mem_a_info, nullptr, &m_VK_ibuf_memory);
            if (result != VK_SUCCESS) {
                SDLOGE("Allocate indice buffer failure!!!");
                return;
            }
        }
    }

    result = vkBindBufferMemory(i_VK_device, m_VK_indices_buffer, m_VK_ibuf_memory, 0);
    if (result != VK_SUCCESS) {
        SDLOGE("Bind indice buffer memory failure!!!");
        return;
    }
    //--- vi. Map memory.
    local_ptr = nullptr;
    result = vkMapMemory(i_VK_device, m_VK_ibuf_memory, 0, static_cast<uint64_t>(sizeof(uint16_t) * quad_indices.size()), 0, &local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map indice buffer memory failure!!!");
        return;
    }

    std::memcpy(local_ptr, quad_indices.data(), static_cast<uint64_t>(sizeof(uint16_t) * quad_indices.size()));

    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = m_VK_ibuf_memory;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;

    vkFlushMappedMemoryRanges(i_VK_device, 1, &mem_ranges);

    vkUnmapMemory(i_VK_device, m_VK_ibuf_memory);

}

void Sample1_DrawTriangle::Render()
{
}