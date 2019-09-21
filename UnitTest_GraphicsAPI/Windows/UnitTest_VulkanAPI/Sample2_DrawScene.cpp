//#define USE_HOST_BUFFER
#include <chrono>
#include <thread>

#include "SDEngine.h"
#include "VulkanAPITestManager.h"
#include "AssimpModelLoader.h"
#include "Sample2_DrawScene.h"

using namespace::SDE::Math;
using namespace::SDE::Basic;
using namespace::SDE::Graphics;

Sample2_DrawScene::Sample2_DrawScene(VulkanAPITestManager *i_mgr)
: Sample("DrawScene", i_mgr)
, m_VK_ver_buffer(VK_NULL_HANDLE)
, m_VK_ver_buf_memory(VK_NULL_HANDLE)
, m_VK_ver_color_buffer(VK_NULL_HANDLE)
, m_VK_ver_color_buf_memory(VK_NULL_HANDLE)
, m_VK_ver_tex_buffer(VK_NULL_HANDLE)
, m_VK_ver_tex_buf_memory(VK_NULL_HANDLE)
, m_VK_indices_buffer(VK_NULL_HANDLE)
, m_VK_ibuf_memory(VK_NULL_HANDLE)
//
, m_VK_main_texture(VK_NULL_HANDLE)
, m_VK_main_texture_memory(VK_NULL_HANDLE)
, m_VK_main_texture_image_view(VK_NULL_HANDLE)
, m_VK_main_texture_sampler(VK_NULL_HANDLE)
//
, m_VK_basic_uniform_buffers{ VK_NULL_HANDLE, VK_NULL_HANDLE }
, m_VK_basic_uniform_buffer_memories{ VK_NULL_HANDLE, VK_NULL_HANDLE }
//
, m_VK_pipeline_layout(VK_NULL_HANDLE)
, m_vert_module(VK_NULL_HANDLE)
, m_frag_module(VK_NULL_HANDLE)
, m_VK_main_shader_set0_layout(VK_NULL_HANDLE)
, m_VK_descriptor_set0_uniforms{ VK_NULL_HANDLE, VK_NULL_HANDLE }
, m_VK_descriptor_pool(VK_NULL_HANDLE)
, m_VK_main_graphics_pipeline(VK_NULL_HANDLE)
//
, m_VK_render_pass(VK_NULL_HANDLE)
//for camera
, m_VK_color_buffer(VK_NULL_HANDLE)
, m_VK_color_buffer_memory(VK_NULL_HANDLE)
, m_VK_depth_buffer(VK_NULL_HANDLE)
, m_VK_depth_buffer_memory(VK_NULL_HANDLE)
//for camera
, m_VK_cmd_pool(VK_NULL_HANDLE)
, m_VK_cmd_buffer(VK_NULL_HANDLE)
, m_VK_frame_buffer(VK_NULL_HANDLE)
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
    ImportAssimpModel();
    //
    CreateBuffers();
    CreateTexture();
    CreateUniformBuffer();
    CreateShaderPrograms();
}

void Sample2_DrawScene::Render()
{
}

void Sample2_DrawScene::Destroy()
{
}

void Sample2_DrawScene::ImportAssimpModel()
{
    SDLOG("Import AssimpModel!!!");
    AssimpModelLoader::GetRef().ImportModel("resources\\models\\Building02\\Building02_m.FBX", m_model);
    //AssimpModelLoader::GetRef().ImportModel("resources\\models\\Build11_fbx\\Build11_fbx.FBX", m_model);
}

void Sample2_DrawScene::CreateBuffers()
{
    SDLOG("Create Buffer!!!");
}

void Sample2_DrawScene::CreateTexture()
{
    SDLOG("Create Texture!!!");
}

void Sample2_DrawScene::CreateUniformBuffer()
{
    SDLOG("Create Uniform Buffer!!!");
    VkResult result = VK_SUCCESS;
    //Create basic uniform buffer
    for (uint32_t i = 0; i < 2; ++i) {
        result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, sizeof(BasicUniformBuffer), m_VK_basic_uniform_buffers[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Create uniform buffer[i] failure!!!", i);
        }

        result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_basic_uniform_buffers[i], m_VK_basic_uniform_buffer_memories[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Allocate uniform buffer[i] failure!!!", i);
        }
    }
}

void Sample2_DrawScene::CreateShaderPrograms()
{
    SDLOG("Create Shader Programs!!!");
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
    SDLOG("--- Vulkan initialize sample 1 render pass.");
    //1. Render Pass.
    //1.1 write attachment descriptions for color and depth attachment.
    //--- Color attachment description.
    VkAttachmentDescription attachment_descs[2] = { {},{} }; //0 :color, 1: depth
    attachment_descs[0].flags = 0;
    attachment_descs[0].format = VK_FORMAT_R8G8B8A8_UNORM;
    attachment_descs[0].samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    attachment_descs[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //Clear before using. If we set VK_ATTACHMENT_LOAD_OP_DONT_CARE, we can't clear via clearcolor.
    attachment_descs[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Store after using. If we set VK_ATTACHMENT_STORE_OP_DONT_CARE, we can't store rendering result to the buffer binded to this attachment.
    attachment_descs[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//
    attachment_descs[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//
    attachment_descs[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_descs[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //--- Depth attachment description.
    attachment_descs[1].flags = 0;
    attachment_descs[1].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
    attachment_descs[1].samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    attachment_descs[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment_descs[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Store after using. If we set VK_ATTACHMENT_STORE_OP_DONT_CARE, we can't store rendering result to the buffer binded to this attachment.
    attachment_descs[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//
    attachment_descs[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//
    attachment_descs[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_descs[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //1.2 write attachment references.
    //--- Bind the output to color attachment. //0 :color, 1: depth
    VkAttachmentReference attachment_refs[2] = { {},{} };
    attachment_refs[0].attachment = 0; //ID of attachment in RenderPass.
    attachment_refs[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //--- Bind the output to depth attachment.
    attachment_refs[1].attachment = 1; //ID of attachment in RenderPass.
    attachment_refs[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //1.3 create subpass for render pass.
    VkSubpassDescription subpass = {};
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &attachment_refs[0];
    subpass.pResolveAttachments = nullptr; //for multisample.
    subpass.pDepthStencilAttachment = &attachment_refs[1];
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;
    //1.4 Create subpass dependecy for present pass.
    std::vector<VkSubpassDependency> sp_dependencies;
    sp_dependencies.resize(2);
    //--- Begin dep.
    sp_dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    sp_dependencies[0].dstSubpass = 0;
    sp_dependencies[0].srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    sp_dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    sp_dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    sp_dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    sp_dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    //--- End dep.
    sp_dependencies[1].srcSubpass = 0;
    sp_dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    sp_dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    sp_dependencies[1].dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    sp_dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    sp_dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    sp_dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    //1.5. Write created information for present pass.
    VkRenderPassCreateInfo pass_c_info = {};
    pass_c_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    pass_c_info.pNext = nullptr;
    pass_c_info.flags = 0; //Not implement in Vulkan, it say 'reversed for future use.
    pass_c_info.attachmentCount = 2;
    pass_c_info.pAttachments = attachment_descs;
    pass_c_info.subpassCount = 1;
    pass_c_info.pSubpasses = &subpass;
    pass_c_info.dependencyCount = static_cast<uint32_t>(sp_dependencies.size());
    pass_c_info.pDependencies = sp_dependencies.data();
    //1.6 create render pass.
    VkResult result = m_mgr->CreateRenderPass(pass_c_info, m_VK_render_pass);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 render pass create failure.");
        return;
    }
    //2. create color and depth buffer.
    SDE::Graphics::Resolution screen_size = m_mgr->GetScreenResolution();
    //--- color buffer
    VkImageCreateInfo cb_c_info = {};
    cb_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    cb_c_info.pNext = nullptr;
    cb_c_info.flags = 0; //We need to set flag when we want to use sparse texture data or sth.
    cb_c_info.imageType = VK_IMAGE_TYPE_2D;
    cb_c_info.mipLevels = 1;// mipmap levels. We don't want to create mip map in here.
    cb_c_info.arrayLayers = 1; //Use it when data is 2d texture array.
    cb_c_info.tiling = VK_IMAGE_TILING_OPTIMAL; //Set texture tiling mode. If the image is linked data in system memory, we need to use VK_IMAGE_TILING_LINEAR.
    cb_c_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    cb_c_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //We will copy data to this image(trasnfer dst) and use it in shader(sampled).
    cb_c_info.samples = VK_SAMPLE_COUNT_1_BIT;
    cb_c_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    cb_c_info.queueFamilyIndexCount = 0;
    cb_c_info.pQueueFamilyIndices = nullptr;
    cb_c_info.extent.width = screen_size.GetWidth();
    cb_c_info.extent.height = screen_size.GetHeight();
    cb_c_info.extent.depth = 1;
    cb_c_info.format = VK_FORMAT_R8G8B8A8_UNORM;

    result = m_mgr->CreateImage(cb_c_info, m_VK_color_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 color buffer create failure.");
        return;
    }

    result = m_mgr->AllocateMemoryAndBindToImage(m_VK_color_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_color_buffer_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 color buffer memory allocate failure.");
        return;
    }

    //--- depth buffer
    VkImageCreateInfo db_c_info = {};
    db_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    db_c_info.pNext = nullptr;
    db_c_info.flags = 0; //We need to set flag when we want to use sparse texture data or sth.
    db_c_info.imageType = VK_IMAGE_TYPE_2D;
    db_c_info.mipLevels = 1;// mipmap levels. We don't want to create mip map in here.
    db_c_info.arrayLayers = 1; //Use it when data is 2d texture array.
    db_c_info.tiling = VK_IMAGE_TILING_OPTIMAL; //Set texture tiling mode. If the image is linked data in system memory, we need to use VK_IMAGE_TILING_LINEAR.
    db_c_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    db_c_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; //We will copy data to this image(trasnfer dst) and use it in shader(sampled).
    db_c_info.samples = VK_SAMPLE_COUNT_1_BIT;
    db_c_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    db_c_info.queueFamilyIndexCount = 0;
    db_c_info.pQueueFamilyIndices = nullptr;
    db_c_info.extent.width = screen_size.GetWidth();
    db_c_info.extent.height = screen_size.GetHeight();
    db_c_info.extent.depth = 1;
    db_c_info.format = VK_FORMAT_D24_UNORM_S8_UINT;

    result = m_mgr->CreateImage(db_c_info, m_VK_depth_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 depth buffer create failure.");
        return;
    }

    result = m_mgr->AllocateMemoryAndBindToImage(m_VK_depth_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_depth_buffer_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 depth buffer memory allocate failure.");
        return;
    }
    //3. Create Frame Buffer.
    VkImageViewCreateInfo img_view_cb_info = {};
    img_view_cb_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    img_view_cb_info.pNext = nullptr;
    img_view_cb_info.flags = 0;
    img_view_cb_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    img_view_cb_info.image = m_VK_color_buffer;
    img_view_cb_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    img_view_cb_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_cb_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_cb_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_cb_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_cb_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    img_view_cb_info.subresourceRange.baseMipLevel = 0;
    img_view_cb_info.subresourceRange.levelCount = 1;
    img_view_cb_info.subresourceRange.baseArrayLayer = 0;
    img_view_cb_info.subresourceRange.layerCount = 1;
    result = m_mgr->CreateImageView(img_view_cb_info, m_VK_color_buffer_image_view);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 color buffer image view failure.");
        return;
    }

    VkImageViewCreateInfo img_view_db_info = {};
    img_view_db_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    img_view_db_info.pNext = nullptr;
    img_view_db_info.flags = 0;
    img_view_db_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    img_view_db_info.image = m_VK_depth_buffer;
    img_view_db_info.format = VK_FORMAT_D24_UNORM_S8_UINT;
    img_view_db_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_db_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_db_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_db_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    img_view_db_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    img_view_db_info.subresourceRange.baseMipLevel = 0;
    img_view_db_info.subresourceRange.levelCount = 1;
    img_view_db_info.subresourceRange.baseArrayLayer = 0;
    img_view_db_info.subresourceRange.layerCount = 1;
    result = m_mgr->CreateImageView(img_view_db_info, m_VK_depth_buffer_image_view);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 depth buffer image view failure.");
        return;
    }

    VkImageView ivs[2] = { m_VK_color_buffer_image_view, m_VK_depth_buffer_image_view };

    VkFramebufferCreateInfo fbo_c_info = {};
    fbo_c_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbo_c_info.pNext = nullptr;
    fbo_c_info.flags = 0; //Reserved for future use
    fbo_c_info.renderPass = m_VK_render_pass;
    fbo_c_info.attachmentCount = 2;
    fbo_c_info.pAttachments = ivs;
    fbo_c_info.width = screen_size.GetWidth();
    fbo_c_info.height = screen_size.GetHeight();
    fbo_c_info.layers = 1;
    result = m_mgr->CreateVkFramebuffer(fbo_c_info, m_VK_frame_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 depth buffer image view failure.");
        return;
    }
}