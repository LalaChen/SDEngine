#include "SDEngine.h"
#include "VulkanAPITestManager.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;

void VulkanAPITestManager::InitializeScreenRendering()
{
    VkResult result = VK_SUCCESS;

    SDLOG("Create Screen Rendering!!!");
    //1. Create basic block decriptor set layout for screen rendering.
    //--- i. Create sampler for main shader.
    VkSamplerCreateInfo sampler_c_info = {};
    sampler_c_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_c_info.pNext = nullptr;
    sampler_c_info.flags = 0; //
    sampler_c_info.minFilter = VK_FILTER_LINEAR;
    sampler_c_info.magFilter = VK_FILTER_LINEAR;
    sampler_c_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    sampler_c_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_c_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_c_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler_c_info.mipLodBias = 0.0f;
    sampler_c_info.anisotropyEnable = VK_FALSE;
    sampler_c_info.maxAnisotropy = 1.0f;
    sampler_c_info.compareEnable = VK_FALSE; //Use depth compare operation.
    sampler_c_info.compareOp = VK_COMPARE_OP_ALWAYS; //Use depth compare operation.
    sampler_c_info.minLod = 0.0f;
    sampler_c_info.maxLod = 1.0f;
    sampler_c_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    sampler_c_info.unnormalizedCoordinates = VK_FALSE;
    result = CreateSampler(sampler_c_info, m_VK_screen_cb_sampler);
    if (result != VK_SUCCESS) {
        SDLOGE("Create sampler failure!!!");
        return;
    }

    //--- ii. Write create descriptor set layout info.
    VkDescriptorSetLayoutBinding var_main_texture = {};
    var_main_texture.binding = 0;
    var_main_texture.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //sampler2D
    var_main_texture.descriptorCount = 1;
    var_main_texture.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; //use at all shader. 
    var_main_texture.pImmutableSamplers = &m_VK_screen_cb_sampler;

    VkDescriptorSetLayoutCreateInfo desc_set_c_info = {}; //One set one layout.
    desc_set_c_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    desc_set_c_info.pNext = nullptr;
    desc_set_c_info.flags = 0;
    desc_set_c_info.bindingCount = 1;
    desc_set_c_info.pBindings = &var_main_texture; //set = 0

    //--- iii. Create descriptor set layout for screen render.
    result = vkCreateDescriptorSetLayout(m_device_handle, &desc_set_c_info, nullptr, &m_VK_present_gp_set_layout);
    if (result != VK_SUCCESS) {
        SDLOGW("Create screen render descriptor failure. Result = %x.", result);
    }
    
    //2.  create graphics pipeline for screen rendering.
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
    vert_input_binding_des_infos[1].stride = sizeof(vec2);
    vert_input_binding_des_infos[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //--- ii. link bind pointer. (glVertexAttribBinding)m_VK_main_shader_set_layouts
    VkVertexInputAttributeDescription vert_input_attrib_des_infos[2] = {
        {}, {}
    };
    //------ bind vertice attribute.
    vert_input_attrib_des_infos[0].binding = 0; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[0].location = 0; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vert_input_attrib_des_infos[0].offset = 0;
    //------ bind tex attribute.
    vert_input_attrib_des_infos[1].binding = 1; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[1].location = 2; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[1].format = VK_FORMAT_R32G32_SFLOAT;
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

    //--- v. create shader module.
    FileData vert_shader, frag_shader;
    if (FileResourceRequester::GetRef().AskFile("Shader/ScreenShader.vert.spv", vert_shader) == false) {
        SDLOGE("shader/ScreenShader.spv isn't exist!!!");
        return;
    }

    if (FileResourceRequester::GetRef().AskFile("Shader/ScreenShader.frag.spv", frag_shader) == false) {
        SDLOGE("shader/ScreenShader.spv isn't exist!!!");
        return;
    }

    result = CreateShaderModule(vert_shader.m_file_content, m_VK_screen_vert_shader);
    if (result != VK_SUCCESS) {
        SDLOGE("Create screen vert shader failure!!!");
        return;
    }
    else {
        SDLOG("Create screen vert shader successfully.");
    }

    result = CreateShaderModule(frag_shader.m_file_content, m_VK_screen_frag_shader);
    if (result != VK_SUCCESS) {
        SDLOGE("Create screen frag shader failure!!!");
        return;
    }
    else {
        SDLOG("Create screen frag shader successfully.");
    }

    std::vector<VkPipelineShaderStageCreateInfo> stage_c_infos;
    VkPipelineShaderStageCreateInfo vert_stage_c_info = {};
    vert_stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_stage_c_info.stage = VK_SHADER_STAGE_VERTEX_BIT; //GL_VERT_SHADER
    vert_stage_c_info.module = m_VK_screen_vert_shader; //target shader. (glCreateShader)
    vert_stage_c_info.pName = "main";
    vert_stage_c_info.pNext = nullptr; // NULL or a pointer to an extension-specific structure.
    vert_stage_c_info.flags = 0; //reverse for future use.
    stage_c_infos.push_back(vert_stage_c_info);

    VkPipelineShaderStageCreateInfo frag_stage_c_info = {};
    frag_stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_stage_c_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT; //GL_FRAG_SHADER
    frag_stage_c_info.module = m_VK_screen_frag_shader; //target shader. (glCreateShader)
    frag_stage_c_info.pName = "main"; //entry function name.
    frag_stage_c_info.pNext = nullptr; // NULL or a pointer to an extension-specific structure.
    frag_stage_c_info.flags = 0; //reverse for future use.
    stage_c_infos.push_back(frag_stage_c_info);

    Resolution res = GetScreenResolution();
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
    raster_state_c_info.cullMode = VK_CULL_MODE_NONE; //glCullFace(GL_BACK)
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
    depth_stencil_c_info.depthBoundsTestEnable = VK_FALSE;
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
    std::vector<VkDescriptorSetLayout> desc_layouts = {
        m_VK_present_gp_set_layout
    };

    VkPipelineLayoutCreateInfo pipeline_layout_c_info = {};
    pipeline_layout_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_c_info.flags = 0;
    pipeline_layout_c_info.pNext = nullptr;
    pipeline_layout_c_info.setLayoutCount = static_cast<uint32_t>(desc_layouts.size());
    pipeline_layout_c_info.pSetLayouts = desc_layouts.data();
    pipeline_layout_c_info.pushConstantRangeCount = 0;
    pipeline_layout_c_info.pPushConstantRanges = nullptr;
    result = CreatePipelineLayout(pipeline_layout_c_info, m_VK_screen_pipeline_layout);
    if (result != VK_SUCCESS) {
        SDLOGE("Create screen pipeline layout failure");
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
    graphics_pipeline_c_info.layout = m_VK_screen_pipeline_layout;
    graphics_pipeline_c_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_c_info.basePipelineIndex = -1;
    graphics_pipeline_c_info.renderPass = m_pre_rp_handle;
    graphics_pipeline_c_info.subpass = 0;

    result = CreateGraphicsPipeline(graphics_pipeline_c_info, VK_NULL_HANDLE, m_VK_screen_pipeline);
    if (result != VK_SUCCESS) {
        SDLOGE("Create screen pipeline failure");
        return;
    }

    //3. create vertice and texture vbo for screen.
    std::vector<vec3> quad_vecs = {
        vec3(-1.0f,  1.0f, 0.001f),
        vec3(-1.0f, -1.0f, 0.001f),
        vec3( 1.0f, -1.0f, 0.001f),
        vec3( 1.0f,  1.0f, 0.001f)
    };

    std::vector<vec2> quad_texs = {
        vec2(0.0f, 0.0f),
        vec2(0.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 0.0f)
    };

    std::vector<uint16_t> quad_indices = {
        0,1,2,
        0,2,3
    };

    result = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(vec3) * quad_vecs.size()), m_VK_screen_ver_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create vertice buffer failure");
        return;
    }
    result = AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, 
        m_VK_screen_ver_buffer, m_VK_screen_ver_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex buffer failure!!!");
        return;
    }
    result = RefreshLocalDeviceBufferData(m_VK_screen_ver_buffer, quad_vecs.data(), 
        static_cast<VkDeviceSize>(sizeof(vec3) * quad_vecs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex buffer failure!!!");
        return;
    }

    result = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(vec2) * quad_texs.size()), m_VK_screen_ver_tex_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create vertice buffer failure");
        return;
    }
    result = AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_screen_ver_tex_buffer,
        m_VK_screen_ver_tex_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex tex buffer failure!!!");
        return;
    }
    result = RefreshLocalDeviceBufferData(m_VK_screen_ver_tex_buffer, quad_texs.data(), static_cast<VkDeviceSize>(sizeof(vec2) * quad_texs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex tex buffer failure!!!");
        return;
    }

    result = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()), m_VK_screen_indices_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create indice buffer failure");
        return;
    }
    result = AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, 
        m_VK_screen_indices_buffer, m_VK_screen_ibuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate indice buffer failure!!!");
        return;
    }
    result = RefreshLocalDeviceBufferData(m_VK_screen_indices_buffer, quad_indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh indice buffer failure!!!");
        return;
    }
    //4. create descriptor pool, set and uniform buffers.
    //--- i. create descriptor pool.
    std::vector<VkDescriptorPoolSize> types;
    VkDescriptorPoolSize image_type = {};
    image_type.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    image_type.descriptorCount = 1; //maximum swap chain.
    types.push_back(image_type);
    result = CreateDescriptorPool(types, 1, false, m_VK_present_gp_set_pool);
    if (result != VK_SUCCESS) {
        SDLOGE("Create descriptor pool failure!!!");
        return;
    }

    VkDescriptorSetAllocateInfo desc_set_a_info = {};
    desc_set_a_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    desc_set_a_info.pNext = nullptr;
    desc_set_a_info.descriptorSetCount = 1;
    desc_set_a_info.descriptorPool = m_VK_present_gp_set_pool;
    desc_set_a_info.pSetLayouts = &m_VK_present_gp_set_layout;
    result = AllocateDescriptorSet(desc_set_a_info, m_VK_screen_desc_set);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate descriptor set failure!!!");
        return;
    }
}

void VulkanAPITestManager::ReleaseScreenRendering()
{
    if (m_VK_screen_cb_sampler != VK_NULL_HANDLE) {
        DestroySampler(m_VK_screen_cb_sampler);
        m_VK_screen_cb_sampler = VK_NULL_HANDLE;
    }

    if (m_VK_screen_ibuf_memory != VK_NULL_HANDLE) {
        ReleaseMemory(m_VK_screen_ibuf_memory);
        m_VK_screen_ibuf_memory = VK_NULL_HANDLE;
    }

    if (m_VK_screen_indices_buffer != VK_NULL_HANDLE) {
        DestroyBuffer(m_VK_screen_indices_buffer);
        m_VK_screen_indices_buffer = VK_NULL_HANDLE;
    }

    if (m_VK_screen_ver_tex_buf_memory != VK_NULL_HANDLE) {
        ReleaseMemory(m_VK_screen_ver_tex_buf_memory);
        m_VK_screen_ver_tex_buf_memory = VK_NULL_HANDLE;
    }

    if (m_VK_screen_ver_tex_buffer != VK_NULL_HANDLE) {
        DestroyBuffer(m_VK_screen_ver_tex_buffer);
        m_VK_screen_ver_tex_buffer = VK_NULL_HANDLE;
    }

    if (m_VK_screen_ver_buf_memory != VK_NULL_HANDLE) {
        ReleaseMemory(m_VK_screen_ver_buf_memory);
        m_VK_screen_ver_buf_memory = VK_NULL_HANDLE;
    }

    if (m_VK_screen_ver_buffer != VK_NULL_HANDLE) {
        DestroyBuffer(m_VK_screen_ver_buffer);
        m_VK_screen_ver_buffer = VK_NULL_HANDLE;
    }

    if (m_VK_screen_pipeline != VK_NULL_HANDLE) {
        DestroyGraphicsPipeline(m_VK_screen_pipeline);
        m_VK_screen_pipeline = VK_NULL_HANDLE;
    }

    if (m_VK_screen_pipeline_layout != VK_NULL_HANDLE) {
        DestroyPipelineLayout(m_VK_screen_pipeline_layout);
        m_VK_screen_pipeline_layout = VK_NULL_HANDLE;
    }

    if (m_VK_screen_frag_shader != VK_NULL_HANDLE) {
        DestroyShaderModule(m_VK_screen_frag_shader);
        m_VK_screen_frag_shader = VK_NULL_HANDLE;
    }

    if (m_VK_screen_vert_shader != VK_NULL_HANDLE) {
        DestroyShaderModule(m_VK_screen_vert_shader);
        m_VK_screen_vert_shader = VK_NULL_HANDLE;
    }
}