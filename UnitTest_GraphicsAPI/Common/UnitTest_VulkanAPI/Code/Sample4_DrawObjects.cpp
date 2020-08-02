#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample4_DrawObjects.h"

#include "UniformBindingType_Vulkan.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

float gClipMat[16] = {
   -1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
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
}

void ObjectData::InitalizeMaterial(
    const ShaderProgramWeakReferenceObject &i_sp_wref,
    const SampleCameraData &i_camera,
    const LightData &i_light,
    const TextureWeakReferenceObject &i_tex_wref)
{
    m_mat_sref = new Material("Material");
    m_mat_sref.GetRef().BindShaderProgram(i_sp_wref);
    UpdateMaterial(i_camera, i_light);
    MaterialUniformBuffer mat_ub; //use default color.
    m_mat_sref.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniformBuffer));
    m_mat_sref.GetRef().SetTexture("mainTexture", i_tex_wref);
    //Set data done. Link with shader program.(Write descirptor)
    m_mat_sref.GetRef().LinkWithShaderProgram();
}

void ObjectData::UpdateMaterial(
    const SampleCameraData &i_camera,
    const LightData &i_light)
{
    BasicUniformBuffer ub = {};
    ub.m_clip = Matrix4X4f(gClipMat);
    ub.m_proj = i_camera.m_proj_mat;
    ub.m_view = i_camera.m_trans.MakeViewMatrix();
    ub.m_view_eye = i_camera.m_trans.m_position;
    ub.m_worid = m_trans.MakeWorldMatrix();

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
    m_mat_sref.GetRef().SetDataToUniformBuffer("basic", &ub, sizeof(BasicUniformBuffer));
    m_mat_sref.GetRef().SetDataToUniformBuffer("light", &lb, sizeof(LightUniformBuffer));
    m_mat_sref.GetRef().Update();
}

void ObjectData::Draw(
    const RenderPassWeakReferenceObject &i_rp_wref,
    const CommandBufferWeakReferenceObject &i_cb_wref,
    uint32_t i_sp_id)
{
    if (m_mesh.IsNull() == false) {
        //1. use material.
        m_mat_sref.GetRef().UseMaterial(i_cb_wref, i_rp_wref, i_sp_id);
        //2. bind mesh vertex attributes.
        m_mesh.GetRef().BindVertexBuffers(i_cb_wref);
        //3. draw mesh.
        m_mesh.GetRef().BindIndexBuffer(i_cb_wref);
        m_mesh.GetRef().Render(i_cb_wref);
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
    CreateCamera();
    CreateRenderPass();
    CreateFramebuffer();
    CreateTexture();
    CreateShaderProgram();
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
        obj.UpdateMaterial(m_camera, m_light);
    }

#if defined(RECORD_EVERY_FRAME)
    RecordCommandBuffer();
#endif

    GraphicsManager::GetRef().SubmitCommandBufferToQueue(m_main_cb_wref);
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
    m_phong_shader_sref.Reset();
}

void Sample4_DrawObjects::CreateRenderPass()
{
    //
    SDLOG("Create render pass");
    //1. Initialize Forward render pass.
    m_forward_rp_sref = new RenderPass("ForwardPass");
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

    m_forward_rp_sref.GetRef().AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    m_forward_rp_sref.GetRef().Initialize();
}

void Sample4_DrawObjects::CreateFramebuffer()
{
    SDLOG("Create Framebuffer and render flow.");

    ClearValue clear_color = { 0.15f, 0.15f, 0.75f, 1.0f };
    ClearValue clear_dands = { 1.0f, 1 };

    m_camera.m_forward_rf = new RenderFlow("ForwardPathRF", ImageOffset(0, 0, 0),
        ImageSize(m_current_res.GetWidth(), m_current_res.GetHeight(), 1));
    m_camera.m_forward_rf.GetRef().RegisterRenderPass(m_forward_rp_sref);
    m_camera.m_forward_rf.GetRef().AllocateFrameBuffer();
    m_camera.m_forward_rf.GetRef().RegisterBufferToFrameBuffer(m_camera.m_color_buffer, 0, clear_color);
    m_camera.m_forward_rf.GetRef().RegisterBufferToFrameBuffer(m_camera.m_depth_buffer, 1, clear_dands);
    m_camera.m_forward_rf.GetRef().Initialize();
}

void Sample4_DrawObjects::CreateCommandBufferAndPool()
{
    m_cp_sref = new CommandPool("Sample4_SinglePool");
    m_cp_sref.GetRef().Initialize();
    m_main_cb_wref = m_cp_sref.GetRef().AllocateCommandBuffer();
#if !defined(SINGLE_FLOW)
    uint32_t max_threads = std::thread::hardware_concurrency();
    //max_threads = 1;
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

void Sample4_DrawObjects::CreateShaderProgram()
{
    //-------------------- PhongShaderWithTexture ---------------------------
    m_phong_shader_sref = new ShaderProgram("PhongShaderWithTexture");
    //1. Write uniform descriptor for this shader program.
    UniformBufferDescriptorStrongReferenceObject basic_ubd_sref = new UniformBufferDescriptor("basic", 0);
    basic_ubd_sref.GetRef().AddVariable("clip", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_clip));
    basic_ubd_sref.GetRef().AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_proj));
    basic_ubd_sref.GetRef().AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_view));
    basic_ubd_sref.GetRef().AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_worid));
    basic_ubd_sref.GetRef().AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(BasicUniformBuffer, m_normal));
    basic_ubd_sref.GetRef().AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(BasicUniformBuffer, m_view_eye));
    basic_ubd_sref.GetRef().AddVariableDone();

    UniformBufferDescriptorStrongReferenceObject light_ubd_sref = new UniformBufferDescriptor("light", 1);
    light_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightUniformBuffer, m_ambient));
    light_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightUniformBuffer, m_diffuse));
    light_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightUniformBuffer, m_specular));
    light_ubd_sref.GetRef().AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightUniformBuffer, m_position));
    light_ubd_sref.GetRef().AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightUniformBuffer, m_direction));
    light_ubd_sref.GetRef().AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_spot_exp));
    light_ubd_sref.GetRef().AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_spot_cos_cutoff));
    light_ubd_sref.GetRef().AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_constant_attenuation));
    light_ubd_sref.GetRef().AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_linear_attenuation));
    light_ubd_sref.GetRef().AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniformBuffer, m_quadratic_attenuation));
    light_ubd_sref.GetRef().AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightUniformBuffer, m_kind));
    light_ubd_sref.GetRef().AddVariableDone();

    UniformBufferDescriptorStrongReferenceObject mat_ubd_sref = new UniformBufferDescriptor("material", 2);
    mat_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_ambient));
    mat_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_diffuse));
    mat_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_specular));
    mat_ubd_sref.GetRef().AddVariable("position", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniformBuffer, m_emission));
    mat_ubd_sref.GetRef().AddVariable("direction", UniformBufferVariableType_FLOAT, offsetof(MaterialUniformBuffer, m_shineness));
    mat_ubd_sref.GetRef().AddVariableDone();

    UniformImagesDescriptorStrongReferenceObject mt_imgd_sref = new UniformImagesDescriptor("mainTexture", 3);

    std::vector<UniformVariableDescriptorStrongReferenceObject> uvd_srefs;
    uvd_srefs.push_back(basic_ubd_sref.StaticCastTo<UniformVariableDescriptor>());
    uvd_srefs.push_back(light_ubd_sref.StaticCastTo<UniformVariableDescriptor>());
    uvd_srefs.push_back(mat_ubd_sref.StaticCastTo<UniformVariableDescriptor>());
    uvd_srefs.push_back(mt_imgd_sref.StaticCastTo<UniformVariableDescriptor>());
    
    //2. Create GraphicsPipeline.
    //2.1 create shader module.
    ShaderModules shader_modules;
    ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("PhongShaderVert");
    vert_shader_sref.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/PhongShader.vert.spv", "main");
    ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("PhongShaderFrag");
    frag_shader_sref.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/PhongShader.frag.spv", "main");
    shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader_sref;
    shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader_sref;

    //2.2 write pipeline parames.
    GraphicsPipelineParam params;
    params.m_primitive_info.m_primitive = Primitive_TRIANGLE;
    params.m_depth_stencil_info.m_depth_test_enable = true;
    params.m_rasterization_info.m_face_culling = FaceCulling_NONE;
    params.m_attachment_blend_state.m_blend_infos.resize(1); //blend default false.
    params.m_dynamic_states.push_back(DynamicState_VIEWPORT);
    params.m_dynamic_states.push_back(DynamicState_SCISSOR);
    GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, 2);
 
    params.m_uniform_binding_infos.resize(4);
    params.m_uniform_binding_infos[0].m_binding_id = 0;
    params.m_uniform_binding_infos[0].m_binding_type = UniformBindingType_UNIFORM_BUFFER;
    params.m_uniform_binding_infos[0].m_element_number = 1;
    params.m_uniform_binding_infos[0].m_target_stages.push_back(ShaderStage_GRAPHICS_ALL);

    params.m_uniform_binding_infos[1].m_binding_id = 1;
    params.m_uniform_binding_infos[1].m_binding_type = UniformBindingType_UNIFORM_BUFFER;
    params.m_uniform_binding_infos[1].m_element_number = 1;
    params.m_uniform_binding_infos[1].m_target_stages.push_back(ShaderStage_GRAPHICS_ALL);

    params.m_uniform_binding_infos[2].m_binding_id = 2;
    params.m_uniform_binding_infos[2].m_binding_type = UniformBindingType_UNIFORM_BUFFER;
    params.m_uniform_binding_infos[2].m_element_number = 1;
    params.m_uniform_binding_infos[2].m_target_stages.push_back(ShaderStage_GRAPHICS_ALL);

    params.m_uniform_binding_infos[3].m_binding_id = 3;
    params.m_uniform_binding_infos[3].m_binding_type = UniformBindingType_COMBINED_IMAGE_SAMPLER;
    params.m_uniform_binding_infos[3].m_element_number = 1;
    params.m_uniform_binding_infos[3].m_target_stages.push_back(ShaderStage_GRAPHICS_ALL);

    //2.3 register uniform variable descriptor to pipeline.
    GraphicsPipelineStrongReferenceObject pipeline_sref = new GraphicsPipeline("PhongShader_Forward");
    pipeline_sref.GetRef().SetGraphicsPipelineParams(params, m_forward_rp_sref, 0);
    for (uint32_t uvd_count = 0; uvd_count < uvd_srefs.size(); ++uvd_count) {
        pipeline_sref.GetRef().RegisterUniformVariableDescriptor(uvd_srefs[uvd_count], uvd_count);
    }
    pipeline_sref.GetRef().Initialize(shader_modules);

    //2.4 prepare datas and then initalize shader structure.
    std::vector<GraphicsPipelineStrongReferenceObject> pipe_srefs;
    pipe_srefs.push_back(pipeline_sref);

    std::vector<RenderPassGroup> rpgs;
    RenderPassGroup forward_rp;
    forward_rp.m_rp_wref = m_forward_rp_sref;
    forward_rp.m_pipe_orders.push_back(0); //use pipeline 0 at sp0.
    rpgs.push_back(forward_rp);
    m_phong_shader_sref.GetRef().RegisterShaderProgramStructure(
        rpgs, pipe_srefs, uvd_srefs);
}

void Sample4_DrawObjects::CreateTexture()
{
    m_tex_sref = new Texture("MainTexture");
    m_tex_sref.GetRef().SetSamplerFilterType(SamplerFilterType_LINEAR, SamplerFilterType_LINEAR);
    m_tex_sref.GetRef().InitializeFromImageResource("Texture/Lenna.png");
}

void Sample4_DrawObjects::CreateObjects()
{
    std::list<ObjectData>::reverse_iterator last_obj_iter;
    //Scene ---- Plane.
    m_floor_sref = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX, 100.0f, 100.0f, 100.0f, 100.0f);
    m_scene_objects.push_back(ObjectData());
    last_obj_iter = m_scene_objects.rbegin();
    (*last_obj_iter).m_mesh = m_floor_sref;
    Vector3f start_pos = Vector3f(m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, 1.0f).scale(
        -1.0f * static_cast<float>(m_cube_row) / 2.0f,
        0.25f,
        -1.0f * static_cast<float>(m_cube_depth) / 2.0f);
    (*last_obj_iter).InitalizeMaterial(m_phong_shader_sref, m_camera, m_light, m_tex_sref);
    (*last_obj_iter).UpdateMaterial(m_camera, m_light);

    //Scene ---- Cubes
    m_cube_sref = BasicShapeCreator::GetRef().CreateCube(Vector3f::Zero, Vector3f(0.25f, 0.25f, 0.25f));
    for (uint32_t row = 0; row < m_cube_row; ++row) {
        for (uint32_t col = 0; col < m_cube_col; ++col) {
            for (uint32_t depth = 0; depth < m_cube_depth; ++depth) {
                m_scene_objects.push_back(ObjectData());
                last_obj_iter = m_scene_objects.rbegin();
                (*last_obj_iter).m_mesh = m_cube_sref;
                (*last_obj_iter).m_texture = m_tex_sref;
                (*last_obj_iter).m_trans.m_position = start_pos + 
                    Vector3f((m_cube_side_length + m_cube_interval) * row, (m_cube_side_length + m_cube_interval) * col, (m_cube_side_length + m_cube_interval) * depth);
                //
                (*last_obj_iter).InitalizeMaterial(m_phong_shader_sref, m_camera, m_light, m_tex_sref);
                (*last_obj_iter).UpdateMaterial(m_camera, m_light);
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
    Viewport vp;
    vp.m_x = 0.0f; vp.m_y = 0.0f;
    vp.m_width = static_cast<float>(m_current_res.GetWidth());
    vp.m_height = static_cast<float>(m_current_res.GetHeight());
    vp.m_min_depth = 0.0f;
    vp.m_max_depth = 1.0f;

    ScissorRegion sr;
    sr.m_x = 0.0f; sr.m_y = 0.0f;
    sr.m_width = vp.m_width; sr.m_height = vp.m_height;

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

    for (ObjectData& obj : m_scene_objects) {
        obj.Draw(m_mgr, m_main_cb_wref);
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
            obj_ref->Draw(m_forward_rp_sref, i_cb_wref, 0);
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

void Sample4_DrawObjects::UpdateCamera()
{
    if (Application::GetRef().GetKeyStateByCode(KEY_A) == KEY_STATUS_PRESS) {
        m_camera.m_trans.AddRotation(m_camera.m_trans.GetTop(), Timer::GetRef().GetProgramDeltaTime() * 10.0f);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_D) == KEY_STATUS_PRESS) {
        m_camera.m_trans.AddRotation(m_camera.m_trans.GetTop(), Timer::GetRef().GetProgramDeltaTime() * -10.0f);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_W) == KEY_STATUS_PRESS) {
        //Camera forward is -z axis.
        Vector3f offset = m_camera.m_trans.GetForward().negative().scale(Timer::GetRef().GetProgramDeltaTime() * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_S) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetForward().scale(Timer::GetRef().GetProgramDeltaTime() * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_Q) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetRight().negative().scale(Timer::GetRef().GetProgramDeltaTime() * 1.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_E) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetRight().scale(Timer::GetRef().GetProgramDeltaTime() * 1.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }
   
    if (Application::GetRef().GetKeyStateByCode(KEY_R) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetTop().scale(Timer::GetRef().GetProgramDeltaTime() * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_F) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetTop().negative().scale(Timer::GetRef().GetProgramDeltaTime() * 2.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }
}