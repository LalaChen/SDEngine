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

/*! \file      Area.h
 *  \brief     Area
 *  \author    Kuan-Chih, Chen
 *  \date      2023/02/08
 *  \copyright MIT License.
 */

#pragma once

#include <string>

#include "SDEngineMacro.h"
#include "Vec.h"
#include "Resolution.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum AreaAlignOrientationEnum
{
    AreaAlignOrientation_LEFT_BOTTOM = 0,
    AreaAlignOrientation_LEFT_TOP,
    AreaAlignOrientation_RIGHT_BOTTOM,
    AreaAlignOrientation_RIGHT_TOP,
    AreaAlignOrientation_MAX_DEFINE_VALUE
};

/*! \class Area2D
 *  Class Area2D is used to represent area in 2D space.
 */
class SDENGINE_CLASS Area2D
{
public:
    Area2D();
    Area2D(float i_x, float i_y, float i_w, float i_h);
    ~Area2D();
public:
    void SetArea2DByOrientationAndSize(AreaAlignOrientationEnum i_orientation, const Area2D &i_target_size, const Resolution &i_resolution);
    vec2 GetPoint(uint32_t i_orientation) const;
public:
    float x;
    float y;
    float w;
    float h;
};

class SDENGINE_CLASS DepthArea2D
{
public:
    DepthArea2D();
    ~DepthArea2D();
public:
    vec3 GetPoint(uint32_t i_orientation) const;
public:
    Area2D area;
    float depth;
};

______________SD_END_GRAPHICS_NAMESPACE______________