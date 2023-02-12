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
#include "Vector3f.h"

#include <glm/gtc/type_ptr.hpp>

#include "SDEngineCommonFunction.h"
#include "Matrix4X4f.h"

using namespace SDE::Basic;

_______________SD_START_MATH_NAMESPACE_______________

//static
const Vector3f Vector3f::PositiveX(1.0f, 0.0f, 0.0f, 0.0f);
const Vector3f Vector3f::PositiveY(0.0f, 1.0f, 0.0f, 0.0f);
const Vector3f Vector3f::PositiveZ(0.0f, 0.0f, 1.0f, 0.0f);
const Vector3f Vector3f::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector3f Vector3f::Origin(0.0f, 0.0f, 0.0f, 1.0f);

//constructor
Vector3f::Vector3f()
: m_vec(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) 
{
}

Vector3f::Vector3f(float i_x, float i_y, float i_z)
: m_vec(glm::vec4(i_x, i_y, i_z, 0.0f))
{
}

Vector3f::Vector3f(float i_x, float i_y, float i_z, float i_w)
: m_vec(glm::vec4(i_x, i_y, i_z, i_w))
{
}

Vector3f::Vector3f(const Vector3f &i_src)
: m_vec(i_src.m_vec)
{
}

Vector3f::Vector3f(const glm::vec4 &b)
: m_vec(b)
{
}

Vector3f::Vector3f(const glm::vec3 &b)
: m_vec(b.x, b.y, b.z, 0.0f)
{
}

Vector3f::~Vector3f()
{
}

//operator
Vector3f& Vector3f::operator =(const Vector3f &b)
{
    if (&b != this) {
        m_vec = b.m_vec;
    }
    return (*this);
}

Vector3f& Vector3f::operator +=(const Vector3f &b)
{
    if (&b == this) {
        glm::vec4 tmp = m_vec;
        m_vec += tmp;
    }
    else {
        m_vec += b.m_vec;
    }
    return *this;
}

Vector3f& Vector3f::operator -=(const Vector3f &b)
{
    if (&b == this) {
        glm::vec4 tmp = m_vec;
        m_vec -= tmp;
    }
    else {
        m_vec -= b.m_vec;
    }
    return *this;
}

Vector3f Vector3f::operator +(const Vector3f &b) const
{
    Vector3f result;
    result.m_vec = m_vec + b.m_vec;
    return result;
}

Vector3f Vector3f::operator -(const Vector3f &b) const
{
    Vector3f result;
    result.m_vec = m_vec - b.m_vec;
    return result;
}

float Vector3f::dot(const Vector3f &b) const
{
    return glm::dot(m_vec, b.m_vec);
}

Vector3f Vector3f::cross(const Vector3f &b) const
{
    return glm::cross(
        glm::vec3(m_vec.x, m_vec.y, m_vec.z), 
        glm::vec3(b.m_vec.x, b.m_vec.y, b.m_vec.z));
}

Vector3f Vector3f::scale(float i_scale_factor) const
{
    return glm::vec4(
        m_vec.x * i_scale_factor, 
        m_vec.y * i_scale_factor,
        m_vec.z * i_scale_factor,
        m_vec.w);
}

Vector3f Vector3f::scale(float i_sx, float i_sy, float i_sz) const
{
    return glm::vec4(
        m_vec.x * i_sx,
        m_vec.y * i_sy,
        m_vec.z * i_sz,
        m_vec.w);
}

float Vector3f::magnitude() const
{
    return glm::length(m_vec);
}

Vector3f Vector3f::normalize() const
{
    return glm::normalize(m_vec);
}

Vector3f Vector3f::negative() const
{
    return glm::vec4(-m_vec.x, -m_vec.y, -m_vec.z, m_vec.w);
}

bool Vector3f::operator==(const Vector3f &b) const
{
    return (m_vec == b.m_vec);
}

bool Vector3f::parallel(const Vector3f &b) const
{
    Vector3f cv = normalize().cross(b.normalize());
    return (cv == Zero);
}

bool Vector3f::vertical(const Vector3f &b) const
{
    float dv = normalize().dot(b.normalize());
    return (fabs(dv - 1.0f) <= 0.000001f);
}

void Vector3f::RepresentPosition()
{
    m_vec.w = 1.0f;
}

void Vector3f::RepresentVector()
{
    m_vec.w = 0.0f;
}

std::string Vector3f::ToString() const
{
    return StringFormat("(%lf,%lf,%lf,%lf)", m_vec.x, m_vec.y, m_vec.z, m_vec.w);
}

const float* Vector3f::GetDataAddr() const
{
    return (const float*)glm::value_ptr(m_vec);
}

________________SD_END_MATH_NAMESPACE________________