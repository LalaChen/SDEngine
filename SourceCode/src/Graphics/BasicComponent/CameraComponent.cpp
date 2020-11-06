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

CameraComponent::CameraComponent(const ObjectName &i_object_name)
: Component(i_object_name)
, m_workspace_type(WorkspaceType_Forward)
, m_clear_color{ 0.15f, 0.15f, 0.75f, 1.0f }
, m_clear_d_and_s{ 1.0f, 1 }
, m_fov(120.0f)
, m_aspect(1.0f)
, m_near(0.01f)
, m_far(1000.0f)
{
    m_screen_size = GraphicsManager::GetRef().GetScreenResolution();
}

CameraComponent::~CameraComponent()
{
    ClearWorkspace();
}

void CameraComponent::Initialize()
{
    if (m_workspace_type == WorkspaceType_Forward) {
        InitializeWorkspaceForForwardPath();
    }
    else if (m_workspace_type == WorkspaceType_Deferred) {
        InitializeWorkspaceForDeferredPath();
    }
}

void CameraComponent::Resize()
{
    ClearWorkspace();
    Initialize();
}

//------------------ Private Part ---------------
void CameraComponent::InitializeWorkspaceForForwardPath()
{
    RenderPassWeakReferenceObject forward_rp_wref = GraphicsManager::GetRef().GetRenderPass("ForwardPath");

    if (forward_rp_wref.IsNull() == false) {
        if (m_color_buf_sref.IsNull() == false) {
            m_color_buf_sref.Reset();
        }
        m_color_buf_sref = new Texture("CameraColorBuffer");
        m_color_buf_sref.GetRef().Initialize2DColorOrDepthBuffer(
            m_screen_size.GetWidth(), m_screen_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
            ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

        if (m_depth_buf_sref.IsNull() == false) {
            m_depth_buf_sref.Reset();
        }
        m_depth_buf_sref = new Texture("CameraDepthBuffer");
        m_depth_buf_sref.GetRef().Initialize2DColorOrDepthBuffer(
            m_screen_size.GetWidth(), m_screen_size.GetHeight(),
            GraphicsManager::GetRef().GetDefaultDepthBufferFormat(),
            ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        m_rf_sref = new RenderFlow("RenderFlow", ImageOffset(0, 0, 0),
            ImageSize(m_screen_size.GetWidth(), m_screen_size.GetHeight(), 1));
    }
    else {
        SDLOGE("Forward render pass doesn't exist. Please check!!!");
    }
}

void CameraComponent::InitializeWorkspaceForDeferredPath()
{
    RenderPassWeakReferenceObject deferred_rp_wref = GraphicsManager::GetRef().GetRenderPass("DeferredPath");

    if (deferred_rp_wref.IsNull() == false) {
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
    }
    else {
        SDLOGE("Deferred render pass doesn't exist. Please check!!!");
    }
}

void CameraComponent::ClearWorkspace()
{
    m_color_buf_sref.Reset();
    m_depth_buf_sref.Reset();
    m_rf_sref.Reset();
}

______________SD_END_GRAPHICS_NAMESPACE______________
