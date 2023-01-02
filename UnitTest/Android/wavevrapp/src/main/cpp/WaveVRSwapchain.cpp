#include "WaveVRSwapchain.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

WaveVRSwapchain::WaveVRSwapchain(
        const ObjectName &i_name,
        const GraphicsQueueWeakReferenceObject &i_queue,
        const Resolution &i_eye_buffer_size)
: GraphicsSwapchain(i_name, i_queue)
, m_tex_queue_size(0)
{
    m_identity.m_screen_size = i_eye_buffer_size;
}

WaveVRSwapchain::~WaveVRSwapchain()
{
    WVR_ReleaseTextureQueue(m_tex_queues[WVR_Eye_Left]);
    WVR_ReleaseTextureQueue(m_tex_queues[WVR_Eye_Right]);
    m_identity = GraphicsSwapchainIdentity();
}

void WaveVRSwapchain::Initialize()
{
    GraphicsManager::GetRef().CreateGraphicsSwapchain(m_identity);

    m_tex_queues[WVR_Eye_Left]  = WVR_ObtainTextureQueue(WVR_TextureTarget_VULKAN, WVR_TextureFormat_RGBA, WVR_TextureType_UnsignedByte, m_identity.m_screen_size.GetWidth(), m_identity.m_screen_size.GetHeight(), 0);
    m_tex_queues[WVR_Eye_Right] = WVR_ObtainTextureQueue(WVR_TextureTarget_VULKAN, WVR_TextureFormat_RGBA, WVR_TextureType_UnsignedByte, m_identity.m_screen_size.GetWidth(), m_identity.m_screen_size.GetHeight(), 0);

    m_tex_queue_size = WVR_GetTextureQueueLength(m_tex_queues[WVR_Eye_Left]);
    m_identity.m_swapchain_images.resize(m_tex_queue_size * 2);

    for (uint32_t eye_id = WVR_Eye_Left; eye_id < WVR_Eye_Both; ++eye_id) {
        m_tq_cb_handles[eye_id].resize(m_tex_queue_size);
        for (uint32_t eb_id = 0; eb_id < m_tq_cb_handles[eye_id].size(); ++eb_id) {
            m_tq_cb_handles[eye_id][eb_id] = WVR_GetVkImage(m_tex_queues[eye_id], eb_id);
            m_identity.m_swapchain_images[eye_id * m_tex_queue_size + eb_id] =
                    reinterpret_cast<CompHandle>(m_tq_cb_handles[eye_id][eb_id]);
        }
    }
}

void WaveVRSwapchain::RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex)
{
    TextureTypeEnum tex_type = SD_WREF(i_tex).GetTextureType();
    if (tex_type != TextureType_TEXTURE_2D_ARRAY) {
        SDLOGE("We need texture with viewType TextureViewType_TEXTURE_2D_ARRAY for VR(Multiview). ErrorType:%d.", tex_type);
        throw ReasonableException("We need texture 2D array tex for VR(Multiview)!!!");
    }
    uint32_t eye_idices[WVR_Eye_Both] = {0};
    eye_idices[0] = WVR_GetAvailableTextureIndex(m_tex_queues[WVR_Eye_Left]);
    eye_idices[1] = WVR_GetAvailableTextureIndex(m_tex_queues[WVR_Eye_Right]);

    //2. Blit image to wave swapchain.
    if (eye_idices[0] < m_tex_queue_size && eye_idices[1] < m_tex_queue_size) {
        //2.1 left
        ImageBlitParam param;
        param.m_src_param.m_aspect = ImageAspect_ASPECT_COLOR;
        param.m_src_param.m_based_layer = 0;
        param.m_src_param.m_layer_count = 1;
        param.m_src_param.m_mip_level = SD_WREF(i_tex).GetMipMapLevels();
        param.m_src_param.m_origin[0] = 0;
        param.m_src_param.m_origin[1] = 0;
        param.m_src_param.m_origin[2] = 0;
        param.m_src_param.m_size[0] = SD_WREF(i_tex).GetWidth();
        param.m_src_param.m_size[1] = SD_WREF(i_tex).GetHeight();
        param.m_src_param.m_size[2] = 1;
        param.m_dst_param.m_aspect = ImageAspect_ASPECT_COLOR;
        param.m_dst_param.m_based_layer = 0;
        param.m_dst_param.m_layer_count = m_identity.m_layer_size;
        param.m_dst_param.m_mip_level = 0;
        param.m_dst_param.m_origin[0] = 0;
        param.m_dst_param.m_origin[1] = 0;
        param.m_dst_param.m_origin[2] = 0;
        param.m_dst_param.m_size[0] = m_identity.m_screen_size.GetWidth();
        param.m_dst_param.m_size[1] = m_identity.m_screen_size.GetHeight();
        param.m_dst_param.m_size[2] = 1;

        GraphicsManager::GetRef().RenderTextureToSwapchain(
                m_identity, eye_idices[WVR_Eye_Left],
                m_queue, m_cmd_buffer, m_present_sema, i_tex, param);
        //2.2 right
        param.m_src_param.m_aspect = ImageAspect_ASPECT_COLOR;
        param.m_src_param.m_based_layer = 1;
        param.m_src_param.m_layer_count = 1;
        param.m_src_param.m_mip_level = SD_WREF(i_tex).GetMipMapLevels();
        param.m_src_param.m_origin[0] = 0;
        param.m_src_param.m_origin[1] = 0;
        param.m_src_param.m_origin[2] = 0;
        param.m_src_param.m_size[0] = SD_WREF(i_tex).GetWidth();
        param.m_src_param.m_size[1] = SD_WREF(i_tex).GetHeight();
        param.m_src_param.m_size[2] = 1;
        param.m_dst_param.m_aspect = ImageAspect_ASPECT_COLOR;
        param.m_dst_param.m_based_layer = 0;
        param.m_dst_param.m_layer_count = m_identity.m_layer_size;
        param.m_dst_param.m_mip_level = 0;
        param.m_dst_param.m_origin[0] = 0;
        param.m_dst_param.m_origin[1] = 0;
        param.m_dst_param.m_origin[2] = 0;
        param.m_dst_param.m_size[0] = m_identity.m_screen_size.GetWidth();
        param.m_dst_param.m_size[1] = m_identity.m_screen_size.GetHeight();
        param.m_dst_param.m_size[2] = 1;

        GraphicsManager::GetRef().RenderTextureToSwapchain(
                m_identity, m_tex_queue_size + eye_idices[WVR_Eye_Right],
                m_queue, m_cmd_buffer, m_present_sema, i_tex, param);

        //Submit frame to WVR
        for (uint32_t eye_id = WVR_Eye_Left; eye_id < WVR_Eye_Both; ++eye_id) {
            WVR_TextureParams_t eye_tex = WVR_GetTexture(m_tex_queues[eye_id], eye_idices[eye_id]);
            WVR_SubmitError e = WVR_SubmitFrame(static_cast<WVR_Eye>(eye_id), &eye_tex);
            if (e != WVR_SubmitError_None) {
                SDLOGE("Submit WVR event failure(%d)!", e);
                throw std::runtime_error("Submit frame error");
            }
        }
    }
}