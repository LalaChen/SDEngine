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
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsSwapchain::GraphicsSwapchain(const ObjectName &i_name, const GraphicsQueueWeakReferenceObject &i_queue)
: Object(i_name)
, m_queue(i_queue)
{
}

GraphicsSwapchain::~GraphicsSwapchain()
{
	GraphicsManager::GetRef().DestroyGraphicsSwapchain(m_identity);
	m_present_sema.Reset();
	m_acq_sema.Reset();
	SD_SREF(m_cmd_pool).RecycleCommandBuffer(m_cmd_buffer);
	m_cmd_pool.Reset();
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

______________SD_END_GRAPHICS_NAMESPACE______________