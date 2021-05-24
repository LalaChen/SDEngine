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
    CreateRenderPass();
    CreateFramebuffer();
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
    m_render_flow.Reset();
    m_color_buffer.Reset();
    m_depth_buffer.Reset();
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
        MeshStrongReferenceObject mesh = new Mesh(model.m_mesh_datas[meshID].m_name);
        uint32_t vaID;
        //--- Vertex Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_VERTEX_BUFFER);
            StaticVertexBufferStrongReferenceObject va = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32_SFLOAT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].size());
            mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, va.StaticCastTo<VertexBuffer>());
        }
        //--- Normal Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_NORMAL_BUFFER);
            StaticVertexBufferStrongReferenceObject va = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32_SFLOAT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].size());
            mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_NORMAL_BUFFER, va.StaticCastTo<VertexBuffer>());
        }
        //--- Tangent Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_TANGENT_BUFFER);
            StaticVertexBufferStrongReferenceObject va = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32_SFLOAT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].size());
            mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_TANGENT_BUFFER, va.StaticCastTo<VertexBuffer>());
        }
        //--- Binormal Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_BINORMAL_BUFFER);
            StaticVertexBufferStrongReferenceObject va = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32_SFLOAT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].size());
            mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_BINORMAL_BUFFER, va.StaticCastTo<VertexBuffer>());
        }
        //--- Tex Coord Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_TEX_COORD_BUFFER);
            StaticVertexBufferStrongReferenceObject va = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32_SFLOAT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].size());
            mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, va.StaticCastTo<VertexBuffer>());
        }
        //--- Color Buffer
        if (model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_COLOR_BUFFER].size() > 0) {
            vaID = static_cast<uint32_t>(VertexBufferUsage_COLOR_BUFFER);
            StaticVertexBufferStrongReferenceObject va = new StaticVertexBuffer(
                StringFormat("%s_%s", model.m_mesh_datas[meshID].m_name.c_str(), VertexBufferUsageEnumNames[vaID].c_str()),
                vaID, VertexBufferFormat_X32Y32Z32W32_SFLOAT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_COLOR_BUFFER].data(),
                model.m_mesh_datas[meshID].m_vertex_attribs[VertexBufferUsage_COLOR_BUFFER].size());
            mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_COLOR_BUFFER, va.StaticCastTo<VertexBuffer>());
        }
        //--- Indice Buffer
        if (model.m_mesh_datas[meshID].m_face_indices.size() > 0) {
            StaticIndexBufferStrongReferenceObject va = new StaticIndexBuffer(
                StringFormat("%s_Indice", model.m_mesh_datas[meshID].m_name.c_str()),IndexBufferFormat_X32_UINT);
            va.GetRef().RefreshBufferData(
                model.m_mesh_datas[meshID].m_face_indices.data(),
                model.m_mesh_datas[meshID].m_face_indices.size());
            mesh.GetRef().RegisterIndexBuffer(va.StaticCastTo<IndexBuffer>());
        }
        m_meshes.push_back(mesh);
    }
    //2. Load texture.
    for (TextureMap::const_iterator tex_iter = model.m_textures.begin();
        tex_iter != model.m_textures.end();
        tex_iter++) {
        TextureResourceMap::iterator res_iter = m_textures.find((*tex_iter).first);
        if (res_iter == m_textures.end()) {
            TextureStrongReferenceObject tex = new Texture(StringFormat("%s_%s", model.m_name.c_str(), (*tex_iter).first.c_str()));
            tex.GetRef().InitializeFromBitmap((*tex_iter).second);
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
    ShaderModuleStrongReferenceObject vert_shader = new ShaderModule("PhongShaderVert");
    vert_shader.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/PhongShader.vert.spv", "main");
    //2.2 create frag shader module.
    ShaderModuleStrongReferenceObject frag_shader = new ShaderModule("PhongShaderFrag");
    frag_shader.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/PhongShader.frag.spv", "main");

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

void Sample2_DrawScene::CreateRenderPass()
{
    SDLOG("--- Vulkan initialize sample 2 render pass.(use system)");
}

void Sample2_DrawScene::CreateFramebuffer()
{
    SDLOG("--- Vulkan initialize sample 2 render flow.");
    Resolution res_size = GraphicsManager::GetRef().GetScreenResolution();
    m_color_buffer = new Texture("Sample2_ColorBuffer");
    m_color_buffer.GetRef().Initialize2DColorOrDepthBuffer(
        res_size.GetWidth(), res_size.GetHeight(),
        GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
        ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

    m_depth_buffer = new Texture("Sample2_DepthBuffer");
    m_depth_buffer.GetRef().Initialize2DColorOrDepthBuffer(
        res_size.GetWidth(), res_size.GetHeight(),
        GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
        ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    ClearValue clear_color = { 0.15f, 0.15f, 0.15f, 1.0f };
    ClearValue clear_dands;
    clear_dands.depth_stencil.depth = 1.0f; clear_dands.depth_stencil.stencil = 1;

    RenderPassWeakReferenceObject rp = GraphicsManager::GetRef().GetRenderPass("ForwardPass");
    m_render_flow = new RenderFlow("ForwardPassFlow", ImageOffset(0, 0, 0), ImageSize(res_size.GetWidth(), res_size.GetHeight(), 1));//1 is means layer 1.
    m_render_flow.GetRef().RegisterRenderPass(rp);
    m_render_flow.GetRef().AllocateFrameBuffer();
    m_render_flow.GetRef().RegisterBufferToFrameBuffer(m_color_buffer, 0, clear_color);
    m_render_flow.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer, 1, clear_dands);
    m_render_flow.GetRef().RegisterBufferToFrameBuffer(m_color_buffer, 2, clear_color);
    m_render_flow.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer, 3, clear_dands);
    m_render_flow.GetRef().RegisterBufferToFrameBuffer(m_color_buffer, 4, clear_color);
    m_render_flow.GetRef().RegisterBufferToFrameBuffer(m_depth_buffer, 5, clear_dands);
    m_render_flow.GetRef().Initialize();
}