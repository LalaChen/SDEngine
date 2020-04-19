#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "Sample4_DrawObjects.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

ObjectData::ObjectData()
{
}

ObjectData::~ObjectData()
{
}

SampleCameraData::SampleCameraData()
{
}

SampleCameraData::~SampleCameraData()
{
}

Sample4_DrawObjects::Sample4_DrawObjects(VulkanAPITestManager *i_mgr)
: Sample("DrawObjects", i_mgr)
, m_cube_side_length(0.25f)
, m_cube_row(5)
, m_cube_col(5)
, m_cube_depth(5)
{

}

Sample4_DrawObjects::~Sample4_DrawObjects()
{
}

void Sample4_DrawObjects::Initialize()
{
    CreateTexture();
    CreateObjects();
    CreateCamera();
    CreateRenderPassAndFramebuffer();
    CreatePipeline();
}

void Sample4_DrawObjects::Render()
{
}

void Sample4_DrawObjects::Resize(Size_ui32 i_width, Size_ui32 i_height)
{
    m_camera.m_color_buffer.Reset();
    m_camera.m_depth_buffer.Reset();
    m_camera.m_forward_rf.Reset();

    m_current_res = m_mgr->GetScreenResolution();
    CreateCamera();
    CreateRenderPassAndFramebuffer();
}

void Sample4_DrawObjects::Destroy()
{
    for (std::list<ObjectData>::iterator obj_iter = m_cube_objects.begin(); obj_iter != m_cube_objects.end();) {
        obj_iter = m_cube_objects.erase(obj_iter);
    }

    m_camera.m_color_buffer.Reset();
    m_camera.m_depth_buffer.Reset();
    m_camera.m_forward_rf.Reset();
    m_tex_sref.Reset();
    m_forward_rp_sref.Reset();
    m_pipeline_sref.Reset();
}

void Sample4_DrawObjects::CreateRenderPassAndFramebuffer()
{
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
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_STORE;
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

    ClearValue clear_color = { 0.15f, 0.15f, 0.15f, 1.0f };
    ClearValue clear_dands;
    clear_dands.depth_stencil.depth = 1.0f; clear_dands.depth_stencil.stencil = 1;

    m_forward_rp_sref.GetRef().AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    m_forward_rp_sref.GetRef().Initialize();
    m_camera.m_forward_rf = new RenderFlow("ForwardPathRF", ImageOffset(0, 0, 0), 
        ImageSize(m_current_res.GetWidth(), m_current_res.GetHeight(), 0));
    m_camera.m_forward_rf.GetRef().RegisterRenderPass(m_forward_rp_sref);
    m_camera.m_forward_rf.GetRef().AllocateFrameBuffer();
    m_camera.m_forward_rf.GetRef().RegisterBufferToFrameBuffer(m_camera.m_color_buffer, 0, clear_color);
    m_camera.m_forward_rf.GetRef().RegisterBufferToFrameBuffer(m_camera.m_depth_buffer, 1, clear_dands);
    m_camera.m_forward_rf.GetRef().Initialize();
}

void Sample4_DrawObjects::CreateCommandBufferAndPool()
{
}

void Sample4_DrawObjects::CreateTexture()
{
    m_tex_sref = new Texture("MainTexture");
    m_tex_sref.GetRef().InitializeFromImageResource("Texture/Lenna.png");
}

void Sample4_DrawObjects::CreateObjects()
{
    std::list<ObjectData>::reverse_iterator last_obj_iter;
    //Scene ---- Plane.
    m_cube_objects.push_back(ObjectData());
    last_obj_iter = m_cube_objects.rbegin();
    (*last_obj_iter).m_mesh = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX, 100.0f, 100.0f);
    //Scene ---- Cubes
    Vector3f start_pos = Vector3f(m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, 1.0f).scale(
        -1.0f * static_cast<float>(m_cube_row) / 2.0f,
        0.0f,
        -1.0f * static_cast<float>(m_cube_depth) / 2.0f);

    for (uint32_t row = 0; row < m_cube_row; ++row) {
        for (uint32_t col = 0; col < m_cube_col; ++col) {
            for (uint32_t depth = 0; depth < m_cube_depth; ++depth) {
                m_cube_objects.push_back(ObjectData());
                last_obj_iter = m_cube_objects.rbegin();
                (*last_obj_iter).m_mesh = BasicShapeCreator::GetRef().CreateCube(Vector3f::Zero, Vector3f(0.25f, 0.25f, 0.25f));
                (*last_obj_iter).m_texture = m_tex_sref;
                (*last_obj_iter).m_trans.m_position = start_pos + 
                    Vector3f(m_cube_side_length * row, m_cube_side_length * col, m_cube_side_length * depth);
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
    m_pipeline_sref = new GraphicsPipeline("PhongShader_Forward");
    m_pipeline_sref.GetRef().SetGraphicsPipelineParams(params, m_forward_rp_sref, 0);
    m_pipeline_sref.GetRef().Initialize(shader_modules);

}