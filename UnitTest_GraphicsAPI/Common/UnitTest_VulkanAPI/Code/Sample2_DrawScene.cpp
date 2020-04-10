//#define USE_HOST_BUFFER
#include <chrono>
#include <thread>

#include "VulkanAPITestManager.h"
#include "AssimpModelLoader.h"
#include "Sample2_DrawScene.h"

using namespace::SDE::Math;
using namespace::SDE::Basic;
using namespace::SDE::Graphics;

Sample2_DrawScene::Sample2_DrawScene(VulkanAPITestManager *i_mgr)
: Sample("DrawScene", i_mgr)
//for camera
, m_VK_cmd_pool(VK_NULL_HANDLE)
, m_VK_cmd_buffer(VK_NULL_HANDLE)
{
}

Sample2_DrawScene::~Sample2_DrawScene()
{
}

void Sample2_DrawScene::Initialize()
{
    CreateCommandBufferAndPool();
    CreateRenderPassAndFramebuffer();
    //
    CreateModel();
    CreateShaderProgram();
}

void Sample2_DrawScene::Render()
{
}

void Sample2_DrawScene::Resize(Size_ui32 i_width, Size_ui32 i_height)
{

}

void Sample2_DrawScene::Destroy()
{
    m_meshes.clear();
    m_render_flow_sref.reset();
    m_color_buffer_sref.reset();
    m_depth_buffer_sref.reset();
}

void Sample2_DrawScene::ImportAssimpModel(ModelData &io_model)
{
    SDLOG("---Import AssimpModel!!!");
    AssimpModelLoader::GetRef().ImportModel("resources\\models\\Building02\\Building02_m.FBX", io_model);
    //AssimpModelLoader::GetRef().ImportModel("resources\\models\\Build11_fbx\\Build11_fbx.FBX", io_model);
}

void Sample2_DrawScene::CreateModel()
{
    SDLOG("Create Meshes!!!");
    ModelData model;
    ImportAssimpModel(model);
    //1. Load meshes.
    for (uint32_t meshID = 0; meshID < model.m_mesh_datas.size(); ++meshID) {
        MeshStrongReferenceObject mesh_sref = new Mesh(model.m_mesh_datas[meshID].m_name);
        uint32_t vaID;
        //--- Vertex Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_VERTEX_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32_SFLOAT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        //--- Normal Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_NORMAL_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32_SFLOAT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_NORMAL_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        //--- Tangent Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_TANGENT_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32_SFLOAT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_TANGENT_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        //--- Binormal Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_BINORMAL_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32_SFLOAT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_BINORMAL_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        //--- Tex Coord Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_TEX_COORD_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32_SFLOAT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        //--- Color Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_COLOR_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_COLOR_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32W32_SFLOAT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_COLOR_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_COLOR_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_COLOR_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        //--- Indice Buffer
        if (model.m_mesh_datas[meshID].m_face_indices.size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_ELEMENT_BUFFER);
            StaticVertexBufferStrongReferenceObject va_sref = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32_UINT);
            va_sref.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_ELEMENT_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_ELEMENT_BUFFER].size());
            mesh_sref.GetRef().RegisterVertexBuffer(VertexBufferUsage_ELEMENT_BUFFER, va_sref.StaticCastTo<VertexBuffer>());
        }
        m_meshes.push_back(mesh_sref);
    }
    //2. Load texture.
    for (TextureMap::const_iterator tex_iter = model.m_textures.begin();
        tex_iter != model.m_textures.end();
        tex_iter++) {
        TextureResourceMap::iterator res_iter = m_textures.find((*tex_iter).first);
        if (res_iter == m_textures.end()) {
            TextureStrongReferenceObject tex_sref = new Texture(StringFormat("%s_%s", model.m_name.c_str(), (*tex_iter).first.c_str()));
            tex_sref.GetRef().InitializeFromBitmap((*tex_iter).second);
        }
    }
}

void Sample2_DrawScene::CreateShaderProgram()
{
    SDLOG("Create Render Flow!!!");
    VkResult result = VK_SUCCESS;
    //1. bind vertex attribute array.
    //(such like glVertexAttribBinding and glVertexAttribFormat.)
    //1.1. bind pointer.
    //one buffer one binding. (glVertexAttribFormat)
    VkVertexInputBindingDescription vert_input_binding_des_infos[3] = {
        {}, {}, {}
    };
    vert_input_binding_des_infos[0].binding = 0;
    vert_input_binding_des_infos[0].stride = sizeof(vec3);
    vert_input_binding_des_infos[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    vert_input_binding_des_infos[1].binding = 1;
    vert_input_binding_des_infos[1].stride = sizeof(vec3);
    vert_input_binding_des_infos[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    vert_input_binding_des_infos[2].binding = 2;
    vert_input_binding_des_infos[2].stride = sizeof(vec2);
    vert_input_binding_des_infos[2].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //1.2. link bind pointer. (glVertexAttribBinding)m_VK_main_shader_set_layouts
    VkVertexInputAttributeDescription vert_input_attrib_des_infos[3] = {
        {},{},{}
    };
    //------ bind vertice attribute.
    vert_input_attrib_des_infos[0].binding = 0; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[0].location = 0; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vert_input_attrib_des_infos[0].offset = 0;
    //------ bind normal attribute.
    vert_input_attrib_des_infos[1].binding = 1; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[1].location = 1; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vert_input_attrib_des_infos[1].offset = 0;
    //------ bind texture attribute.
    vert_input_attrib_des_infos[2].binding = 2; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
    vert_input_attrib_des_infos[2].location = 2; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
    vert_input_attrib_des_infos[2].format = VK_FORMAT_R32G32_SFLOAT;
    vert_input_attrib_des_infos[2].offset = 0;

    VkPipelineVertexInputStateCreateInfo v_input_state_c_info = {};
    v_input_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    v_input_state_c_info.pNext = nullptr;
    v_input_state_c_info.pVertexBindingDescriptions = vert_input_binding_des_infos;
    v_input_state_c_info.vertexBindingDescriptionCount = 3;
    v_input_state_c_info.pVertexAttributeDescriptions = vert_input_attrib_des_infos;
    v_input_state_c_info.vertexAttributeDescriptionCount = 3;

    //1.3. create input assembly states.(GL_TRIANGLE, ...etc.)
    VkPipelineInputAssemblyStateCreateInfo v_input_assembly_state_c_info = {};
    v_input_assembly_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    v_input_assembly_state_c_info.pNext = nullptr;
    v_input_assembly_state_c_info.flags = 0;
    v_input_assembly_state_c_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //GL_TRIANGLE 
    v_input_assembly_state_c_info.primitiveRestartEnable = VK_FALSE; //Use to restart idx (set 0XFFFFFFFF or 0XFFFF) and then to draw fan or strip.

    //1.4. create tessellation state create info.
    VkPipelineTessellationStateCreateInfo tessellation_state_c_info = {};
    tessellation_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation_state_c_info.pNext = nullptr;
    tessellation_state_c_info.flags = 0;
    tessellation_state_c_info.patchControlPoints = 3; //3 : triangle. 4 : quad.

    //2. create shader module.
    //2.1 create vert shader module.
    ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("PhongShaderVert");
    vert_shader_sref.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/PhongShader.vert.spv", "main");
    //2.2 create frag shader module.
    ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("PhongShaderFrag");
    frag_shader_sref.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/PhongShader.frag.spv", "main");

    //3. Create RenderFlow

}

//--------------------------------------- private --------------------------------------
void Sample2_DrawScene::CreateCommandBufferAndPool()
{
    VkResult result = VK_SUCCESS;
    VkCommandPoolCreateInfo cmd_pool_c_info = {};
    cmd_pool_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_c_info.pNext = nullptr;
    cmd_pool_c_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VkCommandPoolCreateFlags

    result = m_mgr->CreateCommandPool(cmd_pool_c_info, m_VK_cmd_pool);
    if (result != VK_SUCCESS) {
        SDLOGE("Create sample1 cmd pool failure!!!");;
        return;
    }

    VkCommandBufferAllocateInfo cmd_buf_a_info = {};
    cmd_buf_a_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd_buf_a_info.pNext = nullptr;
    cmd_buf_a_info.commandPool = m_VK_cmd_pool;
    cmd_buf_a_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //VkCommandBufferLevel
    cmd_buf_a_info.commandBufferCount = 1;

    result = m_mgr->AllocateCommandBuffer(cmd_buf_a_info, m_VK_cmd_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate sample1 cmd buffer failure!!!");
        return;
    }
}

void Sample2_DrawScene::CreateRenderPassAndFramebuffer()
{
    SDLOG("--- Vulkan initialize sample 2 render pass.");
    Resolution res_size = GraphicsManager::GetRef().GetScreenResolution();
    m_color_buffer_sref = new Texture("Sample2_ColorBuffer");
    m_color_buffer_sref.GetRef().Initialize2DColorOrDepthBuffer(
        res_size.GetWidth(), res_size.GetHeight(), TextureFormat_R8G8B8A8_UNORM,
        ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

    m_depth_buffer_sref = new Texture("Sample2_DepthBuffer");
    m_depth_buffer_sref.GetRef().Initialize2DColorOrDepthBuffer(
        res_size.GetWidth(), res_size.GetHeight(), TextureFormat_D32_SFLOAT_S8_UINT,
        ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    ClearValue clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
    ClearValue clear_dands;
    clear_dands.depth_stencil.depth = 1.0f; clear_dands.depth_stencil.stencil = 1;

    RenderPassWeakReferenceObject rp_wref = GraphicsManager::GetRef().GetRenderPass("ForwardPath");
    m_render_flow_sref = new RenderFlow("ForwardPathFlow", ImageOffset(0,0,0), ImageSize(res_size.GetWidth(), res_size.GetHeight(), 1));//1 is means layer 1.
    m_render_flow_sref.GetRef().RegisterRenderPass(rp_wref);
    m_render_flow_sref.GetRef().AllocateFrameBuffer();
    m_render_flow_sref.GetRef().RegisterBufferToFrameBuffer(m_color_buffer_sref, 0, clear_color);
    m_render_flow_sref.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer_sref, 1, clear_dands);
    m_render_flow_sref.GetRef().RegisterBufferToFrameBuffer(m_color_buffer_sref, 2, clear_color);
    m_render_flow_sref.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer_sref, 3, clear_dands);
    m_render_flow_sref.GetRef().RegisterBufferToFrameBuffer(m_color_buffer_sref, 4, clear_color);
    m_render_flow_sref.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer_sref, 5, clear_dands);
    m_render_flow_sref.GetRef().Initialize();
}