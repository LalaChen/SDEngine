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
#include "Matrix4X4f.h"

using namespace SDE::Basic;

//--------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//--------------------------- start of namespace Math ----------------------------
namespace Math
{

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

std::string Matrix4X4f::ToString() const
{
    std::string result =
        StringFormat("m = | %.3lf %.3lf %.3lf %.3lf | \n", m_matrix[ 0], m_matrix[ 1], m_matrix[ 2], m_matrix[ 3]) +
        StringFormat("m = | %.3lf %.3lf %.3lf %.3lf | \n", m_matrix[ 4], m_matrix[ 5], m_matrix[ 6], m_matrix[ 7]) +
        StringFormat("m = | %.3lf %.3lf %.3lf %.3lf | \n", m_matrix[ 8], m_matrix[ 9], m_matrix[10], m_matrix[11]) +
        StringFormat("m = | %.3lf %.3lf %.3lf %.3lf | \n", m_matrix[12], m_matrix[13], m_matrix[14], m_matrix[15]);
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