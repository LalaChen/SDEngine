#include "SDEngine.h"
#include "SampleDrawObjects.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

LightData::LightData()
{
}

LightData::~LightData()
{
}

void LightData::InitializeLightUniformSet(const DescriptorSetLayoutWeakReferenceObject &i_dsl_wref)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject> uv_wrefs;
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    i_dsl_wref.GetRef().GetUniformDescriptorCounts(desc_counts);

    //Visit all descriptor set layout to allocated set and get weak reference uniform variables.
    m_light_pool_sref = new DescriptorPool("CommonPool");
    m_light_pool_sref.GetRef().Initialize(desc_counts, 1, false);
    m_light_set_wref = m_light_pool_sref.GetRef().AllocateDescriptorSet(i_dsl_wref);
    m_light_set_wref.GetRef().GetAllocatedUniformVariables(uv_wrefs);

    //Write descriptor.
    if (m_light_set_wref.IsNull() == false) {
        m_light_set_wref.GetRef().WriteDescriptor();
    }

    if (uv_wrefs.find("light") != uv_wrefs.end()) {
        m_light_wrefs = uv_wrefs["light"].DynamicCastTo<UniformBuffer>();
    }

    if (m_light_wrefs.IsNull() == true) {
        SDLOGE("We can't find light uniform buffer.");
    }
}

void LightData::UpdateLightUniformSet()
{
    if (m_light_data.m_kind == 0) {// direction light.
        m_light_data.m_position = m_trans.GetForward().negative();
    }
    else if (m_light_data.m_kind == 1) { //position light,
        m_light_data.m_position = m_trans.m_position;
    }
    else if (m_light_data.m_kind == 2) { //spot light.
        m_light_data.m_position = m_trans.m_position;
        m_light_data.m_direction = m_trans.GetForward();
    }
    else {
        m_light_data.m_position = m_trans.GetForward();
    }

    if (m_light_wrefs.IsNull() == false) {
        m_light_wrefs.GetRef().SetBufferData(&m_light_data, sizeof(LightParameter));
        m_light_wrefs.GetRef().Update();
    }
}

ObjectData::ObjectData()
{
}

ObjectData::~ObjectData()
{
    m_basic_pool_sref.Reset();
}

void ObjectData::InitializeBasicUniformSet(const DescriptorSetLayoutWeakReferenceObject &i_basic_dsl_wref)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject> uv_wrefs;
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    i_basic_dsl_wref.GetRef().GetUniformDescriptorCounts(desc_counts);

    //Visit all descriptor set layout to allocated set and get weak reference uniform variables.
    m_basic_pool_sref = new DescriptorPool("CommonPool");
    m_basic_pool_sref.GetRef().Initialize(desc_counts, 1, false);
    m_basic_set_wref = m_basic_pool_sref.GetRef().AllocateDescriptorSet(i_basic_dsl_wref);
    m_basic_set_wref.GetRef().GetAllocatedUniformVariables(uv_wrefs);

    //Write descriptor.
    if (m_basic_set_wref.IsNull() == false) {
        m_basic_set_wref.GetRef().WriteDescriptor();
    }

    if (uv_wrefs.find("basic") != uv_wrefs.end()) {
        m_basic_wrefs = uv_wrefs["basic"].DynamicCastTo<UniformBuffer>();
    }

    if (m_basic_wrefs.IsNull() == true) {
        SDLOGE("We can find basic uniform buffer.");
    }
}

void ObjectData::UpdateCommonUniformSet(
    const CameraWeakReferenceObject &i_camera_wref)
{
    BasicParameter ub = {};
    ub.m_proj = i_camera_wref.GetRef().ToMatrix4X4();
    ub.m_view = i_camera.m_trans.MakeViewMatrix();
    ub.m_view_eye = i_camera.m_trans.m_position;
    ub.m_worid = m_trans.MakeWorldMatrix();

    if (m_basic_wrefs.IsNull() == false) {
        m_basic_wrefs.GetRef().SetBufferData(&ub, sizeof(BasicParameter));
        m_basic_wrefs.GetRef().Update();
    }
}

void ObjectData::Draw(
    const RenderPassWeakReferenceObject &i_rp_wref,
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const LightData &i_light_data,
    uint32_t i_sp_id)
{
    std::vector<DescriptorSetWeakReferenceObject> common_set_wrefs = {
        m_basic_set_wref,
        i_light_data.m_light_set_wref
    };
    if (m_mesh.IsNull() == false) {
        //1. use material.
        if (m_shared_mat_wref.IsNull() == false) {
            m_shared_mat_wref.GetRef().UseMaterial(
                i_cb_wref, i_rp_wref,
                common_set_wrefs,
                i_sp_id);
        }
        else {
            m_mat_sref.GetRef().UseMaterial(
                i_cb_wref, i_rp_wref,
                common_set_wrefs,
                i_sp_id);
        }
        //2. bind mesh vertex attributes.
        m_mesh.GetRef().BindVertexBuffers(i_cb_wref);
        //3. draw mesh.
        m_mesh.GetRef().BindIndexBuffer(i_cb_wref);
        m_mesh.GetRef().Render(i_cb_wref);
    }
}

SampleDrawObjects::SampleDrawObjects(const ObjectName &i_sample_name)
: Sample(i_sample_name)
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

SampleDrawObjects::~SampleDrawObjects()
{
}

void SampleDrawObjects::Initialize()
{
    m_current_res = GraphicsManager::GetRef().GetScreenResolution();
    CreateCommonUniformVariablesAndLayouts();
    CreateGeneralUniformVariablesAndLayouts();
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

void SampleDrawObjects::Render()
{
    UpdateCamera();
    for (ObjectData &obj : m_scene_objects) {
        obj.UpdateCommonUniformSet(m_camera);
    }

#if defined(RECORD_EVERY_FRAME)
    RecordCommandBuffer();
#endif

    GraphicsManager::GetRef().SubmitCommandBufferToQueue(m_main_cb_wref);
}

void SampleDrawObjects::Resize(Size_ui32 i_width, Size_ui32 i_height)
{
    m_camera_sref.GetRef().Resize();
    RecordCommandBuffer();
}

void SampleDrawObjects::Destroy()
{
    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin(); obj_iter != m_scene_objects.end();) {
        obj_iter = m_scene_objects.erase(obj_iter);
    }

    m_camera_sref.Reset();

    m_tex_sref.Reset();
    m_forward_rp_sref.Reset();
#if !defined(SINGLE_FLOW)
    for (uint32_t tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].Reset();
    }
#endif
    m_cp_sref.GetRef().RecycleCommandBuffer(m_main_cb_wref);
    m_cp_sref.GetRef().Clear(); //should do nothing.
    m_cp_sref.Reset();

    m_cube_sref.Reset();
    m_floor_sref.Reset();
    m_axes_sref.Reset();

    m_shared_material_sref.Reset();
    m_axes_shared_material_sref.Reset();

    m_phong_shader_sref.Reset();
    m_axes_shader_sref.Reset();

    m_basic_dsl_sref.Reset();
    m_light_dsl_sref.Reset();

    for (DescriptorSetLayoutStrongReferenceObject &gen_sref : m_general_dsl_srefs) {
        gen_sref.Reset();
    }
}

void SampleDrawObjects::CreateRenderPass()
{
    //
    SDLOG("Create render pass");
    //1. Initialize Forward render pass.
    m_forward_rp_sref = new RenderPass("ForwardPass");
    //1.1. prepare attachment references data.
    std::vector<AttachmentDescription> att_descs;
    AttachmentDescription att_desc;
    //--- Color Attachment for sp0(FirstLight).
    att_desc.m_format = GraphicsManager::GetRef().GetDefaultColorBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp0(FirstLight).
    att_desc.m_format = GraphicsManager::GetRef().GetDefaultDepthBufferFormat();
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

    m_forward_rp_sref.GetRef().AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    m_forward_rp_sref.GetRef().Initialize();
}

void SampleDrawObjects::CreateFramebuffer()
{
    SDLOG("Create Framebuffer and render flow.");
    m_camera_sref = new Camera("CameraObject");
    m_camera_sref.GetRef().SetPerspective(120, m_current_res.GetRatio(), 0.01f, 1000.0f);
    m_camera_sref.GetRef().SetTransform(Transform::LookAt(
        Vector3f(1.0f, 1.5f, 7.0f, 1.0f),
        Vector3f(0.0f, 1.5f, 0.0f, 1.0f),
        Vector3f::PositiveY,
        true));
    m_camera_sref.GetRef().Initialize();
}

void SampleDrawObjects::CreateCommandBufferAndPool()
{
    SDLOG("Create CommandBuffer and Pool.");

    m_cp_sref = new CommandPool("Sample4_SinglePool");
    m_cp_sref.GetRef().Initialize();
    m_main_cb_wref = m_cp_sref.GetRef().AllocateCommandBuffer();
#if !defined(SINGLE_FLOW)
    uint32_t max_threads = std::thread::hardware_concurrency();
    //max_threads = 1;
    m_rec_threads.resize(max_threads);
    SDLOG("Hardware concurrency : %u", max_threads);
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

void SampleDrawObjects::CreateCommonUniformVariablesAndLayouts()
{
    //1. new common descriptor set and its uniform variable descriptors.
    UniformBufferDescriptorStrongReferenceObject basic_ubd_sref = new UniformBufferDescriptor("basic", 0);
    basic_ubd_sref.GetRef().AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicParameter, m_proj));
    basic_ubd_sref.GetRef().AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicParameter, m_view));
    basic_ubd_sref.GetRef().AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicParameter, m_worid));
    basic_ubd_sref.GetRef().AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicParameter, m_normal));
    basic_ubd_sref.GetRef().AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(BasicParameter, m_view_eye));
    basic_ubd_sref.GetRef().AddVariableDone();

    m_basic_dsl_sref = new DescriptorSetLayout("RenderDescriptorSetLayout");
    m_basic_dsl_sref.GetRef().AddUniformVariableDescriptors({ basic_ubd_sref.StaticCastTo<UniformVariableDescriptor>()});
    m_basic_dsl_sref.GetRef().Initialize();

    //
    UniformBufferDescriptorStrongReferenceObject light_ubd_sref = new UniformBufferDescriptor("light", 0);
    light_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightParameter, m_ambient));
    light_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightParameter, m_diffuse));
    light_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightParameter, m_specular));
    light_ubd_sref.GetRef().AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightParameter, m_position));
    light_ubd_sref.GetRef().AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightParameter, m_direction));
    light_ubd_sref.GetRef().AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightParameter, m_spot_exp));
    light_ubd_sref.GetRef().AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightParameter, m_spot_cos_cutoff));
    light_ubd_sref.GetRef().AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightParameter, m_constant_attenuation));
    light_ubd_sref.GetRef().AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightParameter, m_linear_attenuation));
    light_ubd_sref.GetRef().AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightParameter, m_quadratic_attenuation));
    light_ubd_sref.GetRef().AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightParameter, m_kind));
    light_ubd_sref.GetRef().AddVariableDone();

    m_light_dsl_sref = new DescriptorSetLayout("LightDescriptorSetLayout");
    m_light_dsl_sref.GetRef().AddUniformVariableDescriptors({ light_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    m_light_dsl_sref.GetRef().Initialize();
}

void SampleDrawObjects::CreateGeneralUniformVariablesAndLayouts()
{
    //1 material ub.
    UniformBufferDescriptorStrongReferenceObject mat_ubd_sref = new UniformBufferDescriptor("material", 0);
    mat_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialParameter, m_ambient));
    mat_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialParameter, m_diffuse));
    mat_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialParameter, m_specular));
    mat_ubd_sref.GetRef().AddVariable("position", UniformBufferVariableType_COLOR4F, offsetof(MaterialParameter, m_emission));
    mat_ubd_sref.GetRef().AddVariable("direction", UniformBufferVariableType_FLOAT, offsetof(MaterialParameter, m_shininess));
    mat_ubd_sref.GetRef().AddVariableDone();
    //2 material uis.
    UniformImagesDescriptorStrongReferenceObject mt_imgd_sref = new UniformImagesDescriptor("mainTexture", 1);
    //------- custom data.
    DescriptorSetLayoutStrongReferenceObject gen_dsl_sref = new DescriptorSetLayout("GeneralMaterialDescriptorSetLayout");
    gen_dsl_sref.GetRef().AddUniformVariableDescriptors({ mat_ubd_sref.StaticCastTo<UniformVariableDescriptor>(), mt_imgd_sref.StaticCastTo<UniformVariableDescriptor>()});
    gen_dsl_sref.GetRef().Initialize();
    m_general_dsl_srefs.push_back(gen_dsl_sref);
}

void SampleDrawObjects::CreateTexture()
{
    m_tex_sref = new Texture("MainTexture");
    m_tex_sref.GetRef().SetSamplerFilterType(SamplerFilterType_LINEAR, SamplerFilterType_LINEAR);
    m_tex_sref.GetRef().InitializeFromImageResource("Texture/Lenna.png");
}

void SampleDrawObjects::CreateShaderProgram()
{
    //-------------------- PhongShaderWithTexture ---------------------------
    {
        m_phong_shader_sref = new ShaderProgram("PhongShaderWithTexture");
        //1. Create GraphicsPipeline.
        //1.1 create shader module.
        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("PhongShaderVert");
        vert_shader_sref.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/PhongShader.vert.spv", "main");
        ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("PhongShaderFrag");
        frag_shader_sref.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/PhongShader.frag.spv", "main");
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

        //1.3 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsl_wrefs;
        std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsl_wrefs;
        common_dsl_wrefs.push_back(m_basic_dsl_sref);
        common_dsl_wrefs.push_back(m_light_dsl_sref);
        //prepare all descriptor set wref.
        dsl_wrefs.push_back(m_basic_dsl_sref);
        dsl_wrefs.push_back(m_light_dsl_sref);
        for (uint32_t layout_count = 0; layout_count < m_general_dsl_srefs.size(); ++layout_count) {
            dsl_wrefs.push_back(m_general_dsl_srefs[layout_count]);
        }

        //2.3 register uniform variable descriptor to pipeline.
        GraphicsPipelineStrongReferenceObject pipeline_sref = new GraphicsPipeline("PhongShader_Forward");
        pipeline_sref.GetRef().SetGraphicsPipelineParams(params, m_forward_rp_sref, dsl_wrefs, 0);
        pipeline_sref.GetRef().Initialize(shader_modules);

        //2.4 prepare datas and then initalize shader structure.
        std::vector<GraphicsPipelineStrongReferenceObject> pipe_srefs;
        pipe_srefs.push_back(pipeline_sref);

        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp_wref = m_forward_rp_sref;
        RenderSubPassPipelineInfo rsp_info;
        rsp_info.m_dsl_wrefs = dsl_wrefs;
        rsp_info.m_pipe_id = 0;
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        m_phong_shader_sref.GetRef().RegisterShaderProgramStructure(
            rp_infos, pipe_srefs, common_dsl_wrefs, m_general_dsl_srefs);
    }
    //-------------------- AxesShaderWithTexture ---------------------------
    {
        m_axes_shader_sref = new ShaderProgram("AxesShaderWithTexture");
        //1. Create GraphicsPipeline.
        //1.1 create shader module.
        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("AxesShaderVert");
        vert_shader_sref.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/AxesShader.vert.spv", "main");
        ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("AxesShaderFrag");
        frag_shader_sref.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/AxesShader.frag.spv", "main");
        shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader_sref;
        shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader_sref;

        //1.2 write pipeline parames.
        GraphicsPipelineParam params;
        params.m_primitive_info.m_primitive = Primitive_TRIANGLE;
        params.m_depth_stencil_info.m_depth_test_enable = true;
        params.m_rasterization_info.m_face_culling = FaceCulling_NONE;
        params.m_attachment_blend_state.m_blend_infos.resize(1); //blend default false.
        params.m_dynamic_states.push_back(DynamicState_VIEWPORT);
        params.m_dynamic_states.push_back(DynamicState_SCISSOR);
        GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, 2);

        //1.3 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsl_wrefs;
        std::vector<DescriptorSetLayoutWeakReferenceObject> common_dsl_wrefs;
        common_dsl_wrefs.push_back(m_basic_dsl_sref);
        common_dsl_wrefs.push_back(m_light_dsl_sref);

        //prepare all descriptor set wref.
        dsl_wrefs.push_back(m_basic_dsl_sref);
        dsl_wrefs.push_back(m_light_dsl_sref);
        for (uint32_t layout_count = 0; layout_count < m_general_dsl_srefs.size(); ++layout_count) {
            dsl_wrefs.push_back(m_general_dsl_srefs[layout_count]);
        }

        //2.3 register uniform variable descriptor to pipeline.
        GraphicsPipelineStrongReferenceObject pipeline_sref = new GraphicsPipeline("AxesShader_Forward");
        pipeline_sref.GetRef().SetGraphicsPipelineParams(params, m_forward_rp_sref, dsl_wrefs, 0);
        pipeline_sref.GetRef().Initialize(shader_modules);

        //2.4 prepare datas and then initalize shader structure.
        std::vector<GraphicsPipelineStrongReferenceObject> pipe_srefs;
        pipe_srefs.push_back(pipeline_sref);

        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp_wref = m_forward_rp_sref;
        RenderSubPassPipelineInfo rsp_info;
        rsp_info.m_dsl_wrefs = dsl_wrefs;
        rsp_info.m_pipe_id = 0;
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        m_axes_shader_sref.GetRef().RegisterShaderProgramStructure(
            rp_infos, pipe_srefs, common_dsl_wrefs, m_general_dsl_srefs);
    }
}

void SampleDrawObjects::CreateSharedMaterial()
{
    {
        m_shared_material_sref = new Material("Material");
        m_shared_material_sref.GetRef().BindShaderProgram(m_phong_shader_sref);
        MaterialParameter mat_ub; //use default color.
        m_shared_material_sref.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialParameter));
        m_shared_material_sref.GetRef().SetTexture("mainTexture", m_tex_sref);
        //Set data done. Link with shader program.(Write descirptor)
        m_shared_material_sref.GetRef().LinkWithShaderProgram();
        m_shared_material_sref.GetRef().Update();
    }

    {
        m_axes_shared_material_sref = new Material("AxesMaterial");
        m_axes_shared_material_sref.GetRef().BindShaderProgram(m_axes_shader_sref);
        MaterialParameter mat_ub; //use default color.
        m_axes_shared_material_sref.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialParameter));
        //Set data done. Link with shader program.(Write descirptor)
        m_axes_shared_material_sref.GetRef().LinkWithShaderProgram();
        m_axes_shared_material_sref.GetRef().Update();
    }
}

void SampleDrawObjects::CreateObjects()
{
    SDLOG("Create Objects.");
    std::list<ObjectData>::reverse_iterator last_obj_iter;
    //Scene ---- Plane.
    m_floor_sref = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX, 100.0f, 100.0f, 100.0f, 100.0f);
    m_scene_objects.push_back(ObjectData());
    last_obj_iter = m_scene_objects.rbegin();
    (*last_obj_iter).m_mesh = m_floor_sref;
    (*last_obj_iter).InitializeBasicUniformSet(m_basic_dsl_sref);
    (*last_obj_iter).UpdateCommonUniformSet(m_camera);
    (*last_obj_iter).m_shared_mat_wref = m_shared_material_sref;

    //Scene ---- Axes.
    m_axes_sref = BasicShapeCreator::GetRef().CreateAxis(0.2f, 20.0f);
    m_scene_objects.push_back(ObjectData());
    last_obj_iter = m_scene_objects.rbegin();
    (*last_obj_iter).m_trans.m_position = Vector3f(0.0f, 0.0001f, 0.0f);
    (*last_obj_iter).m_mesh = m_axes_sref;
    (*last_obj_iter).InitializeBasicUniformSet(m_basic_dsl_sref);
    (*last_obj_iter).UpdateCommonUniformSet(m_camera);
    (*last_obj_iter).m_shared_mat_wref = m_axes_shared_material_sref;

    //Scene ---- Cubes
    Vector3f start_pos = Vector3f(m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, 1.0f).scale(
        -1.0f * static_cast<float>(m_cube_row) / 2.0f,
        0.25f,
        -1.0f * static_cast<float>(m_cube_depth) / 2.0f);
    m_cube_sref = BasicShapeCreator::GetRef().CreateCube(Vector3f::Zero, Vector3f(0.25f, 0.25f, 0.25f));
    for (uint32_t row = 0; row < m_cube_row; ++row) {
        for (uint32_t col = 0; col < m_cube_col; ++col) {
            for (uint32_t depth = 0; depth < m_cube_depth; ++depth) {
                m_scene_objects.push_back(ObjectData());
                last_obj_iter = m_scene_objects.rbegin();
                (*last_obj_iter).m_mesh = m_cube_sref;
                (*last_obj_iter).m_trans.m_position = start_pos + 
                    Vector3f((m_cube_side_length + m_cube_interval) * row, (m_cube_side_length + m_cube_interval) * col, (m_cube_side_length + m_cube_interval) * depth);
                //
                (*last_obj_iter).InitializeBasicUniformSet(m_basic_dsl_sref);
                (*last_obj_iter).UpdateCommonUniformSet(m_camera);
                (*last_obj_iter).m_shared_mat_wref = m_shared_material_sref;
            }
        }
    }
}

void SampleDrawObjects::CreateLight()
{
    m_light.m_trans = Transform::LookAt(Vector3f(1.0f, 3.0f, 1.0f, 1.0f), Vector3f::Origin, Vector3f::PositiveY);
    m_light.InitializeLightUniformSet(m_light_dsl_sref);
    m_light.UpdateLightUniformSet();
    SDLOG("%s",m_light.m_trans.MakeWorldMatrix().ToFormatString("Light","").c_str());
}

void SampleDrawObjects::RecordCommandBuffer()
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
    m_main_cb_wref.GetRef().Begin();
    m_camera.m_forward_rf.GetRef().BeginRenderFlow(m_main_cb_wref);
    GraphicsManager::GetRef().SetViewport(m_main_cb_wref, vp);
    GraphicsManager::GetRef().SetScissor(m_main_cb_wref, sr);

    for (ObjectData &obj : m_scene_objects) {
        obj.Draw(m_forward_rp_sref, m_main_cb_wref, 0);
    }

    m_camera.m_forward_rf.GetRef().EndRenderFlow(m_main_cb_wref);
    m_main_cb_wref.GetRef().End();
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
    m_main_cb_wref.GetRef().Begin();
    m_camera.m_forward_rf.GetRef().BeginRenderFlow(m_main_cb_wref);
    GraphicsManager::GetRef().SetViewport(m_main_cb_wref, vp);
    GraphicsManager::GetRef().SetScissor(m_main_cb_wref, sr);
    uint32_t oID = 0u;
    uint32_t tID = 0u;
    std::list<CommandBufferWeakReferenceObject> secondary_cb_wrefs;
    CommandBufferInheritanceInfo cb_inher_info = m_camera.m_forward_rf.GetRef().GetCurrentInheritanceInfo();
#if defined(RECORD_POOL_V2)
    for (SecondaryCommandPoolThreadStrongReferenceObject &rt_sref : m_rec_threads) {
        rt_sref.GetRef().StartRecording(cb_inher_info, vp, sr);
    }

    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin();
        obj_iter != m_scene_objects.end();
        ++obj_iter, ++oID) {
        ObjectData* obj_ref = &(*obj_iter);

        std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, obj_ref](const CommandBufferWeakReferenceObject &i_cb_wref) {
            obj_ref->Draw(m_forward_rp_sref, i_cb_wref, m_light, 0);
        };

        m_rec_threads[tID].GetRef().AddTask(task_func);

        tID = (tID + 1) % m_rec_threads.size();
    }

#if defined(TIME_MEASURE)
    tm.Record();
#endif

    for (tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].GetRef().WaitAndStopRecording(secondary_cb_wrefs);
    }
#if defined(TIME_MEASURE)
    tm.Record();
#endif
#else
    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin();
        obj_iter != m_scene_objects.end();
        ++obj_iter, ++oID) {
        ObjectData* obj_ref = &(*obj_iter);

        std::function<void(const CommandBufferWeakReferenceObject&)> task_func = [this, cb_inher_info, vp, sr, obj_ref](const CommandBufferWeakReferenceObject &i_cb_wref) {
            i_cb_wref.GetRef().Begin(cb_inher_info);
            GraphicsManager::GetRef().SetViewport(i_cb_wref, vp);
            GraphicsManager::GetRef().SetScissor(i_cb_wref, sr);
            obj_ref->Draw(m_mgr, i_cb_wref);
            i_cb_wref.GetRef().End();
        };

        m_rec_threads[tID].GetRef().AddTask(task_func);

        tID = (tID + 1) % m_rec_threads.size();
    }

#if defined(TIME_MEASURE)
    tm.Record();
#endif

    for (tID = 0; tID < m_rec_threads.size(); ++tID) {
        m_rec_threads[tID].GetRef().Wait(secondary_cb_wrefs);
    }
#if defined(TIME_MEASURE)
    tm.Record();
#endif

#endif
    
    GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(m_main_cb_wref, secondary_cb_wrefs);

#if defined(TIME_MEASURE)
    tm.Record();
#endif
    m_camera.m_forward_rf.GetRef().EndRenderFlow(m_main_cb_wref);
    m_main_cb_wref.GetRef().End();
#if defined(TIME_MEASURE)
    tm.Stop();
    SDLOG("%s", tm.ToString().c_str());
#endif
#endif
}

void SampleDrawObjects::UpdateCamera()
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