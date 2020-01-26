#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanAPITestManager.h"

using namespace::SDE::Basic;
using namespace::SDE::Graphics;

VulkanAPITestManager::VulkanAPITestManager()
: VulkanManager()
, m_cur_sample_idx(0)
{
    m_samples.push_back(new Sample3_MultiSubpass(this));
    m_samples.push_back(new Sample1_DrawTriangle(this));
    m_samples.push_back(new Sample2_DrawScene(this));
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
    std::vector<SampleStrongReferenceObject>::iterator iter;
    for (iter = m_samples.begin(); iter != m_samples.end(); ++iter) {
        (*iter).GetRef().Initialize();
    }
}

void VulkanAPITestManager::ReleaseGraphicsSystem()
{
    std::vector<SampleStrongReferenceObject>::iterator iter;
    for (iter = m_samples.begin(); iter != m_samples.end(); ++iter) {
        (*iter).GetRef().Destroy();
    }
    VulkanManager::ReleaseGraphicsSystem();
}

void VulkanAPITestManager::RenderToScreen()
{
    //Render debug.
    RenderDebug();

    //Get swapchain image.
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR(
        m_VK_device, m_VK_swap_chain, MaxImgAcqirationTime,
        m_VK_acq_img_semaphore, nullptr, &image_index);

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

    if (vkBeginCommandBuffer(m_VK_main_cmd_buffer, &cmd_buf_c_info) != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_VK_main_cmd_buffer);
        return;
    }

    //Begin RenderPass.
    /*
    VkRect2D render_area = {};
    render_area.offset = { 0, 0 };
    render_area.extent = { m_screen_size.GetWidth(), m_screen_size.GetHeight() };

    VkRenderPassBeginInfo rp_begin_info = {};
    rp_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.pNext = nullptr;
    rp_begin_info.renderPass = m_VK_present_render_pass;
    rp_begin_info.framebuffer = m_VK_sc_image_fbs[image_index];
    rp_begin_info.renderArea = render_area;
    rp_begin_info.clearValueCount = 1;
    rp_begin_info.pClearValues = &ClearColor;

    vkCmdBeginRenderPass(m_VK_main_cmd_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    */
    ///*
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
    blit_param.srcOffsets[1].z = 0;
    blit_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.dstSubresource.baseArrayLayer = 0;
    blit_param.dstSubresource.mipLevel = 0;
    blit_param.dstSubresource.layerCount = 1;
    blit_param.dstOffsets[0].x = 0;
    blit_param.dstOffsets[0].y = 0;
    blit_param.dstOffsets[0].z = 0;
    blit_param.dstOffsets[1].x = m_screen_size.GetWidth();
    blit_param.dstOffsets[1].y = m_screen_size.GetHeight();
    blit_param.dstOffsets[1].z = 0;

    vkCmdBlitImage(m_VK_main_cmd_buffer,
        m_samples[m_cur_sample_idx].GetRef().GetColorBuffer(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        m_VK_sc_images[image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit_param,
        VK_FILTER_NEAREST);
    //*/
    /*
    VkImageCopy copy_param = {};
    copy_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_param.srcSubresource.baseArrayLayer = 0;
    copy_param.srcSubresource.mipLevel = 0;
    copy_param.srcSubresource.layerCount = 1;
    copy_param.srcOffset.x = 0;
    copy_param.srcOffset.y = 0;
    copy_param.srcOffset.z = 0;
    copy_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_param.dstSubresource.baseArrayLayer = 0;
    copy_param.dstSubresource.mipLevel = 0;
    copy_param.dstSubresource.layerCount = 1;
    copy_param.dstOffset.x = 0;
    copy_param.dstOffset.y = 0;
    copy_param.dstOffset.z = 0;
    copy_param.extent.width = m_screen_size.GetWidth();
    copy_param.extent.height = m_screen_size.GetHeight();
    copy_param.extent.depth = 1;


    vkCmdCopyImage(m_VK_main_cmd_buffer,
        m_samples[m_cur_sample_idx].GetRef().GetColorBuffer(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        m_VK_sc_images[image_index], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_param);
    //*/
    //End RenderPass.
    //vkCmdEndRenderPass(m_VK_main_cmd_buffer);

    //End command buffer
    if (vkEndCommandBuffer(m_VK_main_cmd_buffer) != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_VK_main_cmd_buffer);
        return;
    }
    //Push command buffer to queue.
    VkSemaphore wait_semaphores[2] = { m_VK_render_scene_semaphore, m_VK_acq_img_semaphore };
    VkPipelineStageFlags submit_wait_flag = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &m_VK_acq_img_semaphore; //wait acq image and render scene.
    submit_info.pWaitDstStageMask = &submit_wait_flag;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &m_VK_present_semaphore; //set present semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_VK_main_cmd_buffer;

    result = vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure!!!");
        return;
    }

    do {
        result = vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_TRUE, MaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
        return;
    }

    //Present to screen
    VkPresentInfoKHR p_info = {};
    p_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    p_info.pNext = nullptr;
    p_info.waitSemaphoreCount = 1;
    p_info.pWaitSemaphores = &m_VK_present_semaphore;
    p_info.swapchainCount = 1;
    p_info.pSwapchains = &m_VK_swap_chain;
    p_info.pImageIndices = &image_index;
    p_info.pResults = nullptr;

    result = vkQueuePresentKHR(m_VK_present_queue, &p_info);
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

//------------- API ------------
//------ Memory Related
VkResult VulkanAPITestManager::AllocateMemoryAndBindToBuffer(VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkBuffer i_VK_buffer, VkDeviceMemory &io_VK_memory)
{
    //1. Get device info.
    VkResult result;
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);

    //2. get requirement info of vertices buffer.
    VkMemoryRequirements mem_req;
    vkGetBufferMemoryRequirements(m_VK_device, i_VK_buffer, &mem_req);
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
            result = vkAllocateMemory(m_VK_device, &buffer_mem_a_info, nullptr, &io_VK_memory);
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
    return vkBindBufferMemory(m_VK_device, i_VK_buffer, io_VK_memory, i_VK_offset);
}

VkResult VulkanAPITestManager::AllocateMemoryAndBindToImage(VkImage i_VK_img, VkFlags i_memo_prop_flags, VkDeviceSize i_VK_offset, VkDeviceMemory &io_VK_memory)
{
    //1. Get device info.
    VkResult result;
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);

    //2. Get requirement info of vertices buffer.
    VkMemoryRequirements mem_req;
    vkGetImageMemoryRequirements(m_VK_device, i_VK_img, &mem_req);
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
            result = vkAllocateMemory(m_VK_device, &buffer_mem_a_info, nullptr, &io_VK_memory);
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
    return vkBindImageMemory(m_VK_device, i_VK_img, io_VK_memory, i_VK_offset);
}

void VulkanAPITestManager::ReleaseMemory(VkDeviceMemory i_VK_memory)
{
    if (i_VK_memory != VK_NULL_HANDLE) {
        vkFreeMemory(m_VK_device, i_VK_memory, nullptr);
    }
}

void* VulkanAPITestManager::MapDeviceMemoryTest(VkDeviceMemory i_VK_memory, const VkDeviceSize &i_size)
{
    VkResult result = VK_SUCCESS;
    void *local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, i_VK_memory, 0, i_size, 0, (void**)&local_ptr);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer memory failure!!!");
        return SD_NULL_HANDLE;
    }
    return local_ptr;
}

void VulkanAPITestManager::UnmapDeviceMemoryTest(VkDeviceMemory i_VK_memory)
{
    vkUnmapMemory(m_VK_device, i_VK_memory);
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

    return vkCreateBuffer(m_VK_device, &vec_buf_c_info, nullptr, &io_VK_buffer);
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
    vkGetBufferMemoryRequirements(m_VK_device, staging_buffer, &mem_req);
    void *local_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, staging_memory, 0, mem_req.size, 0, (void**)&local_ptr);
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
    result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }
    //*/

    //--- v. unmap memory.
    vkUnmapMemory(m_VK_device, staging_memory);

    //2. push copy command.
    //--- i. begin command buffer.
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(m_VK_main_cmd_buffer, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!!", m_VK_main_cmd_buffer);
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

    vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
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
    vkCmdCopyBuffer(m_VK_main_cmd_buffer, staging_buffer, i_VK_buffer, 1, &buf_cpy_info);

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

    vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
        0,
        0, nullptr, //memory barrier
        1, &end_mem_barrier, //buffer memory barrier
        0, nullptr //image memory barrier
    );
    //--- v. end command buffer.
    result = vkEndCommandBuffer(m_VK_main_cmd_buffer);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!!", m_VK_main_cmd_buffer);
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
    submit_info.pCommandBuffers = &m_VK_main_cmd_buffer;

    result = vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure!!!");
        return result;
    }

    do {
        result = vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_TRUE, MaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure!!!");
        return result;
    }

    result = vkResetCommandBuffer(m_VK_main_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
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
    vkGetBufferMemoryRequirements(m_VK_device, i_VK_buffer, &mem_req);
    void *buffer_device_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, i_VK_buffer_mem, 0, mem_req.size, 0, (void **)&buffer_device_ptr);
    if (result == VK_SUCCESS) {
        memcpy(buffer_device_ptr, i_data_ptr, i_size);
    }

    VkMappedMemoryRange mem_ranges = {};
    mem_ranges.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mem_ranges.pNext = nullptr;
    mem_ranges.memory = i_VK_buffer_mem;
    mem_ranges.offset = 0;
    mem_ranges.size = VK_WHOLE_SIZE;
    result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }

    vkUnmapMemory(m_VK_device, i_VK_buffer_mem);
    return result;
}

void VulkanAPITestManager::DestroyBuffer(VkBuffer i_VK_buffer)
{
    if (i_VK_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_VK_device, i_VK_buffer, nullptr);
    }
}

//------ Image Related
VkResult VulkanAPITestManager::CreateImage(const VkImageCreateInfo &i_info, VkImage &io_VK_img)
{
    return vkCreateImage(m_VK_device, &i_info, nullptr, &io_VK_img);
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
        vkGetBufferMemoryRequirements(m_VK_device, staging_buffer, &mem_req);
        void *local_ptr = VK_NULL_HANDLE;
        result = vkMapMemory(m_VK_device, staging_memory, 0, mem_req.size, 0, (void**)&local_ptr);
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
        result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
        if (result != VK_SUCCESS) {
            SDLOGE("Map buffer flush failure!!!");
            return result;
        }

        //--- v. unmap memory.
        vkUnmapMemory(m_VK_device, staging_memory);
        
        //3. push copy command.
        //--- i. begin command buffer.
        VkCommandBufferBeginInfo cmd_buf_c_info = {};
        cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmd_buf_c_info.pNext = nullptr;
        cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        cmd_buf_c_info.pInheritanceInfo = nullptr;

        result = vkBeginCommandBuffer(m_VK_main_cmd_buffer, &cmd_buf_c_info);
        if (result != VK_SUCCESS) {
            SDLOGW("We can't begin command buffer(%x)!!!", m_VK_main_cmd_buffer);
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

        vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
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

        vkCmdCopyBufferToImage(m_VK_main_cmd_buffer, staging_buffer, i_VK_img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buf_to_img_cpy_info);

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

        vkCmdPipelineBarrier(m_VK_main_cmd_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
            0,
            0, nullptr, //memory barrier
            0, nullptr, //buffer memory barrier
            1, &end_mem_barrier //image memory barrier
        );
        //--- v. end command buffer.
        result = vkEndCommandBuffer(m_VK_main_cmd_buffer);
        if (result != VK_SUCCESS) {
            SDLOGW("We can't end command buffer(%x)!!!", m_VK_main_cmd_buffer);
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
        submit_info.pCommandBuffers = &m_VK_main_cmd_buffer;

        result = vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence);
        if (result != VK_SUCCESS) {
            SDLOGW("Submit image command buffer failure!!!");
            return result;
        }

        do {
            result = vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_TRUE, MaxFenceWaitTime);
        } while (result == VK_TIMEOUT);
        if (result != VK_SUCCESS) {
            SDLOGW("Wait sync failure(%d)!!!", result);
            return result;
        }

        //Reset main command buffer sync.
        result = vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence);
        if (result != VK_SUCCESS) {
            SDLOGW("reset main command buffer fence failure!!!");
            return result;
        }

        result = vkResetCommandBuffer(m_VK_main_cmd_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
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
    vkGetImageMemoryRequirements(m_VK_device, i_VK_img, &mem_req);
    void *buffer_device_ptr = VK_NULL_HANDLE;
    result = vkMapMemory(m_VK_device, i_VK_img_mem, 0, mem_req.size, 0, (void **)&buffer_device_ptr);
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
    result = vkFlushMappedMemoryRanges(m_VK_device, 1, &mem_ranges);
    if (result != VK_SUCCESS) {
        SDLOGE("Map buffer flush failure!!!");
        return result;
    }

    //4. unmap.
    vkUnmapMemory(m_VK_device, i_VK_img_mem);
    return result;
}

void VulkanAPITestManager::DestroyImage(VkImage i_VK_image)
{
    if (i_VK_image != VK_NULL_HANDLE) {
        vkDestroyImage(m_VK_device, i_VK_image, nullptr);
    }
}

//----------- Shader Related
VkResult VulkanAPITestManager::CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo &i_c_info, VkDescriptorSetLayout &io_layout)
{
    VkResult result = VK_SUCCESS;

    result = vkCreateDescriptorSetLayout(m_VK_device, &i_c_info, nullptr, &io_layout);
    if (result != VK_SUCCESS) {
        SDLOGW("Create descriptor failure. Result = %x.", result);
    }

    return result;
}

void VulkanAPITestManager::DestroyDesciptorSetLayout(VkDescriptorSetLayout i_VK_layout)
{
    if (i_VK_layout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_VK_device, i_VK_layout, nullptr);
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

    result = vkCreateShaderModule(m_VK_device, &c_info, nullptr, &io_VK_shader_module);
    if (result != VK_SUCCESS) {
        SDLOGW("Failed to create shader module! Result = %x.");
    }

    return result;
}

void VulkanAPITestManager::DestroyShaderModule(VkShaderModule i_VK_shader_module)
{
    if (i_VK_shader_module != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_VK_device, i_VK_shader_module, nullptr);
    }
}

VkResult VulkanAPITestManager::CreatePipelineLayout(const VkPipelineLayoutCreateInfo &i_pipeline_layout_c_info, VkPipelineLayout &io_VK_pipeline_layout)
{
    VkResult result = vkCreatePipelineLayout(m_VK_device, &i_pipeline_layout_c_info, nullptr, &io_VK_pipeline_layout);
    if (result != VK_SUCCESS) {
        SDLOGW("failed to create pipeline layout!");
    }
    return result;
}

void VulkanAPITestManager::DestroyPipelineLayout(VkPipelineLayout i_VK_pipeline_layout)
{
    if (i_VK_pipeline_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_VK_device, i_VK_pipeline_layout, nullptr);
    }
}

VkResult VulkanAPITestManager::CreateGraphicsPipeline(const VkGraphicsPipelineCreateInfo &i_c_info, VkPipelineCache i_VK_pipeline_cache, VkPipeline &io_VK_pipeline)
{
    VkResult result = vkCreateGraphicsPipelines(m_VK_device, i_VK_pipeline_cache, 1, &i_c_info, nullptr, &io_VK_pipeline);
    if (result != VK_SUCCESS) {
        SDLOGW("failed to create pipeline!");
    }

    return result;
}

void VulkanAPITestManager::DestroyGraphicsPipeline(VkPipeline i_VK_pipeline)
{
    if (i_VK_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_VK_device, i_VK_pipeline, nullptr);
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

    VkResult result = vkCreateDescriptorPool(m_VK_device, &descriptor_pool_c_info, nullptr, &io_descriptor_pool);
    if (result != VK_SUCCESS) {
        SDLOG("Could not create a descriptor pool.");
    }
    return result;
}

void VulkanAPITestManager::DestroyDescriptorPool(VkDescriptorPool i_VK_descriptor_pool)
{
    if (i_VK_descriptor_pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_VK_device, i_VK_descriptor_pool, nullptr);
    }
}

VkResult VulkanAPITestManager::AllocateDescriptorSet(const VkDescriptorSetAllocateInfo &i_a_info, VkDescriptorSet &io_descriptor_set)
{
    VkResult result = vkAllocateDescriptorSets(m_VK_device, &i_a_info, &io_descriptor_set);
    if (result != VK_SUCCESS) {
        SDLOG("Could not allocate a descriptor set.");
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
    vkUpdateDescriptorSets(m_VK_device, static_cast<uint32_t>(i_descriptor_w_infos.size()), i_descriptor_w_infos.data(), 0, nullptr);
}

VkResult VulkanAPITestManager::CreateSampler(const VkSamplerCreateInfo &i_c_info, VkSampler &io_VK_sampler)
{
    return vkCreateSampler(m_VK_device, &i_c_info, nullptr, &io_VK_sampler);
}

void VulkanAPITestManager::DestroySampler(VkSampler i_VK_sampler)
{
    if (i_VK_sampler != VK_NULL_HANDLE) {
        vkDestroySampler(m_VK_device, i_VK_sampler, nullptr);
    }
}

VkResult VulkanAPITestManager::CreateImageView(const VkImageViewCreateInfo &i_iv_info, VkImageView &io_VK_image_view)
{
    return vkCreateImageView(m_VK_device, &i_iv_info, nullptr, &io_VK_image_view);
}

void VulkanAPITestManager::DestroyImageView(VkImageView i_VK_image_view)
{
    if (i_VK_image_view != VK_NULL_HANDLE) {
        vkDestroyImageView(m_VK_device, i_VK_image_view, nullptr);
    }
}

//----------- Render Pass
VkResult VulkanAPITestManager::CreateRenderPass(const VkRenderPassCreateInfo &i_rp_c_info, VkRenderPass &io_rp)
{
    return vkCreateRenderPass(m_VK_device, &i_rp_c_info, nullptr, &io_rp);
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
    vkDestroyRenderPass(m_VK_device, i_render_pass, nullptr);
}

VkResult VulkanAPITestManager::CreateVkFramebuffer(const VkFramebufferCreateInfo &i_fb_c_info, VkFramebuffer &io_fb)
{
    return vkCreateFramebuffer(m_VK_device, &i_fb_c_info, nullptr, &io_fb);
}

void VulkanAPITestManager::DestroyFramebuffer(VkFramebuffer i_framebuffer)
{
    vkDestroyFramebuffer(m_VK_device, i_framebuffer, nullptr);
}

//----------- Command Buffer and pool.
VkResult VulkanAPITestManager::CreateCommandPool(const VkCommandPoolCreateInfo &i_cmd_p_c_info, VkCommandPool &io_cmd_pool)
{
    VkCommandPoolCreateInfo cmd_pool_c_info = i_cmd_p_c_info;
    cmd_pool_c_info.queueFamilyIndex = m_VK_picked_queue_family_id;
    return vkCreateCommandPool(m_VK_device, &cmd_pool_c_info, nullptr, &io_cmd_pool);
}

VkResult VulkanAPITestManager::ResetCommandPool(VkCommandPool i_cmd_pool, bool i_clear_resource)
{
    if (i_clear_resource == false) {
        return vkResetCommandPool(m_VK_device, i_cmd_pool, 0);
    }
    else {
        return vkResetCommandPool(m_VK_device, i_cmd_pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
    }
}

void VulkanAPITestManager::DestroyCommandPool(VkCommandPool i_cmd_pool)
{
    vkDestroyCommandPool(m_VK_device, i_cmd_pool, nullptr);
}

VkResult VulkanAPITestManager::AllocateCommandBuffer(const VkCommandBufferAllocateInfo &i_cmd_buf_a_info, VkCommandBuffer &io_cmd_buffer)
{
    return vkAllocateCommandBuffers(m_VK_device, &i_cmd_buf_a_info, &io_cmd_buffer);
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
    vkFreeCommandBuffers(m_VK_device, i_target_cmd_pool, i_buffer_size, i_cmd_buffers);
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
    submit_info.pSignalSemaphores = nullptr; //set render scene semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &i_cmd_buffer;

    result = vkQueueSubmit(m_VK_present_queue, 1, &submit_info, m_VK_main_cmd_buf_fence);
   
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer failure(%d)!!!", result);
    }

    do {
        result = vkWaitForFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence, VK_TRUE, MaxFenceWaitTime);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%d)!!!", result);
        return result;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_VK_device, 1, &m_VK_main_cmd_buf_fence);
    if (result != VK_SUCCESS) {
        SDLOGW("reset command buffer fence failure(%d)!!!", result);
        return result;
    }

    return VK_SUCCESS;
}