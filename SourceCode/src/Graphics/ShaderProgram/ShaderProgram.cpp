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
    std::vector<DescriptorSetLayoutStrongReferenceObject> material_dsl_wrefs;
    std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsl_wrefs;
    std::vector<DescriptorSetLayoutWeakReferenceObject> dsl_wrefs;
    std::vector<GraphicsPipelineStrongReferenceObject> pipe_srefs;
    if (i_sp_proj_str.empty() == false) {
        nlohmann::json sp_root = nlohmann::json::parse(i_sp_proj_str);
        nlohmann::json &sp = sp_root.at("ShaderProgram");
        //1. parse descriptor set layouts.
        nlohmann::json &dsls = sp.at("DescriptorSetLayouts");
        for (uint32_t dsl_id = 0; dsl_id < dsls.size(); ++dsl_id) {
            nlohmann::json &dsl = dsls.at(dsl_id);
            ObjectName dsl_name = dsl.at("Name").get<ObjectName>();
            bool commonDSL = dsl.at("CommonDSL").get<bool>();
            if (commonDSL == true) {
                DescriptorSetLayoutWeakReferenceObject dsl_wref =
                    GraphicsManager::GetRef().GetBasicDescriptorSetLayout(dsl_name);
                if (dsl_wref.IsNull() == false) {
                    common_dsl_wrefs.push_back(dsl_wref);
                    dsl_wrefs.push_back(dsl_wref);
                }
                else {
                    SDLOGE("DSL(%s) isn't common dsl. Load shader(%s) failure."
                        , dsl_name.c_str()
                        , m_object_name.c_str());
                    return false;
                }
            }
            else {
                if (dsl_name.compare("Material") == 0) {
                    std::vector<UniformVariableDescriptorStrongReferenceObject> final_material_basic_uvds;
                    nlohmann::json &uvds = dsl.at("UniformDescriptors");
                    for (uint32_t uvd_id = 0; uvd_id < uvds.size(); ++uvd_id) {
                        nlohmann::json &uvd = uvds.at(uvd_id);
                        ObjectName uvd_name = uvd.at("Name").get<ObjectName>();
                        ObjectName uvd_type = uvd.at("Type").get<ObjectName>();
                        uint32_t uvd_number = uvd.at("Number").get<uint32_t>();
                        UniformVariableDescriptorStrongReferenceObject uvd_sref =
                            GraphicsManager::GetRef().GetDefaultMaterialUniformVariableDescriptor(uvd_name);
                        if (uvd_sref.IsNull() == false) {
                            final_material_basic_uvds.push_back(uvd_sref);
                        }
                        else {
                            if (uvd_type.compare("buffer") == 0) {
                                UniformBufferDescriptorStrongReferenceObject ubd_sref = new UniformBufferDescriptor(uvd_name, uvd_id, uvd_number);
                                nlohmann::json members = uvd.at("Member");
                                for (uint32_t mem_id = 0; mem_id < members.size(); ++mem_id) {
                                    nlohmann::json member = members.at(mem_id);
                                    ObjectName member_name = member.at("Name").get<ObjectName>();
                                    ObjectName member_type = member.at("Type").get<ObjectName>();
                                    uint32_t member_number = member.at("Number").get<uint32_t>();
                                    if (member_type.compare("int") == 0) {
                                        SD_SREF(ubd_sref).AddVariable(member_name, UniformBufferVariableType_INT, member_number);
                                    }
                                    else if (member_type.compare("uint") == 0) {
                                        SD_SREF(ubd_sref).AddVariable(member_name, UniformBufferVariableType_UINT, member_number);
                                    }
                                    else if (member_type.compare("float") == 0) {
                                        SD_SREF(ubd_sref).AddVariable(member_name, UniformBufferVariableType_FLOAT, member_number);
                                    }
                                    else if (member_type.compare("vec4") == 0) {
                                        SD_SREF(ubd_sref).AddVariable(member_name, UniformBufferVariableType_VECTOR3F, member_number);
                                    }
                                    else if (member_type.compare("color") == 0) {
                                        SD_SREF(ubd_sref).AddVariable(member_name, UniformBufferVariableType_COLOR4F, member_number);
                                    }
                                    else if (member_type.compare("mat4") == 0) {
                                        SD_SREF(ubd_sref).AddVariable(member_name, UniformBufferVariableType_MATRIX4X4F, member_number);
                                    }
                                    else {
                                        SDLOGE("Member(%s) isn't wrong type(%s). Load shader(%s) failure."
                                            , member_name.c_str()
                                            , member_type.c_str()
                                            , m_object_name.c_str());
                                        return false;
                                    }
                                    SD_SREF(ubd_sref).AddVariableDone();
                                }
                                final_material_basic_uvds.push_back(ubd_sref.StaticCastTo<UniformVariableDescriptor>());
                            }
                            else if (uvd_type.compare("images") == 0) {
                                uvd_sref = new UniformImagesDescriptor(uvd_name, uvd_id, uvd_number);
                                final_material_basic_uvds.push_back(uvd_sref.StaticCastTo<UniformVariableDescriptor>());
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
                    DescriptorSetLayoutStrongReferenceObject material_dsl_sref = new DescriptorSetLayout("Material");
                    SD_SREF(material_dsl_sref).AddUniformVariableDescriptors(final_material_basic_uvds);
                    SD_SREF(material_dsl_sref).Initialize();
                    material_dsl_wrefs.push_back(material_dsl_sref);
                    dsl_wrefs.push_back(material_dsl_sref);
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
            rp_info.m_rp_wref = GraphicsManager::GetRef().GetRenderPass(rp_name);
            nlohmann::json &spis = rpi.at("SubpassInfos");
            const std::vector<SubpassDescription> &sp_descs = SD_WREF(rp_info.m_rp_wref).GetSubpassDescriptions();
            if (spis.size() != sp_descs.size()) {
                SDLOGE("SubPasses size(%d) in SP[%s] isn't equal with RenderPass[%s]'s(%d)",
                    spis.size(), m_object_name.c_str(),
                    sp_descs.size(), SD_WREF(rp_info.m_rp_wref).GetObjectName().c_str());
                return false;
            }

            for (uint32_t sp_id = 0; sp_id < spis.size(); ++sp_id) {
                uint32_t target_sp_ca_size = static_cast<uint32_t>(sp_descs[sp_id].m_color_attachment_refs.size());
                RenderSubPassPipelineInfo sp_pipe_info;
                nlohmann::json &spi = spis.at(sp_id);
                ObjectName sp_name = spi.at("Name").get<ObjectName>();
                nlohmann::json &pipes = spi.at("Pipelines");
                for (uint32_t pipe_id = 0; pipe_id < pipes.size(); ++pipe_id) {
                    nlohmann::json &pipe = pipes.at(pipe_id);
                    ObjectName pipe_name = pipe.at("Name").get<ObjectName>();
                    GraphicsPipelineStrongReferenceObject pipeline_sref = new GraphicsPipeline(pipe_name);
                    ShaderModules shader_modules;
                    FilePathString vert_fn = pipe.at("VertShader").get<ObjectName>();
                    if (vert_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_VERTEX] = new ShaderModule(pipe_name + "Vert");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_VERTEX]).LoadBinaryShader(
                            ShaderKind_VERTEX,
                            FileResourceRequester::GetRef().CatenateFilePath(vert_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString tess_ctrl_fn = pipe.at("TessCtrlShader").get<ObjectName>();
                    if (tess_ctrl_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_TESS_CTRL] = new ShaderModule(pipe_name + "TessCtrl");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_TESS_CTRL]).LoadBinaryShader(
                            ShaderKind_TESS_CTRL,
                            FileResourceRequester::GetRef().CatenateFilePath(tess_ctrl_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString tess_est_fn = pipe.at("TessEstShader").get<ObjectName>();
                    if (tess_est_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_TESS_EVA] = new ShaderModule(pipe_name + "TessEva");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_TESS_EVA]).LoadBinaryShader(
                            ShaderKind_TESS_EVA,
                            FileResourceRequester::GetRef().CatenateFilePath(tess_est_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString geo_fn = pipe.at("GeoShader").get<ObjectName>();
                    if (geo_fn.empty() == false) {
                        shader_modules.m_shaders[ShdaerKind_GEOMETRY] = new ShaderModule(pipe_name + "Geo");
                        SD_SREF(shader_modules.m_shaders[ShdaerKind_GEOMETRY]).LoadBinaryShader(
                            ShdaerKind_GEOMETRY,
                            FileResourceRequester::GetRef().CatenateFilePath(geo_fn, m_proj_dir_fp),
                            "main"
                        );
                    }

                    FilePathString frag_fn = pipe.at("FragShader").get<ObjectName>();
                    if (frag_fn.empty() == false) {
                        shader_modules.m_shaders[ShaderKind_FRAGMENT] = new ShaderModule(pipe_name + "Frag");
                        SD_SREF(shader_modules.m_shaders[ShaderKind_FRAGMENT]).LoadBinaryShader(
                            ShaderKind_FRAGMENT,
                            FileResourceRequester::GetRef().CatenateFilePath(frag_fn, m_proj_dir_fp),
                            "main"
                        );
                    }
                    CommonDescriptorSetLayouts necessary_dsls;
                    nlohmann::json &used_dsls = pipe.at("UsedDescriptorSetIDs");
                    for (uint32_t udsl_count = 0; udsl_count < used_dsls.size(); ++udsl_count) {
                        nlohmann::json &used_dsl = used_dsls.at(udsl_count);
                        uint32_t udsl_id = used_dsl.get<uint32_t>();
                        if (udsl_id < dsl_wrefs.size()) {
                            necessary_dsls.push_back(dsl_wrefs[udsl_id]);
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

                    nlohmann::json &gpp_root = pipe.at("GraphicsParam");
                    if (gpp_root.is_null() == false) {
                        //
                        SD_GET_JSON_ATTRIBUTE(gpp_root, "BindPoint", uint32_t, gpp.m_pipe_bind_point, PipelineBindPointEnum);
                        SD_GET_JSON_ATTRIBUTE(gpp_root, "PatchCtrlPoints", uint32_t, gpp.m_patch_ctrl_points, uint32_t);
                        uint32_t tex_coord_dim = 2;
                        SD_GET_JSON_ATTRIBUTE(gpp_root, "TexCoordDim", uint32_t, tex_coord_dim, uint32_t);
                        GraphicsManager::GetRef().GetBasicVertexAttribInfos(gpp.m_va_binding_descs, gpp.m_va_location_descs, tex_coord_dim);
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
                                , SD_WREF(rp_info.m_rp_wref).GetObjectName().c_str());
                            gpp.m_attachment_blend_state.m_blend_infos.resize(target_sp_ca_size);
                        }
                        // DynamicState
                        nlohmann::json &dynamic_states_root = gpp_root.at("DynamicState");
                        for (uint32_t dynamic_sid = 0; dynamic_sid < dynamic_states_root.size(); ++dynamic_sid) {
                            gpp.m_dynamic_states.push_back(dynamic_states_root.at(dynamic_sid));
                        }
                    }
                    else {
                        GraphicsManager::GetRef().GetBasicVertexAttribInfos(gpp.m_va_binding_descs, gpp.m_va_location_descs, 2);
                        gpp.m_primitive_info.m_primitive = Primitive_TRIANGLE;
                        gpp.m_attachment_blend_state.m_blend_infos.resize(target_sp_ca_size); //blend default false.
                        gpp.m_dynamic_states.push_back(DynamicState_VIEWPORT);
                        gpp.m_dynamic_states.push_back(DynamicState_SCISSOR);
                    }

                    SD_SREF(pipeline_sref).SetGraphicsPipelineParams(gpp, rp_info.m_rp_wref, necessary_dsls, sp_id);
                    SD_SREF(pipeline_sref).Initialize(shader_modules);

                    RenderStepInfo rs_info;
                    rs_info.m_pipe_sref = pipeline_sref;
                    rs_info.m_dsl_wrefs = necessary_dsls;
                    sp_pipe_info.m_step_infos.push_back(rs_info);
                }
                rp_info.m_sp_pipe_infos.push_back(sp_pipe_info);
            }
            rp_infos.push_back(rp_info);
        }
        //3.
        RegisterShaderProgramStructure(
            rp_infos,
            common_dsl_wrefs,
            material_dsl_wrefs);
        return true;
    }
    else {
        return false;
    }
}

void ShaderProgram::RegisterShaderProgramStructure(
    const RenderPassInfos &i_rp_infos,
    const CommonDescriptorSetLayouts &i_common_dsl_wrefs,
    const DescriptorSetLayouts &i_material_dsl_srefs)
{
    if (i_rp_infos.size() == 0) {
        SDLOGE("No render pass group.");
        return;
    }

    for (uint32_t rpg_count = 0; rpg_count < i_rp_infos.size(); ++rpg_count) {
        if (i_rp_infos[rpg_count].m_rp_wref.IsNull() == true) {
            SDLOGE("Render pass in group[%d] is nullptr.", rpg_count);
            return;
        }
    }

    //1. set prepared information.
    m_rp_infos = i_rp_infos;
    m_common_dsl_wrefs = i_common_dsl_wrefs;
    m_material_dsl_srefs = i_material_dsl_srefs;

    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        m_pipe_amount += m_rp_infos[rp_id].GetPipelineAmount();
    }

    //2.calculate descriptor set count.
    m_descriptor_counts[UniformBindingType_UNIFORM_BUFFER] = 0;
    m_descriptor_counts[UniformBindingType_COMBINED_IMAGE_SAMPLER] = 0;

    for (uint32_t dsl_count = 0; dsl_count < m_material_dsl_srefs.size(); ++dsl_count) {
        uint32_t dsl_dcounts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
        m_material_dsl_srefs[dsl_count].GetRef().GetUniformDescriptorCounts(dsl_dcounts);
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

uint32_t ShaderProgram::GetStepAmount(const RenderPassWeakReferenceObject &i_rp_wref, uint32_t i_sp_id) const
{
    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        if (m_rp_infos[rp_id].m_rp_wref == i_rp_wref) {
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
    std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uv_wrefs,
    std::vector<DescriptorSetWeakReferenceObject> &io_desc_set_wrefs,
    DescriptorPoolWeakReferenceObject &io_pool_wref)
{
    if (m_material_dsl_srefs.size() == 0) {
        SDLOGE("material dsl is 0 of this sp[%s]", m_object_name.c_str());
        return;
    }
    //1. allocate descriptor set for each pipeline.
    io_desc_set_wrefs.resize(m_material_dsl_srefs.size());
    for (uint32_t dsl_count = 0; dsl_count < m_material_dsl_srefs.size(); ++dsl_count) {
        io_desc_set_wrefs[dsl_count] = io_pool_wref.GetRef().AllocateDescriptorSet(m_material_dsl_srefs[dsl_count]);
        io_desc_set_wrefs[dsl_count].GetConstRef().GetAllocatedUniformVariables(io_uv_wrefs);
    }
}

void ShaderProgram::UseProgramWithTargetDescriptorSet(
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const RenderPassWeakReferenceObject &i_rp_wref,
    uint32_t i_sp_idx,
    uint32_t i_step_idx,
    const std::vector<DescriptorSetWeakReferenceObject> &i_desc_set_wrefs)
{
    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        if (m_rp_infos[rp_id].m_rp_wref == i_rp_wref) {
            if (i_sp_idx < m_rp_infos[rp_id].m_sp_pipe_infos.size()) {
                if (i_step_idx < m_rp_infos[rp_id].m_sp_pipe_infos[i_sp_idx].m_step_infos.size()) {
                    SD_SREF(m_rp_infos[rp_id].m_sp_pipe_infos[i_sp_idx].m_step_infos[i_step_idx].m_pipe_sref).UseAndBindDescriptorSets(i_cb_wref, i_desc_set_wrefs);
                }
            }
            return;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________