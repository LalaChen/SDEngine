//#define USE_HOST_BUFFER
#include <chrono>
#include <thread>

#include "SDEngine.h"
#include "VulkanWrapper.h"
#include "VulkanAPITestManager.h"
#include "Sample3_MultiSubpass.h"

using namespace::SDE::Math;
using namespace::SDE::Basic;
using namespace::SDE::Graphics;

Sample3_MultiSubpass::Sample3_MultiSubpass(VulkanAPITestManager *i_mgr)
: Sample("MultiSubpass", i_mgr)
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
, m_VK_basic_uniform_buffers{ VK_NULL_HANDLE }
, m_VK_basic_uniform_buffer_memories{ VK_NULL_HANDLE }
//
, m_VK_pipeline_layouts{VK_NULL_HANDLE}
, m_vert_modules{ VK_NULL_HANDLE }
, m_frag_modules{VK_NULL_HANDLE}
, m_VK_main_shader_set0_layouts{ VK_NULL_HANDLE }
, m_VK_descriptor_set0_uniforms{ VK_NULL_HANDLE }
, m_VK_descriptor_pools{ VK_NULL_HANDLE }
, m_VK_main_graphics_pipelines{ VK_NULL_HANDLE }
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

Sample3_MultiSubpass::~Sample3_MultiSubpass()
{
}

void Sample3_MultiSubpass::Initialize()
{
    m_current_res = m_mgr->GetScreenResolution();
    //
    CreateCommandBufferAndPool();
    CreateRenderPass();
    CreateFramebuffer();
    //
    CreateBuffers();
    CreateTexture();
    CreateUniformBuffer();
    CreateShaderPrograms();
}

void Sample3_MultiSubpass::Render()
{
    if (m_mgr != nullptr) {
        VkResult result = VK_SUCCESS;
        float asratio = static_cast<float>(m_current_res.GetHeight()) / static_cast<float>(m_current_res.GetWidth());
        static float angle = 0.0f;
        static float addAngle = 0.1f;
        angle += addAngle;

        float clip_mat[16] = {
            1.0f,  0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f, 0.0f,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f };

        Matrix4X4f proj;
        proj.perspective(90, 1.0f / asratio, 0.01f, 10.0f);
        //proj.ortho(-1.0f, 1.0f, -1.0f * asratio, 1.0 * asratio, -1.0f, 1.0f);

        Matrix4X4f view;
        Vector3f view_eye(0.0f, 0.0f, 0.5f, 1.0f);
        view.lookAt(view_eye, Vector3f(0.0f, 0.0f, 0.0f, 1.0f), Vector3f(0.0f, 1.0f, 0.0f, 1.0f));
        //1. update uniform buffer.
        //(Object 0)
        //--- clip space.
        m_uniform_buffer_datas[0].m_clip = Matrix4X4f(clip_mat);
        //--- projection space.
        m_uniform_buffer_datas[0].m_proj = proj;
        //--- view space.
        m_uniform_buffer_datas[0].m_view = view;
        m_uniform_buffer_datas[0].m_view_eye = view_eye;
        //--- world space. 
        m_uniform_buffer_datas[0].m_world.rotate(Quaternion(Vector3f::PositiveZ, angle));
        m_uniform_buffer_datas[0].m_world.translate(Vector3f(-0.0f, -0.0f, -0.5f, 1.0f));
        result = m_mgr->RefreshHostDeviceBufferData(m_VK_basic_uniform_buffers[0], m_VK_basic_uniform_buffer_memories[0], &m_uniform_buffer_datas[0], sizeof(BasicUniformBuffer));

        //(Object 1)
        //--- clip space.
        m_uniform_buffer_datas[1].m_clip = Matrix4X4f(clip_mat);
        //--- projection space.
        m_uniform_buffer_datas[1].m_proj = proj;
        //m_uniform_buffer_datas[1].m_proj.ortho(-1.0f, 1.0f, -1.0f * asratio, 1.0 * asratio, -1.0f, 1.0f);
        //--- view space.
        m_uniform_buffer_datas[1].m_view = view;
        m_uniform_buffer_datas[1].m_view_eye = view_eye;
        //--- world space.
        m_uniform_buffer_datas[1].m_world.translate(Vector3f(-0.2f, -0.2f, -0.4f, 1.0f));

        result = m_mgr->RefreshHostDeviceBufferData(m_VK_basic_uniform_buffers[1], m_VK_basic_uniform_buffer_memories[1], &m_uniform_buffer_datas[1], sizeof(BasicUniformBuffer));

        m_uniform_buffer_datas[2].m_clip = Matrix4X4f(clip_mat);
        //--- projection space.
        m_uniform_buffer_datas[2].m_proj = proj;
        //m_uniform_buffer_datas[2].m_proj.ortho(-1.0f, 1.0f, -1.0f * asratio, 1.0 * asratio, -1.0f, 1.0f);
        //--- view space.
        m_uniform_buffer_datas[2].m_view = view;
        m_uniform_buffer_datas[2].m_view_eye = view_eye;
        //--- world space. 
        m_uniform_buffer_datas[2].m_world.translate(Vector3f(0.3f, 0.3f, -0.49f, 1.0f));
        result = m_mgr->RefreshHostDeviceBufferData(m_VK_basic_uniform_buffers[2], m_VK_basic_uniform_buffer_memories[2], &m_uniform_buffer_datas[2], sizeof(BasicUniformBuffer));

        m_uniform_buffer_datas[3].m_clip = Matrix4X4f(clip_mat);
        //--- projection space.
        m_uniform_buffer_datas[3].m_proj = proj;
        //m_uniform_buffer_datas[3].m_proj.ortho(-1.0f, 1.0f, -1.0f * asratio, 1.0 * asratio, -1.0f, 1.0f);
        //--- view space.
        m_uniform_buffer_datas[3].m_view = view;
        m_uniform_buffer_datas[3].m_view_eye = view_eye;
        //--- world space. 
        m_uniform_buffer_datas[3].m_world.translate(Vector3f(-0.3f, -0.3f, -0.48f, 1.0f));
        result = m_mgr->RefreshHostDeviceBufferData(m_VK_basic_uniform_buffers[3], m_VK_basic_uniform_buffer_memories[3], &m_uniform_buffer_datas[3], sizeof(BasicUniformBuffer));

        //2. begin command buffer.
        VkCommandBufferBeginInfo cmd_buf_c_info = {};
        cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmd_buf_c_info.pNext = nullptr;
        cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        cmd_buf_c_info.pInheritanceInfo = nullptr;

        result = m_mgr->BeginCommandBuffer(cmd_buf_c_info, m_VK_cmd_buffer);
        if (result != VK_SUCCESS) {
            SDLOGW("We can't begin command buffer(%d)!!!", result);
            return;
        }
        //2. begin render pass.
        VkRect2D render_area = {};
        render_area.offset = { 0, 0 };
        render_area.extent = { m_current_res.GetWidth(), m_current_res.GetHeight() };

        VkClearValue clear_color; clear_color.color = { 0.75f, 0.15f, 0.15f, 1.0f };
        VkClearValue clear_depth; clear_depth.depthStencil = { 1.0f, 0 };

        VkClearValue clear_values[4] = {//correspond with image view.
            clear_color,
            clear_depth,
            clear_color,
            clear_depth
        };
        //2.1 write render pass begin info.
        VkRenderPassBeginInfo rp_begin_info = {};
        rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_begin_info.pNext = nullptr;
        rp_begin_info.renderPass = m_VK_render_pass;
        rp_begin_info.framebuffer = m_VK_frame_buffer;
        rp_begin_info.renderArea = render_area;
        rp_begin_info.clearValueCount = 4;
        rp_begin_info.pClearValues = clear_values;
        //2.2 Begin render pass.
        m_mgr->BeginRenderPass(rp_begin_info, m_VK_cmd_buffer);

        //3. Start draw scene.
        //3.1 Set viewport.
        //------ Viewport
        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = static_cast<float>(m_current_res.GetWidth());
        viewport.height = static_cast<float>(m_current_res.GetHeight());
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        m_mgr->SetMainViewportDynamically(m_VK_cmd_buffer, viewport);
        m_mgr->SetMainScissorDynamically(m_VK_cmd_buffer, render_area);

        for (uint32_t obj_id = 0; obj_id < OBJ_SIZE / 2; ++obj_id) {
            //3.2 bind uniform variable.(Set uniform variable)
            //Open graphics pipeline.
            m_mgr->BindGraphicsPipeline(m_VK_cmd_buffer, m_VK_main_graphics_pipelines[0]);
            //Bind descriptor sets
            std::vector<VkDescriptorSet> descs = { m_VK_descriptor_set0_uniforms[obj_id] };
            std::vector<uint32_t> dynamic_offs = {};
            m_mgr->BindDescriptorSets(m_VK_cmd_buffer, m_VK_pipeline_layouts[0], VK_PIPELINE_BIND_POINT_GRAPHICS, 0, descs, dynamic_offs);
            //3.4 bind vertex attributes.
            //Bind Vertex Buffer.
            m_mgr->BindVertexBuffer(m_VK_cmd_buffer, m_VK_ver_buffer, 0, 0); //vertex
            m_mgr->BindVertexBuffer(m_VK_cmd_buffer, m_VK_ver_color_buffer, 0, 1); //color
            m_mgr->BindVertexBuffer(m_VK_cmd_buffer, m_VK_ver_tex_buffer, 0, 2); //tex

            //std::vector<VkBuffer> va_buffers = {m_VK_ver_buffer, m_VK_ver_color_buffer};
            //std::vector<VkDeviceSize> va_buffer_offsets = {0, 0};
            //m_mgr->BindVertexBuffers(va_buffers, va_buffer_offsets, 0);

            m_mgr->BindIndiceBuffer(m_VK_cmd_buffer, m_VK_indices_buffer, 0, VK_INDEX_TYPE_UINT16); //Bind indices.

            //3.5 draw vertex attributes.
            m_mgr->DrawByIndice(m_VK_cmd_buffer, 6, 1, 0, 0, 0);
        }

        vkCmdNextSubpass(m_VK_cmd_buffer, VK_SUBPASS_CONTENTS_INLINE);

        for (uint32_t obj_id = OBJ_SIZE / 2; obj_id < OBJ_SIZE; ++obj_id) {
            //3.2 bind uniform variable.(Set uniform variable)
            //Open graphics pipeline.
            m_mgr->BindGraphicsPipeline(m_VK_cmd_buffer, m_VK_main_graphics_pipelines[1]);
            //Bind descriptor sets
            std::vector<VkDescriptorSet> descs = { m_VK_descriptor_set0_uniforms[obj_id] };
            std::vector<uint32_t> dynamic_offs = {};
            m_mgr->BindDescriptorSets(m_VK_cmd_buffer, m_VK_pipeline_layouts[1], VK_PIPELINE_BIND_POINT_GRAPHICS, 0, descs, dynamic_offs);
            //3.4 bind vertex attributes.
            //Bind Vertex Buffer.
            m_mgr->BindVertexBuffer(m_VK_cmd_buffer, m_VK_ver_buffer, 0, 0); //vertex
            m_mgr->BindVertexBuffer(m_VK_cmd_buffer, m_VK_ver_color_buffer, 0, 1); //color
            m_mgr->BindVertexBuffer(m_VK_cmd_buffer, m_VK_ver_tex_buffer, 0, 2); //tex

            //std::vector<VkBuffer> va_buffers = {m_VK_ver_buffer, m_VK_ver_color_buffer};
            //std::vector<VkDeviceSize> va_buffer_offsets = {0, 0};
            //m_mgr->BindVertexBuffers(va_buffers, va_buffer_offsets, 0);

            m_mgr->BindIndiceBuffer(m_VK_cmd_buffer, m_VK_indices_buffer, 0, VK_INDEX_TYPE_UINT16); //Bind indices.

            //3.5 draw vertex attributes.
            m_mgr->DrawByIndice(m_VK_cmd_buffer, 6, 1, 0, 0, 0);
        }

        //3.6 close render pass.
        m_mgr->EndRenderPass(m_VK_cmd_buffer);

        //4. Submit command buffer to queue.
        //4.1 close recording command.
        result = m_mgr->EndCommandBuffer(m_VK_cmd_buffer);
        if (result != VK_SUCCESS) {
            SDLOGW("We can't end command buffer(%d)!!!", result);
        }

        result = m_mgr->SubmitCommandBufferToMainQueue(m_VK_cmd_buffer);
        if (result != VK_SUCCESS) {
            SDLOGE("Submit sample1 cmd buffer failure(%d)!!!", result);
        }

        result = m_mgr->ResetCommandBuffer(m_VK_cmd_buffer, false);
        if (result != VK_SUCCESS) {
            SDLOGE("Reset sample1 cmd pool failure(%d)!!!", result);
        }
    }
}

void Sample3_MultiSubpass::Resize(Size_ui32 i_width, Size_ui32 i_height)
{
    SDLOG("Resize(%u,%u)", i_width, i_height);
    m_current_res.SetResolution(i_width, i_height);
    VkResult result;
    //1. clear old devices.
    m_mgr->DestroyFramebuffer(m_VK_frame_buffer);
    m_VK_frame_buffer = VK_NULL_HANDLE;

    m_mgr->DestroyImage(m_VK_color_buffer);
    m_VK_color_buffer = VK_NULL_HANDLE;
    m_mgr->ReleaseMemory(m_VK_color_buffer_memory);
    m_VK_color_buffer_memory = VK_NULL_HANDLE;

    m_mgr->DestroyImage(m_VK_depth_buffer);
    m_VK_depth_buffer = VK_NULL_HANDLE;
    m_mgr->ReleaseMemory(m_VK_depth_buffer_memory);
    m_VK_depth_buffer_memory = VK_NULL_HANDLE;
    
    for (uint32_t i = 0; i < 2; ++i) {
        m_mgr->DestroyImageView(m_VK_color_buffer_image_views[i]);
        m_VK_color_buffer_image_views[i] = VK_NULL_HANDLE;
        m_mgr->DestroyImageView(m_VK_depth_buffer_image_views[i]);
        m_VK_depth_buffer_image_views[i] = VK_NULL_HANDLE;
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

    for (uint32_t i = 0; i < 2; ++i) {
        result = m_mgr->CreateImageView(img_view_cb_info, m_VK_color_buffer_image_views[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Sample1 color buffer image view failure.");
            return;
        }
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

    for (uint32_t i = 0; i < 2; ++i) {
        result = m_mgr->CreateImageView(img_view_db_info, m_VK_depth_buffer_image_views[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Sample1 depth buffer image view failure.");
            return;
        }
    }

    VkImageView ivs[4] = { m_VK_color_buffer_image_views[0], m_VK_depth_buffer_image_views[0], m_VK_color_buffer_image_views[0], m_VK_depth_buffer_image_views[0] };

    VkFramebufferCreateInfo fbo_c_info = {};
    fbo_c_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbo_c_info.pNext = nullptr;
    fbo_c_info.flags = 0; //Reserved for future use
    fbo_c_info.renderPass = m_VK_render_pass;
    fbo_c_info.attachmentCount = 4;
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


void Sample3_MultiSubpass::Destroy()
{
    if (m_mgr != nullptr) {
        m_mgr->ReleaseMemory(m_VK_ver_buf_memory);
        m_VK_ver_buf_memory = VK_NULL_HANDLE;
        m_mgr->DestroyBuffer(m_VK_ver_buffer);
        m_VK_ver_buffer = VK_NULL_HANDLE;

        m_mgr->ReleaseMemory(m_VK_ver_color_buf_memory);
        m_VK_ver_color_buf_memory = VK_NULL_HANDLE;
        m_mgr->DestroyBuffer(m_VK_ver_color_buffer);
        m_VK_ver_color_buffer = VK_NULL_HANDLE;

        m_mgr->ReleaseMemory(m_VK_ver_tex_buf_memory);
        m_VK_ver_tex_buf_memory = VK_NULL_HANDLE;
        m_mgr->DestroyBuffer(m_VK_ver_tex_buffer);
        m_VK_ver_tex_buffer = VK_NULL_HANDLE;

        m_mgr->ReleaseMemory(m_VK_ibuf_memory);
        m_VK_ibuf_memory = VK_NULL_HANDLE;
        m_mgr->DestroyBuffer(m_VK_indices_buffer);
        m_VK_indices_buffer = VK_NULL_HANDLE;
        //
        m_mgr->ReleaseMemory(m_VK_main_texture_memory);
        m_VK_main_texture_memory = VK_NULL_HANDLE;
        m_mgr->DestroyImage(m_VK_main_texture);
        m_VK_main_texture = VK_NULL_HANDLE;
        m_mgr->DestroySampler(m_VK_main_texture_sampler);
        m_VK_main_texture_sampler = VK_NULL_HANDLE;
        //
        for (uint32_t obj_id = 0; obj_id < OBJ_SIZE; ++obj_id) {
            m_mgr->ReleaseMemory(m_VK_basic_uniform_buffer_memories[obj_id]);
            m_VK_basic_uniform_buffer_memories[obj_id] = VK_NULL_HANDLE;
            m_mgr->DestroyBuffer(m_VK_basic_uniform_buffers[obj_id]);
            m_VK_basic_uniform_buffers[obj_id] = VK_NULL_HANDLE;
        }
        //
        for (uint32_t i = 0; i < COLOR_AND_DEPTH_BUFFER_SIZE; ++i) {
            m_mgr->DestroyDesciptorSetLayout(m_VK_main_shader_set0_layouts[i]);
            m_VK_main_shader_set0_layouts[i] = VK_NULL_HANDLE;

            m_mgr->DestroyDescriptorPool(m_VK_descriptor_pools[i]);
            m_VK_descriptor_pools[i] = VK_NULL_HANDLE;

            m_mgr->DestroyGraphicsPipeline(m_VK_main_graphics_pipelines[i]);
            m_VK_main_graphics_pipelines[i] = VK_NULL_HANDLE;

            m_mgr->DestroyPipelineLayout(m_VK_pipeline_layouts[i]);
            m_VK_pipeline_layouts[i] = VK_NULL_HANDLE;
            m_mgr->DestroyShaderModule(m_frag_modules[i]);
            m_frag_modules[i] = VK_NULL_HANDLE;
            m_mgr->DestroyShaderModule(m_vert_modules[i]);
            m_vert_modules[i] = VK_NULL_HANDLE;
        }
        //
        m_mgr->DestroyRenderPass(m_VK_render_pass);
        m_VK_render_pass = VK_NULL_HANDLE;
        //for camera
        m_mgr->DestroyFramebuffer(m_VK_frame_buffer);
        m_VK_frame_buffer = VK_NULL_HANDLE;

        m_mgr->ReleaseMemory(m_VK_color_buffer_memory);
        m_VK_color_buffer_memory = VK_NULL_HANDLE;
        m_mgr->DestroyImage(m_VK_color_buffer);
        m_VK_color_buffer = VK_NULL_HANDLE;
        m_mgr->ReleaseMemory(m_VK_depth_buffer_memory);
        m_VK_depth_buffer_memory = VK_NULL_HANDLE;
        m_mgr->DestroyImage(m_VK_depth_buffer);
        m_VK_depth_buffer = VK_NULL_HANDLE;

        for (uint32_t i = 0; i < 2; ++i) {
            m_mgr->DestroyImageView(m_VK_color_buffer_image_views[i]);
            m_VK_color_buffer_image_views[i] = VK_NULL_HANDLE;
            m_mgr->DestroyImageView(m_VK_depth_buffer_image_views[i]);
            m_VK_depth_buffer_image_views[i] = VK_NULL_HANDLE;
        }
        //
        m_mgr->FreeCommandBuffers(m_VK_cmd_pool, &m_VK_cmd_buffer, 1);
        m_VK_cmd_buffer = VK_NULL_HANDLE;
        m_mgr->DestroyCommandPool(m_VK_cmd_pool);
        m_VK_cmd_pool = VK_NULL_HANDLE;
    }
}

void Sample3_MultiSubpass::CreateBuffers()
{
    SDLOG("Create Buffer!!!");
    std::vector<vec3> quad_vecs = {
        vec3( 0.5f,  0.5f, -0.01f),
        vec3(-0.5f,  0.5f, -0.01f),
        vec3(-0.5f, -0.5f, -0.01f),
        vec3( 0.5f, -0.5f, -0.01f)
    };

    std::vector<vec2> quad_texs = {
        vec2( 0.0f, 0.0f),
        vec2( 1.0f, 0.0f),
        vec2( 1.0f, 1.0f),
        vec2( 0.0f, 1.0f)
    };

    std::vector<Color4f> quad_colors = {
        Color4f(1.0f, 0.0f, 0.0f, 1.0f),
        Color4f(0.0f, 1.0f, 0.0f, 1.0f),
        Color4f(0.0f, 0.0f, 1.0f, 1.0f),
        Color4f(1.0f, 1.0f, 1.0f, 1.0f)
    };

    std::vector<uint16_t> quad_indices = {
        0,1,2,
        0,2,3
    };

    VkResult result = VK_SUCCESS;

    // *** IMPORTANT ***
    /* 
        VkPhysicalDeviceMemoryProperties::memoryTypeBits of VkBuffer is 1665 at GTX960. Member memoryTypeBits is used to record 
        types that we can allocate buffer with it in device. If it can be allocated with the type i, the [i]th bit will be set 1.
        And then 1665 in binary is 11010000001. It represents we can allocate VkImage with type 0, 7, 9 and 10. We list all memory properties
        of those types and the memory with those types will be allocate at which heap as below :
           Type 0 : (No Property) heap 1 (host memory)
           Type 7 : (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) heap 0 (device memory)
           Type 9 : (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) heap 1 (host memory)
           Type 10 : (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT) heap 1 (host memory)
        It means we can allocate device memory for VkBuffer with those types. 
    */

    //1. create vertex buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(vec3)*quad_vecs.size()), m_VK_ver_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create vertice buffer failure");
        return;
    }
#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_ver_buffer, m_VK_ver_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_ver_buffer, quad_vecs.data(), static_cast<VkDeviceSize>(sizeof(vec3) * quad_vecs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_ver_buffer, m_VK_ver_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate host vertex buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_ver_buffer, m_VK_ver_buf_memory, quad_vecs.data(), static_cast<VkDeviceSize>(sizeof(vec3) * quad_vecs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh host vertex buffer failure!!!");
        return;
    }
#endif

    //2. create color buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(Color4f) * quad_colors.size()), m_VK_ver_color_buffer);

#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_ver_color_buffer, m_VK_ver_color_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex color buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_ver_color_buffer, quad_colors.data(), static_cast<VkDeviceSize>(sizeof(Color4f) * quad_colors.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex color buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_ver_color_buffer, m_VK_ver_color_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex color buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_ver_color_buffer, m_VK_ver_color_buf_memory, quad_colors.data(), static_cast<VkDeviceSize>(sizeof(Color4f) * quad_colors.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex color buffer failure!!!");
        return;
    }
#endif

    //3. create ver tex buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(vec2) * quad_texs.size()), m_VK_ver_tex_buffer);

#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_ver_tex_buffer, m_VK_ver_tex_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate vertex color buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_ver_tex_buffer, quad_texs.data(), static_cast<VkDeviceSize>(sizeof(vec2) * quad_texs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh vertex color buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_ver_tex_buffer, m_VK_ver_tex_buf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate host vertex color buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_ver_tex_buffer, m_VK_ver_tex_buf_memory, quad_texs.data(), static_cast<VkDeviceSize>(sizeof(vec2) * quad_texs.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh host vertex color buffer failure!!!");
        return;
    }
#endif

    //4. create indice buffer.
    //--- i. create buffer information.
    result = m_mgr->CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
        static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()), m_VK_indices_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Create indice buffer failure");
        return;
    }
#ifndef USE_HOST_BUFFER
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_indices_buffer, m_VK_ibuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate indice buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshLocalDeviceBufferData(m_VK_indices_buffer, quad_indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh indice buffer failure!!!");
        return;
    }
#else
    //--- ii. get memory.
    result = m_mgr->AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0, m_VK_indices_buffer, m_VK_ibuf_memory);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate host indice buffer failure!!!");
        return;
    }

    //--- iii. refresh data.
    result = m_mgr->RefreshHostDeviceBufferData(m_VK_indices_buffer, m_VK_ibuf_memory, quad_indices.data(), static_cast<VkDeviceSize>(sizeof(uint16_t) * quad_indices.size()));
    if (result != VK_SUCCESS) {
        SDLOGE("Refresh host indice buffer failure!!!");
        return;
    }
#endif
}

void Sample3_MultiSubpass::CreateTexture()
{
    SDLOG("Create Texture!!!");
    VkResult result = VK_SUCCESS;
    //1. load data and get w and h.
    BitmapStrongReferenceObject bitmap_ref = ImageLoader::GetRef().ReadBitmap("Texture/Lenna.png");
    //2. create info.
    if (bitmap_ref.IsNull() == false) {
        Size_ui32 img_w = bitmap_ref.GetRef().GetWidth();
        Size_ui32 img_h = bitmap_ref.GetRef().GetHeight();
        Size_ui32 img_n_of_c = bitmap_ref.GetRef().GetNumOfChannel();
        Size_ui32 img_buf_size = static_cast<Size_ui32>(bitmap_ref.GetRef().GetBufferSize());
        BitmapPixelValueType bitmap_p_type = bitmap_ref.GetRef().GetPixelValueType();
        const void *img_ptr = reinterpret_cast<const void*>(bitmap_ref.GetRef().GetBitmap());

        VkImageCreateInfo img_c_info = {};
        img_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        img_c_info.pNext = nullptr;
        img_c_info.flags = 0; //We need to set flag when we want to use sparse texture data or sth.
        img_c_info.imageType = VK_IMAGE_TYPE_2D;
        img_c_info.mipLevels = 1;// mipmap levels. We don't want to create mip map in here.
        img_c_info.arrayLayers = 1; //Use it when data is 2d texture array.
        img_c_info.tiling = VK_IMAGE_TILING_OPTIMAL; //Set texture tiling mode. If the image is linked data in system memory, we need to use VK_IMAGE_TILING_LINEAR.
        img_c_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        img_c_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT; //We will copy data to this image(trasnfer dst) and use it in shader(sampled).
        img_c_info.samples = VK_SAMPLE_COUNT_1_BIT;
        img_c_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        img_c_info.queueFamilyIndexCount = 0;
        img_c_info.pQueueFamilyIndices = nullptr;
        img_c_info.extent.width = img_w;
        img_c_info.extent.height = img_h;
        img_c_info.extent.depth = 1;
        if (bitmap_p_type.m_enum == BitmapPixelDataType_UNSIGNED_BYTE && img_n_of_c == 3) {
            img_c_info.format = VK_FORMAT_R8G8B8_UNORM;//byte and [0,1]
        }
        else if (bitmap_p_type.m_enum == BitmapPixelDataType_UNSIGNED_BYTE && img_n_of_c == 4) {
            img_c_info.format = VK_FORMAT_R8G8B8A8_UNORM;//byte and [0,1]
        } 
        else {
            SDLOGE("We can't find suitable image format.");
            return;
        }
        //3. create VKImage.
        result = m_mgr->CreateImage(img_c_info, m_VK_main_texture);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Create image error.");
        }
        // *** IMPORTANT ***
        /*
            VkPhysicalDeviceMemoryProperties::memoryTypeBits of VkImage is 130 at GTX960. Member memoryTypeBits is used to record the types
            that we can allocate buffer with it in device. If it can be allocated with the type i, the [i]th bit will be set 1.
            And then 130 in binary is 10000010. It represent we can allocate VkImage at Type 1 and type 7. We list all memory properties
            of those types and the memory with those types will be allocate at which heap as below :
                Type 1 : (No Property) heap 1 (host memory)
                Type 7 : (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) heap 0 (device memory)
            It means we can allocate device memory for VkImage with those types.
        */
        //4. allocate memory.
        result = m_mgr->AllocateMemoryAndBindToImage(m_VK_main_texture, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_VK_main_texture_memory);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Create image buffer error.");
        }
        //5. update data.
        result = m_mgr->RefreshLocalDeviceImage(m_VK_main_texture, img_ptr, img_w, img_h, 1, img_buf_size);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Refresh image buffer error.");
        }
    }
    else {
        SDLOGW("Image isn't opened.");
    }
}

void Sample3_MultiSubpass::CreateUniformBuffer()
{
    SDLOG("Create Uniform Buffer!!!");
    VkResult result = VK_SUCCESS;
    //Create basic uniform buffer
    for (uint32_t i = 0; i < 4; ++i) {
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

void Sample3_MultiSubpass::CreateShaderPrograms()
{
    SDLOG("Create Shader Programs!!!");
    static const char *vert_fns[2] = {
        "Shader/MainShader.vert.spv",
        "Shader/TexShader.vert.spv"
    };
    static const char *frag_fns[2] = {
        "Shader/MainShader.frag.spv",
        "Shader/TexShader.frag.spv"
    };
    VkResult result = VK_SUCCESS;
    for (uint32_t i = 0; i < COLOR_AND_DEPTH_BUFFER_SIZE; ++i) {
        //1. bind vertex attribute array.
        //(such like glVertexAttribBinding and glVertexAttribFormat.)
        //--- i. bind pointer.
        //------ one buffer one binding. (glVertexAttribFormat)
        VkVertexInputBindingDescription vert_input_binding_des_infos[3] = {
            {}, {}, {}
        };
        vert_input_binding_des_infos[0].binding = 0;
        vert_input_binding_des_infos[0].stride = sizeof(vec3);
        vert_input_binding_des_infos[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        vert_input_binding_des_infos[1].binding = 1;
        vert_input_binding_des_infos[1].stride = sizeof(Color4f);
        vert_input_binding_des_infos[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        vert_input_binding_des_infos[2].binding = 2;
        vert_input_binding_des_infos[2].stride = sizeof(vec2);
        vert_input_binding_des_infos[2].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        //--- ii. link bind pointer. (glVertexAttribBinding)m_VK_main_shader_set_layouts
        VkVertexInputAttributeDescription vert_input_attrib_des_infos[3] = {
            {},{},{}
        };
        //------ bind vertice attribute.
        vert_input_attrib_des_infos[0].binding = 0; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
        vert_input_attrib_des_infos[0].location = 0; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
        vert_input_attrib_des_infos[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        vert_input_attrib_des_infos[0].offset = 0;
        //------ bind color attribute.
        vert_input_attrib_des_infos[1].binding = 1; //input buffer binding. (VkPhysicalDeviceLimits::maxVertexInputBindings)
        vert_input_attrib_des_infos[1].location = 1; //shader location. (VkPhysicalDeviceLimits::maxVertexInputAttributes)
        vert_input_attrib_des_infos[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        vert_input_attrib_des_infos[1].offset = 0;
        //------ bind tex attribute.
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

        //--- iii. create input assembly states.(GL_TRIANGLE, ...etc.)
        VkPipelineInputAssemblyStateCreateInfo v_input_assembly_state_c_info = {};
        v_input_assembly_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        v_input_assembly_state_c_info.pNext = nullptr;
        v_input_assembly_state_c_info.flags = 0;
        v_input_assembly_state_c_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //GL_TRIANGLE 
        v_input_assembly_state_c_info.primitiveRestartEnable = VK_FALSE; //Use to restart idx (set 0XFFFFFFFF or 0XFFFF) and then to draw fan or strip.

        //--- iv. create tessellation state create info.
        VkPipelineTessellationStateCreateInfo tessellation_state_c_info = {};
        tessellation_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        tessellation_state_c_info.pNext = nullptr;
        tessellation_state_c_info.flags = 0;
        tessellation_state_c_info.patchControlPoints = 3; //3 : triangle. 4 : quad.

        //2. Create VkDescriptorSetLayout for main shader.
        //--- i. Create sampler for main shader.
        VkSamplerCreateInfo sampler_c_info = {};
        sampler_c_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler_c_info.pNext = nullptr;
        sampler_c_info.flags = 0; //
        sampler_c_info.minFilter = VK_FILTER_LINEAR;
        sampler_c_info.magFilter = VK_FILTER_LINEAR;
        sampler_c_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        sampler_c_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_c_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_c_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler_c_info.mipLodBias = 0.0f;
        sampler_c_info.anisotropyEnable = VK_FALSE;
        sampler_c_info.maxAnisotropy = 1.0f;
        sampler_c_info.compareEnable = VK_FALSE; //Use depth compare operation.
        sampler_c_info.compareOp = VK_COMPARE_OP_ALWAYS; //Use depth compare operation.
        sampler_c_info.minLod = 0.0f;
        sampler_c_info.maxLod = 1.0f;
        sampler_c_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        sampler_c_info.unnormalizedCoordinates = VK_FALSE;

        result = m_mgr->CreateSampler(sampler_c_info, m_VK_main_texture_sampler);
        if (result != VK_SUCCESS) {
            SDLOGE("Create sampler failure!!!");
            return;
        }

        VkImageViewCreateInfo img_view_c_info = {};
        img_view_c_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        img_view_c_info.pNext = nullptr;
        img_view_c_info.flags = 0;
        img_view_c_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        img_view_c_info.image = m_VK_main_texture;
        img_view_c_info.format = VK_FORMAT_R8G8B8A8_UNORM;
        img_view_c_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        img_view_c_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        img_view_c_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        img_view_c_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        img_view_c_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        img_view_c_info.subresourceRange.baseMipLevel = 0;
        img_view_c_info.subresourceRange.levelCount = 1;
        img_view_c_info.subresourceRange.baseArrayLayer = 0;
        img_view_c_info.subresourceRange.layerCount = 1;

        result = m_mgr->CreateImageView(img_view_c_info, m_VK_main_texture_image_view);
        if (result != VK_SUCCESS) {
            SDLOGE("Create sampler failure!!!");
            return;
        }

        //--- ii. Create basic block decriptor set layout.
        //------ VkDescriptorSetLayoutBinding(BasicUnifromBuffer)
        VkDescriptorSetLayoutBinding var_basic_uniform_buffer = {};
        var_basic_uniform_buffer.binding = 0;
        var_basic_uniform_buffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //uniform buffer
        var_basic_uniform_buffer.descriptorCount = 1; //Only one block.
        var_basic_uniform_buffer.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; //use at all shader.
        var_basic_uniform_buffer.pImmutableSamplers = nullptr;
        //------ VkDescriptorSetLayoutBinding(sampler2D)
        VkDescriptorSetLayoutBinding var_main_texture = {};
        var_main_texture.binding = 1;
        var_main_texture.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //sampler2D
        var_main_texture.descriptorCount = 1;
        var_main_texture.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; //use at all shader. 
        var_main_texture.pImmutableSamplers = nullptr; //Use it while we set nullptr when we write descriptor.

        //--- ii. Write create descriptor set layout info.
        std::vector<VkDescriptorSetLayoutBinding> uniform_var_location_set0;
        uniform_var_location_set0.push_back(var_basic_uniform_buffer);
        uniform_var_location_set0.push_back(var_main_texture);

        VkDescriptorSetLayoutCreateInfo desc_set_c_info = {}; //One set one layout.
        desc_set_c_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        desc_set_c_info.pNext = nullptr;
        desc_set_c_info.flags = 0;
        desc_set_c_info.bindingCount = static_cast<uint32_t>(uniform_var_location_set0.size());
        desc_set_c_info.pBindings = uniform_var_location_set0.data();//set = 0

        //--- iii. Create descriptor set layouts. (Create binding location for uniform variable)
        result = m_mgr->CreateDescriptorSetLayout(desc_set_c_info, m_VK_main_shader_set0_layouts[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Create descriptror set layout failure!!!");
            return;
        }

        //--- iv. Create descriptor pools. (Create pool about VkDescriptorSet)
        std::vector<VkDescriptorPoolSize> types;
        //------ VkDescriptorPoolSize(BasicUnifromBuffer)
        VkDescriptorPoolSize uniform_buffer_type = {};
        uniform_buffer_type.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uniform_buffer_type.descriptorCount = 2; //Count is equal to number of descriptor set.
        types.push_back(uniform_buffer_type);
        //------ VkDescriptorPoolSize(MainTexture)
        VkDescriptorPoolSize image_type = {};
        image_type.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        image_type.descriptorCount = 2;
        types.push_back(image_type);

        result = m_mgr->CreateDescriptorPool(types, 2, false, m_VK_descriptor_pools[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Create descriptror pool failure!!!");
            return;
        }

        //--- v. Allocate descriptor set. (space for uniform data)

        VkDescriptorSetAllocateInfo desc_set_a_info = {};
        desc_set_a_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        desc_set_a_info.pNext = nullptr;
        desc_set_a_info.descriptorPool = m_VK_descriptor_pools[i];
        desc_set_a_info.descriptorSetCount = 1;
        desc_set_a_info.pSetLayouts = &m_VK_main_shader_set0_layouts[i];
        for (uint32_t obj_id = 0; obj_id < OBJ_SIZE / 2; ++obj_id) {
            result = m_mgr->AllocateDescriptorSet(desc_set_a_info, m_VK_descriptor_set0_uniforms[i * 2 + obj_id]);
            if (result != VK_SUCCESS) {
                SDLOGE("Allocate descriptror set uniforms 0 failure(at i = %d for object %dth)!!!", i, obj_id);
                return;
            }
        }

        //--- vi. Update descriptor set.(call it before binding and call it once. don't need to call it every frame.)
        //------ VkDescriptorBufferInfo(BasicUnifromBuffer)
        //Object 0.
        for (uint32_t obj_id = 0; obj_id < OBJ_SIZE / 2; ++obj_id) {
            VkDescriptorBufferInfo basic_uniform_b_info = {};
            basic_uniform_b_info.buffer = m_VK_basic_uniform_buffers[i * 2 + obj_id];
            basic_uniform_b_info.offset = 0;
            basic_uniform_b_info.range = sizeof(BasicUniformBuffer);

            VkWriteDescriptorSet mvp_descriptor_set_info = {};
            mvp_descriptor_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            mvp_descriptor_set_info.pNext = nullptr;
            mvp_descriptor_set_info.dstSet = m_VK_descriptor_set0_uniforms[i * 2 + obj_id];
            mvp_descriptor_set_info.dstBinding = 0; //binding 0, set 0
            mvp_descriptor_set_info.descriptorCount = 1;
            mvp_descriptor_set_info.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            mvp_descriptor_set_info.pBufferInfo = &basic_uniform_b_info;
            mvp_descriptor_set_info.pImageInfo = nullptr;
            mvp_descriptor_set_info.pTexelBufferView = nullptr;
            mvp_descriptor_set_info.dstArrayElement = 0;

            //------ VkDescriptorSetLayoutBinding(MainTexture)
            VkDescriptorImageInfo main_texture_i_info = {};
            main_texture_i_info.sampler = m_VK_main_texture_sampler;
            main_texture_i_info.imageView = m_VK_main_texture_image_view;
            main_texture_i_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkWriteDescriptorSet main_descriptor_set_info = {};
            main_descriptor_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            main_descriptor_set_info.pNext = nullptr;
            main_descriptor_set_info.dstSet = m_VK_descriptor_set0_uniforms[i * 2 + obj_id];
            main_descriptor_set_info.dstBinding = 1; //binding 1, set 0
            main_descriptor_set_info.descriptorCount = 1;
            main_descriptor_set_info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            main_descriptor_set_info.pBufferInfo = nullptr;
            main_descriptor_set_info.pImageInfo = &main_texture_i_info;
            main_descriptor_set_info.pTexelBufferView = nullptr;
            main_descriptor_set_info.dstArrayElement = 0;

            std::vector<VkWriteDescriptorSet> descriptor_set_infos = {
                mvp_descriptor_set_info,
                main_descriptor_set_info
            };

            m_mgr->UpdateDescriptorSet(descriptor_set_infos);
        }
        //3. build shader.
        //--- i. read shader file.
        FileData vert_shader, frag_shader;
        if (FileResourceRequester::GetRef().AskFile(vert_fns[i], vert_shader) == false) {
            SDLOGE("%s isn't exist!!!", vert_fns[i]);
            return;
        }

        if (FileResourceRequester::GetRef().AskFile(frag_fns[i], frag_shader) == false) {
            SDLOGE("%s isn't exist!!!", frag_fns[i]);
            return;
        }
        //--- ii. create shader module.
        result = m_mgr->CreateShaderModule(vert_shader.m_file_content, m_vert_modules[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Create vert shader failure!!!");
            return;
        }

        result = m_mgr->CreateShaderModule(frag_shader.m_file_content, m_frag_modules[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Allocate frag shader failure!!!");
            return;
        }
        //4. Create shader pipeline at main renderpass.
        //--- i. Write shader stage create info for building graphics pipeline.
        std::vector<VkPipelineShaderStageCreateInfo> stage_c_infos[2];
        VkPipelineShaderStageCreateInfo vert_stage_c_info = {};
        vert_stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_stage_c_info.stage = VK_SHADER_STAGE_VERTEX_BIT; //GL_VERT_SHADER
        vert_stage_c_info.module = m_vert_modules[i]; //target shader. (glCreateShader)
        vert_stage_c_info.pName = "main";
        vert_stage_c_info.pNext = nullptr; // NULL or a pointer to an extension-specific structure.
        vert_stage_c_info.flags = 0; //reverse for future use.
        stage_c_infos[i].push_back(vert_stage_c_info);

        VkPipelineShaderStageCreateInfo frag_stage_c_info = {};
        frag_stage_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_stage_c_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT; //GL_FRAG_SHADER
        frag_stage_c_info.module = m_frag_modules[i]; //target shader. (glCreateShader)
        frag_stage_c_info.pName = "main"; //entry function name.
        frag_stage_c_info.pNext = nullptr; // NULL or a pointer to an extension-specific structure.
        frag_stage_c_info.flags = 0; //reverse for future use.
        stage_c_infos[i].push_back(frag_stage_c_info);

        //--- ii. Set states (like glEnable XXX, and set glXXXXFunc)
        Resolution res = m_mgr->GetScreenResolution();
        //------ Viewport and scissor.
        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = static_cast<float>(res.GetWidth());
        viewport.height = static_cast<float>(res.GetHeight());
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor_region = {};
        scissor_region.offset.x = 0;
        scissor_region.offset.y = 0;
        scissor_region.extent.width = res.GetWidth();
        scissor_region.extent.height = res.GetHeight();

        VkPipelineViewportStateCreateInfo viewport_state_c_info = {};
        viewport_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_c_info.pViewports = &viewport;
        viewport_state_c_info.viewportCount = 1;
        viewport_state_c_info.pScissors = &scissor_region;
        viewport_state_c_info.scissorCount = 1;

        //------ rasterize (face order, polygon mode, face culling)
        VkPipelineRasterizationStateCreateInfo raster_state_c_info = {};
        raster_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        raster_state_c_info.pNext = nullptr;
        raster_state_c_info.flags = 0;
        raster_state_c_info.rasterizerDiscardEnable = VK_FALSE;
        raster_state_c_info.polygonMode = VK_POLYGON_MODE_FILL; //glPolygonMode(GL_FILL)
        raster_state_c_info.cullMode = VK_CULL_MODE_BACK_BIT; //glCullFace(GL_BACK)
        raster_state_c_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; //glFrontFace(GL_CCW)
        raster_state_c_info.depthBiasEnable = VK_FALSE; //change depth result by new d = Bsloop * origin d + Bconstant.
        raster_state_c_info.depthBiasConstantFactor = 0.0f;
        raster_state_c_info.depthBiasSlopeFactor = 0.0f;
        raster_state_c_info.depthBiasClamp = 0.0f;
        raster_state_c_info.lineWidth = 1.0f; //glLineWidth(1.0);

        //------ multi sampling.
        VkPipelineMultisampleStateCreateInfo multisample_state_c_info = {};
        multisample_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_state_c_info.pNext = nullptr;
        multisample_state_c_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisample_state_c_info.sampleShadingEnable = VK_FALSE;
        multisample_state_c_info.minSampleShading = 0.0f;
        multisample_state_c_info.pSampleMask = nullptr;
        multisample_state_c_info.alphaToCoverageEnable = VK_FALSE;
        multisample_state_c_info.alphaToOneEnable = VK_FALSE;

        //------ depth and stencil.
        VkPipelineDepthStencilStateCreateInfo depth_stencil_c_info = {};
        depth_stencil_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil_c_info.pNext = nullptr;
        depth_stencil_c_info.flags = 0;
        depth_stencil_c_info.depthTestEnable = VK_TRUE; //glEnable(GL_DEPTH_TEST);
        depth_stencil_c_info.depthWriteEnable = VK_TRUE; //glDepthMask(GL_TRUE);
        depth_stencil_c_info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; //glDepthFunc(GL_LEQ)
        depth_stencil_c_info.minDepthBounds = 0.0f;
        depth_stencil_c_info.maxDepthBounds = 1.0f;
        depth_stencil_c_info.stencilTestEnable = VK_FALSE;
        depth_stencil_c_info.front = {};
        depth_stencil_c_info.back = {};

        //------ alpha blending.
        VkPipelineColorBlendAttachmentState color_blending_attachment_state = {};
        color_blending_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; //glColorMask(all true);
        if (i == 0) color_blending_attachment_state.blendEnable = VK_FALSE;
        else color_blending_attachment_state.blendEnable = VK_TRUE; //glDisable(GL_BLEND);
        color_blending_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //glSeperateBlendFunc for color
        color_blending_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blending_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
        color_blending_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; //glSeperateBlendFunc for alpha
        color_blending_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blending_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blending_c_info = {};
        color_blending_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending_c_info.logicOpEnable = VK_FALSE;
        color_blending_c_info.logicOp = VK_LOGIC_OP_COPY;
        color_blending_c_info.attachmentCount = 1;
        color_blending_c_info.pAttachments = &color_blending_attachment_state; //correspond with target subpass of this pipeline.
        color_blending_c_info.blendConstants[0] = 0.0f;
        color_blending_c_info.blendConstants[1] = 0.0f;
        color_blending_c_info.blendConstants[2] = 0.0f;
        color_blending_c_info.blendConstants[3] = 0.0f;

        //--- iii. Set dynamic state. (we can set related parameters about those state.)
        std::vector<VkDynamicState> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dyn_state_c_info = {};
        dyn_state_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dyn_state_c_info.pNext = nullptr;
        dyn_state_c_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
        dyn_state_c_info.pDynamicStates = dynamic_states.data();
        //--- iv. Set uniform variable location binding.
        std::vector<VkDescriptorSetLayout> desc_layouts = { m_VK_main_shader_set0_layouts[i] };
        VkPipelineLayoutCreateInfo pipeline_layout_c_info = {};
        pipeline_layout_c_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_c_info.flags = 0;
        pipeline_layout_c_info.pNext = nullptr;
        pipeline_layout_c_info.setLayoutCount = static_cast<uint32_t>(desc_layouts.size());
        pipeline_layout_c_info.pSetLayouts = desc_layouts.data();
        pipeline_layout_c_info.pushConstantRangeCount = 0;
        pipeline_layout_c_info.pPushConstantRanges = nullptr;

        result = m_mgr->CreatePipelineLayout(pipeline_layout_c_info, m_VK_pipeline_layouts[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("create pipeline layout failure!!!");
            return;
        }

        //--- v. Create graphics pipeline.
        VkGraphicsPipelineCreateInfo graphics_pipeline_c_info = {};
        graphics_pipeline_c_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_c_info.pNext = nullptr;
        graphics_pipeline_c_info.flags = 0;
        graphics_pipeline_c_info.stageCount = static_cast<uint32_t>(stage_c_infos[i].size()); //shader create info.
        graphics_pipeline_c_info.pStages = stage_c_infos[i].data();
        graphics_pipeline_c_info.pVertexInputState = &v_input_state_c_info;
        graphics_pipeline_c_info.pInputAssemblyState = &v_input_assembly_state_c_info;
        graphics_pipeline_c_info.pTessellationState = nullptr;
        graphics_pipeline_c_info.pViewportState = &viewport_state_c_info;
        graphics_pipeline_c_info.pRasterizationState = &raster_state_c_info;
        graphics_pipeline_c_info.pMultisampleState = &multisample_state_c_info;
        graphics_pipeline_c_info.pDepthStencilState = &depth_stencil_c_info;
        graphics_pipeline_c_info.pColorBlendState = &color_blending_c_info;
        graphics_pipeline_c_info.pDynamicState = &dyn_state_c_info;
        graphics_pipeline_c_info.layout = m_VK_pipeline_layouts[i];
        graphics_pipeline_c_info.basePipelineHandle = VK_NULL_HANDLE;
        graphics_pipeline_c_info.basePipelineIndex = -1;
        graphics_pipeline_c_info.renderPass = m_VK_render_pass;
        graphics_pipeline_c_info.subpass = i; //We should use pipeline in right subpass. Or it will cause error if the attachment amount are different between subpasses. 

       
        result = m_mgr->CreateGraphicsPipeline(graphics_pipeline_c_info, VK_NULL_HANDLE, m_VK_main_graphics_pipelines[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("create pipeline failure!!!");
            return;
        }
    }
}

//--------------------------------------- private  --------------------------------------
void Sample3_MultiSubpass::CreateCommandBufferAndPool()
{
    VkResult result = VK_SUCCESS;
    VkCommandPoolCreateInfo cmd_pool_c_info = {};
    cmd_pool_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_c_info.pNext = nullptr;
    cmd_pool_c_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VkCommandPoolCreateFlags

    result = m_mgr->CreateCommandPool(cmd_pool_c_info, m_VK_cmd_pool);
    if (result != VK_SUCCESS) {
        SDLOGE("Create sample3 cmd pool failure!!!");;
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
        SDLOGE("Allocate sample3 cmd buffer failure!!!");
        return;
    }
}

void Sample3_MultiSubpass::CreateRenderPass()
{
    SDLOG("--- Vulkan initialize sample 1 render pass.");
    //1. Render Pass.
    //1.1 write attachment descriptions for color and depth attachment.
    //--- Color attachment description.
    VkAttachmentDescription attachment_descs[4] = { {},{},{},{} }; //0 :color, 1: depth, 2 :color, 3: depth
    //Sp1 (draw scene firstly.)
    attachment_descs[0].flags = 0;
    attachment_descs[0].format = VK_FORMAT_R8G8B8A8_UNORM;
    attachment_descs[0].samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    attachment_descs[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //Clear before using. If we set VK_ATTACHMENT_LOAD_OP_DONT_CARE, we can't clear via clearcolor.
    attachment_descs[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Store after using. If we set VK_ATTACHMENT_STORE_OP_DONT_CARE, we can't store rendering result to the buffer binded to this attachment.
    attachment_descs[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //
    attachment_descs[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //
    attachment_descs[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_descs[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //--- Depth attachment description.
    attachment_descs[1].flags = 0;
    attachment_descs[1].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
    attachment_descs[1].samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    attachment_descs[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment_descs[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Store after using. If we set VK_ATTACHMENT_STORE_OP_DONT_CARE, we can't store rendering result to the buffer binded to this attachment.
    attachment_descs[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //
    attachment_descs[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //
    attachment_descs[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_descs[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //Sp2 (don't clear data)
    attachment_descs[2].flags = 0;
    attachment_descs[2].format = VK_FORMAT_R8G8B8A8_UNORM;
    attachment_descs[2].samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    attachment_descs[2].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; //Don't clear color and depth buffer.
    attachment_descs[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Store after using.
    attachment_descs[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //
    attachment_descs[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //
    attachment_descs[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_descs[2].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //--- Depth attachment description.
    attachment_descs[3].flags = 0;
    attachment_descs[3].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
    attachment_descs[3].samples = VK_SAMPLE_COUNT_1_BIT; //Get color from sample by sample 1 pixel.
    attachment_descs[3].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachment_descs[3].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment_descs[3].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //
    attachment_descs[3].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //
    attachment_descs[3].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment_descs[3].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //1.2 write attachment references.
    VkAttachmentReference attachment_refs[4] = { {},{},{},{} };
    //sp1
    //--- Bind the output to color attachment. //0 :color, 1: depth
    attachment_refs[0].attachment = 0; //ID of attachment in RenderPass.
    attachment_refs[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //--- Bind the output to depth attachment.
    attachment_refs[1].attachment = 1; //ID of attachment in RenderPass.
    attachment_refs[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //sp2
    //--- Bind the output to color attachment. //2 :color, 3: depth
    attachment_refs[2].attachment = 2; //ID of attachment in RenderPass.
    attachment_refs[2].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //--- Bind the output to depth attachment.
    attachment_refs[3].attachment = 3; //ID of attachment in RenderPass.
    attachment_refs[3].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //1.3 create subpass for render pass.
    VkSubpassDescription subpasses[2] = { {}, {} };
    //sp0
    subpasses[0].flags = 0;
    subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[0].inputAttachmentCount = 0;
    subpasses[0].pInputAttachments = nullptr;
    subpasses[0].colorAttachmentCount = 1;
    subpasses[0].pColorAttachments = &attachment_refs[0];
    subpasses[0].pResolveAttachments = nullptr; //for multisample.
    subpasses[0].pDepthStencilAttachment = &attachment_refs[1];
    subpasses[0].preserveAttachmentCount = 0;
    subpasses[0].pPreserveAttachments = nullptr;
    //sp1
    subpasses[1].flags = 0;
    subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[1].inputAttachmentCount = 0;
    subpasses[1].pInputAttachments = nullptr;
    subpasses[1].colorAttachmentCount = 1;
    subpasses[1].pColorAttachments = &attachment_refs[2];
    subpasses[1].pResolveAttachments = nullptr; //for multisample.
    subpasses[1].pDepthStencilAttachment = &attachment_refs[3];
    subpasses[1].preserveAttachmentCount = 0;
    subpasses[1].pPreserveAttachments = nullptr;
    //1.4 Create subpass dependecies for present pass.
    //*** IMPORTANT ***
    //subpasses are executed asynchronically. So we need dependencies to specify the pipeline barrier between two subpasses.
    //srcStageMask means we try to write data into attachments. And all operations(one command may have many operations) need to be executed
    //before this stage.
    //dstStageMask means we try to read data from attachments And all operations(one command may have many operations) will be executed
    //after this stage.
    std::vector<VkSubpassDependency> sp_dependencies;
    sp_dependencies.resize(3);
    //--- Begin dep.
    sp_dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    sp_dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    sp_dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    sp_dependencies[0].dstSubpass = 0;
    sp_dependencies[0].dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    sp_dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    sp_dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    //--- 0 to 1
    //*** IMPORTANT ***
    //Write after write dependency for sp 0 and 1. To tell behavior about execute two draw commands that are distinct in
    //first and second sub-passes simultaneously. For example of this sample, we will draw to colored lenna quads in first
    //sub-pass and two normals lenna quads in second sub-pass. So we can list our drawing flow as follow :
    //SP0.
    //--- Command for colored lenna 0. C(0,0)
    //--- Command for colored lenna 1. C(0,1)
    //SP1.
    //--- Command for normal lenna 0. C(1,0)
    //--- Command for normal lenna 1. C(1,1)
    //But commands will be executed asynchronously in different sub-passes. That is, we may see situation as follow :
    //--- C(0,0) C(1,0) C(0,1) C(1,1). It's may cause wrong presentation.
    //So we will use subpass dependency for avoiding this situation. There are two main mechanisms for subpass dependency.
    //--- Memory barrier.
    //To tell which stages we need to check memory access in pipeline when two commands are executed simultaneously.
    //--- Pipeline barrier.
    //To indicate non-critical sections in pipeline(src to dst) when two commands are executed simultaneously. In our case, 
    //we consider an extreme case, C(1,0) is executed at C(0,0) is excuting. We need to make C(1,0) is executed after C(0,0)
    //is finished. So we set source pipeline stage as bottom stage. It means GPU need to wait until stage in first command 
    //comes to bottom stage, and then executed second command.
    sp_dependencies[1].srcSubpass = 0;
    sp_dependencies[1].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; //All operators before this stage need be executed. 
    sp_dependencies[1].srcAccessMask = MemoryAccessMask_MEMORY_WRITE;
    sp_dependencies[1].dstSubpass = 1;
    sp_dependencies[1].dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    sp_dependencies[1].dstAccessMask = MemoryAccessMask_MEMORY_READ;
    sp_dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    //--- End dep.
    sp_dependencies[2].srcSubpass = 1;
    sp_dependencies[2].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    sp_dependencies[2].srcAccessMask = MemoryAccessMask_MEMORY_WRITE;
    sp_dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
    sp_dependencies[2].dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    sp_dependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    sp_dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    //1.5. Write created information for present pass.
    VkRenderPassCreateInfo pass_c_info = {};
    pass_c_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    pass_c_info.pNext = nullptr;
    pass_c_info.flags = 0; //Not implement in Vulkan, it say 'reversed for future use.
    pass_c_info.attachmentCount = 4;
    pass_c_info.pAttachments = attachment_descs;
    pass_c_info.subpassCount = 2;
    pass_c_info.pSubpasses = subpasses;
    pass_c_info.dependencyCount = static_cast<uint32_t>(sp_dependencies.size());
    pass_c_info.pDependencies = sp_dependencies.data();
    //1.6 create render pass.
    VkResult result = m_mgr->CreateRenderPass(pass_c_info, m_VK_render_pass);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample3 render pass create failure.");
        return;
    }
}

void Sample3_MultiSubpass::CreateFramebuffer()
{
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

    VkResult result = m_mgr->CreateImage(cb_c_info, m_VK_color_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample3 color buffer create failure.");
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

    for (uint32_t i = 0; i < 2; ++i) {
        result = m_mgr->CreateImageView(img_view_cb_info, m_VK_color_buffer_image_views[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Sample1 color buffer image view failure.");
            return;
        }
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
    /* **** NOTICE ****
        * This test is used to test can we bind same buffer to two different imageviews.
        * Firstly, we create one color and depth buffer and bind them to two color and depth imageviews that will be use
        * at different subpasses individually. Secondly, we divide 4 textured quads to two group. Finally, we draw first group
        * in first subpass and draw second group to second subpass. The result shows my surpose is right.
        * One buffer can be binded to two different view. And if we want to modify result in previous subpass, we can follow
        * the setting in this sample.
        */
    for (uint32_t i = 0; i < 2; ++i) {
        result = m_mgr->CreateImageView(img_view_db_info, m_VK_depth_buffer_image_views[i]);
        if (result != VK_SUCCESS) {
            SDLOGE("Sample1 depth buffer image view failure.");
            return;
        }
    }

    //VkImageView ivs[4] = {m_VK_color_buffer_image_views[0], m_VK_depth_buffer_image_views[0], m_VK_color_buffer_image_views[1], m_VK_depth_buffer_image_views[1] }; //view can share.
    VkImageView ivs[4] = { m_VK_color_buffer_image_views[0], m_VK_depth_buffer_image_views[0], m_VK_color_buffer_image_views[0], m_VK_depth_buffer_image_views[0] };

    VkFramebufferCreateInfo fbo_c_info = {};
    fbo_c_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbo_c_info.pNext = nullptr;
    fbo_c_info.flags = 0; //Reserved for future use
    fbo_c_info.renderPass = m_VK_render_pass;
    fbo_c_info.attachmentCount = 4;
    fbo_c_info.pAttachments = ivs;
    fbo_c_info.width = screen_size.GetWidth();
    fbo_c_info.height = screen_size.GetHeight();
    fbo_c_info.layers = 1;
    result = m_mgr->CreateVkFramebuffer(fbo_c_info, m_VK_frame_buffer);
    if (result != VK_SUCCESS) {
        SDLOGE("Sample1 create frame buffer failure.");
        return;
    }
}