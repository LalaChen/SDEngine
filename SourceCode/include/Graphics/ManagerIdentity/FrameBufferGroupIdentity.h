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

/*! \file      FrameBufferGroupIdentity.h
 *  \brief     Introduce of classes about handle of frame buffer group.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/02/01
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsElementIdentity.h"
#include "Color4f.h"
#include "Texture.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS FrameBufferGroupIdentity : public GraphicsElementIdentity
{
public:
    FrameBufferGroupIdentity();
    ~FrameBufferGroupIdentity();
public:
    /* \var CompHandle m_handle;
     * \brief FrameBufferGroup handle. In Vulkan, it always is NULL. In openGL(GLES),
     *        it's framebufferobject id.
     */
    CompHandle m_handle;

    /* \var std::vector<TextureWeakReferenceObject> m_input_bufs;
     * \brief Keep buffer weak references for input in this group.
     */
    std::vector<TextureWeakReferenceObject> m_input_bufs;

    /* \var std::vector<TextureWeakReferenceObject> m_color_bufs;
     * \brief Keep color buffer weak references in this group.
     */
    std::vector<TextureWeakReferenceObject> m_color_bufs;

    /* \var std::vector<bool> m_clear_color_flags;
     * \brief Record clear flag for each color buffer in this group.
     */
    std::vector<bool> m_clear_color_flags;

    /* \var std::vector<Color4f> m_clear_colors;
     * \brief Record clear color for each color buffer.
     */
    std::vector<Color4f> m_clear_colors;

    /* \var std::vector<TextureWeakReferenceObject> m_ref_bufs;
     * \brief Record resolution buffer in this group.
     */
    std::vector<TextureWeakReferenceObject> m_ref_bufs;

    /* \var std::vector<uint32_t> m_pre_buf_ids;
     * \brief Record preserve buffer id in this group.
     */
    std::vector<uint32_t> m_pre_buf_ids;

    /* \var std::vector<TextureWeakReferenceObject> m_depth_buf;
     * \brief Record depth buffer id in this group.
     */
    TextureWeakReferenceObject m_depth_buf;

    /* \var bool m_clear_depth_or_stencil_flag;
     * \brief Record clear flag of this depth buffer.
     */
    bool m_clear_depth_or_stencil_flag;

    /* \var float m_clear_depth;
     * \brief Record clear depth value of this depth buffer. Default is 1.0f.
     */
    float m_clear_depth;
    
    /* \var uint32_t m_clear_stencil;
     * \brief Record clear stencil value. Default is 1.
     */
    uint32_t m_clear_stencil;
};

______________SD_END_GRAPHICS_NAMESPACE______________