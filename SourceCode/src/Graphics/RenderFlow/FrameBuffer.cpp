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
#include "FrameBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

FrameBuffer::FrameBuffer(const ObjectName &i_object_name, const ImageSize &i_size)
: Object(i_object_name)
{
    m_identity.m_size = i_size;
}

FrameBuffer::~FrameBuffer()
{
    for (uint32_t fbg_ID = 0; fbg_ID < m_fbg_identities.size(); ++fbg_ID) {
        GraphicsManager::GetRef().DestroyFrameBufferGroup(m_fbg_identities[fbg_ID]);
    }
    GraphicsManager::GetRef().DestroyFrameBuffer(m_identity);
}

void FrameBuffer::Initialize()
{
    const std::vector<SubpassDescription>& sp_descs = m_target_rp.GetConstRef().GetSubpassDescriptions();
    const std::vector<AttachmentDescription> &att_descs = m_target_rp.GetConstRef().GetAttachmentDescriptions();

    //1. resize fbg groups for following subpasses.
    m_fbg_identities.resize(sp_descs.size());
    //2. prepare data.
    for (uint32_t spID = 0; spID < sp_descs.size(); ++spID) {
        //2.1 color attachment.
        if (sp_descs[spID].m_color_attachment_refs.size() > 0) {
            m_fbg_identities[spID].m_color_bufs.resize(sp_descs[spID].m_color_attachment_refs.size());
            m_fbg_identities[spID].m_clear_color_flags.resize(sp_descs[spID].m_color_attachment_refs.size());
            m_fbg_identities[spID].m_clear_colors.resize(sp_descs[spID].m_color_attachment_refs.size());

            for (uint32_t attrID = 0; attrID < sp_descs[spID].m_color_attachment_refs.size(); ++attrID) {
                uint32_t bufID = sp_descs[spID].m_color_attachment_refs[attrID].m_attachment_ID;
                if (bufID < m_bufs.size()) {
                    //2.1.1 record clear flag and values.
                    m_fbg_identities[spID].m_clear_colors[attrID] = m_identity.m_clear_values[bufID].ToColor4f();
                    if (att_descs[bufID].m_load_op == AttachmentLoadOperator_CLEAR) {
                        m_fbg_identities[spID].m_clear_color_flags[attrID] = true;
                    }
                    else {
                        m_fbg_identities[spID].m_clear_color_flags[attrID] = false;
                    }
                    //2.1.2 record target buffer.
                    if (m_bufs[bufID].IsNull() == false) {
                        m_fbg_identities[spID].m_color_bufs[attrID] = m_bufs[bufID];
                    }
                    else {
                        SDLOGE("Color BufferID[%d] of FrameBuffer[%s] is null.", bufID, m_object_name.c_str());
                    }
                }
                else {
                    SDLOGE("Color Attachment[%d] of FrameBuffer[%s] is out of range.", bufID, m_object_name.c_str());
                }
            }
        }

        //2.2 input attachment.
        if (sp_descs[spID].m_input_attachment_refs.size() > 0) {
            m_fbg_identities[spID].m_input_bufs.resize(sp_descs[spID].m_input_attachment_refs.size());
            for (uint32_t attrID = 0; attrID < sp_descs[spID].m_input_attachment_refs.size(); ++attrID) {
                uint32_t bufID = sp_descs[spID].m_input_attachment_refs[attrID].m_attachment_ID;
                if (bufID < m_bufs.size()) {
                    if (m_bufs[bufID].IsNull() == false) {
                        m_fbg_identities[spID].m_input_bufs[attrID] = m_bufs[bufID];
                    }
                    else {
                        SDLOGE("Input BufferID[%d] of FrameBuffer[%s] is null.", bufID, m_object_name.c_str());
                    }
                }
                else {
                    SDLOGE("Input Attachment[%d] of FrameBuffer[%s] is out of range.", bufID, m_object_name.c_str());
                }
            }
        }

        //2.3 res attachment.
        if (sp_descs[spID].m_res_attachment_refs.size() > 0) {
            m_fbg_identities[spID].m_ref_bufs.resize(sp_descs[spID].m_res_attachment_refs.size());
            for (uint32_t attrID = 0; attrID < sp_descs[spID].m_res_attachment_refs.size(); ++attrID) {
                uint32_t bufID = sp_descs[spID].m_res_attachment_refs[attrID].m_attachment_ID;
                if (bufID < m_bufs.size()) {
                    if (m_bufs[bufID].IsNull() == false) {
                        m_fbg_identities[spID].m_ref_bufs[attrID] = m_bufs[bufID];
                    }
                    else {
                        SDLOGE("Res BufferID[%d] of FrameBuffer[%s] is null.", bufID, m_object_name.c_str());
                    }
                }
                else {
                    SDLOGE("Res Attachment[%d] of FrameBuffer[%s] is out of range.", bufID, m_object_name.c_str());
                }
            }
        }

        //2.4 pre attachment ids.
        if (sp_descs[spID].m_pre_attachment_ref_ids.size() > 0) {
            m_fbg_identities[spID].m_pre_buf_ids = sp_descs[spID].m_pre_attachment_ref_ids;
        }

        //2.5 depth_fbID.
        uint32_t depth_fbID = sp_descs[spID].m_depth_attachment_ref.m_attachment_ID;
        if (depth_fbID != SD_ERROR_ATTACHMENT_REF) {
            if (depth_fbID < m_bufs.size()) {
                //2.5.1 record clear flag and values.
                m_fbg_identities[spID].m_clear_depth   = m_identity.m_clear_values[depth_fbID].ToDepth();
                m_fbg_identities[spID].m_clear_stencil = m_identity.m_clear_values[depth_fbID].ToStencil();
                if (att_descs[depth_fbID].m_stencil_load_op == AttachmentLoadOperator_CLEAR) {
                    m_fbg_identities[spID].m_clear_depth_or_stencil_flag = true;
                }
                else {
                    m_fbg_identities[spID].m_clear_depth_or_stencil_flag = false;
                }
                //2.5.2 record depth buffer.
                if (m_bufs[depth_fbID].IsNull() == false) {
                    m_fbg_identities[spID].m_depth_buf = m_bufs[depth_fbID];
                }
                else {
                    SDLOGE("Depth BufferID[%d] of FrameBuffer[%s] is null.", depth_fbID, m_object_name.c_str());
                }
            }
            else {
                SDLOGE("Depth Attachment[%d] of FrameBuffer[%s] is out of range.", sp_descs[spID].m_depth_attachment_ref.m_attachment_ID, m_object_name.c_str());
            }
        } 
    }
    //3. Call manager to allocate FrameBuffer comp handle.
    GraphicsManager::GetRef().CreateFrameBuffer(m_identity, m_target_rp, m_bufs);
    //4. for each FrameBufferGroup, allocate FrameBufferGroup comp handle.
    for (uint32_t fbg_ID = 0; fbg_ID < m_fbg_identities.size(); ++fbg_ID) {
        GraphicsManager::GetRef().CreateFrameBufferGroup(m_fbg_identities[fbg_ID]);
    }
}

void FrameBuffer::RegisterTargetRenderPass(const RenderPassWeakReferenceObject &i_rp)
{
    m_target_rp = i_rp;
    m_identity.m_buffer_formats = i_rp.GetConstRef().CreateImageViewFormats();
    m_identity.m_clear_values.resize(m_identity.m_buffer_formats.size());
    m_bufs.resize(m_identity.m_buffer_formats.size());
}

void FrameBuffer::RegisterBuffer(const TextureWeakReferenceObject &i_tex, uint32_t i_idx, const ClearValue &i_clear_value)
{
    if (i_idx < m_bufs.size()) {
        if (i_tex.IsNull() == false) {
            TextureFormatEnum tex_format = i_tex.GetConstRef().GetTextureFormat();
            if (tex_format == m_identity.m_buffer_formats[i_idx]) {
                m_bufs[i_idx] = i_tex;
                m_identity.m_clear_values[i_idx] = i_clear_value;
            }
            else {
                SDLOGE("Register wrong type[%d] tex[%s] to this FB[%s]. CorrectType is [%d]", tex_format,
                    i_tex.GetRef().GetObjectName().c_str(), m_object_name.c_str(),
                    m_identity.m_buffer_formats[i_idx]);
            }
        }
        else {
            SDLOGE("Register null tex to this FB[%s].", m_object_name.c_str());
        }
    }
    else {
        SDLOGE("Idx(%d) is out of range in FrameBuffer(%s).", i_idx, m_object_name.size());
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________