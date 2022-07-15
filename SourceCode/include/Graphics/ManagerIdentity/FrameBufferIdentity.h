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

/*! \file      FrameBufferIdentity.h
 *  \brief     Introduce of classes about handle of frame buffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/01/31
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "GraphicsElementIdentity.h"
#include "ClearValue.h"
#include "ImageParam.h"
#include "ImageViewIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class FrameBufferIdentity
 *  \brief In our graphic system, class FrameBufferIdentity is used to keep framebuffer.
 */
class SDENGINE_CLASS FrameBufferIdentity : public GraphicsElementIdentity
{
public:
    /*! \fn FrameBufferIdentity();
     *  \brief constructor.
     */
    FrameBufferIdentity();

    /*! \fn ~FrameBufferIdentity();
     *  \brief destructor.
     */
    ~FrameBufferIdentity();
public:
    /*! \var CompHandle m_handle;
     *  \brief Framebuffer handle.
     */
    CompHandle m_handle;

    /* \var CompHandle m_device;
     * \brief Device this handle belong.
     */
    CompHandle m_device;

    /*! \var ImageSize m_size;
     *  \brief Size of this framebuffer. Attribute m_length is used to layer.
     */
    ImageSize m_size;

    /*! \var std::vector<TextureFormatEnum> m_buffer_formats;
     *  \brief Buffer formats of this framebuffer.
     */
    std::vector<TextureFormatEnum> m_buffer_formats;

    /*! \var std::vector<ClearValue> m_clear_values;
     *  \brief Clear values about each attachment.
     */
    std::vector<ClearValue> m_clear_values;
};

______________SD_END_GRAPHICS_NAMESPACE______________
