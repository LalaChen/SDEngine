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

#if defined(_WIN32) || defined(_WIN64)

#include "LogManager.h"
#include "OpenGL4Manager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void OpenGL4Manager::CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity)
{
}

void OpenGL4Manager::RenderTextureToSwapchain(const GraphicsSwapchainIdentity &i_identity, const GraphicsQueueWeakReferenceObject &i_queue, const CommandBufferWeakReferenceObject &i_cmd_buffer, const GraphicsSemaphoreWeakReferenceObject &i_acq_sema, const GraphicsSemaphoreWeakReferenceObject &i_present_sema, const TextureWeakReferenceObject &i_texture)
{
}

void OpenGL4Manager::DestroyGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity)
{
}

void OpenGL4Manager::SetViewport(const CommandBufferWeakReferenceObject& i_cb, const Viewport &i_vp)
{
}

void OpenGL4Manager::SetViewports(const CommandBufferWeakReferenceObject& i_cb, const std::vector<Viewport> &i_vps)
{
}

void OpenGL4Manager::SetScissor(const CommandBufferWeakReferenceObject &i_cb, const ScissorRegion &i_region)
{
}

void OpenGL4Manager::SetScissors(const CommandBufferWeakReferenceObject &i_cb, const std::vector<ScissorRegion> &i_regions)
{
}

void OpenGL4Manager::DrawByIndices(const CommandBufferWeakReferenceObject &i_cb, const IndexBufferWeakReferenceObject &i_ib, uint32_t i_first_id, int32_t i_offset, uint32_t i_first_ins_id, uint32_t i_ins_number)
{
}

______________SD_END_GRAPHICS_NAMESPACE______________

#endif