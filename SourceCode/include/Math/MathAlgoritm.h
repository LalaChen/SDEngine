/* ==============  SD Engine License ==============
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

/*! \file      MathAlgoritm.h
 *  \brief     MathAlgoritm is collection with basic math methods.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/31
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "Ray.h"
#include "Resolution.h"
#include "Matrix4X4f.h"
#include "Vector3f.h"

using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;
using SDE::Graphics::Resolution;

_______________SD_START_MATH_NAMESPACE_______________

bool SDENGINE_API CalculateRayTriangleIntersection(
    float &io_distance, float &io_u, float &io_v, Vector3f &io_hitted_point,
    const Vector3f &i_v0, const Vector3f &i_v1, const Vector3f &i_v2,
    const Vector3f &i_ray_origin, const Vector3f &i_ray_direction, const float i_length = -1.0f,
    bool i_culling = true);

________________SD_END_MATH_NAMESPACE________________