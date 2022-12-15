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
	SD_SREF(m_pool).RecycleCommandBuffer(m_cmd_buffer);
	m_pool.Reset();
}

void GraphicsSwapchain::Initialize()
{
	//1. create GraphicsSwapchain.
	GraphicsManager::GetRef().CreateGraphicsSwapchain(m_identity);
	//2. create command pool.
	m_pool = new CommandPool("GraphicsSwapchainCmdPool");
	SD_SREF(m_pool).Initialize();
	m_cmd_buffer = SD_SREF(m_pool).AllocateCommandBuffer("GraphicsSwapchainCmdBuffer");
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

	GraphicsManager::GetRef().RenderTextureToSwapchain(
		m_identity, image_idx,
		m_queue, m_cmd_buffer, m_present_sema, i_tex);
}

______________SD_END_GRAPHICS_NAMESPACE______________