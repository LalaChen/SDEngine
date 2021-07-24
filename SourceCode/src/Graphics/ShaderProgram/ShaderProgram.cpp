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

#include <nlohmann/json.hpp>

#include "SDEngineCommonFunction.h"
#include "FileResourceRequester.h"
#include "LogManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"

using SDE::Basic::FileData;
using SDE::Basic::FileResourceRequester;

#define SD_GET_JSON_ATTRIBUTE(nodeRoot, varName, varType, dst, dstType) \
if (nodeRoot.at(varName).is_null() == false) { \
    dst = static_cast<dstType>(nodeRoot.at(varName).get<varType>()); \
} \

_____________SD_START_GRAPHICS_NAMESPACE_____________

const std::string sOrderNames[RenderOrder_MAX_DEFINE_VALUE] = {
    "Opaque",
    "AlphaBlend",
    "VolumeRendering"
};

const uint32_t sOrderNumbers[RenderOrder_MAX_DEFINE_VALUE] = {
    0,
    1000,
    2000
};

RenderOrder::RenderOrder(const std::string &i_desc) {
    for (uint32_t count = 0; count < RenderOrder_MAX_DEFINE_VALUE; ++count) {
        if (sOrderNames[count].compare(i_desc) == 0) {
            m_order = sOrderNumbers[count];
            break;
        }
    }
}

ShaderProgram::ShaderProgram(const ObjectName &i_name)
: Object(i_name)
, m_descriptor_counts{0}
, m_pipe_amount(0)
, m_registered(false)
{
}

ShaderProgram::~ShaderProgram()
{
}

bool ShaderProgram::LoadFromFile(const FilePathString &i_fp)
{
    FileData file_data;
    bool result = FileResourceRequester::GetRef().AskFile(i_fp, file_data);
    if (result == true) {

        SDE::Basic::SpliteTargetFileToPathAndName(i_fp, m_proj_dir_fp, m_proj_fn);
        return LoadFromSource(file_data.GetDataString());
    }
    else {
        SDLOGE("fp[%s] isn't exist. Please check!!!", i_fp.c_str());
    }
    return true;
}

bool ShaderProgram::LoadFromSource(const std::string &i_sp_proj_str)
{
    std::vector<DescriptorSetLayoutStrongReferenceObject> material_dsls;
    std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsls;
    std::vector<DescriptorSetLayoutWeakReferenceObject> dsls;
    std::vector<GraphicsPipelineStrongReferenceObject> pipes;
    if (i_sp_proj_str.empty() == false) {
        nlohmann::json sp_root = nlohmann::json::parse(i_sp_proj_str);
        nlohmann::json &sp = sp_root.at("ShaderProgram");
        
        nlohmann::json &render_order_root = sp.at("RenderOrder");
        std::string render_order_desc = render_order_root.get<std::string>();
        m_render_order = RenderOrder(render_order_desc);
        //1. parse descriptor set layouts.
        nlohmann::json &dsls_root = sp.at("DescriptorSetLayouts");
        for (uint32_t dsl_id = 0; dsl_id < dsls_root.size(); ++dsl_id) {
            nlohmann::json &dsl_root = dsls_root.at(dsl_id);
            ObjectName dsl_name = dsl_root.at("Name").get<ObjectName>();
            bool commonDSL = dsl_root.at("CommonDSL").get<bool>();
            if (commonDSL == true) {
                DescriptorSetLayoutWeakReferenceObject dsl =
                    GraphicsManager::GetRef().GetBasicDescriptorSetLayout(dsl_name);
                if (dsl.IsNull() == false) {
                    common_dsls.push_back(dsl);
                    dsls.push_back(dsl);
                }
                else {
                    SDLOGE("DSL(%s) isn't common dsl. Load shader(%s) failure."
                        , dsl_name.c_str()
                        , m_object_name.c_str());
                    return false;
                }
            }
            else {
                if (dsl_name.compare(sUniformDescriptorSetLayout_Material) == 0) {
                    std::vector<UniformVariableDescriptorStrongReferenceObject> final_material_basic_uvds;
                    nlohmann::json &uvds = dsl_root.at("UniformDescriptors");
                    for (uint32_t uvd_id = 0; uvd_id < uvds.size(); ++uvd_id) {
                        nlohmann::json &uvd_root = uvds.at(uvd_id);
                        ObjectName uvd_name = uvd_root.at("Name").get<ObjectName>();
                        ObjectName uvd_type = uvd_root.at("Type").get<ObjectName>();
                        uint32_t uvd_number = uvd_root.at("Number").get<uint32_t>();
                        UniformVariableDescriptorStrongReferenceObject uvd =
                            GraphicsManager::GetRef().GetDefaultMaterialUniformVariableDescriptor(uvd_name);
                        if (uvd.IsNull() == false) {
                            final_material_basic_uvds.push_back(uvd);
                        }
                        else {
                            if (uvd_type.compare("buffer") == 0) {
                                UniformBufferDescriptorStrongReferenceObject ubd = new UniformBufferDescriptor(uvd_name, uvd_id, uvd_number);
                                nlohmann::json members = uvd_root.at("Member");
                                for (uint32_t mem_id = 0; mem_id < members.size(); ++mem_id) {
                                    nlohmann::json member_root = members.at(mem_id);
                                    ObjectName member_name = member_root.at("Name").get<ObjectName>();
                                    ObjectName member_type = member_root.at("Type").get<ObjectName>();
                                    uint32_t member_number = member_root.at("Number").get<uint32_t>();
                                    if (member_type.compare("int") == 0) {
                                        SD_SREF(ubd).AddVariable(member_name, UniformBufferVariableType_INT, member_number);
                                    }
                                    else if (member_type.compare("uint") == 0) {
                                        SD_SREF(ubd).AddVariable(member_name, UniformBufferVariableType_UINT, member_number);
                                    }
                                    else if (member_type.compare("float") == 0) {
                                        SD_SREF(ubd).AddVariable(member_name, UniformBufferVariableType_FLOAT, member_number);
                                    }
                                    else if (member_type.compare("vec4") == 0) {
                                        SD_SREF(ubd).AddVariable(member_name, UniformBufferVariableType_VECTOR3F, member_number);
                                    }
                                    else if (member_type.compare("color") == 0) {
                                        SD_SREF(ubd).AddVariable(member_name, UniformBufferVariableType_COLOR4F, member_number);
                                    }
                                    else if (member_type.compare("mat4") == 0) {
                                        SD_SREF(ubd).AddVariable(member_name, UniformBufferVariableType_MATRIX4X4F, member_number);
                                    }
                                    else {
                                        SDLOGE("Member(%s) isn't wrong type(%s). Load shader(%s) failure."
                                            , member_name.c_str()
                                            , member_type.c_str()
                                            , m_object_name.c_str());
                                        return false;
                                    }
                                    SD_SREF(ubd).AddVariableDone();
                                }
                                final_material_basic_uvds.push_back(ubd.StaticCastTo<UniformVariableDescriptor>());
                            }
                            else if (uvd_type.compare("images") == 0) {
                                uvd = new UniformImagesDescriptor(uvd_name, uvd_id, uvd_number);
                                final_material_basic_uvds.push_back(uvd.StaticCastTo<UniformVariableDescriptor>());
                            }
                            else {
                                SDLOGE("Uniform Variable(%s) isn't wrong type(%s). Load shader(%s) failure."
                                    , uvd_name.c_str()
                                    , uvd_type.c_str()
                                    , m_object_name.c_str());
                                return false;
                            }
                        }
                    }
                    //
                    DescriptorSetLayoutStrongReferenceObject material_dsl = new DescriptorSetLayout("Material");
                    SD_SREF(material_dsl).AddUniformVariableDescriptors(final_material_basic_uvds);
                    SD_SREF(material_dsl).Initialize();
                    material_dsls.push_back(material_dsl);
                    dsls.push_back(material_dsl);
                }
                else {
                    SDLOGE("Exist non-material dsl(%s) in sp(%s) We only support material layer in currently."
                        , dsl_name.c_str()
                        , m_object_name.c_str());
                    return false;
                }
            }
        }
        //2. create render pass infos.
        nlohmann::json &rpis = sp.at("RenderPassInfos");
        RenderPassInfos rp_infos;
        for (uint32_t rp_id = 0; rp_id < rpis.size(); ++rp_id) {
            nlohmann::json &rpi = rpis.at(rp_id);
            RenderPassInfo rp_info;
            ObjectName rp_name = rpi.at("Name").get<ObjectName>();
            rp_info.m_rp = GraphicsManager::GetRef().GetRenderPass(rp_name);
            nlohmann::json &spis = rpi.at("SubpassInfos");
            const std::vector<SubpassDescription> &sp_descs = SD_WREF(rp_info.m_rp).GetSubpassDescriptions();
            if (spis.size() != sp_descs.size()) {
                SDLOGE("SubPasses size(%d) in SP[%s] isn't equal with RenderPass[%s]'s(%d)",
                    spis.size(), m_object_name.c_str(),
                    sp_descs.size(), SD_WREF(rp_info.m_rp).GetObjectName().c_str());
                return false;
            }

            for (uint32_t sp_id = 0; sp_id < spis.size(); ++sp_id) {
                uint32_t target_sp_ca_size = static_cast<uint32_t>(sp_descs[sp_id].m_color_attachment_refs.size());
                RenderSubPassPipelineInfo sp_pipe_info;
                nlohmann::json &spi = spis.at(sp_id);
                ObjectName sp_name = spi.at("Name").get<ObjectName>();
                nlohmann::json &pipes = spi.at("Pipelines");
                for (uint32_t pipe_id = 0; pipe_id < pipes.size(); ++pipe_id) {
                    nlohmann::json &pipe_root = pipes.at(pipe_id);
                    ObjectName pipe_name = pipe_root.at("Name").get<ObjectName>();
                    GraphicsPipelineStrongReferenceObject pipeline = new GraphicsPipeline(pipe_name);
                    ShaderModules shader_modules;
                    FilePathString vert_fn = pipe_root.at("VertShader").get<ObjectName>();
                    if (vert_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_VERTEX] = new ShaderModule(pipe_name + "Vert");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_VERTEX]).LoadBinaryShader(
                            ShaderKind_VERTEX,
                            FileResourceRequester::GetRef().CatenateFilePath(vert_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString tess_ctrl_fn = pipe_root.at("TessCtrlShader").get<ObjectName>();
                    if (tess_ctrl_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_TESS_CTRL] = new ShaderModule(pipe_name + "TessCtrl");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_TESS_CTRL]).LoadBinaryShader(
                            ShaderKind_TESS_CTRL,
                            FileResourceRequester::GetRef().CatenateFilePath(tess_ctrl_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString tess_est_fn = pipe_root.at("TessEstShader").get<ObjectName>();
                    if (tess_est_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_TESS_EVA] = new ShaderModule(pipe_name + "TessEva");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_TESS_EVA]).LoadBinaryShader(
                            ShaderKind_TESS_EVA,
                            FileResourceRequester::GetRef().CatenateFilePath(tess_est_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString geo_fn = pipe_root.at("GeoShader").get<ObjectName>();
                    if (geo_fn.empty() == false) {
                        shader_modules.m_shaders[ShdaerKind_GEOMETRY] = new ShaderModule(pipe_name + "Geo");
                        SD_SREF(shader_modules.m_shaders[ShdaerKind_GEOMETRY]).LoadBinaryShader(
                            ShdaerKind_GEOMETRY,
                            FileResourceRequester::GetRef().CatenateFilePath(geo_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString frag_fn = pipe_root.at("FragShader").get<ObjectName>();
                    if (frag_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_FRAGMENT] = new ShaderModule(pipe_name + "Frag");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_FRAGMENT]).LoadBinaryShader(
                            ShaderKind_FRAGMENT,
                            FileResourceRequester::GetRef().CatenateFilePath(frag_fn, m_proj_dir_fp),
                            "main"
                        );
                    }
                    CommonDescriptorSetLayouts necessary_dsls;
                    nlohmann::json &used_dsls_root = pipe_root.at("UsedDescriptorSetIDs");
                    for (uint32_t udsl_count = 0; udsl_count < used_dsls_root.size(); ++udsl_count) {
                        nlohmann::json &used_dsl_root = used_dsls_root.at(udsl_count);
                        uint32_t udsl_id = used_dsl_root.get<uint32_t>();
                        if (udsl_id < dsls.size()) {
                            necessary_dsls.push_back(dsls[udsl_id]);
                        }
                        else {
                            SDLOGE("Error dsl index(%u) in (sp, step) (%d,%d) of rp[%s]."
                                , udsl_id
                                , sp_id, pipe_id
                                , rp_name.c_str());
                            return false;
                        }
                    }
                    GraphicsPipelineParam gpp;

                    nlohmann::json &gpp_root = pipe_root.at("GraphicsParam");
                    if (gpp_root.is_null() == false) {
                        //
                        SD_GET_JSON_ATTRIBUTE(gpp_root, "BindPoint", uint32_t, gpp.m_pipe_bind_point, PipelineBindPointEnum);
                        SD_GET_JSON_ATTRIBUTE(gpp_root, "PatchCtrlPoints", uint32_t, gpp.m_patch_ctrl_points, uint32_t);
                        VertexLocationKindEnum vl_kind = VertexLocationKind_GENERAL;
                        SD_GET_JSON_ATTRIBUTE(gpp_root, "VertexLocationKind", uint32_t, vl_kind, VertexLocationKindEnum);
                        GraphicsManager::GetRef().GetBasicVertexAttribInfos(gpp.m_va_binding_descs, gpp.m_va_location_descs, vl_kind);
                        // PrimitiveInfo
                        nlohmann::json &primitive_root = gpp_root.at("PrimitiveInfo");
                        if (primitive_root.is_null() == false) {
                            SD_GET_JSON_ATTRIBUTE(primitive_root, "Type", uint32_t, gpp.m_primitive_info.m_primitive, PrimitiveEnum);
                            SD_GET_JSON_ATTRIBUTE(primitive_root, "RestartEnable", bool, gpp.m_primitive_info.m_restart_enable, bool);
                        }
                        else {
                            SDLOG("PrimitiveInfo isn't existed in SP[%s].", pipe_name.c_str());
                        }
                        // RasterziationInfo
                        nlohmann::json &rasterziation_root = gpp_root.at("RasterziationInfo");
                        if (rasterziation_root.is_null() == false) {
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "DiscardEnable", bool, gpp.m_rasterization_info.m_discard_enable, bool);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "PolygonMode", uint32_t, gpp.m_rasterization_info.m_polygon_mode, PolygonModeEnum);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "FrontFace", uint32_t, gpp.m_rasterization_info.m_front_face, FrontFaceModeEnum);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "FaceCulling", uint32_t, gpp.m_rasterization_info.m_face_culling, FaceCullingEnum);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "DepthBias", bool, gpp.m_rasterization_info.m_depth_bias_enable, bool);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "DepthBiasConstantFactor", float, gpp.m_rasterization_info.m_depth_bias_constant_factor, bool);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "DepthBiasSlopeFactor", float, gpp.m_rasterization_info.m_depth_bias_slope_factor, bool);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "DepthBiasClamp", float, gpp.m_rasterization_info.m_depth_bias_clamp, float);
                            SD_GET_JSON_ATTRIBUTE(rasterziation_root, "LineWidth", float, gpp.m_rasterization_info.m_line_width, float);
                        }
                        else {
                            SDLOG("RasterziationInfo isn't existed in SP[%s].", pipe_name.c_str());
                        }
                        // DepthStencilInfo
                        nlohmann::json &depth_stencil_root = gpp_root.at("DepthStencilInfo");
                        if (depth_stencil_root.is_null() == false) {
                            SD_GET_JSON_ATTRIBUTE(depth_stencil_root, "DepthTest", bool, gpp.m_depth_stencil_info.m_depth_test_enable, bool);
                            SD_GET_JSON_ATTRIBUTE(depth_stencil_root, "DepthWrite", bool, gpp.m_depth_stencil_info.m_depth_write_enable, bool);
                            SD_GET_JSON_ATTRIBUTE(depth_stencil_root, "CompareOp", uint32_t, gpp.m_depth_stencil_info.m_comp_op, CompareOperatorEnum);
                            SD_GET_JSON_ATTRIBUTE(depth_stencil_root, "StencilTest", bool, gpp.m_depth_stencil_info.m_stencil_test_enable, bool);
                        }
                        else {
                            SDLOG("DepthStencilInfo isn't existed in SP[%s].", pipe_name.c_str());
                        }
                        // ColorBlendState
                        nlohmann::json &color_bs_root = gpp_root.at("ColorBlendState");
                        if (color_bs_root.is_null() == false) {
                            SD_GET_JSON_ATTRIBUTE(color_bs_root, "BlendConstantR", float, gpp.m_attachment_blend_state.m_blend_constants[0], float);
                            SD_GET_JSON_ATTRIBUTE(color_bs_root, "BlendConstantG", float, gpp.m_attachment_blend_state.m_blend_constants[1], float);
                            SD_GET_JSON_ATTRIBUTE(color_bs_root, "BlendConstantB", float, gpp.m_attachment_blend_state.m_blend_constants[2], float);
                            SD_GET_JSON_ATTRIBUTE(color_bs_root, "BlendConstantA", float, gpp.m_attachment_blend_state.m_blend_constants[3], float);
                            SD_GET_JSON_ATTRIBUTE(color_bs_root, "LogicOutput", bool, gpp.m_attachment_blend_state.m_logic_op_enable, bool);
                            SD_GET_JSON_ATTRIBUTE(color_bs_root, "LogicOp", uint32_t, gpp.m_attachment_blend_state.m_logic_op, LogicOperatorEnum);
                            nlohmann::json &ca_blend_infos_root = color_bs_root.at("ColorBlendAttachmentInfo");
                            for (uint32_t attachment_cid = 0; attachment_cid < ca_blend_infos_root.size() && attachment_cid < target_sp_ca_size; ++attachment_cid) {
                                nlohmann::json &ca_blend_info_root = ca_blend_infos_root.at(attachment_cid);
                                ColorBlendAttachmentInfo cb_info;
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "ColorMaskR", bool, cb_info.m_color_mask[0], bool);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "ColorMaskG", bool, cb_info.m_color_mask[1], bool);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "ColorMaskB", bool, cb_info.m_color_mask[2], bool);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "ColorMaskA", bool, cb_info.m_color_mask[3], bool);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "Blending", bool, cb_info.m_blend_enable, bool);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "BlendingSrcColorFactor", uint32_t, cb_info.m_src_color_factor, BlendFactorEnum);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "BlendingDstColorFactor", uint32_t, cb_info.m_dst_color_factor, BlendFactorEnum);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "BlendingColorOp", uint32_t, cb_info.m_color_op, BlendOperatorEnum);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "BlendingSrcAlphaFactor", uint32_t, cb_info.m_src_alpha_factor, BlendFactorEnum);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "BlendingDstAlphaFactor", uint32_t, cb_info.m_dst_alpha_factor, BlendFactorEnum);
                                SD_GET_JSON_ATTRIBUTE(ca_blend_info_root, "BlendingAlphaOp", uint32_t, cb_info.m_alpha_op, BlendOperatorEnum);
                                gpp.m_attachment_blend_state.m_blend_infos.push_back(cb_info);
                            }
                        }
                        else {
                            SDLOG("ColorBlendState isn't existed in SP[%s]. Allocated for the target sp in RP[%s]."
                                , pipe_name.c_str()
                                , SD_WREF(rp_info.m_rp).GetObjectName().c_str());
                            gpp.m_attachment_blend_state.m_blend_infos.resize(target_sp_ca_size);
                        }
                        // DynamicState
                        nlohmann::json &dynamic_states_root = gpp_root.at("DynamicState");
                        for (uint32_t dynamic_sid = 0; dynamic_sid < dynamic_states_root.size(); ++dynamic_sid) {
                            gpp.m_dynamic_states.push_back(dynamic_states_root.at(dynamic_sid));
                        }
                    }
                    else {
                        GraphicsManager::GetRef().GetBasicVertexAttribInfos(gpp.m_va_binding_descs, gpp.m_va_location_descs, VertexLocationKind_GENERAL);
                        gpp.m_primitive_info.m_primitive = Primitive_TRIANGLE;
                        gpp.m_attachment_blend_state.m_blend_infos.resize(target_sp_ca_size); //blend default false.
                        gpp.m_dynamic_states.push_back(DynamicState_VIEWPORT);
                        gpp.m_dynamic_states.push_back(DynamicState_SCISSOR);
                    }

                    SD_SREF(pipeline).SetGraphicsPipelineParams(gpp, rp_info.m_rp, necessary_dsls, sp_id);
                    SD_SREF(pipeline).Initialize(shader_modules);

                    RenderStepInfo rs_info;
                    rs_info.m_pipe = pipeline;
                    rs_info.m_dsls = necessary_dsls;
                    sp_pipe_info.m_step_infos.push_back(rs_info);
                }
                rp_info.m_sp_pipe_infos.push_back(sp_pipe_info);
            }
            rp_infos.push_back(rp_info);
        }
        //3.
        RegisterShaderProgramStructure(
            rp_infos,
            common_dsls,
            material_dsls);
        return true;
    }
    else {
        return false;
    }
}

void ShaderProgram::RegisterShaderProgramStructure(
    const RenderPassInfos &i_rp_infos,
    const CommonDescriptorSetLayouts &i_common_dsls,
    const DescriptorSetLayouts &i_material_dsls)
{
    if (i_rp_infos.size() == 0) {
        SDLOGE("No render pass group.");
        return;
    }

    for (uint32_t rpg_count = 0; rpg_count < i_rp_infos.size(); ++rpg_count) {
        if (i_rp_infos[rpg_count].m_rp.IsNull() == true) {
            SDLOGE("Render pass in group[%d] is nullptr.", rpg_count);
            return;
        }
    }

    //1. set prepared information.
    m_rp_infos = i_rp_infos;
    m_common_dsls = i_common_dsls;
    m_material_dsls = i_material_dsls;

    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        m_pipe_amount += m_rp_infos[rp_id].GetPipelineAmount();
    }

    //2.calculate descriptor set count.
    m_descriptor_counts[UniformBindingType_UNIFORM_BUFFER] = 0;
    m_descriptor_counts[UniformBindingType_COMBINED_IMAGE_SAMPLER] = 0;

    for (uint32_t dsl_count = 0; dsl_count < m_material_dsls.size(); ++dsl_count) {
        uint32_t dsl_dcounts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
        m_material_dsls[dsl_count].GetRef().GetUniformDescriptorCounts(dsl_dcounts);
        for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
            m_descriptor_counts[count] += dsl_dcounts[count];
        }
    }

    m_registered = true;
}

void ShaderProgram::GetDescriptorCount(uint32_t i_d_counts[UniformBindingType_MAX_DEFINE_VALUE]) const
{
    for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
        i_d_counts[count] = m_descriptor_counts[count];
    }
}

uint32_t ShaderProgram::GetStepAmount(const RenderPassWeakReferenceObject &i_rp, uint32_t i_sp_id) const
{
    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        if (m_rp_infos[rp_id].m_rp == i_rp) {
            if (i_sp_id < m_rp_infos[rp_id].m_sp_pipe_infos.size()) {
                return static_cast<uint32_t>(m_rp_infos[rp_id].m_sp_pipe_infos[i_sp_id].m_step_infos.size());
            }
            else {
                return 0;
            }
        }
    }
    return 0;
}

void ShaderProgram::AllocateEssentialObjects(
    std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uvs,
    std::vector<DescriptorSetWeakReferenceObject> &io_desc_sets,
    DescriptorPoolWeakReferenceObject &io_pool)
{
    if (m_material_dsls.size() == 0) {
        SDLOGE("material dsl is 0 of this sp[%s]", m_object_name.c_str());
        return;
    }
    //1. allocate descriptor set for each pipeline.
    io_desc_sets.resize(m_material_dsls.size());
    for (uint32_t dsl_count = 0; dsl_count < m_material_dsls.size(); ++dsl_count) {
        io_desc_sets[dsl_count] = io_pool.GetRef().AllocateDescriptorSet(m_material_dsls[dsl_count]);
        io_desc_sets[dsl_count].GetConstRef().GetAllocatedUniformVariables(io_uvs);
    }
}

void ShaderProgram::UseProgramWithTargetDescriptorSet(
    const CommandBufferWeakReferenceObject &i_cb,
    const RenderPassWeakReferenceObject &i_rp,
    uint32_t i_sp_idx,
    uint32_t i_step_idx,
    const std::vector<DescriptorSetWeakReferenceObject> &i_desc_sets)
{
    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        if (m_rp_infos[rp_id].m_rp == i_rp) {
            if (i_sp_idx < m_rp_infos[rp_id].m_sp_pipe_infos.size()) {
                if (i_step_idx < m_rp_infos[rp_id].m_sp_pipe_infos[i_sp_idx].m_step_infos.size()) {
                    SD_SREF(m_rp_infos[rp_id].m_sp_pipe_infos[i_sp_idx].m_step_infos[i_step_idx].m_pipe).UseAndBindDescriptorSets(i_cb, i_desc_sets);
                }
            }
            return;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________