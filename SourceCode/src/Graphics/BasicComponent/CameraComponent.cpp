/* ==============  SD Engine License ==============
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

#include "GraphicsManager.h"
#include "LogManager.h"
#include "CameraComponent.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_COMPONENT_POOL_TYPE_IMPLEMENTATION(CameraComponent, CameraComponent);

CameraComponent::CameraComponent(const ObjectName &i_object_name)
: Component(i_object_name)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
, m_fov(120.0f)
, m_aspect(1.0f)
, m_near(0.01f)
, m_far(1000.0f)
{
}

CameraComponent::~CameraComponent()
{
    m_rf_sref.Reset();
    m_color_buf_sref.Reset();
    m_depth_buf_sref.Reset();
}

void CameraComponent::Initialize()
{
    Resolution current_res = GraphicsManager::GetRef().GetScreenResolution();
    if (m_color_buf_sref.IsNull() == false) {
        m_color_buf_sref.Reset();
    }
    m_color_buf_sref = new Texture("CameraColorBuffer");
    m_color_buf_sref.GetRef().Initialize2DColorOrDepthBuffer(
        current_res.GetWidth(), current_res.GetHeight(),
        GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
        ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

    if (m_depth_buf_sref.IsNull() == false) {
        m_depth_buf_sref.Reset();
    }
    m_depth_buf_sref = new Texture("CameraDepthBuffer");
    m_depth_buf_sref.GetRef().Initialize2DColorOrDepthBuffer(
        current_res.GetWidth(), current_res.GetHeight(),
        GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
        ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    // Create render flow.
    if (m_custom_rp_wref.IsNull() == false) {
        SDLOG("Use custom render pass(%s)", m_custom_rp_wref.GetRef().GetObjectName().c_str());
        m_rf_sref = new RenderFlow("RenderFlow", ImageOffset(0, 0, 0),
            ImageSize(current_res.GetWidth(), current_res.GetHeight(), 1));
        m_rf_sref.GetRef().RegisterRenderPass(m_custom_rp_wref);
        m_rf_sref.GetRef().AllocateFrameBuffer();
        m_rf_sref.GetRef().RegisterBufferToFrameBuffer(m_color_buf_sref, 0, m_clear_color);
        m_rf_sref.GetRef().RegisterBufferToFrameBuffer(m_depth_buf_sref, 1, m_clear_d_and_s);
        m_rf_sref.GetRef().Initialize();
    }
    else {
        //To Do : Create render flow with Forward or Defer.
    }
}

void CameraComponent::Resize()
{
    m_color_buf_sref.Reset();
    m_depth_buf_sref.Reset();
    m_rf_sref.Reset();

    Initialize();
}

______________SD_END_GRAPHICS_NAMESPACE______________
