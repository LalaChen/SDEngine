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

#include "GraphicsSwapchain.h"
#include "LogManager.h"
#include "GraphicsManager.h"
#include "BasicShapeCreator.h"
#include "RenderFlow.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsSwapchain::GraphicsSwapchain(const ObjectName &i_name, const GraphicsQueueWeakReferenceObject &i_queue)
: Object(i_name)
, m_queue(i_queue)
, m_last_slot_id(0)
, m_current_slot_id(0)
{
}

GraphicsSwapchain::~GraphicsSwapchain()
{
	for (uint32_t id = 0; id < m_swapchain_texs.size(); ++id) {
		m_swapchain_texs[id].Reset();
		m_flows[id].Reset();
	}
	GraphicsManager::GetRef().DestroyGraphicsSwapchain(m_identity);
	m_present_sema.Reset();
	m_acq_sema.Reset();
	SD_SREF(m_cmd_pool).RecycleCommandBuffer(m_cmd_buffer);
	m_cmd_pool.Reset();
	//
}

void GraphicsSwapchain::Initialize()
{
	//1. create GraphicsSwapchain.
	GraphicsManager::GetRef().CreateGraphicsSwapchain(m_identity);
	//2. create command pool.
	m_cmd_pool = new CommandPool("GraphicsSwapchainCmdPool");
	SD_SREF(m_cmd_pool).Initialize();
	m_cmd_buffer = SD_SREF(m_cmd_pool).AllocateCommandBuffer("GraphicsSwapchainCmdBuffer");
	//
	m_acq_sema = new GraphicsSemaphore("AcquireSema", m_identity.m_device);
	SD_SREF(m_acq_sema).Initialize();
	m_present_sema = new GraphicsSemaphore("PresentSema", m_identity.m_device);
	SD_SREF(m_present_sema).Initialize();
	//3. create swapchain texture.
	RenderPassWeakReferenceObject comp_rp = GraphicsManager::GetRef().GetRenderPass(sRenderPass_Composing);
	if (comp_rp.IsNull() == false) {
		ClearValue clear_color;
		clear_color.color.float32[0] = 0.0f;
		clear_color.color.float32[1] = 0.0f;
		clear_color.color.float32[2] = 0.0f;
		clear_color.color.float32[3] = 1.0f;

		m_swapchain_texs.resize(m_identity.m_swapchain_images.size());
		m_flows.resize(m_identity.m_swapchain_images.size());
		m_tex_valids.resize(m_identity.m_swapchain_images.size());
		for (uint32_t id = 0; id < m_swapchain_texs.size(); ++id) {
			m_swapchain_texs[id] = new Texture(SDE::Basic::StringFormat("SwapchainTexImage_%d", id));
			SD_SREF(m_swapchain_texs[id]).InitializeWithSwapchainImage(
				m_identity.m_swapchain_images[id],
				m_identity.m_swapchain_image_views[id],
				m_identity.m_screen_size.GetWidth(),
				m_identity.m_screen_size.GetHeight(),
				m_identity.m_type,
				m_identity.m_view_type,
				m_identity.m_format
			);

			m_flows[id] = new RenderFlow(
				SDE::Basic::StringFormat("RenderFlow_%d", id),
				ImageOffset(0, 0, 0),
				ImageSize(m_identity.m_screen_size.GetWidth(), m_identity.m_screen_size.GetHeight(), 1));
			SD_SREF(m_flows[id]).RegisterRenderPass(comp_rp);
			SD_SREF(m_flows[id]).AllocateFrameBuffer();
			SD_SREF(m_flows[id]).RegisterBufferToFrameBuffer(m_swapchain_texs[id], 0, clear_color);
			SD_SREF(m_flows[id]).Initialize();

			m_tex_valids[id] = false;
		}

		m_mesh = BasicShapeCreator::GetRef().CreateWorldGUI(2.0f, 2.0f);
	}
	else {
		SDLOGE("We can't find rp[%s]", sRenderPass_Composing.c_str());
	}
}

void GraphicsSwapchain::RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex)
{
	uint32_t image_idx = UINT32_MAX;

	GraphicsManager::GetRef().GetReadyTextureOfSwapchain(m_identity, m_acq_sema, image_idx);

	ImageBlitParam param;
	param.m_src_param.m_aspect = ImageAspect_ASPECT_COLOR;
	param.m_src_param.m_based_layer = 0;
	param.m_src_param.m_layer_count = SD_WREF(i_tex).GetLayerCount();
	param.m_src_param.m_mip_level = 0;
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

	if (image_idx < m_identity.m_swapchain_images.size()) {
		GraphicsManager::GetRef().RenderTextureToSwapchain(
			m_identity, image_idx,
			m_queue, m_cmd_buffer, m_present_sema, i_tex, param);
	}
}

void GraphicsSwapchain::RenderLayersToSwapchain(const std::list<GraphicsLayerStrongReferenceObject> &i_layers)
{
	m_last_slot_id = m_current_slot_id;
	GraphicsManager::GetRef().GetReadyTextureOfSwapchain(m_identity, m_acq_sema, m_current_slot_id);
	m_tex_valids[m_current_slot_id] = false;

	Viewport vp;
	vp.m_x = 0.0f; vp.m_y = static_cast<float>(m_identity.m_screen_size.GetHeight());
	vp.m_width = static_cast<float>(m_identity.m_screen_size.GetWidth());
	vp.m_height = -1.0f * static_cast<float>(m_identity.m_screen_size.GetHeight());
	vp.m_min_depth = 0.0f;
	vp.m_max_depth = 1.0f;

	ScissorRegion sr;
	sr.m_x = 0.0f; sr.m_y = 0.0f;
	sr.m_width = vp.m_width;
	sr.m_height = static_cast<float>(m_identity.m_screen_size.GetHeight());

	SD_WREF(m_cmd_buffer).Begin();
	SD_SREF(m_flows[m_current_slot_id]).BeginRenderFlow(m_cmd_buffer);

	GraphicsManager::GetRef().SetViewport(m_cmd_buffer, vp);
	GraphicsManager::GetRef().SetScissor(m_cmd_buffer, sr);

	for (std::list<GraphicsLayerStrongReferenceObject>::const_iterator layer = i_layers.begin();
		layer != i_layers.end(); ++layer) {
		SD_SREF((*layer)).BindUniformVariables(m_cmd_buffer);
		SD_SREF(m_mesh).BindVertexBuffers(m_cmd_buffer);
		SD_SREF(m_mesh).BindIndexBuffer(m_cmd_buffer);
		SD_SREF(m_mesh).Render(m_cmd_buffer);
	}
	SD_SREF(m_flows[m_current_slot_id]).EndRenderFlow(m_cmd_buffer);
	SD_WREF(m_cmd_buffer).End();

	SD_WREF(m_queue).SubmitCommandBuffers({ m_cmd_buffer });

	m_tex_valids[m_current_slot_id] = true;
}

void GraphicsSwapchain::Present()
{
	SD_WREF(m_queue).Present(m_identity, m_current_slot_id, { m_present_sema });
}

______________SD_END_GRAPHICS_NAMESPACE______________