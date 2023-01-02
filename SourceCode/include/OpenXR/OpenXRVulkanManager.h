/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>

#include "SDEngineMacro.h"
#include "VulkanManager.h"
#include "OpenXRConfig.h"

using SDE::Graphics::VulkanManager;

using SDE::Graphics::GraphicsSwapchainIdentity;

using SDE::Graphics::GraphicsQueue;
using SDE::Graphics::GraphicsQueueStrongReferenceObject;
using SDE::Graphics::GraphicsQueueWeakReferenceObject;

using SDE::Graphics::CommandBuffer;
using SDE::Graphics::CommandBufferStrongReferenceObject;
using SDE::Graphics::CommandBufferWeakReferenceObject;

using SDE::Graphics::GraphicsSemaphore;
using SDE::Graphics::GraphicsSemaphoreStrongReferenceObject;
using SDE::Graphics::GraphicsSemaphoreWeakReferenceObject;

using SDE::Graphics::Texture;
using SDE::Graphics::TextureStrongReferenceObject;
using SDE::Graphics::TextureWeakReferenceObject;

using SDE::Graphics::Resolution;
using SDE::Graphics::ImageRegionSpec;
using SDE::Graphics::ImageBlitParam;

______________SD_START_OPENXR_NAMESPACE______________

class SDENGINE_CLASS OpenXRVulkanManager : public VulkanManager
{
public:
    explicit OpenXRVulkanManager();
    virtual ~OpenXRVulkanManager();
public:
    /*! \fn void InitializeGraphicsSystem(const EventArg &i_arg) override;
     *  \param [in] i_arg Vulkan creating arguments.
     *  \brief Initialize graphics API. (link dll, ...)
     */
    void InitializeGraphicsSystem(const EventArg &i_arg) override;

    /*! \fn void ReleaseGraphicsSystem() override;
     *  \brief Release graphics API.
     */
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
    void InitializeSession();
    void InitializeOpenXRSwapChain();
protected:
    XrGraphicsBindingVulkanKHR m_binding;
};


_______________SD_END_OPENXR_NAMESPACE_______________
