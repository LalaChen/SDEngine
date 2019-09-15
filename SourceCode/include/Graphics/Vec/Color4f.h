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

/*! \file      Color4f.h
 *  \brief     Color4f data struct.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/04/14
 *  \copyright MIT License.
 */
#pragma once

#include <cstdint>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#define COLORF_CLAMP_01(channel) \
    if(channel < 0.0f) channel = 0.0f; \
    else if(channel > 1.0f) channel = 1.0f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class Color4B
 *  In our system, Color4B is use to record r,g,b,a with unsigned char format.
 */
class SDENGINE_CLASS Color4B
{
public:
    /*! \fn Color4B();
     *  \brief Defualt constructor of Color4B. Set all channel by zero.
     */
	Color4B():r(0),g(0),b(0),a(0){}

    /*! \fn Color4B(const Color4B &i_src);
     *  \param [in] i_src target source.
     *  \brief Copy constructor of Color4B.
     */
	Color4B(const Color4B &i_src):r(i_src.r),g(i_src.g),b(i_src.b),a(i_src.a){}

    /*! \fn Color4B(uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a);
     *  \param [in] i_r r component.
     *  \param [in] i_g g component.
     *  \param [in] i_b b component.
     *  \param [in] i_a a component.
     *  \brief Constructor of Color4B.
     */
	Color4B(uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a):r(i_r),g(i_g),b(i_b),a(i_a){}
	
    /*! \fn ~Color4B();
     *  \brief Destructor of Color4B.
     */
    ~Color4B(){}
public:
    /*! \fn float GetFloatR() const;
     *  \brief Give red channel by floating pointer. And the return value is in [0,1].
     */
	float GetFloatR() const;

    /*! \fn float GetFloatG() const;
     *  \brief Give green channel by floating pointer. And the return value is in [0,1].
     */
	float GetFloatG() const;

    /*! \fn float GetFloatB() const;
     *  \brief Give blue channel by floating pointer. And the return value is in [0,1].
     */
	float GetFloatB() const;

    /*! \fn float GetFloatA() const;
     *  \brief Give alpha channel by floating pointer. And the return value is in [0,1].
     */
	float GetFloatA() const;
public:
    /*! \var uint8_t r;
     *  \brief red component.
     */
	uint8_t r;

    /*! \var uint8_t g;
     *  \brief green component.
     */
	uint8_t g;

    /*! \var uint8_t b;
     *  \brief blue component.
     */
	uint8_t b;

    /*! \var uint8_t a;
     *  \brief blue component.
     */
	uint8_t a;
};

/*! \class Color4f
 *  In our system, Color4f is use to record r,g,b,a with float pointing format.
    But the valid domain is [0.0, 1,0]. So we will clamp value to [0,1] after each operation done.
 */
class SDENGINE_CLASS Color4f
{
public:
    /*! \fn static Color4f Red(float i_scale = 1.0f);
     *  \param [in] i_scale scale red color.
     *  \brief Create red Color4f.
     */
	static Color4f Red(float i_scale = 1.0f);

    /*! \fn static Color4f Green(float i_scale = 1.0f);
     *  \param [in] i_scale scale green color.
     *  \brief Create green Color4f.
     */
	static Color4f Green(float i_scale = 1.0f);

    /*! \fn static Color4f Blue(float i_scale = 1.0f);
     *  \param [in] i_scale scale blue color.
     *  \brief Create blue Color4f.
     */
	static Color4f Blue(float i_scale = 1.0f);

    /*! \fn static Color4f White(float i_scale = 1.0f);
     *  \param [in] i_scale scale white color.
     *  \brief Create white Color4f.
     */
	static Color4f White(float i_scale = 1.0f);

    /*! \fn sstatic Color4f Black();
     *  \brief Create black Color4f.
     */
	static Color4f Black();
public:
    /*! \fn Color4f();
     *  \brief Defualt constructor of Color4f. Set all channel by zero.
     */
    Color4f():r(0.0f),g(0.0f),b(0.0f),a(0.0f){}

    /*! \fn Color4f(const Color4f &i_src);
     *  \param [in] i_src target source.
     *  \brief Copy constructor of Color4f.
     */
	Color4f(const Color4f &i_src):r(i_src.r),g(i_src.g),b(i_src.b),a(i_src.a){}

    /*! \fn Color4f(const Color4f &i_src);
     *  \param [in] i_r r component.
     *  \param [in] i_g g component.
     *  \param [in] i_b b component.
     *  \param [in] i_a a component.
     *  \brief Constructor of Color4f. Set and then clamp to 0 and 1.
     */
    Color4f(float i_r, float i_g, float i_b, float i_a):r(i_r),g(i_g),b(i_b),a(i_a){}

    /*! \fn ~Color4f();
     *  \brief Destructor of Color4f.
     */
    ~Color4f(){}
public:
    /*! \fn Color4f& operator=(const Color4f &i_src);
     *  \param [in] i_src target Color4f
     *  \brief assign operator.
     */
	Color4f& operator=(const Color4f &i_src);

    /*! \fn Color4f& operator*=(const Color4f &i_src);
     *  \param [in] i_src target Color4f
     *  \brief Multiply and assign operator.
     */
	Color4f& operator*=(const Color4f &i_src);

    /*! \fn Color4f& operator-=(const Color4f &i_src);
     *  \param [in] i_src target Color4f
     *  \brief Substract and assign operator.
     */
    Color4f& operator-=(const Color4f &i_src);

    /*! \fn Color4f& operator+=(const Color4f &i_src);
     *  \param [in] i_src target Color4f
     *  \brief Add and assign operator.
     */
	Color4f& operator+=(const Color4f &i_src);

    /*! \fn Color4f operator*(const Color4f &i_src) const;
     *  \param [in] i_src target Color4f
     *  \brief multiply operator.
     */
	Color4f operator*(const Color4f &i_src) const;

    /*! \fn Color4f operator+(const Color4f &i_src) const;
     *  \param [in] i_src target Color4f
     *  \brief Add operator.
     */
	Color4f operator+(const Color4f &i_src) const;

    /*! \fn Color4f operator+(const Color4f &i_src) const;
     *  \param [in] i_src target Color4f
     *  \brief substract operator.
     */
	Color4f operator-(const Color4f &i_src) const;
public:

    /*! \fn Color4f Scale(float i_value) const;
     *  \param [in] i_value target scale factor.
     *  \brief scale operator.
     */
	Color4f Scale(float i_value) const;
public:

    /*! \fn Color4B ToColor4B() const;
     *  \param [in] i_value target scale factor.
     *  \brief Convert the color to Color4B.
     */
	Color4B ToColor4B() const;
private:
    /*! \fn void Clamp01();
     *  \brief Clamp all components to [0,1].
     */
	void Clamp01();
public:
    /*! \var float r;
     *  \brief red component.
     */
	float r;

    /*! \var float g;
     *  \brief green component.
     */
	float g;

    /*! \var float b;
     *  \brief blue component.
     */
	float b;

    /*! \var float a;
     *  \brief alpha component.
     */
	float a;
};

______________SD_END_GRAPHICS_NAMESPACE______________