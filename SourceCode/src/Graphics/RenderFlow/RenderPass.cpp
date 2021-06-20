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

#include "GraphicsManager.h"
#include "RenderPass.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

RenderPass::RenderPass(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

RenderPass::~RenderPass()
{
}

void RenderPass::AddRenderPassDescription(const std::vector<AttachmentDescription> &i_att_descs, const std::vector<SubpassDescription> &i_sp_descs, const std::vector<SubpassDependency> &i_sp_deps)
{
    m_identity.m_attachment_descs = i_att_descs;
    m_identity.m_subpasses_descs = i_sp_descs;
    m_identity.m_sp_dependencies = i_sp_deps;
}

void RenderPass::SetMultiviewInfo(const MultiviewInfo &i_multiview_info)
{
    m_identity.m_multiview_info = i_multiview_info;
}

void RenderPass::Initialize()
{
    GraphicsManager::GetRef().CreateRenderPass(m_identity);
}

std::vector<TextureFormatEnum> RenderPass::CreateImageViewFormats() const
{
    std::vector<TextureFormatEnum> formats;
    formats.resize(m_identity.m_attachment_descs.size());
    for (uint32_t id = 0; id < formats.size(); ++id) {
        formats[id] = m_identity.m_attachment_descs[id].m_format;
    }
    return formats;
}

______________SD_END_GRAPHICS_NAMESPACE______________