//#define USE_HOST_BUFFER

#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample1_DrawTriangle.h"

using namespace::SDE::Math;
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
//
, m_VK_basic_uniform_buffer(VK_NULL_HANDLE)
, m_VK_basic_uniform_buffer_memory(VK_NULL_HANDLE)
//
, m_VK_pipeline_layout(VK_NULL_HANDLE)
, m_vert_module(VK_NULL_HANDLE)
, m_frag_module(VK_NULL_HANDLE)
, m_VK_main_shader_set0_layout(VK_NULL_HANDLE)
, m_VK_descriptor_set0(VK_NULL_HANDLE)
, m_VK_descriptor_pool(VK_NULL_HANDLE)
, m_VK_main_graphics_pipeline(VK_NULL_HANDLE)
{
}

Sample1_DrawTriangle::~Sample1_DrawTriangle()
{

}

void Sample1_DrawTriangle::Initialize()
{
    CreateBuffers();
    CreateUniformBuffer();
    CreateShaders();
}

void Sample1_DrawTriangle::Render()
{
    if (m_mgr != nullptr) {
        VkResult result = VK_SUCCESS;
        Resolution res = m_mgr->GetScreenResolution();
        //------ Viewport
        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = static_cast<float>(res.GetWidth());
        viewport.height = static_cast<float>(res.GetHeight());
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        m_mgr->SetMainViewportDynamically(viewport);
        //Update uniform buffer.
        //--- clip space.
        float clip_mat[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, -1.0f, 0.0f, 0.0f,
                         0.0f,  0.0f, 0.5f, 0.0f,
                         0.0f,  0.0f, 0.5f, 1.0f };
        m_uniform_buffer_data.m_clip = Matrix4X4f(clip_mat);
        //--- projection space.
        float asratio = viewport.height / viewport.width;
        m_uniform_buffer_data.m_proj.perspective(90, 1.0f / asratio, 0.01f, 10.0f);
        //m_uniform_buffer_data.m_proj.ortho(-1.0f, 1.0f, -1.0f * asratio, 1.0 * asratio, -1.0f, 1.0f);

        //--- view space.
        m_uniform_buffer_data.m_view.lookAt(Vector3f(0.0f, 0.0f, 5.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f, 1.0f), Vector3f(0.0f, 1.0f, 0.0f, 1.0f));

        //--- world space. 
        static float angle = 0.0f;
        static float addAngle = 1.0f;
        angle += addAngle;
        //m_uniform_buffer_data.m_worid.rotate(Quaternion(Vector3f::PositiveZ, angle));
        result = m_mgr->RefreshHostDeviceBufferData(m_VK_basic_uniform_buffer, m_VK_basic_uniform_buffer_memory, &m_uniform_buffer_data, sizeof(BasicUniformBuffer));
        if (result != VK_SUCCESS) {
            SDLOGE("Refresg Host Buffer Data failure!!!");
        }

        //Open graphics pipeline.
        m_mgr->BindGraphicsPipeline(m_VK_main_graphics_pipeline);

        //Bind descriptor sets
        std::vector<VkDescriptorSet> descs = {m_VK_descriptor_set0};
        std::vector<uint32_t> dynamic_offs = {};
        m_mgr->BindDescriptorSets(m_VK_pipeline_layout, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, descs, dynamic_offs);
        
        //Bind Vertex Buffer.
        m_mgr->BindVertexBuffer(m_VK_vertice_buffer, 0, 0); //vertex
        m_mgr->BindVertexBuffer(m_VK_ver_color_buffer, 0, 1); //color
        
        //std::vector<VkBuffer> va_buffers = { m_VK_vertice_buffer, m_VK_ver_color_buffer};
        //std::vector<VkDeviceSize> va_buffer_offsets = {0, 0};
        //m_mgr->BindVertexBuffers(va_buffers, va_buffer_offsets, 0);
       
        m_mgr->BindIndiceBuffer(m_VK_indices_buffer, 0, VK_INDEX_TYPE_UINT16); //Bind indices.

        //Draw
        m_mgr->DrawByIndice(6, 1, 0, 0, 0);
        
    }
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
        //
        m_mgr->ReleaseMemory(m_VK_basic_uniform_buffer_memory);
        m_mgr->DestroyBuffer(m_VK_basic_uniform_buffer);
        m_VK_basic_uniform_buffer_memory = VK_NULL_HANDLE;
        m_VK_basic_uniform_buffer = VK_NULL_HANDLE;
        //
        m_mgr->DestroyDesciptorSetLayout(m_VK_main_shader_set0_layout);
        m_VK_main_shader_set0_layout = VK_NULL_HANDLE;

        m_mgr->DestroyDescriptorPool(m_VK_descriptor_pool);
        m_mgr->DestroyGraphicsPipeline(m_VK_main_graphics_pipeline);
        m_mgr->DestroyPipelineLayout(m_VK_pipeline_layout);
        m_mgr->DestroyShaderModule(m_frag_module);
        m_mgr->DestroyShaderModule(m_vert_module);
    }
}

void Sample1_DrawTriangle::CreateBuffers()
{
    SDLOG("Create Buffer!!!");
    std::vector<vec3> quad_vecs = {
        vec3( 0.5f,  0.5f, -0.01f),
        vec3(-0.5f,  0.5f, -0.01f),
        vec3(-0.5f, -0.5f, -0.01f),
        vec3( 0.5f, -0.5f, -0.01f)
    };

    std::vector<Color4f> quad_colors = {
        Color4f(1.0f, 0.0f, 0.0f, 1.0f),
        Color4f(0.0f, 1.0f, 0.0f, 1.0f),
        Color4f(0.0f, 0.0f, 1.0f, 1.0f),
        Color4f(1.0f, 1.0f, 1.0f, 1.0f)
    };

    //To do : Find out the root cause why it's back face in vulkan.
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
#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_vertice_buffer, m_VK_vbuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_vertice_buffer, quad_vecs.data(), static_cast<VkDeviceSize>(sizeof(vec3) * quad_vecs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_vertice_buffer, m_VK_vbuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_vertice_buffer, m_VK_vbuf_memory, quad_vecs.data(), static_cast<VkDeviceSize>(sizeof(vec3) * quad_vecs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex buffer failure!!!");
        return;
    }
#endif

    //2. create color buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(Color4f) * quad_colors.size()), m_VK_ver_color_buffer);

#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_ver_color_buffer, m_VK_ver_color_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex color buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_ver_color_buffer, quad_colors.data(), static_cast<VkDeviceSize>(sizeof(Color4f) * quad_colors.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex color buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_ver_color_buffer, m_VK_ver_color_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex color buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_ver_color_buffer, m_VK_ver_color_buf_memory, quad_colors.data(), static_cast<VkDeviceSize>(sizeof(Color4f) * quad_colors.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex color buffer failure!!!");
        return;
    }
#endif

    //3. create indice buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()), m_VK_indices_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create indice buffer failure");
        return;
    }
#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_indices_buffer, m_VK_ibuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate indice buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_indices_buffer, quad_indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh indice buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_indices_buffer, m_VK_ibuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate indice buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_indices_buffer, m_VK_ibuf_memory, quad_indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh indice buffer failure!!!");
        return;
    }
#endif
}

void Sample1_DrawTriangle::CreateUniformBuffer()
{
    SDLOG("Create Uniform Buffer!!!");
    VkResult result = VK_SUCCESS;
    //Create basic uniform buffer
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, sizeof(BasicUniformBuffer), m_VK_basic_uniform_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create uniform buffer failure!!!");
    }
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_basic_uniform_buffer, m_VK_basic_uniform_buffer_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate uniform buffer failure!!!");
    }
}

void Sample1_DrawTriangle::CreateShaders()
{
    SDLOG("Create Shader!!!");
    VkResult result = VK_SUCCESS;
    //1. bind vertex attribute array.
    //(such like glVertexAttribBinding and glVertexAttribFormat.)
    //--- i. bind pointer.
    //------ one buffer one binding. (glVertexAttribFormat)
    VkVertexInputBindingDescription vert_input_binding_des_infos[2] = {
        {}, {}
    };
    vert_input_binding_des_infos[0].binding = 0;
    vert_input_binding_des_infos[0].stride = sizeof(vec3);
    vert_input_binding_des_infos[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    vert_input_binding_des_infos[1].binding = 1;
    vert_input_binding_des_infos[1].stride = sizeof(Color4f);
    vert_input_binding_des_infos[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //--- ii. link bind pointer. (glVertexAttribBinding)m_VK_main_shader_set_layouts
    VkVertexInputAttributeDescription vert_input_attrib_des_infos[2] = {
        {},{}
    };
    //------ bind vertice attribute.
    vert_input_attrib_des_infos[0].binding = 0; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[0].location = 0; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vert_input_attrib_des_infos[0].offset = 0;
    //------ bind color attribute.
    vert_input_attrib_des_infos[1].binding = 1; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[1].location = 1; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    vert_input_attrib_des_infos[1].offset = 0;

    VkPipelineVertexInputStateCreateInfo v_input_state_c_info = {};
    v_input_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    v_input_state_c_info.pNext = nullptr;
    v_input_state_c_info.pVertexBindingDescriptions = vert_input_binding_des_infos;
    v_input_state_c_info.vertexBindingDescriptionCount = 2;
    v_input_state_c_info.pVertexAttributeDescriptions = vert_input_attrib_des_infos;
    v_input_state_c_info.vertexAttributeDescriptionCount = 2;

    //--- iii. create input assembly states.(GL_TRIANGLE, ...etc.)
    VkPipelineInputAssemblyStateCreateInfo v_input_assembly_state_c_info = {};
    v_input_assembly_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    v_input_assembly_state_c_info.pNext = nullptr;
    v_input_assembly_state_c_info.flags = 0;
    v_input_assembly_state_c_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //GL_TRIANGLE 
    v_input_assembly_state_c_info.primitiveRestartEnable = VK_FALSE; //Use to restart idx (set 0XFFFFFFFF or 0XFFFF) and then to draw fan or strip.

    //--- iv. create tessellation state create info.
    VkPipelineTessellationStateCreateInfo tessellation_state_c_info = {};
    tessellation_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation_state_c_info.pNext = nullptr;
    tessellation_state_c_info.flags = 0;
    tessellation_state_c_info.patchControlPoints = 3; //3 : triangle. 4 : quad.

    //2. Create VkDescriptorSetLayout for main shader.
    //--- i. Create basic block decriptor set layout.
    //VkDescriptorSetLayoutBinding --- BasicUnifromBuffer
    VkDescriptorSetLayoutBinding var_basic_uniform_buffer = {};
    var_basic_uniform_buffer.binding = 0;
    var_basic_uniform_buffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    var_basic_uniform_buffer.descriptorCount = 1; //Only one block.
    var_basic_uniform_buffer.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; //use at all shader.
    var_basic_uniform_buffer.pImmutableSamplers = nullptr;

    //--- ii. Write create descriptor set layout info.
    std::vector<VkDescriptorSetLayoutBinding> uniform_var_location_set0;
    uniform_var_location_set0.resize(1);
    uniform_var_location_set0.push_back(var_basic_uniform_buffer);

    VkDescriptorSetLayoutCreateInfo desc_set_c_info = {}; //One set one layout.
    desc_set_c_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    desc_set_c_info.pNext = nullptr;
    desc_set_c_info.flags = 0;
    desc_set_c_info.bindingCount = static_cast<uint32_t>(uniform_var_location_set0.size());
    desc_set_c_info.pBindings = uniform_var_location_set0.data();//set = 0

    //--- iii. Create descriptor set layouts. (Create binding location for uniform variable)
    result = m_mgr->CreateDescriptorSetLayout(desc_set_c_info, m_VK_main_shader_set0_layout);
    if (result != VK_SUCCESS) {
        SDLOGE("Create descriptror set layout failure!!!");
        return;
    }

    //--- iv. Create descriptor pools. (Create pool about VkDescriptorSet)
    std::vector<VkDescriptorPoolSize> types;
    VkDescriptorPoolSize uniform_buffer_type = {};
    uniform_buffer_type.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniform_buffer_type.descriptorCount = 1;
    types.push_back(uniform_buffer_type);
    result = m_mgr->CreateDescriptorPool(types, 1, false, m_VK_descriptor_pool);
    if (result != VK_SUCCESS) {
        SDLOGE("Create descriptror pool failure!!!");
        return;
    }

    //--- v. Allocate descriptor set. (space for uniform data)
    VkDescriptorSetAllocateInfo desc_set_a_info = {};
    desc_set_a_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    desc_set_a_info.pNext = nullptr;
    desc_set_a_info.descriptorPool = m_VK_descriptor_pool;
    desc_set_a_info.descriptorSetCount = 1;
    desc_set_a_info.pSetLayouts = &m_VK_main_shader_set0_layout;
    result = m_mgr->AllocateDescriptorSet(desc_set_a_info, m_VK_descriptor_set0);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate descriptror set failure!!!");
        return;
    }

    //--- vi. Update descriptor set.(call it before binding and call it once. don't need to call it every frame.)
    VkDescriptorBufferInfo basic_uniform_b_info = {};
    basic_uniform_b_info.buffer = m_VK_basic_uniform_buffer;
    basic_uniform_b_info.offset = 0;
    basic_uniform_b_info.range = sizeof(BasicUniformBuffer);

    VkWriteDescriptorSet mvp_descriptor_set_info = {};
    mvp_descriptor_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    mvp_descriptor_set_info.pNext = nullptr;
    mvp_descriptor_set_info.dstSet = m_VK_descriptor_set0;
    mvp_descriptor_set_info.descriptorCount = 1;
    mvp_descriptor_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    mvp_descriptor_set_info.pBufferInfo = &basic_uniform_b_info;
    mvp_descriptor_set_info.pImageInfo = nullptr;
    mvp_descriptor_set_info.pTexelBufferView = nullptr;
    mvp_descriptor_set_info.dstArrayElement = 0;
    mvp_descriptor_set_info.dstBinding = 0; //binding 0, set 0

    std::vector<VkWriteDescriptorSet> descriptor_set_infos = { mvp_descriptor_set_info };

    m_mgr->UpdateDescriptorSet(descriptor_set_infos);

    //3. build shader.
    //--- i. read shader file.
    FileData vert_shader, frag_shader;
    if (FileSystemManager::GetRef().OpenFile("shader/MainShader.vert.spv", vert_shader) == false) {
        SDLOGE("shader/MainShaderVert.spv isn't exist!!!");
        return;
    }

    if (FileSystemManager::GetRef().OpenFile("shader/MainShader.frag.spv", frag_shader) == false) {
        SDLOGE("shader/MainShaderFrag.spv isn't exist!!!");
        return;
    }
    //--- ii. create shader module.
    result = m_mgr->CreateShaderModule(vert_shader.m_file_content, m_vert_module);
    if (result != VK_SUCCESS) {
        SDLOGE("Create vert shader failure!!!");
        return;
    }

    result = m_mgr->CreateShaderModule(frag_shader.m_file_content, m_frag_module);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate frag shader failure!!!");
        return;
    }

    //4. Create shader pipeline at main renderpass.
    //--- i. Write shader stage create info for building graphics pipeline.
    std::vector<VkPipelineShaderStageCreateInfo> stage_c_infos;
    VkPipelineShaderStageCreateInfo vert_stage_c_info = {};
    vert_stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_stage_c_info.stage = VK_SHADER_STAGE_VERTEX_BIT; //GL_VERT_SHADER
    vert_stage_c_info.module = m_vert_module; //target shader. (glCreateShader)
    vert_stage_c_info.pName = "main"; 
    vert_stage_c_info.pNext = nullptr; // NULL or a pointer to an extension-specific structure.
    vert_stage_c_info.flags = 0; //reverse for future use.
    stage_c_infos.push_back(vert_stage_c_info);

    VkPipelineShaderStageCreateInfo frag_stage_c_info = {};
    frag_stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_stage_c_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT; //GL_FRAG_SHADER
    frag_stage_c_info.module = m_frag_module; //target shader. (glCreateShader)
    frag_stage_c_info.pName = "main"; //entry function name.
    frag_stage_c_info.pNext = nullptr; // NULL or a pointer to an extension-specific structure.
    frag_stage_c_info.flags = 0; //reverse for future use.
    stage_c_infos.push_back(frag_stage_c_info);
    //--- ii. Set states (like glEnable XXX, and set glXXXXFunc)
    Resolution res = m_mgr->GetScreenResolution();
    //------ Viewport and scissor.
    VkViewport viewport = {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = static_cast<float>(res.GetWidth());
    viewport.height = static_cast<float>(res.GetHeight());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor_region = {};
    scissor_region.offset.x = 0;
    scissor_region.offset.y = 0;
    scissor_region.extent.width = res.GetWidth();
    scissor_region.extent.height = res.GetHeight();

    VkPipelineViewportStateCreateInfo viewport_state_c_info = {};
    viewport_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_c_info.pViewports = &viewport;
    viewport_state_c_info.viewportCount = 1;
    viewport_state_c_info.pScissors = &scissor_region;
    viewport_state_c_info.scissorCount = 1;
    //------ rasterize (face order, polygon mode, face culling)
    VkPipelineRasterizationStateCreateInfo raster_state_c_info = {};
    raster_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_state_c_info.pNext = nullptr;
    raster_state_c_info.flags = 0;
    raster_state_c_info.rasterizerDiscardEnable = VK_FALSE;
    raster_state_c_info.polygonMode = VK_POLYGON_MODE_FILL; //glPolygonMode(GL_FILL)
    raster_state_c_info.cullMode = VK_CULL_MODE_BACK_BIT; //glCullFace(GL_BACK)
    raster_state_c_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; //glFrontFace(GL_CCW)
    raster_state_c_info.depthBiasEnable = VK_FALSE; //change depth result by new d = Bsloop * origin d + Bconstant.
    raster_state_c_info.depthBiasConstantFactor = 0.0f;
    raster_state_c_info.depthBiasSlopeFactor = 0.0f;
    raster_state_c_info.depthBiasClamp = 0.0f;
    raster_state_c_info.lineWidth = 1.0f; //glLineWidth(1.0);
    //------ multi sampling.
    VkPipelineMultisampleStateCreateInfo multisample_state_c_info = {};
    multisample_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_c_info.pNext = nullptr;
    multisample_state_c_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_state_c_info.sampleShadingEnable = VK_FALSE;
    multisample_state_c_info.minSampleShading = 0.0f;
    multisample_state_c_info.pSampleMask = nullptr;
    multisample_state_c_info.alphaToCoverageEnable = VK_FALSE;
    multisample_state_c_info.alphaToOneEnable = VK_FALSE;

    //------ depth and stencil.
    VkPipelineDepthStencilStateCreateInfo depth_stencil_c_info = {};
    depth_stencil_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_c_info.pNext = nullptr;
    depth_stencil_c_info.flags = 0;
    depth_stencil_c_info.depthTestEnable = VK_FALSE; //glEnable(GL_DEPTH_TEST);
    depth_stencil_c_info.depthWriteEnable = VK_TRUE; //glDepthMask(GL_TRUE);
    depth_stencil_c_info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; //glDepthFunc(GL_LEQ)
    depth_stencil_c_info.minDepthBounds = 0.0f;
    depth_stencil_c_info.maxDepthBounds = 1.0f;
    depth_stencil_c_info.stencilTestEnable = VK_FALSE;
    depth_stencil_c_info.front = {};
    depth_stencil_c_info.back = {};
    //------ alpha blending.
    VkPipelineColorBlendAttachmentState color_blending_attachment_state = {};
    color_blending_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; //glColorMask(all true);
    color_blending_attachment_state.blendEnable = VK_FALSE; //glDisable(GL_BLEND);
    color_blending_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //glSeperateBlendFunc for color
    color_blending_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blending_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
    color_blending_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; //glSeperateBlendFunc for alpha
    color_blending_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blending_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blending_c_info = {};
    color_blending_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending_c_info.logicOpEnable = VK_FALSE;
    color_blending_c_info.logicOp = VK_LOGIC_OP_COPY;
    color_blending_c_info.attachmentCount = 1;
    color_blending_c_info.pAttachments = &color_blending_attachment_state; //correspond with target subpass of this pipeline.
    color_blending_c_info.blendConstants[0] = 0.0f;
    color_blending_c_info.blendConstants[1] = 0.0f;
    color_blending_c_info.blendConstants[2] = 0.0f;
    color_blending_c_info.blendConstants[3] = 0.0f;

    //--- iii. Set dynamic state. (we can set related parameters about those state.)
    std::vector<VkDynamicState> dynamic_states = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dyn_state_c_info = {};
    dyn_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dyn_state_c_info.pNext = nullptr;
    dyn_state_c_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dyn_state_c_info.pDynamicStates = dynamic_states.data();
    //--- iv. Set uniform variable location binding.
    std::vector<VkDescriptorSetLayout> desc_layouts = { m_VK_main_shader_set0_layout };
    VkPipelineLayoutCreateInfo pipeline_layout_c_info = {};
    pipeline_layout_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_c_info.flags = 0;
    pipeline_layout_c_info.pNext = nullptr;
    pipeline_layout_c_info.setLayoutCount = static_cast<uint32_t>(desc_layouts.size());
    pipeline_layout_c_info.pSetLayouts = desc_layouts.data();
    pipeline_layout_c_info.pushConstantRangeCount = 0;
    pipeline_layout_c_info.pPushConstantRanges = nullptr;

    result = m_mgr->CreatePipelineLayout(pipeline_layout_c_info, m_VK_pipeline_layout);
    if (result != VK_SUCCESS) {
        SDLOGE("create pipeline layout failure!!!");
        return;
    }

    //--- v. Create graphics pipeline.
    VkGraphicsPipelineCreateInfo graphics_pipeline_c_info = {};
    graphics_pipeline_c_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_c_info.pNext = nullptr;
    graphics_pipeline_c_info.flags = 0;
    graphics_pipeline_c_info.stageCount = static_cast<uint32_t>(stage_c_infos.size()); //shader create info.
    graphics_pipeline_c_info.pStages = stage_c_infos.data();
    graphics_pipeline_c_info.pVertexInputState = &v_input_state_c_info;
    graphics_pipeline_c_info.pInputAssemblyState = &v_input_assembly_state_c_info;
    graphics_pipeline_c_info.pTessellationState = nullptr;
    graphics_pipeline_c_info.pViewportState = &viewport_state_c_info;
    graphics_pipeline_c_info.pRasterizationState = &raster_state_c_info;
    graphics_pipeline_c_info.pMultisampleState = &multisample_state_c_info;
    graphics_pipeline_c_info.pDepthStencilState = &depth_stencil_c_info;
    graphics_pipeline_c_info.pColorBlendState = &color_blending_c_info;
    graphics_pipeline_c_info.pDynamicState = &dyn_state_c_info;
    graphics_pipeline_c_info.layout = m_VK_pipeline_layout;
    graphics_pipeline_c_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_c_info.basePipelineIndex = -1;

    result = m_mgr->CreateMainRenderPassGraphicsPipeline(graphics_pipeline_c_info, nullptr, 0, m_VK_main_graphics_pipeline);
    if (result != VK_SUCCESS) {
        SDLOGE("create pipeline failure!!!");
        return;
    }
}