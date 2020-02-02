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

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "Texture.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS FrameBufferGroupIdentity
{
public:
    FrameBufferGroupIdentity();
    ~FrameBufferGroupIdentity();
public:
    CompHandle m_fbg_handle;
    std::vector<TextureWeakReferenceObject> m_input_buf_wrefs;
    std::vector<TextureWeakReferenceObject> m_color_buf_wrefs;
    std::vector<TextureWeakReferenceObject> m_ref_buf_wrefs;
    std::vector<uint32_t> m_pre_buf_ids;
    TextureWeakReferenceObject m_depth_buf_wref;
};

______________SD_END_GRAPHICS_NAMESPACE______________