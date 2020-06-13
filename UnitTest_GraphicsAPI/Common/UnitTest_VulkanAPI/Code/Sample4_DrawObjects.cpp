#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample4_DrawObjects.h"

#include "UniformBindingType_Vulkan.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

float gClipMat[16] = {
    1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f,  0.0f, 1.0f, 0.0f,
    0.0f,  0.0f, 0.0f, 1.0f };

VkDescriptorType DescriptorTypes[UniformBindingType_MAX_DEFINE_VALUE] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
};

ObjectMaterialData::ObjectMaterialData()
: m_desc_pool(VK_NULL_HANDLE)
, m_desc_set(VK_NULL_HANDLE)
, m_basic_ub(VK_NULL_HANDLE)
, m_basic_mem(VK_NULL_HANDLE)
, m_light_ub(VK_NULL_HANDLE)
, m_light_mem(VK_NULL_HANDLE)
, m_material_ub(VK_NULL_HANDLE)
, m_material_mem(VK_NULL_HANDLE)
{
}

ObjectMaterialData::~ObjectMaterialData()
{
}

void ObjectMaterialData::Initialize(VulkanAPITestManager *i_mgr, const GraphicsPipelineWeakReferenceObject &i_pipeline_wref, const TextureWeakReferenceObject &i_main_tex_wref)
{
    VkResult result = VK_SUCCESS;
    VkDescriptorSetLayout layout = reinterpret_cast<VkDescriptorSetLayout>(i_pipeline_wref.GetConstRef().GetDescriptorLayoutHandle());
    const GraphicsPipelineParam &params = i_pipeline_wref.GetConstRef().GetPipelineParams();
    m_pipeline_wref = i_pipeline_wref;
    m_main_tex_wref = i_main_tex_wref;
    //1. caculate uniform variable types and their numbers.
    uint32_t type_numbers[UniformBindingType_MAX_DEFINE_VALUE] = {0};

    for (const UniformBinding &binding: params.m_uniform_binding_infos) {
        type_numbers[binding.m_binding_type]++;
    }
    //2. Create descriptor pool size.
    //------ VkDescriptorPoolSize(Light and Basic Buffer. Both 1)
    std::vector<VkDescriptorPoolSize> type_sizes;
    type_sizes.resize(UniformBindingType_MAX_DEFINE_VALUE);

    for (uint32_t type_id = 0; type_id < UniformBindingType_MAX_DEFINE_VALUE; ++type_id) {
        type_sizes[type_id] = {};
        type_sizes[type_id].type = DescriptorTypes[static_cast<UniformBindingTypeEnum>(type_id)];
        type_sizes[type_id].descriptorCount = type_numbers[type_id];
    }
    //3. Create descriptor.
    result = i_mgr->CreateDescriptorPool(type_sizes, 1, false, m_desc_pool);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate descriptror pool failure!!!");
        return;
    }

    //4. allocate descriptor set.
    VkDescriptorSetAllocateInfo desc_set_a_info = {};
    desc_set_a_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    desc_set_a_info.pNext = nullptr;
    desc_set_a_info.descriptorPool = m_desc_pool;
    desc_set_a_info.descriptorSetCount = 1;
    desc_set_a_info.pSetLayouts = &layout;
    result = i_mgr->AllocateDescriptorSet(desc_set_a_info, m_desc_set);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate descriptor set failure!!!");
        return;
    }

    //5. create uniform buffer.
    result = i_mgr->CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, sizeof(BasicUniformBuffer), m_basic_ub);
    if (result != VK_SUCCESS) {
        SDLOGE("Create basic uniform buffer failure!!!");
        return;
    }
    result = i_mgr->AllocateMemoryAndBindToBuffer(
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0, m_basic_ub, m_basic_mem);
    if (result != VK_SUCCESS) {
        SDLOGE("Create basic uniform buffer memory failure!!!");
    }

    result = i_mgr->CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, sizeof(LightUniformBuffer), m_light_ub);
    if (result != VK_SUCCESS) {
        SDLOGE("Create light uniform buffer failure!!!");
        return;
    }

    result = i_mgr->AllocateMemoryAndBindToBuffer(
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0, m_light_ub, m_light_mem);
    if (result != VK_SUCCESS) {
        SDLOGE("Create light uniform buffer memory failure!!!");
        return;
    }

    result = i_mgr->CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, sizeof(MaterialUniformBuffer), m_material_ub);
    if (result != VK_SUCCESS) {
        SDLOGE("Create material uniform buffer failure!!!");
        return;
    }

    result = i_mgr->AllocateMemoryAndBindToBuffer(
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0, m_material_ub, m_material_mem);
    if (result != VK_SUCCESS) {
        SDLOGE("Create material uniform buffer memory failure!!!");
        return;
    }

    //6. create write descriptor set infos.
    //6.1 for uniform buffer.
    VkDescriptorBufferInfo basic_uniform_b_info = {};
    basic_uniform_b_info.buffer = m_basic_ub;
    basic_uniform_b_info.offset = 0;
    basic_uniform_b_info.range = sizeof(BasicUniformBuffer);
    VkWriteDescriptorSet basic_set_info = {};
    basic_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    basic_set_info.pNext = nullptr;
    basic_set_info.dstSet = m_desc_set;
    basic_set_info.dstBinding = 0; //binding 0, set 0
    basic_set_info.descriptorCount = 1;
    basic_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    basic_set_info.pBufferInfo = &basic_uniform_b_info;
    basic_set_info.pImageInfo = nullptr;
    basic_set_info.pTexelBufferView = nullptr;
    basic_set_info.dstArrayElement = 0;

    VkDescriptorBufferInfo light_uniform_b_info = {};
    light_uniform_b_info.buffer = m_light_ub;
    light_uniform_b_info.offset = 0;
    light_uniform_b_info.range = sizeof(LightUniformBuffer);
    VkWriteDescriptorSet light_set_info = {};
    light_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    light_set_info.pNext = nullptr;
    light_set_info.dstSet = m_desc_set;
    light_set_info.dstBinding = 1; //binding 1, set 0
    light_set_info.descriptorCount = 1;
    light_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    light_set_info.pBufferInfo = &light_uniform_b_info;
    light_set_info.pImageInfo = nullptr;
    light_set_info.pTexelBufferView = nullptr;
    light_set_info.dstArrayElement = 0;

    VkDescriptorBufferInfo mat_uniform_b_info = {};
    mat_uniform_b_info.buffer = m_material_ub;
    mat_uniform_b_info.offset = 0;
    mat_uniform_b_info.range = sizeof(MaterialUniformBuffer);
    VkWriteDescriptorSet mat_set_info = {};
    mat_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    mat_set_info.pNext = nullptr;
    mat_set_info.dstSet = m_desc_set;
    mat_set_info.dstBinding = 2; //binding 2, set 0
    mat_set_info.descriptorCount = 1;
    mat_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    mat_set_info.pBufferInfo = &mat_uniform_b_info;
    mat_set_info.pImageInfo = nullptr;
    mat_set_info.pTexelBufferView = nullptr;
    mat_set_info.dstArrayElement = 0;

    //6.2 for texture.
    VkDescriptorImageInfo main_texture_i_info = {};
    main_texture_i_info.sampler = reinterpret_cast<VkSampler>(i_main_tex_wref.GetConstRef().GetSamplerHandle());
    main_texture_i_info.imageView = reinterpret_cast<VkImageView>(i_main_tex_wref.GetConstRef().GetViewHandle());
    main_texture_i_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkWriteDescriptorSet main_tex_set_info = {};
    main_tex_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    main_tex_set_info.pNext = nullptr;
    main_tex_set_info.dstSet = m_desc_set;
    main_tex_set_info.dstBinding = 3; //binding 3, set 0
    main_tex_set_info.descriptorCount = 1;
    main_tex_set_info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    main_tex_set_info.pBufferInfo = nullptr;
    main_tex_set_info.pImageInfo = &main_texture_i_info;
    main_tex_set_info.pTexelBufferView = nullptr;
    main_tex_set_info.dstArrayElement = 0;

    std::vector<VkWriteDescriptorSet> descriptor_set_infos = {
        basic_set_info,
        light_set_info,
        mat_set_info,
        main_tex_set_info
    };
    i_mgr->UpdateDescriptorSet(descriptor_set_infos);

    i_mgr->RefreshHostDeviceBufferData(m_material_ub, m_material_mem, &m_material, sizeof(MaterialUniformBuffer));
}

void ObjectMaterialData::Release(VulkanAPITestManager* i_mgr)
{
    if (m_desc_set != VK_NULL_HANDLE) {
        m_desc_set = VK_NULL_HANDLE; //free by pool.
    }

    if (m_desc_pool != VK_NULL_HANDLE) {
        i_mgr->DestroyDescriptorPool(m_desc_pool);
        m_desc_pool = VK_NULL_HANDLE;
    }

    if (m_basic_ub != VK_NULL_HANDLE) {
        i_mgr->DestroyBuffer(m_basic_ub);
        m_basic_ub = VK_NULL_HANDLE;
    }

    if (m_light_ub != VK_NULL_HANDLE) {
        i_mgr->DestroyBuffer(m_light_ub);
        m_light_ub = VK_NULL_HANDLE;
    }

    if (m_material_ub != VK_NULL_HANDLE) {
        i_mgr->DestroyBuffer(m_material_ub);
        m_material_ub = VK_NULL_HANDLE;
    }

    if (m_basic_mem != VK_NULL_HANDLE) {
        i_mgr->ReleaseMemory(m_basic_mem);
        m_basic_mem = VK_NULL_HANDLE;
    }

    if (m_light_mem != VK_NULL_HANDLE) {
        i_mgr->ReleaseMemory(m_light_mem);
        m_light_mem = VK_NULL_HANDLE;
    }

    if (m_material_mem != VK_NULL_HANDLE) {
        i_mgr->ReleaseMemory(m_material_mem);
        m_material_mem = VK_NULL_HANDLE;
    }
}

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

void ObjectData::UpdateMaterial(VulkanAPITestManager *i_mgr, const SampleCameraData &i_camera, const LightData &i_light)
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
    i_mgr->RefreshHostDeviceBufferData(m_material.m_basic_ub, m_material.m_basic_mem, &ub, sizeof(BasicUniformBuffer));
    i_mgr->RefreshHostDeviceBufferData(m_material.m_light_ub, m_material.m_light_mem, &lb, sizeof(LightUniformBuffer));
}

void ObjectData::Draw(VulkanAPITestManager* i_mgr, const CommandBufferWeakReferenceObject &i_cb_wref)
{
   //1. use program.
   m_material.m_pipeline_wref.GetRef().Use(i_cb_wref);
   //2. bind descriptor set.
   std::vector<VkDescriptorSet> descs = { m_material.m_desc_set };
   std::vector<uint32_t> dynamic_offs = {};
   i_mgr->BindDescriptorSets(
        reinterpret_cast<VkCommandBuffer>(i_cb_wref.GetConstRef().GetHandle()),
        reinterpret_cast<VkPipelineLayout>(m_material.m_pipeline_wref.GetConstRef().GetPipelineLayoutHandle()),
        VK_PIPELINE_BIND_POINT_GRAPHICS, 0, descs, dynamic_offs);
   //3. bind mesh vertex attributes.
   m_mesh.GetRef().BindVertexBuffers(i_cb_wref);
   //4. draw mesh.
   m_mesh.GetRef().BindIndexBuffer(i_cb_wref);
   m_mesh.GetRef().Render(i_cb_wref);
}


Sample4_DrawObjects::Sample4_DrawObjects(VulkanAPITestManager *i_mgr)
: Sample("DrawObjects", i_mgr)
, m_cube_interval(0.05f)
, m_cube_side_length(0.25f)
, m_cube_row(10)
, m_cube_col(10)
, m_cube_depth(10)
{

}

Sample4_DrawObjects::~Sample4_DrawObjects()
{
}

void Sample4_DrawObjects::Initialize()
{
    m_current_res = m_mgr->GetScreenResolution();
    CreateRenderPassAndFramebuffer();
    CreatePipeline();
    CreateTexture();
    CreateLight();
    CreateObjects();
    CreateCommandBufferAndPool();
    //
    RecordCommandBuffer();
}

void Sample4_DrawObjects::Render()
{
    //TimerMeasurer tm("OneFrame");
    //tm.Start();

    UpdateCamera();
    for (ObjectData &obj : m_scene_objects) {
        obj.UpdateMaterial(m_mgr, m_camera, m_light);
    }
    //tm.Record();
#if defined(RECORD_EVERY_FRAME)
    RecordCommandBuffer();
#endif


    GraphicsManager::GetRef().SubmitCommandBufferToQueue(m_main_cb_wref);
    //tm.Stop();
    //SDLOG("TimeMeasure : %s", tm.ToString().c_str());
}

void Sample4_DrawObjects::Resize(Size_ui32 i_width, Size_ui32 i_height)
{
    m_camera.m_color_buffer.Reset();
    m_camera.m_depth_buffer.Reset();
    m_camera.m_forward_rf.Reset();

    m_current_res = m_mgr->GetScreenResolution();
    CreateRenderPassAndFramebuffer();
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
    m_pipeline_sref.Reset();
    for (CommandBufferWeakReferenceObject &cmd_wref : m_secondary_cb_wrefs) {
        m_cmd_pool_sref.GetRef().RecycleCommandBuffer(cmd_wref);
    }
    m_cmd_pool_sref.GetRef().RecycleCommandBuffer(m_main_cb_wref);
    m_cmd_pool_sref.Reset();
}

void Sample4_DrawObjects::CreateRenderPassAndFramebuffer()
{
    //
    CreateCamera();
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

    ClearValue clear_color = { 0.15f, 0.15f, 0.75f, 1.0f };
    ClearValue clear_dands = {1.0f, 1};

    m_forward_rp_sref.GetRef().AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    m_forward_rp_sref.GetRef().Initialize();
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
    m_cmd_pool_sref = new CommandPool("Sample4_SinglePool");
    m_cmd_pool_sref.GetRef().Initialize();
    m_main_cb_wref = m_cmd_pool_sref.GetRef().AllocateCommandBuffer();
#if !defined(SINGLE_FLOW)
    for (uint32_t oID = 0; oID < m_scene_objects.size(); ++oID) {
        m_secondary_cb_wrefs.push_back(m_cmd_pool_sref.GetRef().AllocateCommandBuffer(StringFormat("CommandBuffer%d",oID), CommandBufferLevel_SECONDARY));
    }
#endif
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
    m_scene_objects.push_back(ObjectData());
    last_obj_iter = m_scene_objects.rbegin();
    (*last_obj_iter).m_mesh = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX, 100.0f, 100.0f, 100.0f, 100.0f);
    Vector3f start_pos = Vector3f(m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, 1.0f).scale(
        -1.0f * static_cast<float>(m_cube_row) / 2.0f,
        0.25f,
        -1.0f * static_cast<float>(m_cube_depth) / 2.0f);
    (*last_obj_iter).m_material.Initialize(m_mgr, m_pipeline_sref, m_tex_sref);
    (*last_obj_iter).UpdateMaterial(m_mgr, m_camera, m_light);

    //Scene ---- Cubes
    for (uint32_t row = 0; row < m_cube_row; ++row) {
        for (uint32_t col = 0; col < m_cube_col; ++col) {
            for (uint32_t depth = 0; depth < m_cube_depth; ++depth) {
                m_scene_objects.push_back(ObjectData());
                last_obj_iter = m_scene_objects.rbegin();
                (*last_obj_iter).m_mesh = BasicShapeCreator::GetRef().CreateCube(Vector3f::Zero, Vector3f(0.25f, 0.25f, 0.25f));
                (*last_obj_iter).m_texture = m_tex_sref;
                (*last_obj_iter).m_trans.m_position = start_pos + 
                    Vector3f((m_cube_side_length + m_cube_interval) * row, (m_cube_side_length + m_cube_interval) * col, (m_cube_side_length + m_cube_interval) * depth);
                //
                (*last_obj_iter).m_material.Initialize(m_mgr, m_pipeline_sref, m_tex_sref);
                (*last_obj_iter).UpdateMaterial(m_mgr, m_camera, m_light);
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
        Vector3f(0.0f, 1.5f, 7.0f, 1.0f),
        Vector3f::Origin,
        Vector3f::PositiveY,
        true);
    SDLOG("%s", m_camera.m_trans.MakeWorldMatrix().ToFormatString("Camera", "").c_str());
}

void Sample4_DrawObjects::CreateLight()
{
    m_light.m_trans = Transform::LookAt(Vector3f(1.0f, 3.0f, 1.0f, 1.0f), Vector3f::Origin, Vector3f::PositiveY);
    SDLOG("%s",m_light.m_trans.MakeWorldMatrix().ToFormatString("Light","").c_str());
}

void Sample4_DrawObjects::CreatePipeline()
{
    ShaderModules shader_modules;
    ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("PhongShaderVert");
    vert_shader_sref.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/PhongShader.vert.spv", "main");
    ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("PhongShaderFrag");
    frag_shader_sref.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/PhongShader.frag.spv", "main");
    shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader_sref;
    shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader_sref;

    GraphicsPipelineParam params;
    params.m_primitive_info.m_primitive = Primitive_TRIANGLE;
    params.m_depth_stencil_info.m_depth_test_enable = true;
    params.m_rasterization_info.m_face_culling = FaceCulling_NONE;
    params.m_attachment_blend_state.m_blend_infos.resize(1); //blend default false.
    params.m_dynamic_states.push_back(DynamicState_VIEWPORT);
    params.m_dynamic_states.push_back(DynamicState_SCISSOR);
    GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, 2);
    //
    params.m_uniform_binding_infos.resize(4);
    //
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
  
    m_pipeline_sref = new GraphicsPipeline("PhongShader_Forward");
    m_pipeline_sref.GetRef().SetGraphicsPipelineParams(params, m_forward_rp_sref, 0);
    m_pipeline_sref.GetRef().Initialize(shader_modules);
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
    //1. Begin Command Buffer
    m_main_cb_wref.GetRef().Begin();
    m_camera.m_forward_rf.GetRef().BeginRenderFlow(m_main_cb_wref);
    GraphicsManager::GetRef().SetViewport(m_main_cb_wref, vp);
    GraphicsManager::GetRef().SetScissor(m_main_cb_wref, sr);

    for (ObjectData& obj : m_scene_objects) {
        obj.Draw(m_mgr, m_cmd_buf_wrefs[0]);
    }

    m_camera.m_forward_rf.GetRef().EndRenderFlow(m_main_cb_wref);
    m_main_cb_wref.GetRef().End();
#else
    m_main_cb_wref.GetRef().Begin();
    m_camera.m_forward_rf.GetRef().BeginRenderFlow(m_main_cb_wref);
    uint32_t oID = 0u;
    for (std::list<ObjectData>::iterator obj_iter = m_scene_objects.begin(); obj_iter != m_scene_objects.end(); ++obj_iter, ++oID) {
        CommandBufferInheritanceInfo cb_inher_info = m_camera.m_forward_rf.GetRef().GetCurrentInheritanceInfo();
        m_secondary_cb_wrefs[oID].GetRef().Begin(cb_inher_info);
        GraphicsManager::GetRef().SetViewport(m_secondary_cb_wrefs[oID], vp);
        GraphicsManager::GetRef().SetScissor(m_secondary_cb_wrefs[oID], sr);
        (*obj_iter).Draw(m_mgr, m_main_cb_wref);
        m_secondary_cb_wrefs[oID].GetRef().End();
    }
    GraphicsManager::GetRef().ExecuteCommandsToPrimaryCommandBuffer(m_main_cb_wref, m_secondary_cb_wrefs);

    m_camera.m_forward_rf.GetRef().EndRenderFlow(m_main_cb_wref);
    m_main_cb_wref.GetRef().End();
#endif
}

void Sample4_DrawObjects::UpdateCamera()
{
    if (Application::GetRef().GetKeyStateByCode(KEY_A) == KEY_STATUS_PRESS) {
        m_camera.m_trans.AddRotation(m_camera.m_trans.GetTop(), Timer::GetRef().GetProgramDeltaTime() * -5.0f);
    }
    if (Application::GetRef().GetKeyStateByCode(KEY_D) == KEY_STATUS_PRESS) {
        m_camera.m_trans.AddRotation(m_camera.m_trans.GetTop(), Timer::GetRef().GetProgramDeltaTime() * 5.0f);
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
        Vector3f offset = m_camera.m_trans.GetRight().scale(Timer::GetRef().GetProgramDeltaTime() * 1.0f);//speed 2m/s
        m_camera.m_trans.AddTranslation(offset);
    }
    if (Application::GetRef().GetKeyStateByCode(KEY_E) == KEY_STATUS_PRESS) {
        Vector3f offset = m_camera.m_trans.GetRight().negative().scale(Timer::GetRef().GetProgramDeltaTime() * 1.0f);//speed 2m/s
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