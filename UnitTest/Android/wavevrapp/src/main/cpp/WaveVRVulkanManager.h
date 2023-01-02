#pragma once

#include "WaveCommonFunction.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;

class WaveVRVulkanManager : public VulkanManager
{
public:
    explicit WaveVRVulkanManager();
    virtual ~WaveVRVulkanManager();
public:
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    void ReleaseGraphicsSystem() override;
public:
    void CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) override;

    void GetReadyTextureOfSwapchain(
            const GraphicsSwapchainIdentity &i_identity,
            const GraphicsSemaphoreWeakReferenceObject &i_acq_sema,
            uint32_t &io_idx) override;

    void RenderTextureToSwapchain(
            const GraphicsSwapchainIdentity &i_identity, uint32_t i_idx,
            const GraphicsQueueWeakReferenceObject &i_queue,
            const CommandBufferWeakReferenceObject &i_cmd_buffer,
            const GraphicsSemaphoreWeakReferenceObject &i_present_sema,
            const TextureWeakReferenceObject &i_texture,
            const ImageBlitParam &i_param) override;
protected:
    void InitializeWaveVR();
};