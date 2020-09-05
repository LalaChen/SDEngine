#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanWrapper.h"
#include "VulkanAPITestManager.h"

using namespace::SDE::Basic;
using namespace::SDE::Graphics;

VulkanAPITestManager::VulkanAPITestManager()
: VulkanManager()
, m_cur_sample_idx(0)
//
, m_red_error_img(VK_NULL_HANDLE)
, m_red_error_img_mem(VK_NULL_HANDLE)
//
, m_VK_present_gp_set_layout(VK_NULL_HANDLE)
, m_VK_present_gp_set_pool(VK_NULL_HANDLE)
, m_VK_screen_cb_sampler(VK_NULL_HANDLE)
, m_VK_screen_vert_shader(VK_NULL_HANDLE)
, m_VK_screen_frag_shader(VK_NULL_HANDLE)
, m_VK_screen_pipeline_layout(VK_NULL_HANDLE)
, m_VK_screen_pipeline(VK_NULL_HANDLE)
, m_VK_screen_desc_set(VK_NULL_HANDLE)
, m_VK_target_cb_image_view(VK_NULL_HANDLE)
//
, m_VK_screen_ver_buffer(VK_NULL_HANDLE)
, m_VK_screen_ver_buf_memory(VK_NULL_HANDLE)
, m_VK_screen_ver_tex_buffer(VK_NULL_HANDLE)
, m_VK_screen_ver_tex_buf_memory(VK_NULL_HANDLE)
, m_VK_screen_indices_buffer(VK_NULL_HANDLE)
, m_VK_screen_ibuf_memory(VK_NULL_HANDLE)
{
    m_samples.push_back(new Sample4_DrawObjects(this));
    m_samples.push_back(new Sample3_MultiSubpass(this));
    m_samples.push_back(new Sample1_DrawTriangle(this));
    //m_samples.push_back(new Sample2_DrawScene(this));
}

VulkanAPITestManager::~VulkanAPITestManager()
{
    for (SampleStrongReferenceObject &sample : m_samples) {
        if (sample != nullptr) {
            sample.reset();
        }
    }
    m_samples.resize(0);
    m_samples.shrink_to_fit();
}

void VulkanAPITestManager::InitializeGraphicsSystem(const EventArg &i_arg)
{
    VulkanManager::InitializeGraphicsSystem(i_arg);
    InitializePresentRenderPass();
    InitializeSCImageViewsAndFBs();
    InitializeScreenRendering();
}

void VulkanAPITestManager::Initialize()
{
    VulkanManager::Initialize();

    std::vector<SampleStrongReferenceObject>::iterator iter;
    for (iter = m_samples.begin(); iter != m_samples.end(); ++iter) {
        (*iter).GetRef().Initialize();
    }

    //Initialize error texture.
    VkResult result = VK_SUCCESS;

    uint8_t error_tex[16] = {
        148,0,211,255,
        148,0,211,255,
        148,0,211,255,
        148,0,211,255
    };

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
    img_c_info.extent.width = 2;
    img_c_info.extent.height = 2;
    img_c_info.extent.depth = 1;
    img_c_info.format = VK_FORMAT_R8G8B8A8_UNORM;//byte and [0,1]
    result = CreateImage(img_c_info, m_red_error_img);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Create red texture error.");
    }
    result = AllocateMemoryAndBindToImage(m_red_error_img, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, m_red_error_img_mem);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Create red texture memory error.");
    }
    result = RefreshLocalDeviceImage(m_red_error_img, error_tex, 2, 2, 1, 16);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Refresh red texture memory error.");
    }
}

void VulkanAPITestManager::Release()
{
    if (m_red_error_img_mem != VK_NULL_HANDLE) {
        vkFreeMemory(m_device_handle, m_red_error_img_mem, nullptr);
        m_red_error_img_mem = VK_NULL_HANDLE;
    }

    if (m_red_error_img != VK_NULL_HANDLE) {
        vkDestroyImage(m_device_handle, m_red_error_img, nullptr);
        m_red_error_img = VK_NULL_HANDLE;
    }
    //
    std::vector<SampleStrongReferenceObject>::iterator iter;
    for (iter = m_samples.begin(); iter != m_samples.end(); ++iter) {
        (*iter).GetRef().Destroy();
    }
    //
    VulkanManager::Release();
}

void VulkanAPITestManager::ReleaseGraphicsSystem()
{
    ReleaseScreenRendering();
    VulkanManager::ReleaseGraphicsSystem();
}

void VulkanAPITestManager::RenderToScreen()
{
    //Render debug.
    RenderDebug();

    //Get swapchain image.
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(
        m_device_handle, m_sc_handle, sMaxImgAcqirationTime,
        m_acq_img_sema_handle, VK_NULL_HANDLE, &image_index);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        SDLOGW("We can't get nxt swapchain image.");
        return;
    }

    //Begin command buffer
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(m_main_cb_handle, &cmd_buf_c_info) != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_main_cb_handle);
        return;
    }

    /*
    VkImageBlit blit_param = {};
    blit_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.srcSubresource.baseArrayLayer = 0;
    blit_param.srcSubresource.mipLevel = 0;
    blit_param.srcSubresource.layerCount = 1;
    blit_param.srcOffsets[0].x = 0;
    blit_param.srcOffsets[0].y = 0;
    blit_param.srcOffsets[0].z = 0;
    blit_param.srcOffsets[1].x = m_screen_size.GetWidth();
    blit_param.srcOffsets[1].y = m_screen_size.GetHeight();
    blit_param.srcOffsets[1].z = 1;
    blit_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.dstSubresource.baseArrayLayer = 0;
    blit_param.dstSubresource.mipLevel = 0;
    blit_param.dstSubresource.layerCount = 1;
    blit_param.dstOffsets[0].x = 0;
    blit_param.dstOffsets[0].y = 0;
    blit_param.dstOffsets[0].z = 0;
    blit_param.dstOffsets[1].x = m_screen_size.GetWidth();
    blit_param.dstOffsets[1].y = m_screen_size.GetHeight();
    blit_param.dstOffsets[1].z = 1;

    if (m_samples[m_cur_sample_idx].GetRef().GetColorBuffer() != VK_NULL_HANDLE) {
        vkCmdBlitImage(m_main_cb_handle,
            m_samples[m_cur_sample_idx].GetRef().GetColorBuffer(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_sc_img_handles[image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit_param,
            VK_FILTER_NEAREST);
    }
    //*/

    /*
    VkImageBlit blit_param = {};
    blit_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.srcSubresource.baseArrayLayer = 0;
    blit_param.srcSubresource.mipLevel = 0;
    blit_param.srcSubresource.layerCount = 1;
    blit_param.srcOffsets[0].x = 0;
    blit_param.srcOffsets[0].y = 0;
    blit_param.srcOffsets[0].z = 0;
    blit_param.srcOffsets[1].x = 2;
    blit_param.srcOffsets[1].y = 2;
    blit_param.srcOffsets[1].z = 1;
    blit_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.dstSubresource.baseArrayLayer = 0;
    blit_param.dstSubresource.mipLevel = 0;
    blit_param.dstSubresource.layerCount = 1;
    blit_param.dstOffsets[0].x = 0;
    blit_param.dstOffsets[0].y = 0;
    blit_param.dstOffsets[0].z = 0;
    blit_param.dstOffsets[1].x = m_screen_size.GetWidth();
    blit_param.dstOffsets[1].y = m_screen_size.GetHeight();
    blit_param.dstOffsets[1].z = 1;

    if (m_red_error_img != VK_NULL_HANDLE) {
        vkCmdBlitImage(m_main_cb_handle,
            m_red_error_img, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_sc_img_handles[image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit_param,
            VK_FILTER_NEAREST);
    }
    //*/

    ///*
    //Begin RenderPass.
    VkRect2D render_area = {};
    render_area.offset = { 0, 0 };
    render_area.extent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };

    VkRenderPassBeginInfo rp_begin_info = {};
    rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.pNext = nullptr;
    rp_begin_info.renderPass = m_pre_rp_handle;
    rp_begin_info.framebuffer = m_sc_fb_handles[image_index];
    rp_begin_info.renderArea = render_area;
    rp_begin_info.clearValueCount = 1;
    rp_begin_info.pClearValues = &sClearColor;

    vkCmdBeginRenderPass(m_main_cb_handle, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    //Render to screen (To do : Compose buffers.)
    VkImageView cb_iv = m_samples[m_cur_sample_idx].GetRef().GetColorBufferImageView();
    if (cb_iv != VK_NULL_HANDLE) {
        if (cb_iv != m_VK_target_cb_image_view) {
            m_VK_target_cb_image_view = cb_iv;

            VkDescriptorImageInfo main_texture_i_info = {};
            main_texture_i_info.sampler = m_VK_screen_cb_sampler;
            main_texture_i_info.imageView = m_VK_target_cb_image_view;
            main_texture_i_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkWriteDescriptorSet main_descriptor_set_info = {};
            main_descriptor_set_info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            main_descriptor_set_info.pNext = nullptr;
            main_descriptor_set_info.dstSet = m_VK_screen_desc_set;
            main_descriptor_set_info.dstBinding = 0; //binding 0, set 0
            main_descriptor_set_info.descriptorCount = 1;
            main_descriptor_set_info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            main_descriptor_set_info.pBufferInfo = nullptr;
            main_descriptor_set_info.pImageInfo = &main_texture_i_info;
            main_descriptor_set_info.pTexelBufferView = nullptr;
            main_descriptor_set_info.dstArrayElement = 0;

            std::vector<VkWriteDescriptorSet> descriptor_set_infos = {
                main_descriptor_set_info
            };

            UpdateDescriptorSet(descriptor_set_infos);
        }
        //
        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = m_screen_size.GetHeight();
        viewport.width = static_cast<float>(m_screen_size.GetWidth());
        viewport.height = -1.0f * static_cast<float>(m_screen_size.GetHeight());
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        SetMainViewportDynamically(m_main_cb_handle, viewport);
        SetMainScissorDynamically(m_main_cb_handle, render_area);

        BindGraphicsPipeline(m_main_cb_handle, m_VK_screen_pipeline);

        std::vector<VkDescriptorSet> descs = { m_VK_screen_desc_set };
        std::vector<uint32_t> dynamic_offs = {};
        BindDescriptorSets(m_main_cb_handle, m_VK_screen_pipeline_layout, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, descs, dynamic_offs);

        //Bind Vertex Buffer.
        BindVertexBuffer(m_main_cb_handle, m_VK_screen_ver_buffer, 0, 0); //vertex
        BindVertexBuffer(m_main_cb_handle, m_VK_screen_ver_tex_buffer, 0, 1); //tex
        BindIndiceBuffer(m_main_cb_handle, m_VK_screen_indices_buffer, 0, VK_INDEX_TYPE_UINT16); //Bind indices.
        DrawByIndice(m_main_cb_handle, 6, 1, 0, 0, 0);
    }

    //End RenderPass.
    vkCmdEndRenderPass(m_main_cb_handle);
    //*/

    //End command buffer
    if (vkEndCommandBuffer(m_main_cb_handle) != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_main_cb_handle);
        return;
    }
    //Push command buffer to queue.
    VkSemaphore wait_semaphores[1] = { m_acq_img_sema_handle };
    VkPipelineStageFlags submit_wait_flags[1] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores; //wait acq image and render scene.
    submit_info.pWaitDstStageMask = submit_wait_flags;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &m_pre_sema_handle; //set present semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_main_cb_handle;

    result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure!!!");
        return;
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, sMaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
        return;
    }

    //Present to screen
    VkPresentInfoKHR p_info = {};
    p_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    p_info.pNext = nullptr;
    p_info.waitSemaphoreCount = 1;
    p_info.pWaitSemaphores = &m_pre_sema_handle;
    p_info.swapchainCount = 1;
    p_info.pSwapchains = &m_sc_handle;
    p_info.pImageIndices = &image_index;
    p_info.pResults = nullptr;

    result = vkQueuePresentKHR(m_present_q_handle, &p_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't present image by queue.");
        return;
    }
}

void VulkanAPITestManager::RenderDebug()
{
    //Test 1. Draw Trangle.
    if (m_cur_sample_idx < m_samples.size()) {
        m_samples[m_cur_sample_idx].GetRef().Render();
    }
}

void VulkanAPITestManager::Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h)
{
    VulkanManager::Resize(i_ns_handle, i_w, i_h);
    InitializeSCImageViewsAndFBs();
    for (uint32_t sample_idx = 0; sample_idx < m_samples.size(); ++sample_idx) {
        m_samples[sample_idx]->Resize(i_w, i_h);
    }
}

void VulkanAPITestManager::SetCurrentSampleIndex(uint32_t i_sample_idx)
{
    if (i_sample_idx < m_samples.size()) {
        m_cur_sample_idx = i_sample_idx;
    }
}

//------------- API ------------
//------ Memory Related
VkResult VulkanAPITestManager::AllocateMemoryAndBindToBuffer(VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkBuffer i_VK_buffer, VkDeviceMemory &io_VK_memory)
{
    //1. Get device info.
    VkResult result;
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_phy_device_handle, &phy_dev_memory_props);

    //2. get requirement info of vertices buffer.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_device_handle, i_VK_buffer, &mem_req);
    SDLOG("Req info : Size(%llu) Alignment(%llu) MemType(%u)", mem_req.size, mem_req.alignment, mem_req.memoryTypeBits);

    //3. allocate memory space following memory type and prop flag.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported =
            ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & i_memo_prop_flags) == i_memo_prop_flags);

        //--- i. Check this mem type.
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            //------ find suitable type.
            VkMemoryAllocateInfo buffer_mem_a_info = {};
            buffer_mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            buffer_mem_a_info.pNext = nullptr;
            buffer_mem_a_info.allocationSize = mem_req.size;
            buffer_mem_a_info.memoryTypeIndex = mem_type_ID;
            result = vkAllocateMemory(m_device_handle, &buffer_mem_a_info, nullptr, &io_VK_memory);
            if (result != VK_SUCCESS) {
                SDLOGE("Allocate memory failure.");
                return result;
            }
            else {
                break;
            }
        }
    }

    //5. bind memory
    return vkBindBufferMemory(m_device_handle, i_VK_buffer, io_VK_memory, i_VK_offset);
}

VkResult VulkanAPITestManager::AllocateMemoryAndBindToImage(VkImage i_VK_img, VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkDeviceMemory &io_VK_memory)
{
    //1. Get device info.
    VkResult result;
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_phy_device_handle, &phy_dev_memory_props);

    //2. Get requirement info of vertices buffer.
    VkMemoryRequirements mem_req;
    vkGetImageMemoryRequirements(m_device_handle, i_VK_img, &mem_req);
    SDLOG("Req info : Size(%llu) Alignment(%llu) MemType(%u)", mem_req.size, mem_req.alignment, mem_req.memoryTypeBits);

    //3. Allocate memory space following memory type and prop flag.
    for (uint32_t mem_type_ID = 0; mem_type_ID < phy_dev_memory_props.memoryTypeCount; ++mem_type_ID) {
        bool is_req_mem_of_this_type = mem_req.memoryTypeBits & (1 << mem_type_ID);
        bool is_req_mem_type_supported =
            ((phy_dev_memory_props.memoryTypes[mem_type_ID].propertyFlags & i_memo_prop_flags) == i_memo_prop_flags);

        //--- i. Check this mem type.
        if (is_req_mem_of_this_type == true && is_req_mem_type_supported == true) {
            //------ find suitable type.
            VkMemoryAllocateInfo buffer_mem_a_info = {};
            buffer_mem_a_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            buffer_mem_a_info.pNext = nullptr;
            buffer_mem_a_info.allocationSize = mem_req.size;
            buffer_mem_a_info.memoryTypeIndex = mem_type_ID;
            result = vkAllocateMemory(m_device_handle, &buffer_mem_a_info, nullptr, &io_VK_memory);
            if (result != VK_SUCCESS) {
                SDLOGE("Allocate memory failure.");
                return result;
            }
            else {
                break;
            }
        }
    }
    //4. Bind memory to image.
    return vkBindImageMemory(m_device_handle, i_VK_img, io_VK_memory, i_VK_offset);
}

void VulkanAPITestManager::ReleaseMemory(VkDeviceMemory i_VK_memory)
{
    if (i_VK_memory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device_handle, i_VK_memory, nullptr);
    }
}

void* VulkanAPITestManager::MapDeviceMemoryTest(VkDeviceMemory i_VK_memory, const VkDeviceSize &i_size)
{
    VkResult result = VK_SUCCESS;
    void *local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_device_handle, i_VK_memory, 0, i_size, 0, (void**)&local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
        return SD_NULL_HANDLE;
    }
    return local_ptr;
}

void VulkanAPITestManager::UnmapDeviceMemoryTest(VkDeviceMemory i_VK_memory)
{
    vkUnmapMemory(m_device_handle, i_VK_memory);
}

//------ Buffer Related
VkResult VulkanAPITestManager::CreateBuffer(VkBufferUsageFlags i_buffer_usage, VkSharingMode i_sharing_mode, VkDeviceSize i_size, VkBuffer &io_VK_buffer)
{
    //1. create buffer information.
    VkBufferCreateInfo vec_buf_c_info = {};
    vec_buf_c_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vec_buf_c_info.pNext = nullptr;
    vec_buf_c_info.flags = 0;
    vec_buf_c_info.usage = i_buffer_usage;
    vec_buf_c_info.size = i_size;
    vec_buf_c_info.sharingMode = i_sharing_mode;
    if (i_sharing_mode == VK_SHARING_MODE_EXCLUSIVE) {
        vec_buf_c_info.queueFamilyIndexCount = 0; //VK_SHARING_MODE_EXCLUSIVE don't need.
        vec_buf_c_info.pQueueFamilyIndices = nullptr;
    } 
    else {
        SDLOGW("Not support concurrent mode (I haven't design for concurrent res now)!!");
    }

    return vkCreateBuffer(m_device_handle, &vec_buf_c_info, nullptr, &io_VK_buffer);
}

VkResult VulkanAPITestManager::RefreshLocalDeviceBufferData(VkBuffer i_VK_buffer, void *i_data_ptr, VkDeviceSize i_data_size)
{
    VkResult result;
    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    //1. create staging buffer in host and then refresh data into the one.
    //--- i. create staging buffer(host memory).
    result = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, i_data_size, staging_buffer);
    if (result != VK_SUCCESS) {
        SDLOGW("Create staging buffer failure.");
        return result;
    }

    result = AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0u, staging_buffer, staging_memory);
    if (result != VK_SUCCESS) {
        SDLOGW("Allocate staging memory failure.");
        return result;
    }

    //--- ii. map memory.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_device_handle, staging_buffer, &mem_req);
    void *local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_device_handle, staging_memory, 0, mem_req.size, 0, (void**)&local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
        return result;
    }
    //--- iii. memory copy.
    std::memcpy(local_ptr, i_data_ptr, i_data_size);

    //--- iv. flush 
    /*
    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = staging_memory;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_device_handle, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }
    //*/

    //--- v. unmap memory.
    vkUnmapMemory(m_device_handle, staging_memory);

    //2. push copy command.
    //--- i. begin command buffer.
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(m_main_cb_handle, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_main_cb_handle);
        return result;
    }
    //--- ii. set buffer memory barrier (block transfer).
    VkBufferMemoryBarrier beg_mem_barrier = {};
    beg_mem_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    beg_mem_barrier.pNext = nullptr;
    beg_mem_barrier.srcAccessMask = 0; //The buffer doesn't have access state.
    beg_mem_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    beg_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    beg_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    beg_mem_barrier.buffer = i_VK_buffer;
    beg_mem_barrier.offset = 0;
    beg_mem_barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(m_main_cb_handle, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr, //memory barrier
        1, &beg_mem_barrier, //buffer memory barrier
        0, nullptr //image memory barrier
    );

    //--- iii. copy buffer.
    VkBufferCopy buf_cpy_info = {};
    buf_cpy_info.size = i_data_size;
    buf_cpy_info.srcOffset = 0;
    buf_cpy_info.dstOffset = 0;
    vkCmdCopyBuffer(m_main_cb_handle, staging_buffer, i_VK_buffer, 1, &buf_cpy_info);

    //--- iv. set buffer memory barrier (block transfer).
    VkBufferMemoryBarrier end_mem_barrier = {};
    end_mem_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    end_mem_barrier.pNext = nullptr;
    end_mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    end_mem_barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    end_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    end_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    end_mem_barrier.buffer = i_VK_buffer;
    end_mem_barrier.offset = 0;
    end_mem_barrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(m_main_cb_handle, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        0,
        0, nullptr, //memory barrier
        1, &end_mem_barrier, //buffer memory barrier
        0, nullptr //image memory barrier
    );
    //--- v. end command buffer.
    result = vkEndCommandBuffer(m_main_cb_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_main_cb_handle);
        return result;
    }

    //3. Submit command.
    VkPipelineStageFlags submit_wait_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr;
    submit_info.pWaitDstStageMask = &submit_wait_flag;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_main_cb_handle;

    result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure!!!");
        return result;
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, sMaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
        return result;
    }

    result = vkResetCommandBuffer(m_main_cb_handle, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer failure!!!");
        return result;
    }

    ReleaseMemory(staging_memory);
    DestroyBuffer(staging_buffer);

    return VK_SUCCESS;
}

VkResult VulkanAPITestManager::RefreshHostDeviceBufferData(VkBuffer i_VK_buffer, VkDeviceMemory i_VK_buffer_mem, void *i_data_ptr, VkDeviceSize i_size)
{
    VkResult result;
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_device_handle, i_VK_buffer, &mem_req);
    void *buffer_device_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_device_handle, i_VK_buffer_mem, 0, mem_req.size, 0, (void **)&buffer_device_ptr);
    if (result == VK_SUCCESS) {
        memcpy(buffer_device_ptr, i_data_ptr, i_size);
    }

    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = i_VK_buffer_mem;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_device_handle, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }

    vkUnmapMemory(m_device_handle, i_VK_buffer_mem);
    return result;
}

void VulkanAPITestManager::DestroyBuffer(VkBuffer i_VK_buffer)
{
    if (i_VK_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device_handle, i_VK_buffer, nullptr);
    }
}

//------ Image Related
VkResult VulkanAPITestManager::CreateImage(const VkImageCreateInfo &i_info, VkImage &io_VK_img)
{
    return vkCreateImage(m_device_handle, &i_info, nullptr, &io_VK_img);
}

VkResult VulkanAPITestManager::RefreshLocalDeviceImage(VkImage i_VK_img, const void *i_data_ptr, uint32_t i_width, uint32_t i_height, uint32_t i_depth, VkDeviceSize i_data_size)
{
    VkResult result = VK_NOT_READY;
    VkBuffer staging_buffer = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    if (i_data_ptr != nullptr) {
        //1. create staging buffer in host and then refresh data into the one.
        //--- i. create staging buffer(host memory).
        result = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, i_data_size, staging_buffer);
        if (result != VK_SUCCESS) {
            SDLOGW("Create staging buffer failure.");
            return result;
        }

        result = AllocateMemoryAndBindToBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0u, staging_buffer, staging_memory);
        if (result != VK_SUCCESS) {
            SDLOGW("Allocate staging memory failure.");
            return result;
        }

        //2. refresh host stage buffer.
        //--- ii. map memory.
        VkMemoryRequirements mem_req;
        vkGetBufferMemoryRequirements(m_device_handle, staging_buffer, &mem_req);
        void *local_ptr = VK_NULL_HANDLE;
        result = vkMapMemory(m_device_handle, staging_memory, 0, mem_req.size, 0, (void**)&local_ptr);
        if (result != VK_SUCCESS) {
            SDLOGE("Map buffer memory failure!!!");
            return result;
        }
        //--- iii. memory copy.
        std::memcpy(local_ptr, i_data_ptr, i_data_size);

        //--- iv. flush 
        VkMappedMemoryRange mem_ranges = {};
        mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mem_ranges.pNext = nullptr;
        mem_ranges.memory = staging_memory;
        mem_ranges.offset = 0;
        mem_ranges.size = VK_WHOLE_SIZE;
        result = vkFlushMappedMemoryRanges(m_device_handle, 1, &mem_ranges);
        if (result != VK_SUCCESS) {
            SDLOGE("Map buffer flush failure!!!");
            return result;
        }

        //--- v. unmap memory.
        vkUnmapMemory(m_device_handle, staging_memory);
        
        //3. push copy command.
        //--- i. begin command buffer.
        VkCommandBufferBeginInfo cmd_buf_c_info = {};
        cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmd_buf_c_info.pNext = nullptr;
        cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        cmd_buf_c_info.pInheritanceInfo = nullptr;

        result = vkBeginCommandBuffer(m_main_cb_handle, &cmd_buf_c_info);
        if (result != VK_SUCCESS) {
            SDLOGW("We can't begin command buffer(%x)!!!", m_main_cb_handle);
            return result;
        }

        //--- ii. set buffer memory barrier (block transfer).
        VkImageMemoryBarrier beg_mem_barrier = {};
        beg_mem_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        beg_mem_barrier.pNext = nullptr;
        beg_mem_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        beg_mem_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        beg_mem_barrier.srcAccessMask = 0; //The buffer doesn't have access state.
        beg_mem_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        beg_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        beg_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        beg_mem_barrier.image = i_VK_img;
        beg_mem_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        beg_mem_barrier.subresourceRange.baseMipLevel = 0;
        beg_mem_barrier.subresourceRange.levelCount = 1;
        beg_mem_barrier.subresourceRange.baseArrayLayer = 0;
        beg_mem_barrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(m_main_cb_handle, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, nullptr, //memory barrier
            0, nullptr, //buffer memory barrier
            1, &beg_mem_barrier //image memory barrier
        );

        //--- iii. copy buffer.
        VkBufferImageCopy buf_to_img_cpy_info = {};
        buf_to_img_cpy_info.bufferOffset = 0;
        buf_to_img_cpy_info.bufferRowLength = 0;
        buf_to_img_cpy_info.bufferImageHeight = 0;
        buf_to_img_cpy_info.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        buf_to_img_cpy_info.imageSubresource.mipLevel = 0;
        buf_to_img_cpy_info.imageSubresource.baseArrayLayer = 0;
        buf_to_img_cpy_info.imageSubresource.layerCount = 1;
        buf_to_img_cpy_info.imageOffset.x = 0;
        buf_to_img_cpy_info.imageOffset.y = 0;
        buf_to_img_cpy_info.imageOffset.z = 0;
        buf_to_img_cpy_info.imageExtent.width = i_width;
        buf_to_img_cpy_info.imageExtent.height = i_height;
        buf_to_img_cpy_info.imageExtent.depth = i_depth;

        vkCmdCopyBufferToImage(m_main_cb_handle, staging_buffer, i_VK_img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buf_to_img_cpy_info);

        //--- iv. set buffer memory barrier (block transfer).
        VkImageMemoryBarrier end_mem_barrier = {};
        end_mem_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        end_mem_barrier.pNext = nullptr;
        end_mem_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        end_mem_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        end_mem_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        end_mem_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        end_mem_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        end_mem_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        end_mem_barrier.image = i_VK_img;
        end_mem_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        end_mem_barrier.subresourceRange.baseMipLevel = 0;
        end_mem_barrier.subresourceRange.levelCount = 1;
        end_mem_barrier.subresourceRange.baseArrayLayer = 0;
        end_mem_barrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(m_main_cb_handle, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
            0,
            0, nullptr, //memory barrier
            0, nullptr, //buffer memory barrier
            1, &end_mem_barrier //image memory barrier
        );
        //--- v. end command buffer.
        result = vkEndCommandBuffer(m_main_cb_handle);
        if (result != VK_SUCCESS) {
            SDLOGW("We can't end command buffer(%x)!!!", m_main_cb_handle);
            return result;
        }

        //4. Submit command.
        VkPipelineStageFlags submit_wait_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;
        submit_info.waitSemaphoreCount = 0;
        submit_info.pWaitSemaphores = nullptr;
        submit_info.signalSemaphoreCount = 0;
        submit_info.pSignalSemaphores = nullptr;
        submit_info.pWaitDstStageMask = &submit_wait_flag;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_main_cb_handle;

        result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);
        if (result != VK_SUCCESS) {
            SDLOGW("Submit image command buffer failure!!!");
            return result;
        }

        do {
            result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, sMaxFenceWaitTime);
        } while (result == VK_TIMEOUT);
        if (result != VK_SUCCESS) {
            SDLOGW("Wait sync failure(%d)!!!", result);
            return result;
        }

        //Reset main command buffer sync.
        result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
        if (result != VK_SUCCESS) {
            SDLOGW("reset main command buffer fence failure!!!");
            return result;
        }

        result = vkResetCommandBuffer(m_main_cb_handle, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        if (result != VK_SUCCESS) {
            SDLOGW("reset main command buffer failure!!!");
            return result;
        }

        ReleaseMemory(staging_memory);
        DestroyBuffer(staging_buffer);
    }
    return result;
}

VkResult VulkanAPITestManager::RefreshHostDeviceImage(VkImage i_VK_img, VkDeviceMemory i_VK_img_mem, const void *i_data_ptr, VkDeviceSize i_size)
{
    VkResult result;
    VkMemoryRequirements mem_req;
    //1. map.
    vkGetImageMemoryRequirements(m_device_handle, i_VK_img, &mem_req);
    void *buffer_device_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_device_handle, i_VK_img_mem, 0, mem_req.size, 0, (void **)&buffer_device_ptr);
    //2. refresh memory.
    if (result == VK_SUCCESS) {
        memcpy(buffer_device_ptr, i_data_ptr, i_size);
    }

    //3. flush data.
    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = i_VK_img_mem;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_device_handle, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }

    //4. unmap.
    vkUnmapMemory(m_device_handle, i_VK_img_mem);
    return result;
}

void VulkanAPITestManager::DestroyImage(VkImage i_VK_image)
{
    if (i_VK_image != VK_NULL_HANDLE) {
        vkDestroyImage(m_device_handle, i_VK_image, nullptr);
    }
}

//----------- Shader Related
VkResult VulkanAPITestManager::CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo &i_c_info, VkDescriptorSetLayout &io_layout)
{
    VkResult result = VK_SUCCESS;

    result = vkCreateDescriptorSetLayout(m_device_handle, &i_c_info, nullptr, &io_layout);
    if (result != VK_SUCCESS) {
        SDLOGW("Create descriptor failure. Result = %x.", result);
    }

    return result;
}

void VulkanAPITestManager::DestroyDesciptorSetLayout(VkDescriptorSetLayout i_VK_layout)
{
    if (i_VK_layout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_device_handle, i_VK_layout, nullptr);
    }
}

VkResult VulkanAPITestManager::CreateShaderModule(const std::vector<SDE::Basic::UByte> &i_program_binary, VkShaderModule &io_VK_shader_module)
{
    VkResult result = VK_SUCCESS;

    VkShaderModuleCreateInfo c_info = {};
    c_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    c_info.pNext = nullptr;
    c_info.flags = 0;
    c_info.codeSize = i_program_binary.size();
    c_info.pCode = reinterpret_cast<const uint32_t*>(i_program_binary.data());

    result = vkCreateShaderModule(m_device_handle, &c_info, nullptr, &io_VK_shader_module);
    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create shader module! Result = %x.");
    }

    return result;
}

void VulkanAPITestManager::DestroyShaderModule(VkShaderModule i_VK_shader_module)
{
    if (i_VK_shader_module != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_device_handle, i_VK_shader_module, nullptr);
    }
}

VkResult VulkanAPITestManager::CreatePipelineLayout(const VkPipelineLayoutCreateInfo &i_pipeline_layout_c_info, VkPipelineLayout &io_VK_pipeline_layout)
{
    VkResult result = vkCreatePipelineLayout(m_device_handle, &i_pipeline_layout_c_info, nullptr, &io_VK_pipeline_layout);
    if (result != VK_SUCCESS) {
        SDLOGW("failed to create pipeline layout!");
    }
    return result;
}

void VulkanAPITestManager::DestroyPipelineLayout(VkPipelineLayout i_VK_pipeline_layout)
{
    if (i_VK_pipeline_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_device_handle, i_VK_pipeline_layout, nullptr);
    }
}

VkResult VulkanAPITestManager::CreateGraphicsPipeline(const VkGraphicsPipelineCreateInfo &i_c_info, VkPipelineCache i_VK_pipeline_cache, VkPipeline &io_VK_pipeline)
{
    VkResult result = vkCreateGraphicsPipelines(m_device_handle, i_VK_pipeline_cache, 1, &i_c_info, nullptr, &io_VK_pipeline);
    if (result != VK_SUCCESS) {
        SDLOGW("failed to create pipeline!");
    }

    return result;
}

void VulkanAPITestManager::DestroyGraphicsPipeline(VkPipeline i_VK_pipeline)
{
    if (i_VK_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_device_handle, i_VK_pipeline, nullptr);
    }
}

VkResult VulkanAPITestManager::CreateDescriptorPool(const std::vector<VkDescriptorPoolSize> &i_descriptor_pool_sizes, uint32_t i_max_set_num, bool i_free_individual_sets, VkDescriptorPool &io_descriptor_pool)
{
    VkDescriptorPoolCreateInfo descriptor_pool_c_info = {};
    descriptor_pool_c_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_c_info.pNext = nullptr;
    if (i_free_individual_sets == true) {
        descriptor_pool_c_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    }
    else {
        descriptor_pool_c_info.flags = 0u;
    }
    descriptor_pool_c_info.maxSets = i_max_set_num;
    descriptor_pool_c_info.poolSizeCount = static_cast<uint32_t>(i_descriptor_pool_sizes.size());
    descriptor_pool_c_info.pPoolSizes = i_descriptor_pool_sizes.data();

    VkResult result = vkCreateDescriptorPool(m_device_handle, &descriptor_pool_c_info, nullptr, &io_descriptor_pool);
    if (result != VK_SUCCESS) {
        SDLOG("Could not create a descriptor pool.");
    }
    return result;
}

void VulkanAPITestManager::DestroyDescriptorPool(VkDescriptorPool i_VK_descriptor_pool)
{
    if (i_VK_descriptor_pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_device_handle, i_VK_descriptor_pool, nullptr);
    }
}

VkResult VulkanAPITestManager::AllocateDescriptorSet(const VkDescriptorSetAllocateInfo &i_a_info, VkDescriptorSet &io_descriptor_set)
{
    VkResult result = vkAllocateDescriptorSets(m_device_handle, &i_a_info, &io_descriptor_set);
    if (result != VK_SUCCESS) {
        SDLOG("Could not allocate a descriptor set. result(%d(%x))", result, result);
    }
    return result;
}

void VulkanAPITestManager::BindDescriptorSets(VkCommandBuffer i_VK_cmd_buffer, VkPipelineLayout i_VK_pipeline_layout, VkPipelineBindPoint i_VK_pipeline_type, uint32_t i_first_set_id, const std::vector<VkDescriptorSet> &i_VK_desc_sets, const std::vector<uint32_t> &i_dynamic_offsets)
{
    vkCmdBindDescriptorSets(i_VK_cmd_buffer, i_VK_pipeline_type,
        i_VK_pipeline_layout, i_first_set_id,
        static_cast<uint32_t>(i_VK_desc_sets.size()), i_VK_desc_sets.data(),
        static_cast<uint32_t>(i_dynamic_offsets.size()), i_dynamic_offsets.data());
}

void VulkanAPITestManager::UpdateDescriptorSet(const std::vector<VkWriteDescriptorSet> &i_descriptor_w_infos)
{
    vkUpdateDescriptorSets(m_device_handle, static_cast<uint32_t>(i_descriptor_w_infos.size()), i_descriptor_w_infos.data(), 0, VK_NULL_HANDLE);
}

VkResult VulkanAPITestManager::CreateSampler(const VkSamplerCreateInfo &i_c_info, VkSampler &io_VK_sampler)
{
    return vkCreateSampler(m_device_handle, &i_c_info, nullptr, &io_VK_sampler);
}

void VulkanAPITestManager::DestroySampler(VkSampler i_VK_sampler)
{
    if (i_VK_sampler != VK_NULL_HANDLE) {
        vkDestroySampler(m_device_handle, i_VK_sampler, nullptr);
    }
}

VkResult VulkanAPITestManager::CreateImageView(const VkImageViewCreateInfo &i_iv_info, VkImageView &io_VK_image_view)
{
    return vkCreateImageView(m_device_handle, &i_iv_info, nullptr, &io_VK_image_view);
}

void VulkanAPITestManager::DestroyImageView(VkImageView i_VK_image_view)
{
    if (i_VK_image_view != VK_NULL_HANDLE) {
        vkDestroyImageView(m_device_handle, i_VK_image_view, nullptr);
    }
}

//----------- Render Pass
VkResult VulkanAPITestManager::CreateRenderPass(const VkRenderPassCreateInfo &i_rp_c_info, VkRenderPass &io_rp)
{
    return vkCreateRenderPass(m_device_handle, &i_rp_c_info, nullptr, &io_rp);
}

void VulkanAPITestManager::BeginRenderPass(const VkRenderPassBeginInfo &i_rp_b_info, const VkCommandBuffer &i_cmd_buffer)
{
    vkCmdBeginRenderPass(i_cmd_buffer, &i_rp_b_info, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanAPITestManager::EndRenderPass(const VkCommandBuffer &i_cmd_buffer)
{
    vkCmdEndRenderPass(i_cmd_buffer);
}

void VulkanAPITestManager::DestroyRenderPass(VkRenderPass i_render_pass)
{
    vkDestroyRenderPass(m_device_handle, i_render_pass, nullptr);
}

VkResult VulkanAPITestManager::CreateVkFramebuffer(const VkFramebufferCreateInfo &i_fb_c_info, VkFramebuffer &io_fb)
{
    return vkCreateFramebuffer(m_device_handle, &i_fb_c_info, nullptr, &io_fb);
}

void VulkanAPITestManager::DestroyFramebuffer(VkFramebuffer i_framebuffer)
{
    vkDestroyFramebuffer(m_device_handle, i_framebuffer, nullptr);
}

//----------- Command Buffer and pool.
VkResult VulkanAPITestManager::CreateCommandPool(const VkCommandPoolCreateInfo &i_cmd_p_c_info, VkCommandPool &io_cmd_pool)
{
    VkCommandPoolCreateInfo cmd_pool_c_info = i_cmd_p_c_info;
    cmd_pool_c_info.queueFamilyIndex = m_final_queue_fam_id;
    return vkCreateCommandPool(m_device_handle, &cmd_pool_c_info, nullptr, &io_cmd_pool);
}

VkResult VulkanAPITestManager::ResetCommandPool(VkCommandPool i_cmd_pool, bool i_clear_resource)
{
    if (i_clear_resource == false) {
        return vkResetCommandPool(m_device_handle, i_cmd_pool, 0);
    }
    else {
        return vkResetCommandPool(m_device_handle, i_cmd_pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
    }
}

void VulkanAPITestManager::DestroyCommandPool(VkCommandPool i_cmd_pool)
{
    vkDestroyCommandPool(m_device_handle, i_cmd_pool, nullptr);
}

VkResult VulkanAPITestManager::AllocateCommandBuffer(const VkCommandBufferAllocateInfo &i_cmd_buf_a_info, VkCommandBuffer &io_cmd_buffer)
{
    return vkAllocateCommandBuffers(m_device_handle, &i_cmd_buf_a_info, &io_cmd_buffer);
}

VkResult VulkanAPITestManager::BeginCommandBuffer(const VkCommandBufferBeginInfo &i_cmd_buf_b_info, VkCommandBuffer i_cmd_buffer)
{
    return vkBeginCommandBuffer(i_cmd_buffer, &i_cmd_buf_b_info);
}

VkResult VulkanAPITestManager::EndCommandBuffer(VkCommandBuffer i_cmd_buffer)
{
    return vkEndCommandBuffer(i_cmd_buffer);
}

VkResult VulkanAPITestManager::ResetCommandBuffer(VkCommandBuffer i_cmd_buffer, bool i_reset_resource)
{
    if (i_reset_resource == false) {
        return vkResetCommandBuffer(i_cmd_buffer, 0);
    }
    else {
        return vkResetCommandBuffer(i_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    }
}

void VulkanAPITestManager::FreeCommandBuffers(VkCommandPool i_target_cmd_pool, VkCommandBuffer *i_cmd_buffers, uint32_t i_buffer_size)
{
    vkFreeCommandBuffers(m_device_handle, i_target_cmd_pool, i_buffer_size, i_cmd_buffers);
}

//----------- Set Dynamic State
void VulkanAPITestManager::SetViewportsDynamically(VkCommandBuffer i_cmd_buffer, const std::vector<VkViewport> &i_viewports)
{
    vkCmdSetViewport(i_cmd_buffer, 0, static_cast<uint32_t>(i_viewports.size()), i_viewports.data());
}

void VulkanAPITestManager::SetMainViewportDynamically(VkCommandBuffer i_cmd_buffer, const VkViewport &i_viewport)
{
    vkCmdSetViewport(i_cmd_buffer, 0, 1, &i_viewport);

}

void VulkanAPITestManager::SetScissorDynamically(VkCommandBuffer i_cmd_buffer, const std::vector<VkRect2D> &i_rect)
{
    vkCmdSetScissor(i_cmd_buffer, 0, static_cast<uint32_t>(i_rect.size()), i_rect.data());
}

void VulkanAPITestManager::SetMainScissorDynamically(VkCommandBuffer i_cmd_buffer, const VkRect2D &i_rect)
{
    vkCmdSetScissor(i_cmd_buffer, 0, 1, &i_rect);
}

//----------- Draw function.
void VulkanAPITestManager::BindVertexBuffer(VkCommandBuffer i_cmd_buffer, VkBuffer i_VK_buffer, VkDeviceSize i_VK_offset, uint32_t i_binding_id)
{
    if (i_VK_buffer != VK_NULL_HANDLE) {
        vkCmdBindVertexBuffers(i_cmd_buffer, i_binding_id, 1, &i_VK_buffer, &i_VK_offset);
    }
}

void VulkanAPITestManager::BindVertexBuffers(VkCommandBuffer i_cmd_buffer, const std::vector<VkBuffer> &i_va_buffers, const std::vector<VkDeviceSize> &i_va_offset, uint32_t i_first_bind_id)
{
    if (i_va_buffers.size() != 0) {
        vkCmdBindVertexBuffers(i_cmd_buffer, i_first_bind_id, static_cast<uint32_t>(i_va_buffers.size()), i_va_buffers.data(), i_va_offset.data());
    }
}

void VulkanAPITestManager::BindIndiceBuffer(VkCommandBuffer i_cmd_buffer, VkBuffer i_VK_buffer, VkDeviceSize i_VK_offset, VkIndexType i_index_type)
{
    if (i_VK_buffer != VK_NULL_HANDLE) {
        vkCmdBindIndexBuffer(i_cmd_buffer, i_VK_buffer, i_VK_offset, i_index_type);
    }
}

void VulkanAPITestManager::BindGraphicsPipeline(VkCommandBuffer i_cmd_buffer, VkPipeline i_VK_graphics_pipeline)
{
    if (i_VK_graphics_pipeline != VK_NULL_HANDLE) {
        vkCmdBindPipeline(i_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, i_VK_graphics_pipeline);
    }
}

void VulkanAPITestManager::DrawByIndice(VkCommandBuffer i_cmd_buffer, uint32_t i_indice_size, uint32_t i_instance_count, uint32_t i_first_index, int32_t i_vertex_offset, uint32_t i_first_instance)
{
    vkCmdDrawIndexed(i_cmd_buffer, i_indice_size, i_instance_count, i_first_index, i_vertex_offset, i_first_instance);
}

//---------- Queue ----------
VkResult VulkanAPITestManager::SubmitCommandBufferToMainQueue(VkCommandBuffer i_cmd_buffer)
{
    VkResult result;
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr; //wait acq image.
    submit_info.pWaitDstStageMask = nullptr;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &i_cmd_buffer;

    result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);
   
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure(%d)!!!", result);
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, sMaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("reset command buffer fence failure(%d)!!!", result);
        return result;
    }

    return VK_SUCCESS;
}