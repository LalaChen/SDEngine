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
    m_fb_identity.m_size = i_size;
}

FrameBuffer::~FrameBuffer()
{
    for (uint32_t fbg_ID = 0; fbg_ID < m_fbg_identities.size(); ++fbg_ID) {
        GraphicsManager::GetRef().DestroyFrameBufferGroup(m_fbg_identities[fbg_ID]);
    }
    GraphicsManager::GetRef().DestroyFrameBuffer(m_fb_identity);
}

void FrameBuffer::Initialize(const std::vector<SubpassDescription> &i_sp_descs, const std::vector<AttachmentDescription> &i_att_descs)
{
    //1. resize fbg groups for following subpasses.
    m_fbg_identities.resize(i_sp_descs.size());
    //2. prepare data.
    for (uint32_t spID = 0; spID < i_sp_descs.size(); ++spID) {
        //2.1 color attachment.
        if (i_sp_descs[spID].m_color_attachment_refs.size() > 0) {
            m_fbg_identities[spID].m_color_buf_wrefs.resize(i_sp_descs[spID].m_color_attachment_refs.size());
            m_fbg_identities[spID].m_clear_color_flags.resize(i_sp_descs[spID].m_color_attachment_refs.size());
            m_fbg_identities[spID].m_clear_colors.resize(i_sp_descs[spID].m_color_attachment_refs.size());

            for (uint32_t attrID = 0; attrID < i_sp_descs[spID].m_color_attachment_refs.size(); ++attrID) {
                uint32_t bufID = i_sp_descs[spID].m_color_attachment_refs[attrID].m_attachment_ID;
                if (bufID < m_buf_wrefs.size()) {
                    //2.1.1 record clear flag and values.
                    m_fbg_identities[spID].m_clear_colors[attrID] = m_fb_identity.m_clear_values[bufID].ToColor4f();
                    if (i_att_descs[bufID].m_load_op == AttachmentLoadOperator_CLEAR) {
                        m_fbg_identities[spID].m_clear_color_flags[attrID] = true;
                    }
                    else {
                        m_fbg_identities[spID].m_clear_color_flags[attrID] = false;
                    }
                    //2.1.2 record target buffer.
                    if (m_buf_wrefs[bufID].IsNull() == false) {
                        m_fbg_identities[spID].m_color_buf_wrefs[attrID] = m_buf_wrefs[bufID];
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
        if (i_sp_descs[spID].m_input_attachment_refs.size() > 0) {
            m_fbg_identities[spID].m_input_buf_wrefs.resize(i_sp_descs[spID].m_input_attachment_refs.size());
            for (uint32_t attrID = 0; attrID < i_sp_descs[spID].m_input_attachment_refs.size(); ++attrID) {
                uint32_t bufID = i_sp_descs[spID].m_input_attachment_refs[attrID].m_attachment_ID;
                if (bufID < m_buf_wrefs.size()) {
                    if (m_buf_wrefs[bufID].IsNull() == false) {
                        m_fbg_identities[spID].m_input_buf_wrefs[attrID] = m_buf_wrefs[bufID];
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
        if (i_sp_descs[spID].m_res_attachment_refs.size() > 0) {
            m_fbg_identities[spID].m_ref_buf_wrefs.resize(i_sp_descs[spID].m_res_attachment_refs.size());
            for (uint32_t attrID = 0; attrID < i_sp_descs[spID].m_res_attachment_refs.size(); ++attrID) {
                uint32_t bufID = i_sp_descs[spID].m_res_attachment_refs[attrID].m_attachment_ID;
                if (bufID < m_buf_wrefs.size()) {
                    if (m_buf_wrefs[bufID].IsNull() == false) {
                        m_fbg_identities[spID].m_ref_buf_wrefs[attrID] = m_buf_wrefs[bufID];
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
        if (i_sp_descs[spID].m_pre_attachment_ref_ids.size() > 0) {
            m_fbg_identities[spID].m_pre_buf_ids = i_sp_descs[spID].m_pre_attachment_ref_ids;
        }

        //2.5 depth_fbID.
        uint32_t depth_fbID = i_sp_descs[spID].m_depth_attachment_ref.m_attachment_ID;
        if (depth_fbID != SD_ERROR_ATTACHMENT_REF) {
            if (depth_fbID < m_buf_wrefs.size()) {
                //2.5.1 record clear flag and values.
                m_fbg_identities[spID].m_clear_depth   = m_fb_identity.m_clear_values[depth_fbID].ToDepth();
                m_fbg_identities[spID].m_clear_stencil = m_fb_identity.m_clear_values[depth_fbID].ToStencil();
                if (i_att_descs[depth_fbID].m_stencil_load_op == AttachmentLoadOperator_CLEAR) {
                    m_fbg_identities[spID].m_clear_depth_or_stencil_flag = true;
                }
                else {
                    m_fbg_identities[spID].m_clear_depth_or_stencil_flag = false;
                }
                //2.5.2 record depth buffer.
                if (m_buf_wrefs[depth_fbID].IsNull() == false) {
                    m_fbg_identities[spID].m_depth_buf_wref = m_buf_wrefs[depth_fbID];
                }
                else {
                    SDLOGE("Depth BufferID[%d] of FrameBuffer[%s] is null.", depth_fbID, m_object_name.c_str());
                }
            }
            else {
                SDLOGE("Depth Attachment[%d] of FrameBuffer[%s] is out of range.", i_sp_descs[spID].m_depth_attachment_ref.m_attachment_ID, m_object_name.c_str());
            }
        } 
    }
    //3. Call manager to allocate FrameBuffer comp handle.
    GraphicsManager::GetRef().CreateFrameBuffer(m_fb_identity, m_buf_wrefs);
    //4. for each FrameBufferGroup, allocate FrameBufferGroup comp handle.
    for (uint32_t fbg_ID = 0; fbg_ID < m_fbg_identities.size(); ++fbg_ID) {
        GraphicsManager::GetRef().CreateFrameBufferGroup(m_fbg_identities[fbg_ID]);
    }
}

void FrameBuffer::AddRenderPassInfos(const std::vector<ImageViewIdentity> &i_iv_descs, const CompHandle i_rp_handle)
{
    m_fb_identity.m_rp_handle = i_rp_handle;
    m_fb_identity.m_iv_identities = i_iv_descs;
    m_fb_identity.m_clear_values.resize(m_fb_identity.m_iv_identities.size());
    m_buf_wrefs.resize(m_fb_identity.m_iv_identities.size());
}

void FrameBuffer::RegisterBuffer(const TextureWeakReferenceObject &i_tex_wref, uint32_t i_idx, const ClearValue& i_clear_value)
{
    if (i_idx < m_buf_wrefs.size()) {
        if (i_tex_wref.IsNull() == false) {
            TextureFormatEnum tex_format = i_tex_wref.GetConstRef().GetTextureFormat();
            if (tex_format == m_fb_identity.m_iv_identities[i_idx].m_format) {
                m_buf_wrefs[i_idx] = i_tex_wref;
                m_fb_identity.m_clear_values[i_idx] = i_clear_value;
            }
            else {
                SDLOGE("Register wrong type[%d] tex[%s] to this FB[%s]. CorrectType is [%d]", tex_format,
                    i_tex_wref.GetRef().GetObjectName().c_str(), m_object_name.c_str(),
                    m_fb_identity.m_iv_identities[i_idx].m_format);
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