#include "WaveVRVulkanManager.h"

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

#include "ImageAspect_Vulkan.h"
#include "WaveVRSwapchain.h"

using namespace SDE;
using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

WaveVRVulkanManager::WaveVRVulkanManager()
: VulkanManager()
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

        m_instance = vk_c_arg.m_instance;
        m_surface = vk_c_arg.m_surface;

        if (m_instance != nullptr) {
            //egl like
            InitializeVulkanEnvironment();
            InitializeSettings();
            InitializeGraphicsQueues();
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
}

void WaveVRVulkanManager::InitializeWaveVR()
{
    //1. setup WaveVR VulkanSystemInfo_t
    WVR_VulkanSystemInfo_t vk_info = {
            m_instance,
            m_phy_device,
            m_device
    };
    if (WVR_VulkanInit(&vk_info) == false) {
        SDLOGE("Fail to initialize Vulkan info!");
        throw std::runtime_error("Fail to initialize Vulkan info!");
    }
    //2. set eye resolution.
    uint32_t screen_w, screen_h;
    WVR_GetRenderTargetSize(&screen_w, &screen_h);
    Resolution eye_buffer_size;
    eye_buffer_size.SetResolution(screen_w, screen_h);
    //
    m_swapchain = new WaveVRSwapchain("WaveVRSwapchain", m_present_queue, eye_buffer_size);
    SD_SREF(m_swapchain).Initialize();
}

void WaveVRVulkanManager::CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity)
{
    VkPhysicalDevice &phy_device   = reinterpret_cast<VkPhysicalDevice&>(io_identity.m_phy_device);
    VkDevice         &device       = reinterpret_cast<VkDevice&>(io_identity.m_device);
    phy_device = m_phy_device;
    device     = m_device;
    io_identity.m_layer_size = 1;
    io_identity.SetValid();
}

void WaveVRVulkanManager::GetReadyTextureOfSwapchain(
        const GraphicsSwapchainIdentity &i_identity,
        const GraphicsSemaphoreWeakReferenceObject &i_acq_sema,
        uint32_t &io_idx)
{

}

void WaveVRVulkanManager::RenderTextureToSwapchain(
        const GraphicsSwapchainIdentity &i_identity, uint32_t i_idx,
        const GraphicsQueueWeakReferenceObject &i_queue,
        const CommandBufferWeakReferenceObject &i_cmd_buffer,
        const GraphicsSemaphoreWeakReferenceObject &i_present_sema,
        const TextureWeakReferenceObject &i_texture,
        const ImageBlitParam &i_param)
{
    VkResult result = VK_SUCCESS;
    VkDevice device = reinterpret_cast<VkDevice>(i_identity.m_device);

    const CommandBufferIdentity &cmd_identity = SD_WREF(m_graphics_identity_getter).GetIdentity(i_cmd_buffer);
    VkCommandBuffer cmd_buffer = reinterpret_cast<VkCommandBuffer>(cmd_identity.m_handle);

    const TextureIdentity &tex_identity = SD_WREF(m_graphics_identity_getter).GetIdentity(i_texture);
    VkImage src_image = reinterpret_cast<VkImage>(tex_identity.m_handle);

    //1. Record Image Copy to Swapchain Image.
    BeginCommandBuffer(cmd_identity, CommandBufferInheritanceInfo());

    VkImageBlit blit_param = {};
    blit_param.srcSubresource.aspectMask = ImageAspect_Vulkan::Convert(i_param.m_src_param.m_aspect);
    blit_param.srcSubresource.baseArrayLayer = i_param.m_src_param.m_based_layer;
    blit_param.srcSubresource.mipLevel = i_param.m_src_param.m_mip_level;
    blit_param.srcSubresource.layerCount = i_param.m_src_param.m_layer_count;
    blit_param.srcOffsets[0].x = i_param.m_src_param.m_origin[0];
    blit_param.srcOffsets[0].y = i_param.m_src_param.m_origin[1];
    blit_param.srcOffsets[0].z = i_param.m_src_param.m_origin[2];
    blit_param.srcOffsets[1].x = i_param.m_src_param.m_size[0];
    blit_param.srcOffsets[1].y = i_param.m_src_param.m_size[1];
    blit_param.srcOffsets[1].z = i_param.m_src_param.m_size[2];
    blit_param.dstSubresource.aspectMask = ImageAspect_Vulkan::Convert(i_param.m_dst_param.m_aspect);
    blit_param.dstSubresource.baseArrayLayer = i_param.m_dst_param.m_based_layer;
    blit_param.dstSubresource.mipLevel = i_param.m_dst_param.m_mip_level;
    blit_param.dstSubresource.layerCount = i_param.m_dst_param.m_layer_count;
    blit_param.dstOffsets[0].x = i_param.m_dst_param.m_origin[0];
    blit_param.dstOffsets[0].y = i_param.m_dst_param.m_origin[1] + i_param.m_dst_param.m_size[1];
    blit_param.dstOffsets[0].z = i_param.m_dst_param.m_origin[2];
    blit_param.dstOffsets[1].x = i_param.m_dst_param.m_size[0];
    blit_param.dstOffsets[1].y = i_param.m_dst_param.m_size[1] * -1.0f;
    blit_param.dstOffsets[1].z = i_param.m_dst_param.m_size[2];


    VkImage dst_image = reinterpret_cast<VkImage>(i_identity.m_swapchain_images[i_idx]);

    BlitVkImages(cmd_buffer, src_image, dst_image, blit_param);

    EndCommandBuffer(cmd_identity);
    //3. submit to present queue.
    SD_SREF(i_queue).SubmitCommandBuffers({ i_cmd_buffer });
}