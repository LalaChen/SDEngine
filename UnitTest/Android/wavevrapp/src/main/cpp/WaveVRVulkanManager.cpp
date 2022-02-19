#include <list>
#include <algorithm>

#include <wvr/wvr.h>
#include <wvr/wvr_types.h>
#include <wvr/wvr_render.h>
#include <wvr/wvr_device.h>
#include <wvr/wvr_projection.h>
#include <wvr/wvr_overlay.h>
#include <wvr/wvr_system.h>
#include <wvr/wvr_events.h>
#include <wvr/wvr_vulkan.h>

#include "WaveVRVulkanManager.h"

using namespace SDE;
using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

WaveVRVulkanManager::WaveVRVulkanManager()
: VulkanManager()
, m_tex_queues{nullptr, nullptr}
, m_tex_queue_size(0)
, m_current_eb_idices{0, 0}
{
}

WaveVRVulkanManager::~WaveVRVulkanManager()
{
}

void WaveVRVulkanManager::InitializeGraphicsSystem(const EventArg &i_arg)
{
    SDLOG("Initialize WaveVRVulkanManager.");

    if (typeid(i_arg).hash_code() == typeid(VulkanCreationArg).hash_code()) {
        VulkanCreationArg vk_c_arg = dynamic_cast<const VulkanCreationArg&>(i_arg);

        m_ins_handle = vk_c_arg.m_instance;
        m_sur_handle = vk_c_arg.m_surface;

        if (m_ins_handle != nullptr) {
            //egl like
            InitializeDebugMessage();
            InitializePhysicalDevice();
            InitializeSettings();
            InitializeDevice();
            InitializeCommandPoolAndBuffers();
            //graphics
            InitializeWaveVR();
            PrintSystemInformation();
        }
        else {
            throw std::runtime_error("VkInstance in arg is nullptr!!!");
        }
    }
}

void WaveVRVulkanManager::ReleaseGraphicsSystem()
{
    VulkanManager::ReleaseGraphicsSystem();
    WVR_ReleaseTextureQueue(m_tex_queues[WVR_Eye_Left]);
    WVR_ReleaseTextureQueue(m_tex_queues[WVR_Eye_Right]);
    m_tq_cb_handles[WVR_Eye_Left].resize(0);
    m_tq_cb_handles[WVR_Eye_Right].resize(0);
    m_current_eb_idices[WVR_Eye_Left] = 0;
    m_current_eb_idices[WVR_Eye_Right] = 0;
    m_tex_queue_size = 0;
}

void WaveVRVulkanManager::InitializeWaveVR()
{
    //1. set eye resolution.
    uint32_t screen_w, screen_h;
    WVR_GetRenderTargetSize(&screen_w, &screen_h);
    m_screen_size.SetResolution(screen_w, screen_h);
    //2. setup WaveVR VulkanSystemInfo_t
    WVR_VulkanSystemInfo_t vk_info = {
        m_ins_handle,
        m_phy_device_handle,
        m_device_handle
    };
    if (WVR_VulkanInit(&vk_info) == false) {
        SDLOGE("Fail to initialize Vulkan info!");
        throw std::runtime_error("Fail to initialize Vulkan info!");
    }

    m_tex_queues[WVR_Eye_Left] = WVR_ObtainTextureQueue(WVR_TextureTarget_VULKAN, WVR_TextureFormat_RGBA, WVR_TextureType_UnsignedByte, m_screen_size.GetWidth(), m_screen_size.GetHeight(), 0);
    m_tex_queues[WVR_Eye_Right] = WVR_ObtainTextureQueue(WVR_TextureTarget_VULKAN, WVR_TextureFormat_RGBA, WVR_TextureType_UnsignedByte, m_screen_size.GetWidth(), m_screen_size.GetHeight(), 0);
    m_tex_queue_size = WVR_GetTextureQueueLength(m_tex_queues[WVR_Eye_Left]);

    for (uint32_t eye_id = WVR_Eye_Left; eye_id < WVR_Eye_Both; ++eye_id) {
        m_tq_cb_handles[eye_id].resize(m_tex_queue_size);
        for (uint32_t eb_id = 0; eb_id < m_tq_cb_handles[eye_id].size(); ++eb_id) {
            m_tq_cb_handles[eye_id][eb_id] = WVR_GetVkImage(m_tex_queues[eye_id], eb_id);
        }
    }

}

void WaveVRVulkanManager::RenderTexture2DToScreen(const TextureWeakReferenceObject &i_tex)
{
    VkResult result = VK_SUCCESS;
    const TextureIdentity &tex_identity = GetIdentity(i_tex);
    if (tex_identity.m_texture_view_type != TextureViewType_TEXTURE_2D_ARRAY) {
        SDLOGE("We need texture with viewType TextureViewType_TEXTURE_2D_ARRAY for VR(Multiview). ErrorType:%d.", tex_identity.m_texture_view_type);
        throw std::runtime_error("We need texture 2D array tex for VR(Multiview)!!!");
    }

    if (m_present_q_handle == VK_NULL_HANDLE) {
        SDLOGE("m_present_q_handle is nullptr!!!");
        throw std::runtime_error("m_present_q_handle is nullptr!!!");
    }

    if (m_main_cb_fence_handle == VK_NULL_HANDLE) {
        SDLOGE("m_main_cb_fence_handle is nullptr!!!");
        throw std::runtime_error("m_main_cb_fence_handle is nullptr!!!");
    }

    if (m_main_cb_handle == VK_NULL_HANDLE) {
        SDLOGE("m_main_cb_handle is nullptr!!!");
        throw std::runtime_error("m_main_cb_handle is nullptr!!!");
    }

    //Begin command buffer
    VkCommandBufferBeginInfo cmd_buf_c_info = {};
    cmd_buf_c_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_c_info.pNext = nullptr;
    cmd_buf_c_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_buf_c_info.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(m_main_cb_handle, &cmd_buf_c_info);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't begin command buffer(%x)!!! error(%x)", m_main_cb_handle, result);
        return;
    }

    //Copy camera image to eye buffer.
    for (uint32_t eye_id = WVR_Eye_Left; eye_id < WVR_Eye_Both; ++eye_id) {

        VkImageBlit blit_param = {};
        blit_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit_param.srcSubresource.baseArrayLayer = eye_id;
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
        blit_param.dstOffsets[0].y = m_screen_size.GetHeight();//inverse y back. Because wave app don't need flip y for viewport.
        blit_param.dstOffsets[0].z = 0;
        blit_param.dstOffsets[1].x = m_screen_size.GetWidth();
        blit_param.dstOffsets[1].y = 0;
        blit_param.dstOffsets[1].z = 1;

        vkCmdBlitImage(m_main_cb_handle,
                       reinterpret_cast<VkImage>(tex_identity.m_image_handle),
                       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                       m_tq_cb_handles[eye_id][m_current_eb_idices[eye_id]],
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit_param,
                       VK_FILTER_NEAREST);
    }

    //End command buffer
    result = vkEndCommandBuffer(m_main_cb_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("We can't end command buffer(%x)!!! error(%x)!!!", m_main_cb_handle, result);
        return;
    }

    //Push command buffer to queue.

    VkPipelineStageFlags submit_wait_flags[1] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr; //wait acq image.
    submit_info.pWaitDstStageMask = submit_wait_flags;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr; //set present semaphore.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_main_cb_handle;

    result = vkQueueSubmit(m_present_q_handle, 1, &submit_info, m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("Submit command buffer to PresentQueue(%p) failure(%x)!!!", m_present_q_handle, result);
    }

    do {
        result = vkWaitForFences(m_device_handle, 1, &m_main_cb_fence_handle, VK_TRUE, m_vulkan_config.m_max_fence_wait_time);
    } while (result == VK_TIMEOUT);
    if (result != VK_SUCCESS) {
        SDLOGW("Wait sync failure(%x)!!!", result);
        return;
    }

    //Reset main command buffer sync.
    result = vkResetFences(m_device_handle, 1, &m_main_cb_fence_handle);
    if (result != VK_SUCCESS) {
        SDLOGW("reset main command buffer fence failure(%x)!!!", result);
    }

    //Submit frame to WVR
    for (uint32_t eye_id = WVR_Eye_Left; eye_id < WVR_Eye_Both; ++eye_id) {
        WVR_TextureParams_t eye_tex = WVR_GetTexture(m_tex_queues[eye_id], m_current_eb_idices[eye_id]);
        WVR_SubmitError e = WVR_SubmitFrame(static_cast<WVR_Eye>(eye_id), &eye_tex);
        if (e != WVR_SubmitError_None) {
            SDLOGE("Submit WVR event failure(%d)!", e);
            throw std::runtime_error("Submit frame error");
        }
    }

    m_fps_counter.AddCount();

    //Get Next index.
    m_current_eb_idices[WVR_Eye_Left] = WVR_GetAvailableTextureIndex(m_tex_queues[WVR_Eye_Left]);
    m_current_eb_idices[WVR_Eye_Right] = WVR_GetAvailableTextureIndex(m_tex_queues[WVR_Eye_Right]);
}