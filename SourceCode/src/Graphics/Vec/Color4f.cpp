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

#include "SDEngineCommonFunction.h"
#include "Color4f.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

//----------------------------------- class Color4B -----------------------------------
float Color4B::GetFloatR() const
{
    return static_cast<float>(r) / 255.0f;
}
float Color4B::GetFloatG() const
{
    return static_cast<float>(g) / 255.0f;
}
float Color4B::GetFloatB() const
{
    return static_cast<float>(b) / 255.0f;
}
float Color4B::GetFloatA() const
{
    return static_cast<float>(a) / 255.0f;
}
//----------------------------------- class Color4f -----------------------------------
//---- Static Member Function
Color4f Color4f::Red(float i_scale)
{
    return Color4f(i_scale*1.0f, 0.0f, 0.0f, 1.0f);
}

Color4f Color4f::Green(float i_scale)
{
    return Color4f(0.0f, i_scale*1.0f, 0.0f, 1.0f);
}

Color4f Color4f::Blue(float i_scale)
{
    return Color4f(0.0f, 0.0f, i_scale * 1.0f, 1.0f);
}

Color4f Color4f::White(float i_scale)
{
    return Color4f(i_scale * 1.0f, i_scale * 1.0f, i_scale * 1.0f, 1.0f);
}

Color4f Color4f::Black()
{
    return Color4f(0.0f, 0.0f, 0.0f, 0.0f);
}
//---- Member Function
Color4f& Color4f::operator =(const Color4f &i_src)
{
    if (&i_src != this) {
        r = i_src.r;
        g = i_src.g;
        b = i_src.b;
        a = i_src.a;
    }

    Clamp01();

    return *this;
}

Color4f& Color4f::operator +=(const Color4f &i_src)
{
    if (&i_src == this) {
        r += i_src.r;
        g += i_src.g;
        b += i_src.b;
        a += i_src.a;
    }

    Clamp01();

    return *this;
}

Color4f& Color4f::operator -=(const Color4f &i_src)
{
    if (&i_src == this) {
        r -= i_src.r;
        g -= i_src.g;
        b -= i_src.b;
        a -= i_src.a;
    }

    Clamp01();

    return *this;
}

Color4f& Color4f::operator *=(const Color4f &i_src)
{
    if (&i_src == this) {
        r *= i_src.r;
        g *= i_src.g;
        b *= i_src.b;
        a *= i_src.a;
    }

    Clamp01();

    return *this;
}

Color4f Color4f::operator +(const Color4f &i_src) const
{
    Color4f result;

    result.r = r + i_src.r;
    result.g = g + i_src.g;
    result.b = b + i_src.b;
    result.a = a + i_src.a;

    result.Clamp01();

    return result;
}

Color4f Color4f::operator -(const Color4f &i_src) const
{
    Color4f result;

    result.r = r - i_src.r;
    result.g = g - i_src.g;
    result.b = b - i_src.b;
    result.a = a - i_src.a;

    result.Clamp01();

    return result;
}

Color4f Color4f::operator *(const Color4f &i_src) const
{
    Color4f result;

    result.r = r * i_src.r;
    result.g = g * i_src.g;
    result.b = b * i_src.b;
    result.a = a * i_src.a;

    result.Clamp01();

    return result;
}

Color4f Color4f::Scale(float i_value) const
{
    Color4f result;

    result.r = r * i_value;
    result.g = g * i_value;
    result.b = b * i_value;
    result.a = a * i_value;

    result.Clamp01();

    return result;
}

Color4B Color4f::ToColor4B() const
{
    return Color4B((unsigned char)(r * 255.0f),
        (unsigned char)(g * 255.0f),
        (unsigned char)(b * 255.0f),
        (unsigned char)(a * 255.0f));
}

std::string Color4f::ToString() const
{
    return StringFormat("(%lf, %lf, %lf, %lf)", r, g, b, a);
}

void Color4f::Clamp01()
{
    COLORF_CLAMP_01(r);
    COLORF_CLAMP_01(g);
    COLORF_CLAMP_01(b);
    COLORF_CLAMP_01(a);
}

______________SD_END_GRAPHICS_NAMESPACE______________