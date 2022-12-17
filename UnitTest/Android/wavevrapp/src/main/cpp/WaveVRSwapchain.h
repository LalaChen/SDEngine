#pragma once

#include <SDEngine.h>

using namespace SDE::Basic;
using namespace SDE::Graphics;

class WaveVRSwapchain : public GraphicsSwapchain
{
public:
    explicit WaveVRSwapchain(const ObjectName &i_name, const GraphicsQueueWeakReferenceObject &i_queue, const Resolution &i_eye_buffer_size);
    virtual ~WaveVRSwapchain();
public:
    void Initialize() override;
    void RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex) override;

protected:
    WVR_TextureQueueHandle_t m_tex_queues[WVR_Eye_Both];
    std::vector<VkImage> m_tq_cb_handles[WVR_Eye_Both]
    uint32_t m_tex_queue_size;
};