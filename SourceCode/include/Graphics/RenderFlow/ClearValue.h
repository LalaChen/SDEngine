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

/*! \file      ClearValue.h
 *  \brief     Introduce of class about ClearValue.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/02/02
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "Color4f.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

union ClearColorValue
{
    float float32[4];
    int32_t int32[4];
    uint32_t uint32[4];
};

struct ClearDepthStencilValue
{
    float depth;
    uint32_t stencil;
};

union ClearValue
{
    ClearColorValue color;
    ClearDepthStencilValue depth_stencil;
public:
    Color4f ToColor4f() const;
    uint32_t ToStencil() const;
    float ToDepth() const;
};

inline Color4f ClearValue::ToColor4f() const
{
    return Color4f(color.float32[0], color.float32[1], color.float32[2], color.float32[3]);
}

inline uint32_t ClearValue::ToStencil() const
{
    return depth_stencil.stencil;
}

inline float ClearValue::ToDepth() const
{
    return depth_stencil.depth;
}



______________SD_END_GRAPHICS_NAMESPACE______________