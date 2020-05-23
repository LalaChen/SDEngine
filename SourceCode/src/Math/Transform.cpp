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

#include "Transform.h"

_______________SD_START_MATH_NAMESPACE_______________

//----------------------------------- static part -----------------------------------
Transform Transform::DecomposeMatrixToTransform(const Matrix4X4f &i_src)
{
	Transform result;
    Matrix4X4f::decompose(i_src, result.m_scale, result.m_rotation, result.m_skew, result.m_position, result.m_perspective);
	return result;
}
Transform Transform::LookAt(const Vector3f &i_start, const Vector3f &i_focus, const Vector3f &i_up, bool i_is_view)
{
    Transform result;
    Vector3f forward = (i_focus - i_start).normalize();
    Vector3f vup = i_up.normalize();
    forward.RepresentVector();
    if (i_is_view == false) {
        //glm::quatLookAt return quaternion which forward is -z axis(right hand coordinate). So we need to inverse forward.
        forward = forward.negative();
    }
    result.m_position = i_start;
    if (forward.parallel(i_up) == false) {
        result.m_rotation = Quaternion::LookAt(forward, vup);
    }
    return result;
}

//-----------------------------------------------------------------------------------
Transform::Transform()
: m_position(0.0f, 0.0f, 0.0f, 1.0f)
, m_rotation(0.0f, 0.0f, 0.0f, 1.0f)
, m_scale(1.0f, 1.0f, 1.0f, 0.0f)
{
}

Transform::Transform(const Matrix4X4f &i_trans)
{
    Matrix4X4f::decompose(i_trans, m_scale, m_rotation, m_skew, m_position, m_perspective);
}

Transform::~Transform()
{
}

Matrix4X4f Transform::MakeWorldMatrix() const
{
    Matrix4X4f result;
    result.translate(m_position);
    result.rotate(m_rotation);
    result.scale(m_scale);
    return result;
}

Matrix4X4f Transform::MakeViewMatrix() const
{
    return MakeWorldMatrix().inverse();
}

Matrix4X4f Transform::MakeNormalMatrix() const
{
    Matrix4X4f result = MakeWorldMatrix();
    result.m_matrix[3][0] = 0.0f; result.m_matrix[3][1] = 0.0f; result.m_matrix[3][2] = 0.0f;
    return result;
}

Vector3f Transform::GetForward() const
{
    return m_rotation.rotate(Vector3f::PositiveZ);
}

Vector3f Transform::GetRight() const
{
    return m_rotation.rotate(Vector3f::PositiveX);
}

Vector3f Transform::GetTop() const
{
    return m_rotation.rotate(Vector3f::PositiveY);
}

void Transform::AddTranslation(const Vector3f &i_offset)
{
    m_position += i_offset;
}

void Transform::AddRotation(const Vector3f &i_axis, float i_angle)
{
    Quaternion new_rot = Quaternion(i_axis, i_angle) * m_rotation;
    m_rotation = new_rot;
}

Transform& Transform::operator=(const Transform &i_src)
{
    if (&i_src != this) {
        m_position = i_src.m_position;
        m_rotation = i_src.m_rotation;
        m_scale = i_src.m_scale;
        m_skew = i_src.m_skew;
        m_perspective = i_src.m_perspective;
    }
    return *this;
}

________________SD_END_MATH_NAMESPACE________________