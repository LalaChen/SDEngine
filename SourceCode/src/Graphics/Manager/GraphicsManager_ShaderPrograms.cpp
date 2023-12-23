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
#include "GraphicsManager.h"
#include "BasicUniforms.h"
#include "LightUniforms.h"
#include "MaterialUniforms.h"
#include "ModelData.h"
#include "LogManager.h"
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
        SDLOGE("We can't get shader program[%s].", i_sp_name.c_str());
        return ShaderProgramStrongReferenceObject();
    }
}

void GraphicsManager::RegisterShaderProgram(const ShaderProgramStrongReferenceObject &i_sp)
{
    std::map<ObjectName, ShaderProgramStrongReferenceObject>::const_iterator sp_iter = 
        m_shader_program_maps.find(SD_SREF(i_sp).GetObjectName());
    if (sp_iter == m_shader_program_maps.end()) {
        m_shader_program_maps[SD_SREF(i_sp).GetObjectName()] = i_sp;
    }
    else {
        SDLOGE("SP[%s] is existed.", SD_SREF(i_sp).GetObjectName().c_str());
    }
}

void GraphicsManager::RegisterShaderProgram(const ObjectName &i_sp_name, const FilePathString &i_path)
{
    std::map<ObjectName, ShaderProgramStrongReferenceObject>::const_iterator sp_iter = m_shader_program_maps.find(i_sp_name);
    if (sp_iter == m_shader_program_maps.end()) {
        ShaderProgramStrongReferenceObject new_shader = new ShaderProgram(i_sp_name);
        if (SD_SREF(new_shader).LoadFromFile(i_path) == true) {
            m_shader_program_maps[i_sp_name] = new_shader;
            SDLOGE("SP[%s] registered.", i_sp_name.c_str());
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
    UniformBufferDescriptorStrongReferenceObject camera_ubd = new UniformBufferDescriptor(sUniformBuffer_Camera, 0);
    SD_SREF(camera_ubd).AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_proj));
    SD_SREF(camera_ubd).AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_view));
    SD_SREF(camera_ubd).AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(CameraUniforms, m_view_eye));
    SD_SREF(camera_ubd).AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject camera_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_Camera);
    SD_SREF(camera_dsl).AddUniformVariableDescriptors({ camera_ubd.StaticCastTo<UniformVariableDescriptor>() });
    SD_SREF(camera_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_Camera] = camera_dsl;

    UniformBufferDescriptorStrongReferenceObject vr_camera_ubd = new UniformBufferDescriptor(sUniformBuffer_VRCamera, 0);
    SD_SREF(vr_camera_ubd).AddVariable("projs", UniformBufferVariableType_MATRIX4X4F, offsetof(VRCameraUniforms, m_projs), 2);
    SD_SREF(vr_camera_ubd).AddVariable("views", UniformBufferVariableType_MATRIX4X4F, offsetof(VRCameraUniforms, m_views), 2);
    SD_SREF(vr_camera_ubd).AddVariable("viewEyes", UniformBufferVariableType_VECTOR3F, offsetof(VRCameraUniforms, m_view_eyes), 2);
    SD_SREF(vr_camera_ubd).AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject vr_camera_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_VRCamera);
    SD_SREF(vr_camera_dsl).AddUniformVariableDescriptors({ vr_camera_ubd.StaticCastTo<UniformVariableDescriptor>() });
    SD_SREF(vr_camera_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_VRCamera] = vr_camera_dsl;

    //2. For Meshes.
    //Use for MVP matrices at all subpasses at Forward Pass and Defered Pass.
    UniformBufferDescriptorStrongReferenceObject geometry_ubd = new UniformBufferDescriptor(sUniformBuffer_MeshRender_Geometry, 0);
    SD_SREF(geometry_ubd).AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_world));
    SD_SREF(geometry_ubd).AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_normal));
    SD_SREF(geometry_ubd).AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject geometry_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_MeshRender);
    SD_SREF(geometry_dsl).AddUniformVariableDescriptors({ geometry_ubd.StaticCastTo<UniformVariableDescriptor>() });
    SD_SREF(geometry_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_MeshRender] = geometry_dsl;

    //3. For main light about forward rendering.
    UniformBufferDescriptorStrongReferenceObject light_ubd = new UniformBufferDescriptor(sUniformBuffer_Light, 0);
    SD_SREF(light_ubd).AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_ambient));
    SD_SREF(light_ubd).AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_diffuse));
    SD_SREF(light_ubd).AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_specular));
    SD_SREF(light_ubd).AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_position));
    SD_SREF(light_ubd).AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_direction));
    SD_SREF(light_ubd).AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_exp));
    SD_SREF(light_ubd).AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_cos_cutoff));
    SD_SREF(light_ubd).AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_constant_attenuation));
    SD_SREF(light_ubd).AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_linear_attenuation));
    SD_SREF(light_ubd).AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_quadratic_attenuation));
    SD_SREF(light_ubd).AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightUniforms, m_kind));
    SD_SREF(light_ubd).AddVariableDone();

    UniformImagesDescriptorStrongReferenceObject cascade_shadow_map_ui = new UniformImagesDescriptor(sUniformImages_Light_ShadowMaps, 1, 4);

    DescriptorSetLayoutStrongReferenceObject light_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_Light);
    SD_SREF(light_dsl).AddUniformVariableDescriptors({
        light_ubd.StaticCastTo<UniformVariableDescriptor>(),
        cascade_shadow_map_ui.StaticCastTo<UniformVariableDescriptor>()});
    SD_SREF(light_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_Light] = light_dsl;

    //4. GUI Offset Material.
    UniformBufferDescriptorStrongReferenceObject gui_ubd = new UniformBufferDescriptor(sUniformBuffer_GUI_Offset, 0);
    SD_SREF(gui_ubd).AddVariable("offsets", UniformBufferVariableType_VECTOR3F, 0);
    SD_SREF(gui_ubd).AddVariableDone();
    UniformImagesDescriptorStrongReferenceObject gui_font_ui = new UniformImagesDescriptor(sUniformImages_GUI_Font, 1);

    DescriptorSetLayoutStrongReferenceObject gui_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_GUI);
    SD_SREF(gui_dsl).AddUniformVariableDescriptors({
        gui_ubd.StaticCastTo<UniformVariableDescriptor>(),
        gui_font_ui.StaticCastTo<UniformVariableDescriptor>()});
    SD_SREF(gui_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_GUI] = gui_dsl;

    //5. Compositing Material.
    UniformBufferDescriptorStrongReferenceObject uvdata_ubd = new UniformBufferDescriptor(sUniformBuffer_Compositing_UVData, 0);
    SD_SREF(uvdata_ubd).AddVariable("uvData", UniformBufferVariableType_VECTOR3F, 0);
    SD_SREF(uvdata_ubd).AddVariableDone();
    UniformImagesDescriptorStrongReferenceObject main_texture_ui = new UniformImagesDescriptor(sUniformImage_Compositing_Texture, 1);

    DescriptorSetLayoutStrongReferenceObject compositing_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_Compositing);
    SD_SREF(compositing_dsl).AddUniformVariableDescriptors({
        uvdata_ubd.StaticCastTo<UniformVariableDescriptor>(),
        main_texture_ui.StaticCastTo<UniformVariableDescriptor>()});
    SD_SREF(compositing_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_Compositing] = compositing_dsl;

    //6. Compositing Material.
    UniformBufferDescriptorStrongReferenceObject uvdatas_ubd = new UniformBufferDescriptor(sUniformBuffer_VRCompositing_UVDatas, 0);
    SD_SREF(uvdatas_ubd).AddVariable("uvDatas", UniformBufferVariableType_VECTOR3F, 0);
    SD_SREF(uvdatas_ubd).AddVariableDone();
    UniformImagesDescriptorStrongReferenceObject vr_main_texture_ui = new UniformImagesDescriptor(sUniformImage_Compositing_Texture, 1);

    DescriptorSetLayoutStrongReferenceObject vr_compositing_dsl = new DescriptorSetLayout(sUniformDescriptorSetLayout_VRCompositing);
    SD_SREF(vr_compositing_dsl).AddUniformVariableDescriptors({
        uvdatas_ubd.StaticCastTo<UniformVariableDescriptor>(),
        vr_main_texture_ui.StaticCastTo<UniformVariableDescriptor>()});
    SD_SREF(vr_compositing_dsl).Initialize();
    m_basic_dsl_maps[sUniformDescriptorSetLayout_VRCompositing] = vr_compositing_dsl;
    //
    InitializeBasicDescriptorSetLayoutImpl();
}

void GraphicsManager::InitializeBasicDescriptorSetLayoutImpl()
{
}

void GraphicsManager::InitializeBasicMaterialUniformDescriptors()
{
    //4. For Material.
    UniformBufferDescriptorStrongReferenceObject mat_ubd = new UniformBufferDescriptor("material", 0);
    SD_SREF(mat_ubd).AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_ambient));
    SD_SREF(mat_ubd).AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_diffuse));
    SD_SREF(mat_ubd).AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_specular));
    SD_SREF(mat_ubd).AddVariable("emission", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_emission));
    SD_SREF(mat_ubd).AddVariable("shininess", UniformBufferVariableType_FLOAT, offsetof(MaterialUniforms, m_shininess));
    SD_SREF(mat_ubd).AddVariableDone();

    UniformImagesDescriptorStrongReferenceObject mt_imgd = new UniformImagesDescriptor("textures", 1, 11);
    m_material_basic_uvd_maps[sUniformBuffer_Material] = mat_ubd.StaticCastTo<UniformVariableDescriptor>();
    m_material_basic_uvd_maps[sUniformImages_Material_Textures] = mt_imgd.StaticCastTo<UniformVariableDescriptor>();

    InitializeBasicMaterialUniformDescriptorsImpl();
}

void GraphicsManager::InitializeBasicMaterialUniformDescriptorsImpl()
{
}

void GraphicsManager::InitializeBasicShaderPrograms()
{
    RegisterShaderProgram("AxesShader", "Common/ShaderProgram/AxesShading/AxesShading.json");
    RegisterShaderProgram("BasicShader", "Common/ShaderProgram/BasicShading/BasicShading.json");
    RegisterShaderProgram("NoLightShader", "Common/ShaderProgram/NoLightShading/NoLightShading.json");
    RegisterShaderProgram("GUIShader", "Common/ShaderProgram/GUIShading/GUIShading.json");
    RegisterShaderProgram("AlphaNoLightingShader", "Common/ShaderProgram/AlphaNoLightShading/AlphaNoLightShading.json");
    RegisterShaderProgram("LayerComposingShader", "Common/ShaderProgram/LayerComposing/LayerComposing.json");

    InitializeBasicShaderProgramsImpl();
}

void GraphicsManager::InitializeBasicShaderProgramsImpl()
{
}

/*
void GraphicsManager::InitializeBasicShaderPrograms()
{
    ShaderProgramStrongReferenceObject shader_program;
    {//BasicShading
        shader_program = new ShaderProgram("BasicShading");
        //1. Create GraphicsPipelines.
        //1.1 create shader module.
        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader = new ShaderModule("PhongShaderVert");
        SD_SREF(vert_shader).LoadBinaryShader(
            ShaderKind_VERTEX, std::vector<UByte>(
                gVulkan_BasicShader_SP0_vert_spv,
                gVulkan_BasicShader_SP0_vert_spv + (sizeof(gVulkan_BasicShader_SP0_vert_spv) / sizeof(UByte))),
            "main");
        ShaderModuleStrongReferenceObject frag_shader = new ShaderModule("PhongShaderFrag");
        SD_SREF(frag_shader).LoadBinaryShader(
            ShaderKind_FRAGMENT, std::vector<UByte>(
                gVulkan_BasicShader_SP0_frag_spv,
                gVulkan_BasicShader_SP0_frag_spv + (sizeof(gVulkan_BasicShader_SP0_frag_spv) / sizeof(UByte))),
            "main");
        shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader;
        shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader;

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

        DescriptorSetLayoutStrongReferenceObject material_dsl = new DescriptorSetLayout("Material");
        SD_SREF(material_dsl).AddUniformVariableDescriptors(final_material_basic_uvds);
        SD_SREF(material_dsl).Initialize();

        //1.4 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsls;
        dsls.push_back(m_basic_dsl_maps[sUniformDescriptorSetLayout_Camera]);
        dsls.push_back(m_basic_dsl_maps[sUniformDescriptorSetLayout_MeshRender]);
        dsls.push_back(m_basic_dsl_maps[sUniformDescriptorSetLayout_Light]);
        dsls.push_back(material_dsl);

        std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsls;
        common_dsls.push_back(m_basic_dsl_maps[sUniformDescriptorSetLayout_Camera]);
        common_dsls.push_back(m_basic_dsl_maps[sUniformDescriptorSetLayout_MeshRender]);
        common_dsls.push_back(m_basic_dsl_maps[sUniformDescriptorSetLayout_Light]);

        //1.5 create pipelines.
        GraphicsPipelineStrongReferenceObject pipeline_sp0 = new GraphicsPipeline("BasicShaderSPO");
        SD_SREF(pipeline_sp0).SetGraphicsPipelineParams(params, GetRenderPass(sRenderPass_Forward), dsls, 0);
        SD_SREF(pipeline_sp0).Initialize(shader_modules);

        //1.6 prepare datas and then initalize shader structure.
        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp = GetRenderPass(sRenderPass_Forward);
        RenderSubPassPipelineInfo rsp_info;
        RenderStepInfo rs_info;
        rs_info.m_dsls = dsls;
        rs_info.m_pipe = pipeline_sp0;
        rsp_info.m_step_infos.push_back(rs_info);
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        SD_SREF(shader_program).RegisterShaderProgramStructure(
            rp_infos, common_dsls, {material_dsl});

        m_shader_program_maps["BasicShading"] = shader_program;
    }
}
*/

______________SD_END_GRAPHICS_NAMESPACE______________