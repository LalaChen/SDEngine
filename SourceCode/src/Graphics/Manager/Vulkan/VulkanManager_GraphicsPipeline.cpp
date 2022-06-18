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

#include "VertexBufferFormat_Vulkan.h"
#include "VertexInputRate_Vulkan.h"
#include "Primitive_Vulkan.h"
#include "UniformBindingType_Vulkan.h"
#include "ShaderKind_Vulkan.h"
#include "PolygonMode_Vulkan.h"
#include "FaceCulling_Vulkan.h"
#include "FrontFaceMode_Vulkan.h"
#include "SampleCount_Vulkan.h"
#include "CompareOp_Vulkan.h"
#include "StencilOperator_Vulkan.h"
#include "BlendOperator_Vulkan.h"
#include "BlendFactor_Vulkan.h"
#include "LogicOperator_Vulkan.h"
#include "DynamicState_Vulkan.h"
#include "PipelineBindPoint_Vulkan.h"

#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________
//-------- ShaderModule --------
void VulkanManager::CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content)
{
    VkShaderModule &shader_module = reinterpret_cast<VkShaderModule&>(io_identity.m_handle);
    VkResult result = CreateVKShaderModule(shader_module, i_content.data(), i_content.size());
    if (result != VK_SUCCESS) {
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::DeleteShaderModule(ShaderModuleIdentity &io_identity)
{
    VkShaderModule &shader_module = reinterpret_cast<VkShaderModule&>(io_identity.m_handle);
    DestroyVKShaderModule(shader_module);
    io_identity.SetInvalid();
    io_identity = ShaderModuleIdentity();
}

//-------- GraphicsPipeline --------
void VulkanManager::CreateGraphicsPipeline(GraphicsPipelineIdentity &io_identity, const ShaderModules &i_shaders, const RenderPassWeakReferenceObject &i_rp, const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_dsls)
{
    const RenderPassIdentity &rp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_rp);
    VkPipeline &pipeline_handle = reinterpret_cast<VkPipeline&>(io_identity.m_handle);
    VkPipelineLayout &pipeline_layout_handle = reinterpret_cast<VkPipelineLayout&>(io_identity.m_pipeline_layout_handle);
    VkRenderPass rp_handle = reinterpret_cast<VkRenderPass>(rp_identity.m_handle);
    std::vector<VkVertexInputBindingDescription> va_input_binding_descs;
    std::vector<VkVertexInputAttributeDescription> va_input_location_descs;

    if (rp_handle == VK_NULL_HANDLE) {
        SDLOG("rp_handle is nullptr.");
        throw std::runtime_error("rp_handle is nullptr.");
    }

    std::vector<VkDescriptorSetLayout> vk_ds_layouts;
    for (const DescriptorSetLayoutWeakReferenceObject &dsl : i_dsls) {
        const DescriptorSetLayoutIdentity &dsl_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(dsl);
        if (dsl_identity.m_handle == VK_NULL_HANDLE) {
            SDLOG("dsl_identity.m_handle is nullptr.");
            throw std::runtime_error("dsl_identity.m_handle is nullptr.");
        }
        else {
            vk_ds_layouts.push_back(reinterpret_cast<VkDescriptorSetLayout>(dsl_identity.m_handle));
        }
    }

    va_input_binding_descs.resize(io_identity.m_params.m_va_binding_descs.size());
    va_input_location_descs.resize(io_identity.m_params.m_va_location_descs.size());

    //1. Prepare vertex attribute information.
    for (uint32_t vaID = 0; vaID < va_input_binding_descs.size(); ++vaID) {
        va_input_binding_descs[vaID] = {};
        va_input_binding_descs[vaID].binding = io_identity.m_params.m_va_binding_descs[vaID].m_binding_id;
        va_input_binding_descs[vaID].stride = io_identity.m_params.m_va_binding_descs[vaID].m_stride;
        va_input_binding_descs[vaID].inputRate = VertexInputRate_Vulkan::Convert(io_identity.m_params.m_va_binding_descs[vaID].m_input_rate);
    }

    for (uint32_t vaID = 0; vaID < va_input_location_descs.size(); ++vaID) {
        va_input_location_descs[vaID] = {};
        va_input_location_descs[vaID].location = io_identity.m_params.m_va_location_descs[vaID].m_location;
        va_input_location_descs[vaID].binding = io_identity.m_params.m_va_location_descs[vaID].m_binding_id;
        va_input_location_descs[vaID].format = VertexBufferFormat_Vulkan::Convert(io_identity.m_params.m_va_location_descs[vaID].m_format);
        va_input_location_descs[vaID].offset = io_identity.m_params.m_va_location_descs[vaID].m_offset;
    }

    VkPipelineVertexInputStateCreateInfo v_input_state_c_info = {};
    v_input_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    v_input_state_c_info.pNext = nullptr;
    v_input_state_c_info.flags = 0;
    v_input_state_c_info.pVertexBindingDescriptions = va_input_binding_descs.data();
    v_input_state_c_info.vertexBindingDescriptionCount = static_cast<uint32_t>(va_input_binding_descs.size());
    v_input_state_c_info.pVertexAttributeDescriptions = va_input_location_descs.data();
    v_input_state_c_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(va_input_location_descs.size());

    VkPipelineInputAssemblyStateCreateInfo v_input_assembly_state_c_info = {};
    v_input_assembly_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    v_input_assembly_state_c_info.pNext = nullptr;
    v_input_assembly_state_c_info.flags = 0;
    v_input_assembly_state_c_info.topology = Primitive_Vulkan::Convert(io_identity.m_params.m_primitive_info.m_primitive); //GL_TRIANGLE 
    v_input_assembly_state_c_info.primitiveRestartEnable = ConvertBoolean(io_identity.m_params.m_primitive_info.m_restart_enable);

    VkPipelineTessellationStateCreateInfo tessellation_state_c_info = {};
    tessellation_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation_state_c_info.pNext = nullptr;
    tessellation_state_c_info.flags = 0;
    tessellation_state_c_info.patchControlPoints = io_identity.m_params.m_patch_ctrl_points; //3 : triangle. 4 : quad.

    //2. Prepare ShaderModules.
    std::vector<VkPipelineShaderStageCreateInfo> stage_c_infos;
    for (uint32_t sID = 0; sID < ShaderKind_GRAPHICS_SHADER_NUMBER; ++sID) {
        if (i_shaders.m_shaders[sID].IsNull() == false) {
            const ShaderModuleIdentity &module_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_shaders.m_shaders[sID]);
            VkPipelineShaderStageCreateInfo stage_c_info = {};
            stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stage_c_info.pNext = nullptr;
            stage_c_info.flags = 0;
            stage_c_info.stage = ShaderKind_Vulkan::Convert(SD_CONST_SREF(i_shaders.m_shaders[sID]).GetShaderKind());
            stage_c_info.pName = SD_CONST_SREF(i_shaders.m_shaders[sID]).GetEntryNameCStr();
            stage_c_info.module = reinterpret_cast<VkShaderModule>(module_identity.m_handle);
            stage_c_infos.push_back(stage_c_info);
        }
    }

    //4. Set shader flag.
    VkViewport viewport = {};
    viewport.x = io_identity.m_params.m_viewport.m_x;
    viewport.y = io_identity.m_params.m_viewport.m_y;
    viewport.width = io_identity.m_params.m_viewport.m_width;
    viewport.height = io_identity.m_params.m_viewport.m_height;
    viewport.minDepth = io_identity.m_params.m_viewport.m_min_depth;
    viewport.maxDepth = io_identity.m_params.m_viewport.m_max_depth;

    VkRect2D scissor_region = {};
    scissor_region.offset.x = static_cast<uint32_t>(io_identity.m_params.m_scissor_region.m_x);
    scissor_region.offset.y = static_cast<uint32_t>(io_identity.m_params.m_scissor_region.m_y);
    scissor_region.extent.width = static_cast<uint32_t>(io_identity.m_params.m_scissor_region.m_width);
    scissor_region.extent.height = static_cast<uint32_t>(io_identity.m_params.m_scissor_region.m_height);

    VkPipelineViewportStateCreateInfo viewport_state_c_info = {};
    viewport_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_c_info.pNext = nullptr;
    viewport_state_c_info.flags = 0;
    viewport_state_c_info.pViewports = &viewport;
    viewport_state_c_info.viewportCount = 1;
    viewport_state_c_info.pScissors = &scissor_region;
    viewport_state_c_info.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo raster_state_c_info = {};
    raster_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_state_c_info.pNext = nullptr;
    raster_state_c_info.flags = 0;
    raster_state_c_info.rasterizerDiscardEnable = ConvertBoolean(io_identity.m_params.m_rasterization_info.m_discard_enable);
    raster_state_c_info.polygonMode = PolygonMode_Vulkan::Convert(io_identity.m_params.m_rasterization_info.m_polygon_mode);
    raster_state_c_info.cullMode = FaceCulling_Vulkan::Convert(io_identity.m_params.m_rasterization_info.m_face_culling);
    raster_state_c_info.frontFace = FrontFaceMode_Vulkan::Convert(io_identity.m_params.m_rasterization_info.m_front_face);
    raster_state_c_info.depthBiasEnable = ConvertBoolean(io_identity.m_params.m_rasterization_info.m_depth_bias_enable); //change depth result by new d = Bsloop * origin d + Bconstant.
    raster_state_c_info.depthBiasConstantFactor = io_identity.m_params.m_rasterization_info.m_depth_bias_constant_factor;
    raster_state_c_info.depthBiasSlopeFactor = io_identity.m_params.m_rasterization_info.m_depth_bias_slope_factor;
    raster_state_c_info.depthBiasClamp = io_identity.m_params.m_rasterization_info.m_depth_bias_clamp;
    raster_state_c_info.lineWidth = io_identity.m_params.m_rasterization_info.m_line_width;

    VkPipelineMultisampleStateCreateInfo multisample_state_c_info = {};
    multisample_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_c_info.pNext = nullptr;
    multisample_state_c_info.flags = 0;
    multisample_state_c_info.rasterizationSamples = SampleCount_Vulkan::Convert(io_identity.m_params.m_sample_shading_info.m_sample_count);
    multisample_state_c_info.sampleShadingEnable = ConvertBoolean(io_identity.m_params.m_sample_shading_info.m_sample_shading_enable);
    multisample_state_c_info.minSampleShading = io_identity.m_params.m_sample_shading_info.m_min_sample_shading;
    multisample_state_c_info.pSampleMask = io_identity.m_params.m_sample_shading_info.m_sample_mask.data();
    multisample_state_c_info.alphaToCoverageEnable = ConvertBoolean(io_identity.m_params.m_sample_shading_info.m_alpha_to_coverage_enable);
    multisample_state_c_info.alphaToOneEnable = ConvertBoolean(io_identity.m_params.m_sample_shading_info.m_alpha_to_one_enable);

    VkPipelineDepthStencilStateCreateInfo depth_stencil_c_info = {};
    depth_stencil_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_c_info.pNext = nullptr;
    depth_stencil_c_info.flags = 0;
    depth_stencil_c_info.depthTestEnable = ConvertBoolean(io_identity.m_params.m_depth_stencil_info.m_depth_test_enable);
    depth_stencil_c_info.depthWriteEnable = ConvertBoolean(io_identity.m_params.m_depth_stencil_info.m_depth_write_enable);
    depth_stencil_c_info.depthCompareOp = CompareOp_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_comp_op);
    depth_stencil_c_info.minDepthBounds = io_identity.m_params.m_depth_stencil_info.m_min_depth_bounds;
    depth_stencil_c_info.maxDepthBounds = io_identity.m_params.m_depth_stencil_info.m_max_depth_bounds;
    depth_stencil_c_info.stencilTestEnable = ConvertBoolean(io_identity.m_params.m_depth_stencil_info.m_stencil_test_enable);

    depth_stencil_c_info.front.compareMask = io_identity.m_params.m_depth_stencil_info.m_front.m_compare_mask;
    depth_stencil_c_info.front.compareOp = CompareOp_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_front.m_compare);
    depth_stencil_c_info.front.depthFailOp = StencilOperator_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_front.m_depth_fail_op);
    depth_stencil_c_info.front.failOp = StencilOperator_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_front.m_fail_op);
    depth_stencil_c_info.front.passOp = StencilOperator_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_front.m_pass_op);
    depth_stencil_c_info.front.reference = io_identity.m_params.m_depth_stencil_info.m_front.m_reference;
    depth_stencil_c_info.front.writeMask = io_identity.m_params.m_depth_stencil_info.m_front.m_write_mask;
    
    depth_stencil_c_info.back.compareMask = io_identity.m_params.m_depth_stencil_info.m_back.m_compare_mask;
    depth_stencil_c_info.back.compareOp = CompareOp_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_back.m_compare);
    depth_stencil_c_info.back.depthFailOp = StencilOperator_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_back.m_depth_fail_op);
    depth_stencil_c_info.back.failOp = StencilOperator_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_back.m_fail_op);
    depth_stencil_c_info.back.passOp = StencilOperator_Vulkan::Convert(io_identity.m_params.m_depth_stencil_info.m_back.m_pass_op);
    depth_stencil_c_info.back.reference = io_identity.m_params.m_depth_stencil_info.m_back.m_reference;
    depth_stencil_c_info.back.writeMask = io_identity.m_params.m_depth_stencil_info.m_back.m_write_mask;

    std::vector<VkPipelineColorBlendAttachmentState> color_blend_states;
    color_blend_states.resize(io_identity.m_params.m_attachment_blend_state.m_blend_infos.size());
    for (uint32_t attr_id = 0; attr_id < color_blend_states.size(); ++attr_id) {
        color_blend_states[attr_id] = {};
        color_blend_states[attr_id].blendEnable = ConvertBoolean(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_blend_enable);
        color_blend_states[attr_id].alphaBlendOp = BlendOperator_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_alpha_op);
        color_blend_states[attr_id].srcAlphaBlendFactor = BlendFactor_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_src_alpha_factor);
        color_blend_states[attr_id].dstAlphaBlendFactor = BlendFactor_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_dst_alpha_factor);
        color_blend_states[attr_id].colorBlendOp = BlendOperator_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_color_op);
        color_blend_states[attr_id].srcColorBlendFactor = BlendFactor_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_src_color_factor);
        color_blend_states[attr_id].dstColorBlendFactor = BlendFactor_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_dst_color_factor);
        if (io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_color_mask[0] == true) {
            color_blend_states[attr_id].colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
        }
        if (io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_color_mask[1] == true) {
            color_blend_states[attr_id].colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
        }
        if (io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_color_mask[2] == true) {
            color_blend_states[attr_id].colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
        }
        if (io_identity.m_params.m_attachment_blend_state.m_blend_infos[attr_id].m_color_mask[3] == true) {
            color_blend_states[attr_id].colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
        }
    }

    VkPipelineColorBlendStateCreateInfo color_blending_c_info = {};
    color_blending_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending_c_info.logicOpEnable = ConvertBoolean(io_identity.m_params.m_attachment_blend_state.m_logic_op_enable);
    color_blending_c_info.logicOp = LogicOperator_Vulkan::Convert(io_identity.m_params.m_attachment_blend_state.m_logic_op);
    color_blending_c_info.attachmentCount = static_cast<uint32_t>(color_blend_states.size());
    color_blending_c_info.pAttachments = color_blend_states.data(); //correspond with target subpass of this pipeline.
    color_blending_c_info.blendConstants[0] = 0.0f;
    color_blending_c_info.blendConstants[1] = 0.0f;
    color_blending_c_info.blendConstants[2] = 0.0f;
    color_blending_c_info.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamic_states;
    for (DynamicStateEnum &ds : io_identity.m_params.m_dynamic_states) {
        dynamic_states.push_back(DynamicState_Vulkan::Convert(ds));
    }

    VkPipelineDynamicStateCreateInfo dyn_state_c_info = {};
    dyn_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dyn_state_c_info.pNext = nullptr;
    dyn_state_c_info.flags = 0;
    dyn_state_c_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dyn_state_c_info.pDynamicStates = dynamic_states.data();

    //5. create pipeline layout.
    VkPipelineLayoutCreateInfo pipeline_layout_c_info = {};
    pipeline_layout_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_c_info.pNext = nullptr;
    pipeline_layout_c_info.flags = 0;
    pipeline_layout_c_info.setLayoutCount = static_cast<uint32_t>(vk_ds_layouts.size());
    pipeline_layout_c_info.pSetLayouts = vk_ds_layouts.data();
    pipeline_layout_c_info.pushConstantRangeCount = 0;
    pipeline_layout_c_info.pPushConstantRanges = nullptr;

    if (CreateVKPipelineLayout(pipeline_layout_handle, pipeline_layout_c_info) != VK_SUCCESS) {
        SDLOGE("create pipeline layout failure.");
        throw std::runtime_error("create pipeline layout failure.");
    }

    //6. Create pipeline.
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
    graphics_pipeline_c_info.layout = pipeline_layout_handle;
    graphics_pipeline_c_info.basePipelineHandle = VK_NULL_HANDLE;
    graphics_pipeline_c_info.basePipelineIndex = -1;
    graphics_pipeline_c_info.renderPass = rp_handle;
    graphics_pipeline_c_info.subpass = io_identity.m_subpass_id;

    VkResult result = CreateVKPipeline(pipeline_handle, graphics_pipeline_c_info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create pipeline failure")
        return;
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::BindGraphicsPipeline(const GraphicsPipelineIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, const std::vector<DescriptorSetWeakReferenceObject> &i_dss)
{
    const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    VkPipeline pipe_handle = reinterpret_cast<VkPipeline>(i_identity.m_handle);
    VkPipelineLayout pipe_layout_handle = reinterpret_cast<VkPipelineLayout>(i_identity.m_pipeline_layout_handle);
    VkPipelineBindPoint pipe_bp = PipelineBindPoint_Vulkan::Convert(i_identity.m_params.m_pipe_bind_point);
    std::vector<VkDescriptorSet> ds_handles;
    for (const DescriptorSetWeakReferenceObject &ds : i_dss) {
        const DescriptorSetIdentity &ds_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(ds);
        ds_handles.push_back(reinterpret_cast<VkDescriptorSet>(ds_identity.m_handle));
    }
    BindVkPipeline(cb_handle, pipe_handle, pipe_bp);
    BindVkDescriptorSets(cb_handle, pipe_bp, pipe_layout_handle, ds_handles);
}

void VulkanManager::DestroyGraphicsPipeline(GraphicsPipelineIdentity &io_identity)
{
    VkPipeline &pipeline_handle = reinterpret_cast<VkPipeline&>(io_identity.m_handle);
    VkPipelineLayout &pipeline_layout_handle = reinterpret_cast<VkPipelineLayout&>(io_identity.m_pipeline_layout_handle);

    DestroyVKPipelineLayout(pipeline_layout_handle);
    DestroyVKPipeline(pipeline_handle);

    io_identity.SetInvalid();
    io_identity = GraphicsPipelineIdentity();
}

______________SD_END_GRAPHICS_NAMESPACE______________