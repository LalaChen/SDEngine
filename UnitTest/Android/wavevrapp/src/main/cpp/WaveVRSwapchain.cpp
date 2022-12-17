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
            m_identity.m_swapchain_images[eye_id * m_tex_queue_size + eb_id] = m_tq_cb_handles[eye_id][eb_id];
        }
    }
}

void WaveVRSwapchain::RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex)
{
    const TextureIdentity &tex_identity = SD_WREF(m_graphics_identity_getter).GetIdentity(i_tex);
    if (tex_identity.m_texture_view_type != TextureViewType_TEXTURE_2D_ARRAY) {
        SDLOGE("We need texture with viewType TextureViewType_TEXTURE_2D_ARRAY for VR(Multiview). ErrorType:%d.", tex_identity.m_texture_view_type);
        throw ReasonableException("We need texture 2D array tex for VR(Multiview)!!!");
    }

}