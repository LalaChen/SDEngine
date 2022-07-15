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

/*! \file      GraphicsIdentityGetter.h
 *  \brief     Introduce of class GraphicsIdentityGetter
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/12
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"

#include "ManagerIdentity.h"
#include "GraphicsPipeline.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "ShaderProgram.h"
#include "RenderPass.h"
#include "GraphicsQueue.h"
#include "GraphicsSynchronousElements.h"
#include "GraphicsSwapchain.h"

#include "Object.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GraphicsIdentityGetter);

class SDENGINE_CLASS GraphicsIdentityGetter : public Object
{
public:
    GraphicsIdentityGetter(const ObjectName &i_name);
    ~GraphicsIdentityGetter();
public:
    const ShaderModuleIdentity& GetIdentity(const ShaderModuleWeakReferenceObject &i_module) const;
    const TextureIdentity& GetIdentity(const TextureWeakReferenceObject &i_tex) const;
    const SamplerIdentity& GetIdentityFromTexture(const TextureWeakReferenceObject &i_tex) const;
    const FrameBufferIdentity& GetIdentity(const FrameBufferWeakReferenceObject &i_fb) const;
    const CommandBufferIdentity& GetIdentity(const CommandBufferWeakReferenceObject &i_cb) const;
    const CommandPoolIdentity& GetIdentity(const CommandPoolWeakReferenceObject &i_cp) const;
    const RenderPassIdentity& GetIdentity(const RenderPassWeakReferenceObject &i_rp) const;
    const VertexBufferIdentity& GetIdentity(const VertexBufferWeakReferenceObject &i_vb) const;
    const IndexBufferIdentity& GetIdentity(const IndexBufferWeakReferenceObject &i_ib) const;
    const UniformBufferIdentity& GetIdentity(const UniformBufferWeakReferenceObject &i_ub) const;
    const GraphicsPipelineIdentity& GetIdentity(const GraphicsPipelineWeakReferenceObject &i_pipe) const;
    const DescriptorPoolIdentity& GetIdentity(const DescriptorPoolWeakReferenceObject &i_pool) const;
    const DescriptorSetIdentity& GetIdentity(const DescriptorSetWeakReferenceObject &i_desc) const;
    const DescriptorSetLayoutIdentity& GetIdentity(const DescriptorSetLayoutWeakReferenceObject &i_ds_layout) const;
    const GraphicsQueueIdentity& GetIdentity(const GraphicsQueueWeakReferenceObject &i_queue) const;
    const GraphicsFenceIdentity& GetIdentity(const GraphicsFenceWeakReferenceObject &i_fence) const;
    const GraphicsSemaphoreIdentity& GetIdentity(const GraphicsSemaphoreWeakReferenceObject &i_semaphore) const;
    const GraphicsSwapchainIdentity& GetIdentity(const GraphicsSwapchainWeakReferenceObject &i_swapchain) const;
};


______________SD_END_GRAPHICS_NAMESPACE______________