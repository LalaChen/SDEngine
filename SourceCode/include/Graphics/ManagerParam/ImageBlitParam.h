/*
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

/*! \file      ImageBlitParam.h
 *  \brief     ImageBlit data struct.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/12/18
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

#include "ImageAspect.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS ImageRegionSpec
{
public:
    ImageRegionSpec()
    : m_origin{0, 0, 0}
    , m_size{0, 0, 0}
    , m_aspect(ImageAspect_ASPECT_COLOR)
    , m_based_layer(0)
    , m_layer_count(1)
    , m_mip_level(0)
    {
    }
    ~ImageRegionSpec()
    {

    }
public:
    uint32_t m_origin[3];
    uint32_t m_size[3];
    ImageAspectEnum m_aspect;
    uint32_t m_based_layer;
    uint32_t m_layer_count;
    uint32_t m_mip_level;
};

class SDENGINE_CLASS ImageBlitParam
{
public:
    ImageBlitParam() {}
    ~ImageBlitParam() {}
public:
    ImageRegionSpec m_src_param;
    ImageRegionSpec m_dst_param;
};

______________SD_END_GRAPHICS_NAMESPACE______________