#pragma once

#include <SDEnginePlatform.h>
#include <SDEngine.h>

using SDE::Basic::EventArg;
using SDE::Graphics::VulkanManager;
using SDE::Graphics::TextureWeakReferenceObject;

class WaveVRVulkanManager : public VulkanManager
{
public:
    explicit WaveVRVulkanManager();
    virtual ~WaveVRVulkanManager();
public:
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    void ReleaseGraphicsSystem() override;
public:
    void RenderTexture2DToScreen(const TextureWeakReferenceObject &i_tex) override;
protected:
    void InitializeWaveVR();
protected:
    WVR_TextureQueueHandle_t m_tex_queues[WVR_Eye_Both];
    std::vector<VkImage> m_tq_cb_handles[WVR_Eye_Both];
    uint32_t m_tex_queue_size;
    uint32_t m_current_eb_idices[WVR_Eye_Both];
};