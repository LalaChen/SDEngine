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

#include "BasicPipelines.h"
#include "BasicUniforms.h"
#include "LightUniforms.h"
#include "MaterialUniforms.h"
#include "ModelData.h"
#include "LogManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

DescriptorSetLayoutWeakReferenceObject GraphicsManager::GetBasicDescriptorSetLayout(const ObjectName &i_dsl_name) const
{
    DescriptorSetLayoutWeakReferenceObject result;
    std::map<ObjectName, DescriptorSetLayoutStrongReferenceObject>::const_iterator dsl_iter = m_basic_dsl_maps.find(i_dsl_name);
    if (dsl_iter != m_basic_dsl_maps.end()) {
        return (*dsl_iter).second;
    }
    else {
        return DescriptorSetLayoutWeakReferenceObject();
    }
}

void GraphicsManager::GetBasicDescriptorSetLayouts(std::vector<DescriptorSetLayoutWeakReferenceObject> &io_dsl_wrefs)
{
    io_dsl_wrefs.push_back(m_basic_dsl_maps["Camera"]);
    io_dsl_wrefs.push_back(m_basic_dsl_maps["MeshRender"]);
    io_dsl_wrefs.push_back(m_basic_dsl_maps["Light"]);
}

UniformVariableDescriptorStrongReferenceObject GraphicsManager::GetDefaultMaterialUniformVariableDescriptor(const ObjectName &i_uvd_name) const
{
    UniformVariableDescriptorStrongReferenceObject result;
    std::map<ObjectName, UniformVariableDescriptorStrongReferenceObject>::const_iterator uvd_iter = m_material_basic_uvd_maps.find(i_uvd_name);
    if (uvd_iter != m_material_basic_uvd_maps.end()) {
        return (*uvd_iter).second;
    }
    else {
        return UniformVariableDescriptorStrongReferenceObject();
    }
}

ShaderProgramWeakReferenceObject GraphicsManager::GetShaderProgram(const ObjectName &i_sp_name) const
{
    std::map<ObjectName, ShaderProgramStrongReferenceObject>::const_iterator sp_iter = m_shader_program_maps.find(i_sp_name);
    if (sp_iter != m_shader_program_maps.end()) {
        return (*sp_iter).second;
    }
    else {
        return ShaderProgramStrongReferenceObject();
    }
}

void GraphicsManager::RegisterShaderProgram(const ShaderProgramStrongReferenceObject &i_sp_sref)
{
    std::map<ObjectName, ShaderProgramStrongReferenceObject>::const_iterator sp_iter = 
        m_shader_program_maps.find(SD_SREF(i_sp_sref).GetObjectName());
    if (sp_iter == m_shader_program_maps.end()) {
        m_shader_program_maps[SD_SREF(i_sp_sref).GetObjectName()] = i_sp_sref;
    }
    else {
        SDLOGE("SP[%s] is existed.", SD_SREF(i_sp_sref).GetObjectName().c_str());
    }
}

void GraphicsManager::RegisterShaderProgram(const ObjectName &i_sp_name, const FilePathString &i_path)
{
    std::map<ObjectName, ShaderProgramStrongReferenceObject>::const_iterator sp_iter = m_shader_program_maps.find(i_sp_name);
    if (sp_iter == m_shader_program_maps.end()) {
        ShaderProgramStrongReferenceObject new_shader_sref = new ShaderProgram(i_sp_name);
        if (SD_SREF(new_shader_sref).LoadFromFile(i_path) == true) {
            m_shader_program_maps[i_sp_name] = new_shader_sref;
        }
        else {
            SDLOGE("SP[%s] can't be created.", i_sp_name.c_str());
        }
    }
    else {
        SDLOGE("SP[%s] is existed.", i_sp_name.c_str());
    }
}

void GraphicsManager::InitializeBasicDescriptorSetLayout()
{
    //1. new common descriptor set and its uniform variable descriptors.
    UniformBufferDescriptorStrongReferenceObject camera_ubd_sref = new UniformBufferDescriptor("camera", 0);
    SD_SREF(camera_ubd_sref).AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_proj));
    SD_SREF(camera_ubd_sref).AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_view));
    SD_SREF(camera_ubd_sref).AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(CameraUniforms, m_view_eye));
    SD_SREF(camera_ubd_sref).AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject camera_dsl_sref = new DescriptorSetLayout("Camera");
    SD_SREF(camera_dsl_sref).AddUniformVariableDescriptors({ camera_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    SD_SREF(camera_dsl_sref).Initialize();
    m_basic_dsl_maps["Camera"] = camera_dsl_sref;

    //2. For Meshes.
    //Use for MVP matrices at all subpasses at Forward Pass and Defered Pass.
    UniformBufferDescriptorStrongReferenceObject geometry_ubd_sref = new UniformBufferDescriptor("geometry", 0);
    SD_SREF(geometry_ubd_sref).AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_world));
    SD_SREF(geometry_ubd_sref).AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_normal));
    SD_SREF(geometry_ubd_sref).AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject geometry_dsl_sref = new DescriptorSetLayout("MeshRender");
    SD_SREF(geometry_dsl_sref).AddUniformVariableDescriptors({ geometry_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    SD_SREF(geometry_dsl_sref).Initialize();
    m_basic_dsl_maps["MeshRender"] = geometry_dsl_sref;

    //3. For main light about forward rendering.
    UniformBufferDescriptorStrongReferenceObject light_ubd_sref = new UniformBufferDescriptor("light", 0);
    SD_SREF(light_ubd_sref).AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_ambient));
    SD_SREF(light_ubd_sref).AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_diffuse));
    SD_SREF(light_ubd_sref).AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_specular));
    SD_SREF(light_ubd_sref).AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_position));
    SD_SREF(light_ubd_sref).AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_direction));
    SD_SREF(light_ubd_sref).AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_exp));
    SD_SREF(light_ubd_sref).AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_cos_cutoff));
    SD_SREF(light_ubd_sref).AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_constant_attenuation));
    SD_SREF(light_ubd_sref).AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_linear_attenuation));
    SD_SREF(light_ubd_sref).AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_quadratic_attenuation));
    SD_SREF(light_ubd_sref).AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightUniforms, m_kind));
    SD_SREF(light_ubd_sref).AddVariableDone();

    UniformImagesDescriptorStrongReferenceObject cascade_shadow_map_ui_sref = new UniformImagesDescriptor("shadowMaps", 1, 4);

    DescriptorSetLayoutStrongReferenceObject light_dsl_sref = new DescriptorSetLayout("Light");
    SD_SREF(light_dsl_sref).AddUniformVariableDescriptors({
        light_ubd_sref.StaticCastTo<UniformVariableDescriptor>(),
        cascade_shadow_map_ui_sref.StaticCastTo<UniformVariableDescriptor>()});
    SD_SREF(light_dsl_sref).Initialize();
    m_basic_dsl_maps["Light"] = light_dsl_sref;
}

void GraphicsManager::InitializeBasicMaterialUniformDescriptors()
{
    //4. For Material.
    UniformBufferDescriptorStrongReferenceObject mat_ubd_sref = new UniformBufferDescriptor("material", 0);
    SD_SREF(mat_ubd_sref).AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_ambient));
    SD_SREF(mat_ubd_sref).AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_diffuse));
    SD_SREF(mat_ubd_sref).AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_specular));
    SD_SREF(mat_ubd_sref).AddVariable("emission", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_emission));
    SD_SREF(mat_ubd_sref).AddVariable("shininess", UniformBufferVariableType_FLOAT, offsetof(MaterialUniforms, m_shininess));
    SD_SREF(mat_ubd_sref).AddVariableDone();

    UniformImagesDescriptorStrongReferenceObject mt_imgd_sref = new UniformImagesDescriptor("mainTexture", 1, 1);
    m_material_basic_uvd_maps["material"] = mat_ubd_sref.StaticCastTo<UniformVariableDescriptor>();
    m_material_basic_uvd_maps["mainTexture"] = mt_imgd_sref.StaticCastTo<UniformVariableDescriptor>();
}

void GraphicsManager::InitializeBasicShaderPrograms()
{
    ShaderProgramStrongReferenceObject shader_program;
    {//BasicShading
        shader_program = new ShaderProgram("BasicShading");
        //1. Create GraphicsPipelines.
        //1.1 create shader module.
        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("PhongShaderVert");
        vert_shader_sref.GetRef().LoadBinaryShader(
            ShaderKind_VERTEX, std::vector<UByte>(
                gVulkan_BasicShader_SP0_vert_spv,
                gVulkan_BasicShader_SP0_vert_spv + (sizeof(gVulkan_BasicShader_SP0_vert_spv) / sizeof(UByte))),
            "main");
        ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("PhongShaderFrag");
        frag_shader_sref.GetRef().LoadBinaryShader(
            ShaderKind_FRAGMENT, std::vector<UByte>(
                gVulkan_BasicShader_SP0_frag_spv,
                gVulkan_BasicShader_SP0_frag_spv + (sizeof(gVulkan_BasicShader_SP0_frag_spv) / sizeof(UByte))),
            "main");
        shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader_sref;
        shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader_sref;

        //1.2 write pipeline parames.
        GraphicsPipelineParam params;
        params.m_primitive_info.m_primitive = Primitive_TRIANGLE;
        params.m_depth_stencil_info.m_depth_test_enable = true;
        params.m_rasterization_info.m_face_culling = FaceCulling_BACK_FACE;
        params.m_rasterization_info.m_front_face = FrontFaceMode_COUNTER_CLOCKWISE;
        params.m_attachment_blend_state.m_blend_infos.resize(1); //blend default false.
        params.m_dynamic_states.push_back(DynamicState_VIEWPORT);
        params.m_dynamic_states.push_back(DynamicState_SCISSOR);
        GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, 2);

        //1.3 Create general DescritporSetLayouts.
        std::vector<UniformVariableDescriptorStrongReferenceObject> final_material_basic_uvds;
        final_material_basic_uvds.push_back(GetDefaultMaterialUniformVariableDescriptor("material"));
        final_material_basic_uvds.push_back(GetDefaultMaterialUniformVariableDescriptor("mainTexture"));

        DescriptorSetLayoutStrongReferenceObject material_dsl_sref = new DescriptorSetLayout("Material");
        SD_SREF(material_dsl_sref).AddUniformVariableDescriptors(final_material_basic_uvds);
        SD_SREF(material_dsl_sref).Initialize();

        //1.4 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsl_wrefs;
        dsl_wrefs.push_back(m_basic_dsl_maps["Camera"]);
        dsl_wrefs.push_back(m_basic_dsl_maps["MeshRender"]);
        dsl_wrefs.push_back(m_basic_dsl_maps["Light"]);
        dsl_wrefs.push_back(material_dsl_sref);

        std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsl_wrefs;
        common_dsl_wrefs.push_back(m_basic_dsl_maps["Camera"]);
        common_dsl_wrefs.push_back(m_basic_dsl_maps["MeshRender"]);
        common_dsl_wrefs.push_back(m_basic_dsl_maps["Light"]);

        //1.5 create pipelines.
        GraphicsPipelineStrongReferenceObject pipeline_sp0_sref = new GraphicsPipeline("BasicShaderSPO");
        pipeline_sp0_sref.GetRef().SetGraphicsPipelineParams(params, GetRenderPass("ForwardPass"), dsl_wrefs, 0);
        pipeline_sp0_sref.GetRef().Initialize(shader_modules);

        //1.6 prepare datas and then initalize shader structure.
        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp_wref = GetRenderPass("ForwardPass");
        RenderSubPassPipelineInfo rsp_info;
        RenderStepInfo rs_info;
        rs_info.m_dsl_wrefs = dsl_wrefs;
        rs_info.m_pipe_sref = pipeline_sp0_sref;
        rsp_info.m_step_infos.push_back(rs_info);
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        shader_program.GetRef().RegisterShaderProgramStructure(
            rp_infos, common_dsl_wrefs, {material_dsl_sref});

        m_shader_program_maps["BasicShading"] = shader_program;
    }
}


______________SD_END_GRAPHICS_NAMESPACE______________