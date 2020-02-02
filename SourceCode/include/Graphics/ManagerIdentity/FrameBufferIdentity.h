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

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ImageViewIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS FrameBufferIdentity
{
public:
    FrameBufferIdentity();
    ~FrameBufferIdentity();
public:
    CompHandle m_fb_handle;
    CompHandle m_rp_handle;
    Size_ui32 m_width;
    Size_ui32 m_height;
    Size_ui32 m_layer;
    std::vector<ImageViewIdentity> m_iv_identities;
};

______________SD_END_GRAPHICS_NAMESPACE______________
