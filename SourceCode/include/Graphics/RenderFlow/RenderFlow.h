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
#include "Color4f.h"
#include "CommandBuffer.h"
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
    /*! \fn explicit RenderFlow(const ObjectName &i_object_name, const Resolution &i_resolution);
     *  \param [in] i_object_name Name of this object.
     *  \param [in] i_render_pos start position of render area.
     *  \param [in] i_render_size size of render area.
     *  \brief Constructor of RenderFlow
     */
    explicit RenderFlow(const ObjectName &i_object_name, const ImageOffset &i_render_pos, const ImageSize &i_render_size);

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
    /*! \fn void RegisterRenderPass(const RenderPassWeakReferenceObject &i_rp);
     *  \param [in] i_rp Target render pass.
     *  \brief Create frame buffer for this render flow.
     */
    void RegisterRenderPass(const RenderPassWeakReferenceObject &i_rp);

    /*! \fn void AllocateFrameBuffer();
     *  \brief Allocate frame buffer for this render flow. Please use this function after RegisterRenderPass.
     */
    void AllocateFrameBuffer();

    /* \fn void RegisterBufferToFrameBuffer(const TextureWeakReferenceObject &i_tex, uint32_t i_idx);
     * \param [in] i_tex Texture we want to register its to image view.
     * \param [in] i_idx we want to register buffer for which image view.
     * \param [in] i_clear_value Clear value.
     * \brief Register buffer to frame buffer. Please register color or depth buffer one by one with the order following 
     *        attachment description in render pass after AllocateFrameBuffer.
     */
    void RegisterBufferToFrameBuffer(const TextureWeakReferenceObject &i_tex, uint32_t i_idx, const ClearValue &i_clear_value);
public:
    /* \fn void BeginRenderFlow(const CommandBufferWeakReferenceObject &i_cb);
     * \param [in] i_cb Command buffer weak reference.
     * \param [in] i_start_pos Start Position.
     * \param [in] i_render_size Render Size.
     * \brief Start this render flow. We will start first step.
     */
    void BeginRenderFlow(const CommandBufferWeakReferenceObject &i_cb);

    /* \fn void GoToNextStep(const CommandBufferWeakReferenceObject &i_cb);
     * \param [in] i_cb Command buffer weak reference.
     * \brief Go to next step of this render flow.
     */
    void GoToNextStep(const CommandBufferWeakReferenceObject &i_cb);

    /* \fn void EndRenderFlow(const CommandBufferWeakReferenceObject &i_cb);
     * \param [in] i_cb Command buffer weak reference.
     * \brief End this render flow.
     */
    void EndRenderFlow(const CommandBufferWeakReferenceObject &i_cb);
   
    /* \fn CommandBufferInheritanceInfo GetCurrentInheritanceInfo() const;
     *
     */
    CommandBufferInheritanceInfo GetCurrentInheritanceInfo() const;

protected:
    /*! \var RenderPassWeakReferenceObject m_rp;
     *  \brief Target render pass.
     */
    RenderPassWeakReferenceObject m_rp;

    /*! \var FrameBufferStrongReferenceObject m_fb;
     *  \brief Target frame buffer.
     */
    FrameBufferStrongReferenceObject m_fb;

    /*! \var ImageOffset m_position;
     *  \brief Tell the start position of render area.
     */
    ImageOffset m_position;

    /*! \var ImageSize m_size;
     *  \brief Tell the size of render area.
     */
    ImageSize m_size;

    /*! \var uint32_t m_current_step;
     *  \brief Current step.(Subpass ID)
     */
    uint32_t m_current_step;
};

______________SD_END_GRAPHICS_NAMESPACE______________