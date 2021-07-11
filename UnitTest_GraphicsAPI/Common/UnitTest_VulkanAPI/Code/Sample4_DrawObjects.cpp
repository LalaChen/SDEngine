#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample4_DrawObjects.h"

#include "UniformBindingType_Vulkan.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

float gClipMat[16] = {
     1.0f,  0.0f, 0.0f, 0.0f,
     0.0f,  1.0f, 0.0f, 0.0f,
     0.0f,  0.0f, 1.0f, 0.0f,
     0.0f,  0.0f, 0.0f, 1.0f };

VkDescriptorType DescriptorTypes[UniformBindingType_MAX_DEFINE_VALUE] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
};

LightData::LightData()
{
}

LightData::~LightData()
{
}

SampleCameraData::SampleCameraData()
{
}

SampleCameraData::~SampleCameraData()
{
}

ObjectData::ObjectData()
{
}

ObjectData::~ObjectData()
{
    m_common_pool.Reset();
}

void ObjectData::InitializeCommonUniformSet(const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_common_dsls)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    for (const DescriptorSetLayoutWeakReferenceObject &dsl : i_common_dsls) {
        dsl.GetRef().GetUniformDescriptorCounts(desc_counts);
    }
    m_common_pool = new DescriptorPool("CommonPool");
    m_common_pool.GetRef().Initialize(desc_counts, 1, false);

    for (const DescriptorSetLayoutWeakReferenceObject &dsl : i_common_dsls) {
        m_common_sets.push_back(m_common_pool.GetRef().AllocateDescriptorSet(dsl));
        m_common_sets[m_common_sets.size() - 1].GetRef().GetAllocatedUniformVariables(uvs);
    }

    for (DescriptorSetWeakReferenceObject &ds : m_common_sets) {
        if (ds.IsNull() == false) {
            ds.GetRef().WriteDescriptor();
        }
    }

    if (uvs.find("basic") != uvs.end()) {
        m_basics = uvs["basic"].DynamicCastTo<UniformBuffer>();
    }
    if (uvs.find("light") != uvs.end()) {
        m_lights = uvs["light"].DynamicCastTo<UniformBuffer>();
    }

    if (m_basics.IsNull() == true) {
        SDLOGE("We can find basic uniform buffer.");
    }

    if (m_lights.IsNull() == true) {
        SDLOGE("We can find light uniform buffer.");
    }
}

void ObjectData::UpdateCommonUniformSet(
    const SampleCameraData &i_camera,
    const LightData &i_light)
{
    BasicUniformBuffer ub = {};
    ub.m_clip = Matrix4X4f(gClipMat);
    ub.m_proj = i_camera.m_proj_mat;
    ub.m_view = i_camera.m_trans.MakeViewMatrix();
    ub.m_view_eye = i_camera.m_trans.m_position;
    ub.m_world = m_trans.MakeWorldMatrix();

    LightUniformBuffer lb = {};
    lb = i_light.m_light_data;
    if (lb.m_kind == 0) {// direction light.
        lb.m_position = i_light.m_trans.GetForward().negative();
    }
    else if (lb.m_kind == 1) { //position light,
        lb.m_position = i_light.m_trans.m_position;
    }
    else if (lb.m_kind == 2) { //spot light.
        lb.m_position = i_light.m_trans.m_position;
        lb.m_direction = i_light.m_trans.GetForward();
    }
    else {
        lb.m_position = i_light.m_trans.GetForward();
    }

    if (m_basics.IsNull() == false && m_lights.IsNull() == false) {
        m_basics.GetRef().SetBufferData(&ub, sizeof(BasicUniformBuffer));
        m_basics.GetRef().Update();
        m_lights.GetRef().SetBufferData(&lb, sizeof(LightUniformBuffer));
        m_lights.GetRef().Update();
    }
}

void ObjectData::Draw(
    const RenderPassWeakReferenceObject &i_rp,
    const CommandBufferWeakReferenceObject &i_cb,
    uint32_t i_sp_id)
{
    if (m_mesh.IsNull() == false) {
        MaterialWeakReferenceObject used_material;
        //1. use material.
        if (m_shared_mat.IsNull() == false) {
            used_material = m_shared_mat;
        }
        else {
            used_material = m_mat;
        }

        uint32_t step_amount = SD_WREF(m_shared_mat).GetStepAmount(i_rp, i_sp_id);

        for (uint32_t step_id = 0; step_id < step_amount; ++step_id) {
            m_shared_mat.GetRef().UseMaterial(
                i_cb, i_rp,
                m_common_sets,
                i_sp_id, step_id);
            //2. bind mesh vertex attributes.
            m_mesh.GetRef().BindVertexBuffers(i_cb);
            //3. draw mesh.
            m_mesh.GetRef().BindIndexBuffer(i_cb);
            m_mesh.GetRef().Render(i_cb);
        }
    }
}

Sample4_DrawObjects::Sample4_DrawObjects(VulkanAPITestManager *i_mgr)
: Sample("DrawObjects", i_mgr)
, m_cube_interval(0.05f)
, m_cube_side_length(0.25f)
#if defined(_WIN32) || defined(_WIN64)
, m_cube_row(15)
, m_cube_col(15)
, m_cube_depth(15)
#else
, m_cube_row(10)
, m_cube_col(10)
, m_cube_depth(10)
#endif
{

}

Sample4_DrawObjects::~Sample4_DrawObjects()
{
}

void Sample4_DrawObjects::Initialize()
{
    m_current_res = m_mgr->GetScreenResolution();
    CreateCommonUniformVariablesAndLayouts();
    CreateGeneralUniformVariablesAndLayouts();
    CreateCamera();
    CreateRenderPass();
    CreateFramebuffer();
    CreateTexture();
    CreateShaderProgram();
    CreateSharedMaterial();
    CreateLight();
    CreateObjects();
    CreateCommandBufferAndPool();
    //
    RecordCommandBuffer();
}

void Sample4_DrawObjects::Render()
{
    UpdateCamera();
    for (ObjectData &obj : m_scene_objects) {
        obj.UpdateCommonUniformSet(m_camera, m_light);
    }

#if defined(RECORD_EVERY_FRAME)
    RecordCommandBuffer();
#endif

    GraphicsManager::GetRef().SubmitCommandBufferToQueue(m_main_cb);
}

void Sample4_DrawObjects::Resize(Size_ui32 i_width, Size_ui32 i_height)
{
    m_camera.m_color_buffer.Reset();
    m_camera.m_depth_buffer.Reset();
    m_camera.m_forward_rf.Reset();

    m_current_res = m_mgr->GetScreenResolution();
    CreateCamera();
    CreateFramebuffer();
    RecordCommandBuffer();
}

void Sample4_DrawObjects::Destroy()
{
    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin(); obj_iter != m_scene_objects.end();) {
        obj_iter = m_scene_objects.erase(obj_iter);
    }

    m_camera.m_color_buffer.Reset();
    m_camera.m_depth_buffer.Reset();
    m_camera.m_forward_rf.Reset();
    m_tex.Reset();
    m_forward_rp.Reset();
#if !defined(SINGLE_FLOW)
    for (uint32_t tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].Reset();
    }
#endif
    m_cp.GetRef().RecycleCommandBuffer(m_main_cb);
    m_cp.GetRef().Clear(); //should do nothing.
    m_cp.Reset();

    m_cube.Reset();
    m_floor.Reset();
    m_axes.Reset();

    m_shared_material.Reset();
    m_axes_shared_material.Reset();

    m_phong_shader.Reset();
    m_axes_shader.Reset();

    for (DescriptorSetLayoutStrongReferenceObject &dsl : m_common_dsls) {
        dsl.Reset();
    }

    for (DescriptorSetLayoutStrongReferenceObject &gen : m_general_dsls) {
        gen.Reset();
    }
}

void Sample4_DrawObjects::CreateRenderPass()
{
    //
    SDLOG("Create render pass");
    //1. Initialize Forward render pass.
    m_forward_rp = new RenderPass("ForwardPass");
    //1.1. prepare attachment references data.
    std::vector<AttachmentDescription> att_descs;
    AttachmentDescription att_desc;
    //--- Color Attachment for sp0(FirstLight).
    att_desc.m_format = m_mgr->GetDefaultColorBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp0(FirstLight).
    att_desc.m_format = m_mgr->GetDefaultDepthBufferFormat();
    att_desc.m_initial_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);

    //1.2. prepare attachment references data.
    std::vector<SubpassDescription> sp_descs;
    SubpassDescription sp_desc;
    AttachmentReference atta_ref;
    //--- sp0 FirstOrNoLight
    sp_desc = SubpassDescription();
    sp_desc.m_name = "FirstOrNoLight";
    sp_desc.m_bind_point = PipelineBindPoint_GRAPHICS;
    //--- color attachment reference.
    atta_ref.m_attachment_ID = 0;
    atta_ref.m_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    sp_desc.m_color_attachment_refs.push_back(atta_ref);
    //--- depth attachment reference.
    sp_desc.m_depth_attachment_ref.m_attachment_ID = 1;
    sp_desc.m_depth_attachment_ref.m_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    sp_descs.push_back(sp_desc);
    //1.3 SubpassDependency.
    std::vector<SubpassDependency> sp_denps;
    SubpassDependency sp_denp;
    //sp external to sp0
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_dst_spID = 0;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);
    //sp0 and sp external
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = 0;
    sp_denp.m_dst_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);

    m_forward_rp.GetRef().AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    m_forward_rp.GetRef().Initialize();
}

void Sample4_DrawObjects::CreateFramebuffer()
{
    SDLOG("Create Framebuffer and render flow.");

    ClearValue clear_color = { 0.15f, 0.15f, 0.75f, 1.0f };
    ClearValue clear_dands = { 1.0f, 1 };

    m_camera.m_forward_rf = new RenderFlow("ForwardPassRF", ImageOffset(0, 0, 0),
        ImageSize(m_current_res.GetWidth(), m_current_res.GetHeight(), 1));
    m_camera.m_forward_rf.GetRef().RegisterRenderPass(m_forward_rp);
    m_camera.m_forward_rf.GetRef().AllocateFrameBuffer();
    m_camera.m_forward_rf.GetRef().RegisterBufferToFrameBuffer(m_camera.m_color_buffer, 0, clear_color);
    m_camera.m_forward_rf.GetRef().RegisterBufferToFrameBuffer(m_camera.m_depth_buffer, 1, clear_dands);
    m_camera.m_forward_rf.GetRef().Initialize();
}

void Sample4_DrawObjects::CreateCommandBufferAndPool()
{
    SDLOG("Create CommandBuffer and Pool.");

    m_cp = new CommandPool("Sample4_SinglePool");
    m_cp.GetRef().Initialize();
    m_main_cb = m_cp.GetRef().AllocateCommandBuffer();
#if !defined(SINGLE_FLOW)
    uint32_t max_threads = std::thread::hardware_concurrency();
    max_threads = 1;
    m_rec_threads.resize(max_threads);
#if defined(RECORD_POOL_V2)
    for (uint32_t tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID] = new SecondaryCommandPoolThread(StringFormat("RecordThread_%d", tID));
        m_rec_threads[tID].GetRef().Initialize();
    }
#else
    for (uint32_t tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID] = new CommandRecordingThread(StringFormat("RecordThread_%d", tID));
        m_rec_threads[tID].GetRef().Initialize(1);
    }
#endif
#endif
}

void Sample4_DrawObjects::CreateCommonUniformVariablesAndLayouts()
{
    //1. new common descriptor set and its uniform variable descriptors.
    UniformBufferDescriptorStrongReferenceObject basic_ubd = new UniformBufferDescriptor("basic", 0);
    basic_ubd.GetRef().AddVariable("clip", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_clip));
    basic_ubd.GetRef().AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_proj));
    basic_ubd.GetRef().AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_view));
    basic_ubd.GetRef().AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_world));
    basic_ubd.GetRef().AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_normal));
    basic_ubd.GetRef().AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(BasicUniformBuffer, m_view_eye));
    basic_ubd.GetRef().AddVariableDone();

    UniformBufferDescriptorStrongReferenceObject light_ubd = new UniformBufferDescriptor("light", 1);
    light_ubd.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightUniformBuffer, m_ambient));
    light_ubd.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightUniformBuffer, m_diffuse));
    light_ubd.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightUniformBuffer, m_specular));
    light_ubd.GetRef().AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightUniformBuffer, m_position));
    light_ubd.GetRef().AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightUniformBuffer, m_direction));
    light_ubd.GetRef().AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_spot_exp));
    light_ubd.GetRef().AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_spot_cos_cutoff));
    light_ubd.GetRef().AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_constant_attenuation));
    light_ubd.GetRef().AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_linear_attenuation));
    light_ubd.GetRef().AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_quadratic_attenuation));
    light_ubd.GetRef().AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightUniformBuffer, m_kind));
    light_ubd.GetRef().AddVariableDone();

    //To do : modify binding number.
    DescriptorSetLayoutStrongReferenceObject basic_dsl = new DescriptorSetLayout("RenderDescriptorSetLayout");
    basic_dsl.GetRef().AddUniformVariableDescriptors({ basic_ubd.StaticCastTo<UniformVariableDescriptor>(), light_ubd.StaticCastTo<UniformVariableDescriptor>() });
    basic_dsl.GetRef().Initialize();
    m_common_dsls.push_back(basic_dsl);
}

void Sample4_DrawObjects::CreateGeneralUniformVariablesAndLayouts()
{
    //1 material ub.
    UniformBufferDescriptorStrongReferenceObject mat_ubd = new UniformBufferDescriptor("material", 0);
    mat_ubd.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_ambient));
    mat_ubd.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_diffuse));
    mat_ubd.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_specular));
    mat_ubd.GetRef().AddVariable("position", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_emission));
    mat_ubd.GetRef().AddVariable("direction", UniformBufferVariableType_FLOAT, offsetof(MaterialUniformBuffer, m_shineness));
    mat_ubd.GetRef().AddVariableDone();
    //2 material uis.
    UniformImagesDescriptorStrongReferenceObject mt_imgd = new UniformImagesDescriptor("mainTexture", 1);
    //------- custom data.
    //
    DescriptorSetLayoutStrongReferenceObject gen_dsl = new DescriptorSetLayout("GeneralMaterialDescriptorSetLayout");
    gen_dsl.GetRef().AddUniformVariableDescriptors({ mat_ubd.StaticCastTo<UniformVariableDescriptor>(), mt_imgd.StaticCastTo<UniformVariableDescriptor>()});
    gen_dsl.GetRef().Initialize();
    m_general_dsls.push_back(gen_dsl);
}

void Sample4_DrawObjects::CreateTexture()
{
    m_tex = new Texture("MainTexture");
    m_tex.GetRef().SetSamplerFilterType(SamplerFilterType_LINEAR, SamplerFilterType_LINEAR);
    m_tex.GetRef().InitializeFromImageResource("Texture/Lenna.png");
}

void Sample4_DrawObjects::CreateShaderProgram()
{
    //-------------------- PhongShaderWithTexture ---------------------------
    {
        m_phong_shader = new ShaderProgram("PhongShaderWithTexture");
        //1. Create GraphicsPipeline.
        //1.1 create shader module.
        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader = new ShaderModule("PhongShaderVert");
        vert_shader.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/PhongShader.vert.spv", "main");
        ShaderModuleStrongReferenceObject frag_shader = new ShaderModule("PhongShaderFrag");
        frag_shader.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/PhongShader.frag.spv", "main");
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
        GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, VertexLocationKind_GENERAL);

        //1.3 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsls;
        std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsls;
        for (uint32_t layout_count = 0; layout_count < m_common_dsls.size(); ++layout_count) {
            dsls.push_back(m_common_dsls[layout_count]);
            common_dsls.push_back(m_common_dsls[layout_count]);
        }
        for (uint32_t layout_count = 0; layout_count < m_general_dsls.size(); ++layout_count) {
            dsls.push_back(m_general_dsls[layout_count]);
        }

        //2.3 register uniform variable descriptor to pipeline.
        GraphicsPipelineStrongReferenceObject pipeline = new GraphicsPipeline("PhongShader_Forward");
        pipeline.GetRef().SetGraphicsPipelineParams(params, m_forward_rp, dsls, 0);
        pipeline.GetRef().Initialize(shader_modules);

        //2.4 prepare datas and then initalize shader structure.
        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp = m_forward_rp;
        RenderSubPassPipelineInfo rsp_info;
        RenderStepInfo rs_info;
        rs_info.m_dsls = dsls;
        rs_info.m_pipe = pipeline;
        rsp_info.m_step_infos.push_back(rs_info);
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        m_phong_shader.GetRef().RegisterShaderProgramStructure(
            rp_infos, common_dsls, m_general_dsls);
    }
    //-------------------- AxesShaderWithTexture ---------------------------
    {
        m_axes_shader = new ShaderProgram("AxesShaderWithTexture");
        //1. Create GraphicsPipeline.
        //1.1 create shader module.
        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader = new ShaderModule("AxesShaderVert");
        vert_shader.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/AxesShader.vert.spv", "main");
        ShaderModuleStrongReferenceObject frag_shader = new ShaderModule("AxesShaderFrag");
        frag_shader.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/AxesShader.frag.spv", "main");
        shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader;
        shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader;

        //1.2 write pipeline parames.
        GraphicsPipelineParam params;
        params.m_primitive_info.m_primitive = Primitive_TRIANGLE;
        params.m_depth_stencil_info.m_depth_test_enable = true;
        params.m_rasterization_info.m_face_culling = FaceCulling_NONE;
        params.m_attachment_blend_state.m_blend_infos.resize(1); //blend default false.
        params.m_dynamic_states.push_back(DynamicState_VIEWPORT);
        params.m_dynamic_states.push_back(DynamicState_SCISSOR);
        GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, VertexLocationKind_GENERAL);

        //1.3 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsls;
        std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsls;
        for (uint32_t layout_count = 0; layout_count < m_common_dsls.size(); ++layout_count) {
            dsls.push_back(m_common_dsls[layout_count]);
            common_dsls.push_back(m_common_dsls[layout_count]);
        }
        for (uint32_t layout_count = 0; layout_count < m_general_dsls.size(); ++layout_count) {
            dsls.push_back(m_general_dsls[layout_count]);
        }

        //2.3 register uniform variable descriptor to pipeline.
        GraphicsPipelineStrongReferenceObject pipeline = new GraphicsPipeline("AxesShader_Forward");
        pipeline.GetRef().SetGraphicsPipelineParams(params, m_forward_rp, dsls, 0);
        pipeline.GetRef().Initialize(shader_modules);

        //2.4 prepare datas and then initalize shader structure.
        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp = m_forward_rp;
        RenderSubPassPipelineInfo rsp_info;
        RenderStepInfo rs_info;
        rs_info.m_dsls = dsls;
        rs_info.m_pipe = pipeline;
        rsp_info.m_step_infos.push_back(rs_info);
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        m_axes_shader.GetRef().RegisterShaderProgramStructure(
            rp_infos, common_dsls, m_general_dsls);
    }
}

void Sample4_DrawObjects::CreateSharedMaterial()
{
    {
        m_shared_material = new Material("Material");
        m_shared_material.GetRef().BindShaderProgram(m_phong_shader);
        MaterialUniformBuffer mat_ub; //use default color.
        m_shared_material.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniformBuffer));
        m_shared_material.GetRef().SetTexture("mainTexture", m_tex);
        //Set data done. Link with shader program.(Write descirptor)
        m_shared_material.GetRef().LinkWithShaderProgram();
        m_shared_material.GetRef().Update();
    }

    {
        m_axes_shared_material = new Material("AxesMaterial");
        m_axes_shared_material.GetRef().BindShaderProgram(m_axes_shader);
        MaterialUniformBuffer mat_ub; //use default color.
        m_axes_shared_material.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniformBuffer));
        //Set data done. Link with shader program.(Write descirptor)
        m_axes_shared_material.GetRef().LinkWithShaderProgram();
        m_axes_shared_material.GetRef().Update();
    }
}

void Sample4_DrawObjects::CreateObjects()
{
    SDLOG("Create Objects.");
    std::vector<DescriptorSetLayoutWeakReferenceObject> dsls;
    for (const DescriptorSetLayoutWeakReferenceObject &dsl : m_common_dsls) {
        dsls.push_back(dsl);
    }
    std::list<ObjectData>::reverse_iterator last_obj_iter;
    //Scene ---- Plane.
    m_floor = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX, 100.0f, 100.0f, 100.0f, 100.0f);
    m_scene_objects.push_back(ObjectData());
    last_obj_iter = m_scene_objects.rbegin();
    (*last_obj_iter).m_mesh = m_floor;
    (*last_obj_iter).InitializeCommonUniformSet(dsls);
    (*last_obj_iter).UpdateCommonUniformSet(m_camera, m_light);
    (*last_obj_iter).m_shared_mat = m_shared_material;

    //Scene ---- Axes.
    m_axes = BasicShapeCreator::GetRef().CreateAxis(0.2f, 20.0f);
    m_scene_objects.push_back(ObjectData());
    last_obj_iter = m_scene_objects.rbegin();
    (*last_obj_iter).m_trans.m_position = Vector3f(0.0f, 0.0001f, 0.0f);
    (*last_obj_iter).m_mesh = m_axes;
    (*last_obj_iter).InitializeCommonUniformSet(dsls);
    (*last_obj_iter).UpdateCommonUniformSet(m_camera, m_light);
    (*last_obj_iter).m_shared_mat = m_axes_shared_material;

    //Scene ---- Cubes
    Vector3f start_pos = Vector3f(m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, 1.0f).scale(
        -1.0f * static_cast<float>(m_cube_row) / 2.0f,
        0.25f,
        -1.0f * static_cast<float>(m_cube_depth) / 2.0f);
    m_cube = BasicShapeCreator::GetRef().CreateCube(Vector3f::Zero, Vector3f(0.25f, 0.25f, 0.25f));
    for (uint32_t row = 0; row < m_cube_row; ++row) {
        for (uint32_t col = 0; col < m_cube_col; ++col) {
            for (uint32_t depth = 0; depth < m_cube_depth; ++depth) {
                m_scene_objects.push_back(ObjectData());
                last_obj_iter = m_scene_objects.rbegin();
                (*last_obj_iter).m_mesh = m_cube;
                (*last_obj_iter).m_trans.m_position = start_pos + 
                    Vector3f((m_cube_side_length + m_cube_interval) * row, (m_cube_side_length + m_cube_interval) * col, (m_cube_side_length + m_cube_interval) * depth);
                //
                (*last_obj_iter).InitializeCommonUniformSet(dsls);
                (*last_obj_iter).UpdateCommonUniformSet(m_camera, m_light);
                (*last_obj_iter).m_shared_mat = m_shared_material;
            }
        }
    }
}

void Sample4_DrawObjects::CreateCamera()
{
    if (m_camera.m_color_buffer.IsNull() == false) {
        m_camera.m_color_buffer.Reset();
    }
    m_camera.m_color_buffer = new Texture("CameraColorBuffer");
    m_camera.m_color_buffer.GetRef().Initialize2DColorOrDepthBuffer(
        m_current_res.GetWidth(), m_current_res.GetHeight(),
        GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
        ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

    if (m_camera.m_depth_buffer.IsNull() == false) {
        m_camera.m_depth_buffer.Reset();
    }
    m_camera.m_depth_buffer = new Texture("CameraDepthBuffer");
    m_camera.m_depth_buffer.GetRef().Initialize2DColorOrDepthBuffer(
        m_current_res.GetWidth(), m_current_res.GetHeight(),
        GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
        ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    m_camera.m_proj_mat.perspective(120, m_current_res.GetRatio(), 0.01f, 1000.0f);

    m_camera.m_trans = Transform::LookAt(
        Vector3f(1.0f, 1.5f, 7.0f, 1.0f),
        Vector3f(0.0f, 1.5f, 0.0f, 1.0f),
        Vector3f::PositiveY,
        true);
    SDLOG("%s", m_camera.m_trans.MakeWorldMatrix().ToFormatString("Camera", "").c_str());
}

void Sample4_DrawObjects::CreateLight()
{
    m_light.m_trans = Transform::LookAt(Vector3f(1.0f, 3.0f, 1.0f, 1.0f), Vector3f::Origin, Vector3f::PositiveY);
    SDLOG("%s",m_light.m_trans.MakeWorldMatrix().ToFormatString("Light","").c_str());
}

void Sample4_DrawObjects::RecordCommandBuffer()
{
    //Negative viewport.
    Viewport vp;
    vp.m_x = 0.0f; vp.m_y = static_cast<float>(m_current_res.GetHeight());
    vp.m_width = static_cast<float>(m_current_res.GetWidth());
    vp.m_height = -1.0f * static_cast<float>(m_current_res.GetHeight());
    vp.m_min_depth = 0.0f;
    vp.m_max_depth = 1.0f;

    ScissorRegion sr;
    sr.m_x = 0.0f; sr.m_y = 0.0f;
    sr.m_width = vp.m_width; sr.m_height = m_current_res.GetHeight();

#if defined(SINGLE_FLOW)
#if defined(TIME_MEASURE)
    TimerMeasurer tm("RecordCommandBufferSingle");
    tm.Start();
#endif
    //1. Begin Command Buffer
    m_main_cb.GetRef().Begin();
    m_camera.m_forward_rf.GetRef().BeginRenderFlow(m_main_cb);
    GraphicsManager::GetRef().SetViewport(m_main_cb, vp);
    GraphicsManager::GetRef().SetScissor(m_main_cb, sr);

    for (ObjectData& obj : m_scene_objects) {
        obj.Draw(m_forward_rp, m_main_cb, 0);
    }

    m_camera.m_forward_rf.GetRef().EndRenderFlow(m_main_cb);
    m_main_cb.GetRef().End();
#if defined(TIME_MEASURE)
    tm.Stop();
    SDLOG("%s", tm.ToString().c_str());
#endif
#else
    /*
     * Notice : We can't record command buffers created from same command pool simultaneously.(Pool isn't thread-safe!!!) Or it will crash in my test.
     */
#if defined(TIME_MEASURE)
    TimerMeasurer tm("RecordCommandBufferMulti");
    tm.Start();
#endif
    m_main_cb.GetRef().Begin();
    m_camera.m_forward_rf.GetRef().BeginRenderFlow(m_main_cb);
    GraphicsManager::GetRef().SetViewport(m_main_cb, vp);
    GraphicsManager::GetRef().SetScissor(m_main_cb, sr);
    uint32_t oID = 0u;
    uint32_t tID = 0u;
    std::list<CommandBufferWeakReferenceObject> secondary_cbs;
    CommandBufferInheritanceInfo cb_inher_info = m_camera.m_forward_rf.GetRef().GetCurrentInheritanceInfo();
#if defined(RECORD_POOL_V2)
    for (SecondaryCommandPoolThreadStrongReferenceObject &rt : m_rec_threads) {
        rt.GetRef().StartRecording(cb_inher_info, vp, sr);
    }

    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin();
        obj_iter != m_scene_objects.end();
        ++obj_iter, ++oID) {
        ObjectData* obj_ref = &(*obj_iter);

        std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, obj_ref](const CommandBufferWeakReferenceObject &i_cb) {
            obj_ref->Draw(m_forward_rp, i_cb, 0);
        };

        m_rec_threads[tID].GetRef().AddTask(task_func);

        tID = (tID + 1) % m_rec_threads.size();
    }

#if defined(TIME_MEASURE)
    tm.Record();
#endif

    for (tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].GetRef().WaitAndStopRecording(secondary_cbs);
    }
#if defined(TIME_MEASURE)
    tm.Record();
#endif
#else
    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin();
        obj_iter != m_scene_objects.end();
        ++obj_iter, ++oID) {
        ObjectData* obj_ref = &(*obj_iter);

        std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, cb_inher_info, vp, sr, obj_ref](const CommandBufferWeakReferenceObject &i_cb) {
            i_cb.GetRef().Begin(cb_inher_info);
            GraphicsManager::GetRef().SetViewport(i_cb, vp);
            GraphicsManager::GetRef().SetScissor(i_cb, sr);
            obj_ref->Draw(m_forward_rp, i_cb, 0);
            i_cb.GetRef().End();
        };

        m_rec_threads[tID].GetRef().AddTask(task_func);

        tID = (tID + 1) % m_rec_threads.size();
    }

#if defined(TIME_MEASURE)
    tm.Record();
#endif

    for (tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].GetRef().Wait(secondary_cbs);
    }
#if defined(TIME_MEASURE)
    tm.Record();
#endif

#endif
    
    GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(m_main_cb, secondary_cbs);

#if defined(TIME_MEASURE)
    tm.Record();
#endif
    m_camera.m_forward_rf.GetRef().EndRenderFlow(m_main_cb);
    m_main_cb.GetRef().End();
#if defined(TIME_MEASURE)
    tm.Stop();
    SDLOG("%s", tm.ToString().c_str());
#endif
#endif
}

void Sample4_DrawObjects::UpdateCamera()
{
    if (Application::GetRef().GetKeyStateByCode(KEY_A) == KEY_STATUS_PRESS) {
        m_camera.m_trans.AddRotation(m_camera.m_trans.GetTop(), static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 10.0f);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_D) == KEY_STATUS_PRESS) {
        m_camera.m_trans.AddRotation(m_camera.m_trans.GetTop(), static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * -10.0f);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_W) == KEY_STATUS_PRESS) {
        //Camera forward is -z axis.
        Vector3f offset = m_camera.m_trans.GetForward().negative().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_S) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetForward().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_Q) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetRight().negative().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 1.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_E) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetRight().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 1.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }
   
    if (Application::GetRef().GetKeyStateByCode(KEY_R) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetTop().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_F) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetTop().negative().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }
}