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

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDEngineCommonFunction.h"
#include "Quaternion.h"
#include "Vector3f.h"
#include "Matrix4X4f.h"

using namespace SDE::Basic;

//--------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//--------------------------- start of namespace Math ----------------------------
namespace Math
{
//static
bool Matrix4X4f::decompose(const Matrix4X4f &i_mat, Vector3f &io_scale, Quaternion &io_rot, Vector3f &io_skew, Vector3f &io_translation, Vector3f &io_prespective)
{
    glm::vec3 scale, trans, skew;
    bool result = glm::decompose(i_mat.m_matrix, scale, io_rot.m_quat, trans, skew, io_prespective.m_vec);
    if (result == true) {
        io_scale = Vector3f(scale);
        io_skew = Vector3f(skew);
        io_translation = Vector3f(trans);
    }
    return result;
}
//matrix
Matrix4X4f::Matrix4X4f()
: m_matrix(glm::identity<glm::mat4>())
{
}

Matrix4X4f::Matrix4X4f(const Matrix4X4f &i_src)
: m_matrix(i_src.m_matrix)
{
}

Matrix4X4f::Matrix4X4f(const glm::mat4 &i_src)
: m_matrix(i_src)
{
}

Matrix4X4f::Matrix4X4f(float i_datas[])
{
    m_matrix = glm::make_mat4(i_datas);
}

Matrix4X4f::~Matrix4X4f()
{
}

//operator
Matrix4X4f& Matrix4X4f::operator =(const Matrix4X4f &i_src)
{
    if (&i_src == this) {
        return (*this);
    }
    else {
        m_matrix = i_src.m_matrix;
        return (*this);
    }
}

Matrix4X4f& Matrix4X4f::operator *=(const Matrix4X4f &i_src)
{
    if (&i_src == this) {
        Matrix4X4f self(i_src);
        m_matrix *= self.m_matrix;
    }
    else {
        m_matrix *= i_src.m_matrix;
    }
    return (*this);
}

Matrix4X4f Matrix4X4f::operator *(const Matrix4X4f &i_src) const
{
    Matrix4X4f result;
    result.m_matrix = m_matrix * i_src.m_matrix;
    return result;
}

Vector3f Matrix4X4f::operator *(const Vector3f &i_src) const
{
    return Vector3f(m_matrix * i_src.m_vec);
}

Matrix4X4f Matrix4X4f::inverse() const
{
    return Matrix4X4f(glm::inverse(m_matrix));
}

Matrix4X4f Matrix4X4f::transpose() const
{
    return Matrix4X4f(glm::transpose(m_matrix));
}

//--------------- World Space Using --------------------
void Matrix4X4f::translate(const Vector3f &i_trans)
{
    m_matrix *= glm::translate(glm::mat4(1.0f), glm::vec3(i_trans.m_vec.x, i_trans.m_vec.y, i_trans.m_vec.z));
}

void Matrix4X4f::rotate(const Quaternion &i_rotate)
{
    Matrix4X4f rot = i_rotate.toMatrix4X4f();
    m_matrix[0][0] = rot.m_matrix[0][0]; m_matrix[0][1] = rot.m_matrix[0][1]; m_matrix[0][2] = rot.m_matrix[0][2];
    m_matrix[1][0] = rot.m_matrix[1][0]; m_matrix[1][1] = rot.m_matrix[1][1]; m_matrix[1][2] = rot.m_matrix[1][2];
    m_matrix[2][0] = rot.m_matrix[2][0]; m_matrix[2][1] = rot.m_matrix[2][1]; m_matrix[2][2] = rot.m_matrix[2][2];
}

void Matrix4X4f::scale(float i_scale)
{
    m_matrix *= glm::scale(glm::mat4(1.0f), glm::vec3(i_scale, i_scale, i_scale));
}

void Matrix4X4f::scale(const Vector3f &i_scale)
{
    m_matrix *= glm::scale(glm::mat4(1.0f), glm::vec3(i_scale.m_vec.x, i_scale.m_vec.y, i_scale.m_vec.z));
}
//----------------- View Space Using ---------------------
void Matrix4X4f::lookAt(const Vector3f &i_eye, const Vector3f &i_view_center, const Vector3f &i_up)
{
    m_matrix = glm::lookAt(
        glm::vec3(i_eye.m_vec.x, i_eye.m_vec.y, i_eye.m_vec.z),
        glm::vec3(i_view_center.m_vec.x, i_view_center.m_vec.y, i_view_center.m_vec.z),
        glm::vec3(i_up.m_vec.x, i_up.m_vec.y, i_up.m_vec.z));
}
//--------------- Project Space Using --------------------
void Matrix4X4f::perspective(float i_fovy, float i_aspect, float i_near, float i_far)
{
    m_matrix = glm::perspective(i_fovy, i_aspect, i_near, i_far);
}

void Matrix4X4f::ortho(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far)
{
    m_matrix = glm::ortho(i_left, i_right, i_bottom, i_top, i_near, i_far);
}

std::string Matrix4X4f::ToString() const
{
    std::string result =
        StringFormat("\n m = | %.3lf %.3lf %.3lf %.3lf |", m_matrix[0][0], m_matrix[0][1], m_matrix[0][2], m_matrix[0][3]) +
        StringFormat("\n m = | %.3lf %.3lf %.3lf %.3lf |", m_matrix[1][0], m_matrix[1][1], m_matrix[1][2], m_matrix[1][3]) +
        StringFormat("\n m = | %.3lf %.3lf %.3lf %.3lf |", m_matrix[2][0], m_matrix[2][1], m_matrix[2][2], m_matrix[2][3]) +
        StringFormat("\n m = | %.3lf %.3lf %.3lf %.3lf |", m_matrix[3][0], m_matrix[3][1], m_matrix[3][2], m_matrix[3][3]);
    return result;
}

const float* Matrix4X4f::GetDataAddr() const
{
    return (const float*)glm::value_ptr(m_matrix);
}

//--------------------------- end of namespace Math ----------------------------
}
//--------------------------- end of namespace SDE ----------------------------
}