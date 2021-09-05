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

#include "Ray.h"

using namespace SDE::Math;

______________SD_START_PHYSICS_NAMESPACE_____________

Ray::Ray()
: m_is_endless(false)
, m_length(0.0f)
{
}

Ray::~Ray()
{
}

void Ray::InitializeByScreen(const Matrix4X4f &i_proj_mat, const Matrix4X4f &i_view_mat, const Resolution &i_res, const TouchButton &i_tb, float i_near, float i_length)
{
    Matrix4X4f proj_inv_mat = i_proj_mat.inverse();
    Matrix4X4f view_inv_mat = i_view_mat.inverse();
    Vector3f end, start;
    float sc[2] = {0.0f, 0.0f};
    float sw = static_cast<float>(i_res.GetWidth()), sh = static_cast<float>(i_res.GetHeight());
    float hsw = sw / 2.0f, hsh = sh / 2.0f;
    sc[0] = (i_tb.m_x - hsw) / sw * 2.0f;
    sc[1] = (sh - i_tb.m_y - hsh) / sh * 2.0f;
    float ze;
    float new_w;
    Vector3f proj_p;
    //calculate end.
    ze = -(i_near + i_length);
    new_w = -ze;
    proj_p.m_vec.x = sc[0] * (new_w);
    proj_p.m_vec.y = sc[1] * (new_w);
    proj_p.m_vec.z = i_proj_mat.m_matrix[2][2] * ze + i_proj_mat.m_matrix[3][2];
    proj_p.m_vec.w = (new_w);

    end = view_inv_mat * proj_inv_mat * proj_p;
    //calculate start.
    ze = -(i_near);
    new_w = -ze;

    proj_p.m_vec.x = sc[0] * (new_w);
    proj_p.m_vec.y = sc[1] * (new_w);
    proj_p.m_vec.z = i_proj_mat.m_matrix[2][2] * ze + i_proj_mat.m_matrix[3][2];
    proj_p.m_vec.w = (new_w);
    start = view_inv_mat * proj_inv_mat * proj_p;

    m_origin = start;
    m_orientation = (end - start).normalize();
    m_orientation.RepresentVector();
    m_length = i_length;
    m_is_endless = true;
}

void Ray::InitializeByStartAndEnd(const Vector3f &i_start, const Vector3f &i_end)
{
    m_origin = i_start;
    m_orientation = i_end - i_start;
    m_orientation.RepresentVector();
    m_length = m_orientation.magnitude();
    m_orientation = m_orientation.normalize();
    m_is_endless = false;
}

Transform Ray::CalculateTransform(const Vector3f &i_v) const
{
    Vector3f end = m_origin + m_orientation; end.RepresentPosition();
    return Transform::LookAt(m_origin, end, i_v);
}

_______________SD_END_PHYSICS_NAMESPACE______________