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

/*! \file      Ray.h
 *  \brief     Ray class is used to present ray.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/31
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "Application.h"
#include "Resolution.h"
#include "Transform.h"

using SDE::Basic::TouchButton;
using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;
using SDE::Math::Transform;
using SDE::Graphics::Resolution;

______________SD_START_PHYSICS_NAMESPACE_____________

class SDENGINE_CLASS Ray
{
public:
    Ray();
    ~Ray();
public:
    void InitializeByScreen(const Matrix4X4f &i_proj_mat, const Matrix4X4f &i_view_mat, const Resolution &i_res, const TouchButton &i_tb, float i_near, float i_length = 1000.0f);
    void InitializeByStartAndEnd(const Vector3f &i_start, const Vector3f &i_end);
public:
    bool IsEndless() const;
    bool IsValid() const;
    Transform CalculateTransform(const Vector3f &i_v = Vector3f::PositiveY) const;
public:
    Vector3f m_origin;
    Vector3f m_orientation;
    float m_length;
protected:
    bool m_is_endless;
};

inline bool Ray::IsEndless() const
{
    return m_is_endless;
}

inline bool Ray::IsValid() const
{
    return (std::fabs(m_orientation.magnitude()) > 0.000001);
}


_______________SD_END_PHYSICS_NAMESPACE______________