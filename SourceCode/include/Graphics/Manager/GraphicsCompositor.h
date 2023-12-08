/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Tai-Yang, Chen

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

/*! \file      GraphicsCompositor.h
 *  \brief     Introduce of classes GraphicsCompositor.
 *  \author    Tai-Yang, Chen
 *  \date      2023/03/19
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "GraphicsSwapchain.h"
#include "RenderFlow.h"
#include "DescriptorPool.h"
#include "UniformImages.h"
#include "UniformBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS GraphicsCompositor : public Object
{
public:
	explicit GraphicsCompositor(const ObjectName &i_name, const GraphicsSwapchainWeakReferenceObject &i_swapchain);
	virtual ~GraphicsCompositor();
public:
	void Initialize();
	void Render(const std::vector<TextureWeakReferenceObject> &i_tex);
	TextureWeakReferenceObject GetValidTexture();
protected:
	std::vector<RenderFlowStrongReferenceObject> m_flows;
	std::vector<TextureStrongReferenceObject> m_compose_texs;
	std::vector<bool> m_tex_valids;
	uint32_t m_last_slot_id;
	uint32_t m_current_slot_id;
	uint64_t m_frame_count;
	CommandPoolStrongReferenceObject m_cmd_pools;
	CommandBufferWeakReferenceObject m_cmd_buffer;
	GraphicsSwapchainWeakReferenceObject m_swapchain;
protected:
	DescriptorPoolStrongReferenceObject m_dp;
	UniformBufferWeakReferenceObject m_ub;
	DescriptorSetWeakReferenceObject m_ds;
};

______________SD_END_GRAPHICS_NAMESPACE______________