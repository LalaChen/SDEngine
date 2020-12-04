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
    //std::map<ObjectName, DescriptorSetLayoutStrongReferenceObject>::const_iterator dsl_iter;
    //for (dsl_iter = m_basic_dsl_maps.begin();
    //     dsl_iter != m_basic_dsl_maps.end();
    //     dsl_iter++) {
    //    io_dsl_wrefs.push_back((*dsl_iter).second);
    //}
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

void GraphicsManager::InitializeBasicDescriptorSetLayout()
{
    //1. For Cameras.
    UniformBufferDescriptorStrongReferenceObject camera_ubd_sref = new UniformBufferDescriptor("camera", 0);
    camera_ubd_sref.GetRef().AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_proj));
    camera_ubd_sref.GetRef().AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_view));
    camera_ubd_sref.GetRef().AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(CameraUniforms, m_view_eye));
    camera_ubd_sref.GetRef().AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject camera_dsl_sref = new DescriptorSetLayout("Camera");
    camera_dsl_sref.GetRef().AddUniformVariableDescriptors({ camera_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    camera_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["Camera"] = camera_dsl_sref;

    //2. For Meshes.
    //Use for MVP matrices at all subpasses at Forward Pass and Defered Pass.
    UniformBufferDescriptorStrongReferenceObject basic_ubd_sref = new UniformBufferDescriptor("geometry", 0);
    basic_ubd_sref.GetRef().AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_world));
    basic_ubd_sref.GetRef().AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_normal));
    basic_ubd_sref.GetRef().AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject basic_dsl_sref = new DescriptorSetLayout("MeshRender");
    basic_dsl_sref.GetRef().AddUniformVariableDescriptors({ basic_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    basic_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["MeshRender"] = basic_dsl_sref;

    //3. For light.
    //Use for Record Light Parameter.
    UniformBufferDescriptorStrongReferenceObject light_ubd_sref = new UniformBufferDescriptor("light", 0);
    light_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_ambient));
    light_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_diffuse));
    light_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_specular));
    light_ubd_sref.GetRef().AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_position));
    light_ubd_sref.GetRef().AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_direction));
    light_ubd_sref.GetRef().AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_exp));
    light_ubd_sref.GetRef().AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_cos_cutoff));
    light_ubd_sref.GetRef().AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_constant_attenuation));
    light_ubd_sref.GetRef().AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_linear_attenuation));
    light_ubd_sref.GetRef().AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_quadratic_attenuation));
    light_ubd_sref.GetRef().AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightUniforms, m_kind));
    light_ubd_sref.GetRef().AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject light_dsl_sref = new DescriptorSetLayout("Light");
    light_dsl_sref.GetRef().AddUniformVariableDescriptors({ light_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    light_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["Light"] = light_dsl_sref;
}

void GraphicsManager::InitializeBasicMaterialUniformDescriptors()
{
    //4. For Material.
    UniformBufferDescriptorStrongReferenceObject mat_ubd_sref = new UniformBufferDescriptor("material", 0);
    mat_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_ambient));
    mat_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_diffuse));
    mat_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_specular));
    mat_ubd_sref.GetRef().AddVariable("emission", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_emission));
    mat_ubd_sref.GetRef().AddVariable("shininess", UniformBufferVariableType_FLOAT, offsetof(MaterialUniforms, m_shininess));
    mat_ubd_sref.GetRef().AddVariableDone();

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
        GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, 2);

        //1.3 Create general DescritporSetLayouts.
        std::vector<UniformVariableDescriptorStrongReferenceObject> final_material_basic_uvds;
        final_material_basic_uvds.push_back(GetDefaultMaterialUniformVariableDescriptor("material"));
        final_material_basic_uvds.push_back(GetDefaultMaterialUniformVariableDescriptor("mainTexture"));

        DescriptorSetLayoutStrongReferenceObject material_dsl_sref = new DescriptorSetLayout("Material");
        material_dsl_sref.GetRef().AddUniformVariableDescriptors(final_material_basic_uvds);
        material_dsl_sref.GetRef().Initialize();

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
        pipeline_sp0_sref.GetRef().SetGraphicsPipelineParams(params, GetRenderPass("ForwardPath"), dsl_wrefs, 0);
        pipeline_sp0_sref.GetRef().Initialize(shader_modules);

        //1.6 prepare datas and then initalize shader structure.
        std::vector<GraphicsPipelineStrongReferenceObject> pipe_srefs;
        pipe_srefs.push_back(pipeline_sp0_sref);

        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp_wref = GetRenderPass("ForwardPath");
        RenderSubPassPipelineInfo rsp_info;
        rsp_info.m_dsl_wrefs = dsl_wrefs;
        rsp_info.m_pipe_id = 0;
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        shader_program.GetRef().RegisterShaderProgramStructure(
            rp_infos, pipe_srefs, common_dsl_wrefs, {material_dsl_sref});

        m_shader_program_maps["BasicShading"] = shader_program;
    }
}


______________SD_END_GRAPHICS_NAMESPACE______________