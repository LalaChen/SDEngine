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

#include "OpenGL4Manager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void OpenGL4Manager::CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content)
{
}

void OpenGL4Manager::DeleteShaderModule(ShaderModuleIdentity &io_identity)
{
}

void OpenGL4Manager::CreateGraphicsPipeline(GraphicsPipelineIdentity &io_identity, const ShaderModules &i_shaders, const RenderPassWeakReferenceObject &i_rp_wref)
{
}

void OpenGL4Manager::BindGraphicsPipeline(const GraphicsPipelineIdentity& i_identity, const CommandBufferWeakReferenceObject& i_cb_wref)
{
}

void OpenGL4Manager::DestroyGraphicsPipeline(GraphicsPipelineIdentity &io_identity)
{
}

void OpenGL4Manager::CreateRenderPass(RenderPassIdentity &io_identity)
{
}

void OpenGL4Manager::BeginRenderPass(const CommandBufferWeakReferenceObject &i_cb_wref, const FrameBufferWeakReferenceObject &i_fb_wref, const RenderPassWeakReferenceObject &i_rp_wref, const ImageOffset &i_start_pos, const ImageSize &i_render_size)
{
}

void OpenGL4Manager::GoToNextStepOfRenderPass(const CommandBufferWeakReferenceObject &i_cb_wref, const FrameBufferWeakReferenceObject &i_fb_wref, uint32_t i_sp_id)
{
}

void OpenGL4Manager::EndRenderPass(const CommandBufferWeakReferenceObject &i_cb_wref)
{
}

void OpenGL4Manager::DestroyRenderPass(RenderPassIdentity &io_identity)
{
}

void OpenGL4Manager::CreateFrameBuffer(FrameBufferIdentity &io_identity, const RenderPassWeakReferenceObject &i_rp_wref, const std::vector<TextureWeakReferenceObject> &i_buf_wrefs)
{
}

void OpenGL4Manager::CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity)
{
    //For each group, generate FBO and bind color and depth buffer to texture.
}

void OpenGL4Manager::DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity)
{

}

void OpenGL4Manager::DestroyFrameBuffer(FrameBufferIdentity &io_identity)
{

}

______________SD_END_GRAPHICS_NAMESPACE______________

#endif