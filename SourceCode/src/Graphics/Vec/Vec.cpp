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

#include <iostream>
#include <fstream>
#include <cmath>

#include "SDEngineCommonFunction.h"
#include "Vec.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

//---------------------------------- vec3 static ------------------------------------
const vec3 vec3::Zero(0.0f,0.0f,0.0f);

vec3 vec3::normalize(const vec3 &i_v)
{
    float magnitude = sqrt(i_v.x*i_v.x + i_v.y*i_v.y + i_v.z*i_v.z);
    vec3 r;
    r.x = i_v.x / magnitude;
    r.y = i_v.y / magnitude;
    r.z = i_v.z / magnitude;
    return r;
}
//-------------------------------------- vec3 --------------------------------------
//--- operator function
vec3 vec3::normalize() const
{
    vec3 result;
    float mag = magnitude();
    result.x = x / mag;
    result.y = y / mag;
    result.z = z / mag;
    return result;
}
//-- a cross b
vec3 vec3::cross(const vec3 &i_b) const
{
    vec3 r;//result
    r.x = y * i_b.z - z * i_b.y;
    r.y = z * i_b.x - x * i_b.z;
    r.z = x * i_b.y - y * i_b.x;
    return r;
}

vec3 vec3::scale(float i_s) const
{
    vec3 r;//result
    r.x = x * i_s;
    r.y = y * i_s;
    r.z = z * i_s;
    return r;
}

float vec3::dot(const vec3 &i_b) const
{
    return x * i_b.x + y * i_b.y + z * i_b.z;
}

vec3 vec3::operator +(const vec3 &i_b) const
{
    vec3 r;//result
    r.x = x + i_b.x;
    r.y = y + i_b.y;
    r.z = z + i_b.z;
    return r;
}

vec3 vec3::operator -(const vec3 &i_b) const
{
    vec3 r;//result
    r.x = x - i_b.x;
    r.y = y - i_b.y;
    r.z = z - i_b.z;
    return r;
}

vec3& vec3::operator =(const vec3 &i_b)
{
    if(this == &i_b) return *this;
    x = i_b.x; y = i_b.y; z = i_b.z;

    return *this;
}

vec3& vec3::operator +=(const vec3 &i_b)
{
    vec3 t;
    t.x = x + i_b.x;
    t.y = y + i_b.y;
    t.z = z + i_b.z;
    x = t.x; y = t.y; z = t.z;
    return *this;
}

vec3& vec3::operator -=(const vec3 &i_b)
{
    vec3 t;
    t.x = x - i_b.x;
    t.y = y - i_b.y;
    t.z = z - i_b.z;
    x = t.x; y = t.y; z = t.z;
    return *this;
}

float vec3::magnitude() const
{
    return sqrt(x*x + y*y + z*z);
}

std::string vec3::ToString() const
{
    return SDE::Basic::StringFormat("(%lf,%lf,%lf)", x, y, z);
}
//----------------------------------- vec2 static ------------------------------------
vec2 vec2::normalize(const vec2 &i_v)
{
    float magnitude = sqrt(i_v.x*i_v.x + i_v.y*i_v.y);
    vec2 r;
    r.x = i_v.x / magnitude;
    r.y = i_v.y / magnitude;
    return r;
}

//-------------------------------------- vec2 --------------------------------------
vec2 vec2::normalize() const
{
    float magnitude = sqrt(x*x + y*y);
    vec2 r;
    r.x = x / magnitude;
    r.y = y / magnitude;
    return r;
}

float vec2::dot(const vec2 &i_b) const
{
    return x * i_b.x + y * i_b.y;
}

float vec2::magnitude() const
{
    return sqrt(x * x + y * y);
}

vec2 vec2::scale(float i_s) const
{
    vec2 r;//result
    r.x = x * i_s;
    r.y = y * i_s;
    return r;
}

vec2 vec2::operator +(const vec2 &i_b) const
{
    vec2 r;//result
    r.x = x + i_b.x;
    r.y = y + i_b.y;
    return r;
}

vec2 vec2::operator -(const vec2 &i_b) const
{
    vec2 r;//result
    r.x = x - i_b.x;
    r.y = y - i_b.y;
    return r;
}

vec2& vec2::operator =(const vec2& i_b)
{
    if(this == &i_b) return *this;
    x = i_b.x; y = i_b.y;

    return *this;
}

vec2& vec2::operator +=(const vec2& i_b)
{
    vec2 t;
    t.x = x + i_b.x;
    t.y = y + i_b.y;
    x = t.x; y = t.y;
    return *this;
}

vec2& vec2::operator -=(const vec2& i_b)
{
    vec2 t;
    t.x = x - i_b.x;
    t.y = y - i_b.y;
    x = t.x; y = t.y;
    return *this;
}

//----------------------------------------- debug function -----------------------------------------
std::string vec2::ToString() const
{
    return SDE::Basic::StringFormat("(%lf,%lf)", x, y);
}

______________SD_END_GRAPHICS_NAMESPACE______________