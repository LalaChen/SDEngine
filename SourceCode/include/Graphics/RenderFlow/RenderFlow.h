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

/*! \file      RenderPass.h
 *  \brief     Introduce of class about render pass.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/01/31
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderPass.h"

#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(RenderFlow);

/*! \class RenderFlow
 *  \brief In our graphic system, class RenderFlow is used to manipulate rendering flow and 
 *         frame buffer.
 */
class SDENGINE_CLASS RenderFlow : public Object
{
public:
    /*! \fn explicit RenderFlow(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of RenderFlow
     */
    explicit RenderFlow(const ObjectName &i_object_name);

    /*! \fn virtual ~RenderFlow();
     *  \brief Destructor of RenderFlow.
     */
    virtual ~RenderFlow();
public:
    /*! \fn void Initialize();
     *  \brief Trigger initialization. Please use this function after calling all RegisterBufferToFrameBuffer done.
     */
    void Initialize();
public:
    /*! \fn void RegisterRenderPass(const RenderPassWeakReferenceObject &i_rp_wref);
     *  \param [in] i_rp_wref Target render pass.
     *  \brief Create frame buffer for this render flow.
     */
    void RegisterRenderPass(const RenderPassWeakReferenceObject &i_rp_wref);

    /*! \fn void AllocateFrameBuffer();
     *  \brief Allocate frame buffer for this render flow. Please use this function after RegisterRenderPass.
     */
    void AllocateFrameBuffer();

    /* \fn void RegisterBufferToFrameBuffer(const TextureWeakReferenceObject &i_tex_wref, uint32_t i_idx);
     * \param [in] i_tex_wref Texture we want to register its to image view.
     * \param [in] i_idx we want to register buffer for which image view.
     * \brief Register buffer to frame buffer. Please register color or depth buffer one by one with the order following 
     *        attachment description in render pass after AllocateFrameBuffer.
     */
    void RegisterBufferToFrameBuffer(const TextureWeakReferenceObject& i_tex_wref, uint32_t i_idx);
protected:
    /*! \var RenderPassWeakReferenceObject m_rp_wref;
     *  \brief Target render pass.
     */
    RenderPassWeakReferenceObject m_rp_wref;

    /*! \var FrameBufferStrongReferenceObject m_fb_sref;
     *  \brief Target frame buffer.
     */
    FrameBufferStrongReferenceObject m_fb_sref;
};


______________SD_END_GRAPHICS_NAMESPACE______________