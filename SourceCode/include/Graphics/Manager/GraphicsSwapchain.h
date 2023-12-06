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

/*! \file      GraphicsSwapchain.h
 *  \brief     Introduce of class GraphicsSwapchain
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/25
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"

#include "Resolution.h"
#include "Object.h"
#include "Texture.h"
#include "Mesh.h"
#include "RenderFlow.h"
#include "GraphicsLayer.h"
#include "GraphicsQueue.h"
#include "CommandPool.h"

#include "GraphicsSwapchainIdentity.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GraphicsSwapchain);

class SDENGINE_CLASS GraphicsSwapchain : public Object
{
public:
	friend class GraphicsIdentityGetter;
public:
	explicit GraphicsSwapchain(const ObjectName &i_name, const GraphicsQueueWeakReferenceObject &i_queue);
	virtual ~GraphicsSwapchain();
public:
	virtual void Initialize();
	virtual void RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex);
	virtual void RenderLayersToSwapchain(const std::list<GraphicsLayerStrongReferenceObject> &i_layers);
	virtual void Present();
public:
	Resolution GetResolution() const;
	uint32_t GetTextureSize() const;
protected:
	std::vector<TextureStrongReferenceObject> m_swapchain_texs;
	std::vector<RenderFlowStrongReferenceObject> m_flows;
	std::vector<bool> m_tex_valids;

	uint32_t m_last_slot_id;
	uint32_t m_current_slot_id;
	uint64_t m_frame_count;
protected:
	GraphicsSwapchainIdentity m_identity;
	CommandPoolStrongReferenceObject m_cmd_pool;
	CommandBufferWeakReferenceObject m_cmd_buffer;
	GraphicsQueueWeakReferenceObject m_queue;
	GraphicsSemaphoreStrongReferenceObject m_acq_sema;
	GraphicsSemaphoreStrongReferenceObject m_present_sema;
	MeshStrongReferenceObject m_mesh;
};

inline Resolution GraphicsSwapchain::GetResolution() const
{
	return m_identity.m_screen_size;
}

inline uint32_t GraphicsSwapchain::GetTextureSize() const
{
	return static_cast<uint32_t>(m_identity.m_swapchain_images.size());
}

______________SD_END_GRAPHICS_NAMESPACE______________