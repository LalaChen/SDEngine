#include "Vector3f.h"

//--------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//-------------------------- start of namespace Math --------------------------
namespace Math
{

Vector3f Vector3f::Zero(0.0f, 0.0f, 0.0f, 0.0f);

//operator
Vector3f& Vector3f::operator =(const Vector3f &b)
{
    if (&b == this) {
        return (*this);
    }
    else {
        m_vec = b.m_vec;
        return (*this);
    }
}

Vector3f& Vector3f::operator +=(const Vector3f &b)
{
    if (this == &b) {
        return *this;
    }
    else {
        m_vec += b.m_vec;
        return *this;
    }
}

Vector3f& Vector3f::operator -=(const Vector3f &b)
{
    if (this == &b) {
        return *this;
    }
    else {
        m_vec -= b.m_vec;
        return *this;
    }
}

Vector3f Vector3f::operator +(const Vector3f &b) const
{
    Vector3f result;
    result.m_vec = m_vec + result.m_vec;
    return result;
}

Vector3f Vector3f::operator -(const Vector3f &b) const
{
    Vector3f result;
    result.m_vec = m_vec - result.m_vec;
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

bool Vector3f::is_parallel(const Vector3f &b) const
{
    Vector3f cv = normalize().cross(b.normalize());
    return (cv == Zero);
}

bool Vector3f::is_vertical(const Vector3f &b) const
{
    float dv = normalize().dot(b.normalize());
    return (fabs(dv - 1.0f) <= 0.000001f);
}

//--------------------------- end of namespace Math ----------------------------
}
//--------------------------- end of namespace SDE ----------------------------
}