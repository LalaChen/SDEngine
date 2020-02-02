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

#include "LogManager.h"
#include "GraphicsManager.h"
#include "RenderFlow.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

RenderFlow::RenderFlow(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

RenderFlow::~RenderFlow()
{
}

void RenderFlow::RegisterRenderPass(const RenderPassWeakReferenceObject &i_rp_wref)
{
    m_rp_wref = i_rp_wref;
}

void RenderFlow::AllocateFrameBuffer()
{
    if (m_rp_wref.IsNull() == false) {
        m_fb_sref = new FrameBuffer(m_object_name + "_FrameBuffer");
        //1. Create ImageView Desciption from Attachment Desciption.
        m_fb_sref.GetRef().AddRenderPassInfos(m_rp_wref.GetRef().CreateImageViewDescriptions(), m_rp_wref.GetRef().GetHandle());
    }
    else {
        SDLOGE("No target render pass in RenderFlow(%s).", m_object_name.c_str());
    }
}

void RenderFlow::RegisterBufferToFrameBuffer(const TextureWeakReferenceObject& i_tex_wref, uint32_t i_idx)
{
    if (m_fb_sref.IsNull() == false) {
        m_fb_sref.GetRef().RegisterBuffer(i_tex_wref, i_idx);
    }
    else {
        SDLOGE("No target frame buffe in RenderFlow(%s).", m_object_name.c_str());
    }
}

void RenderFlow::Initialize()
{
    if (m_fb_sref.IsNull() == false && m_rp_wref.IsNull() == false) {
        m_fb_sref.GetRef().Initialize(m_rp_wref.GetRef().GetSubpassDescriptions());
    }
    else {
        SDLOGE("One or both of render pass or frame buffer are null. RP(%d). FB(%d)."
            ,m_rp_wref.IsNull(), m_fb_sref.IsNull());
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________