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
#include "Quaternion.h"

#include <glm/gtc/type_ptr.hpp>

#include "SDEngineCommonFunction.h"
#include "MathConstant.h"
#include "Matrix4X4f.h"
#include "Vector3f.h"

using namespace SDE::Basic;

_______________SD_START_MATH_NAMESPACE_______________

//static
float Quaternion::Angle(const Quaternion &i_a, const Quaternion &i_b)
{
    glm::quat inv_a = glm::inverse(i_a.m_quat);
    glm::quat rst = i_b.m_quat * inv_a;
    return glm::angle(rst);
}

Quaternion Quaternion::Slerp(const Quaternion &i_src, const Quaternion &i_dest, float i_ratio)
{
    return glm::slerp(i_src.m_quat, i_dest.m_quat, i_ratio);
}

Quaternion Quaternion::Lerp(const Quaternion &i_src, const Quaternion &i_dest, float i_ratio)
{
    return glm::lerp(i_src.m_quat, i_dest.m_quat, i_ratio);
}

Quaternion Quaternion::LookAt(const Vector3f &i_forward, const Vector3f &i_up)
{
    //glm::quatLookAt see forward -z.
    return glm::quatLookAt(
        glm::vec3(i_forward.m_vec.x, i_forward.m_vec.y, i_forward.m_vec.z),
        glm::vec3(i_up.m_vec.x, i_up.m_vec.y, i_up.m_vec.z));
}

Quaternion Quaternion::ConvertFromMatrix(const Matrix4X4f &i_mat)
{
    return glm::toQuat(i_mat.m_matrix);
}
//constructor
Quaternion::Quaternion()
{
}

Quaternion::Quaternion(const Quaternion &i_src)
: m_quat(i_src.m_quat)
{
}

Quaternion::Quaternion(const Vector3f &i_axis, float i_angle)
{
    glm::vec3 unit_vec = glm::normalize(glm::vec3(i_axis.m_vec.x, i_axis.m_vec.y, i_axis.m_vec.z));
    m_quat = glm::angleAxis(i_angle * ONE_DEGREE_OF_PI, unit_vec);
}

Quaternion::Quaternion(float i_x_angle, float i_y_angle, float i_z_angle)
{
    m_quat = glm::quat(
        glm::vec3(
            i_x_angle * ONE_DEGREE_OF_PI,
            i_y_angle * ONE_DEGREE_OF_PI,
            i_z_angle * ONE_DEGREE_OF_PI));
}

Quaternion::Quaternion(float i_x, float i_y, float i_z, float i_w)
: m_quat(i_w, i_x, i_y, i_z)
{
}

Quaternion::Quaternion(const glm::quat &i_src)
: m_quat(i_src)
{
}

Quaternion::~Quaternion()
{
}
//operator
Quaternion Quaternion::operator* (const Quaternion &b) const
{
    return m_quat * b.m_quat;
}

Quaternion& Quaternion::operator= (const Quaternion &b)
{
    if (&b != this) {
        m_quat = b.m_quat;
    }
    return (*this);
}

Quaternion& Quaternion::operator*= (const Quaternion &b)
{
    if (&b == this) {
        glm::quat tmp = m_quat;
        m_quat *= tmp;
    }
    else {
        m_quat *= b.m_quat;
    }
    return (*this);
}

Quaternion Quaternion::inverse() const
{
    return glm::inverse(m_quat);
}

Quaternion Quaternion::conjugation() const
{
    return glm::conjugate(m_quat);
}

Vector3f Quaternion::rotate(const Vector3f &b) const
{
    glm::vec3 rst = glm::rotate(m_quat, glm::vec3(b.m_vec.x, b.m_vec.y, b.m_vec.z));
    return glm::vec4(rst.x, rst.y, rst.z, b.m_vec.w);
}

Quaternion Quaternion::normalize() const
{
    return glm::normalize(m_quat);
}

Vector3f Quaternion::toEulerianAngles() const
{
    return glm::eulerAngles(m_quat);
}

Matrix4X4f Quaternion::toMatrix4X4f() const
{
    return glm::toMat4(m_quat);
}

std::string Quaternion::ToString() const
{
    return StringFormat("Q([%lf,%lf,%lf],%lf)", m_quat.x, m_quat.y, m_quat.z, m_quat.w);
}

const float* Quaternion::GetDataAddr() const
{
    return glm::value_ptr(m_quat);
}

________________SD_END_MATH_NAMESPACE________________