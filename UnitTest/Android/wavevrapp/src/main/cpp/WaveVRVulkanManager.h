#pragma once

#include "WaveCommonFunction.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;

static const std::string sRenderPass_WaveVRComposing = "WaveVRComposingPass";

class WaveVRVulkanManager : public VulkanManager
{
public:
    explicit WaveVRVulkanManager();
    virtual ~WaveVRVulkanManager();
public:
    void CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) override;

    void GetReadyTextureOfSwapchain(
            const GraphicsSwapchainIdentity &i_identity,
            const GraphicsSemaphoreWeakReferenceObject &i_acq_sema,
            uint32_t &io_idx) override;
protected:
    void InitializeGraphicsSystemImpl(const EventArg &i_arg) override;
protected:
    void InitializeDefaultRenderPassesImpl() override;
    void InitializeBasicShaderProgramsImpl() override;
protected:
    void InitializeMainSwapchain() override;
protected:
    void InitializeWaveVR();
};