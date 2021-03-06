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

/*! \file      FrameBuffer.h
 *  \brief     Introduce of class about FrameBuffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/01/31
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "ClearValue.h"
#include "ImageViewIdentity.h"
#include "FrameBufferGroupIdentity.h"
#include "FrameBufferIdentity.h"
#include "SubpassDescription.h"
#include "RenderPass.h"
#include "Object.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(FrameBuffer);

/*! \class FrameBuffer
 *  \brief In our graphic system, class FrameBuffer is used to link render pass and 
 *         frame buffer.
 */
class SDENGINE_CLASS FrameBuffer : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit FrameBuffer(const ObjectName &i_object_name, const ImageSize &i_size);
     *  \param [in] i_object_name Name of this framebuffer.
     *  \param [in] i_size Size of framebuffer. z is used to store layer.
     *  \brief Constructor of FrameBuffer
     */
    explicit FrameBuffer(const ObjectName &i_object_name, const ImageSize &i_size);

    /*! \fn virtual ~FrameBuffer();
     *  \brief Destructor of FrameBuffer
     */
    virtual ~FrameBuffer();
public:
    /*! \fn void Initialize();
     *  \brief Initialize framebuffer. Please call this function after calling RegisterTargetRenderPass and RegisterBuffer.
     */
    void Initialize();
public:
    /*! \fn void RegisterTargetRenderPass(const RenderPassWeakReferenceObject &i_rp);
     *  \param [in] i_rp Target RenderPass.
     *  \brief Register target RenderPass for specifying formats between buffers and this Framebuffer.
     */
    void RegisterTargetRenderPass(const RenderPassWeakReferenceObject &i_rp);

    /* \fn void RegisterBuffer(const TextureWeakReferenceObject &i_tex, uint32_t i_idx, const ClearValue &i_clear_value);
     * \param [in] i_tex Texture we want to register its to image view.
     * \param [in] i_idx we want to register buffer for which image view.
     * \param [in] i_clear_value Clear value.
     */
    void RegisterBuffer(const TextureWeakReferenceObject &i_tex, uint32_t i_idx, const ClearValue &i_clear_value);
public:
    CompHandle GetHandle() const;
protected:

    /* \var FrameBufferIdentity m_identity;
     * \brief Identity for frame buffer.
     */
    FrameBufferIdentity m_identity;

    /*! \var RenderPassWeakReferenceObject m_target_rp;
     *  \brief Identity about render pass.
     */
    RenderPassWeakReferenceObject m_target_rp;

    /*! \var std::vector<FrameBufferGroupIdentity> m_fbg_identities;
     *  \brief Indicate which framebuffer using for each step.
     */
    std::vector<FrameBufferGroupIdentity> m_fbg_identities;

    /*! \var std::vector<TextureWeakReferenceObject> m_bufs;
     *  \brief Buffers for this render pass. The number should be equal to image view identity.
     */
    std::vector<TextureWeakReferenceObject> m_bufs;
};

inline CompHandle FrameBuffer::GetHandle() const
{
    return m_identity.m_fb_handle;
}

______________SD_END_GRAPHICS_NAMESPACE______________