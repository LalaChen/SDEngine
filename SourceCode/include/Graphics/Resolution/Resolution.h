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

/*! \file      Resolution.h
	\brief     The class resolution is used to record the size of framebuffer or screen.
	\author    Kuan-Chih, Chen
	\date      2019/02/02
	\copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

using SDE::Graphics::Size_ui;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class Resolution
	Class Resolution is used to record the size of framebuffer or screen. We also calcuate \n
	aspect ratio of this resolution.
*/
class SDENGINE_CLASS Resolution
{
public:
	/*! \var const static Size_ui DEFAULT_WIDTH;
		\brief default width(px) of resolution. It's 1920.
	*/
	const static Size_ui DEFAULT_WIDTH = 1920;

	/*! \var const static Size_ui DEFAULT_HEIGHT;
		\brief default height(px) of resolution. It's 1080.
	*/
	const static Size_ui DEFAULT_HEIGHT = 1080;
public:

	/*! \fn Resolution();
		\brief Constructor of Resolution. We will initialize by default w and h.
	*/
	Resolution();

	/*! \fn Resolution(Size_ui i_w, Size_ui i_h);
	    \param i_w Width of resolution.
		\param i_h Height of resolution.
		\brief Constructor of Resolution. We will initialize by i_w and i_h.
	*/
	Resolution(Size_ui i_w, Size_ui i_h);

	/*! \fn Resolution(const Resolution &i_src);
	    \param i_src The target resolution.
		\brief Copy Constructor of Resolution.
	*/
	Resolution(const Resolution &i_src);

	/*! \fn ~Resolution();
		\brief destructor of Resolution.
	*/
	~Resolution();
public:
	/*! \fn void SetResolution(Size_ui i_w, Size_ui i_h);
     	\param i_w Width of resolution.
		\param i_h Height of resolution.
		\brief Setter of class Resolution. We will set this one by i_w and i_h.
	*/
	void SetResolution(Size_ui i_w, Size_ui i_h);

	/*! \fn void GetWindowSize(Size_ui &io_w, Size_ui &io_h) const;
		\param io_w The storage of width.
		\param io_h The storage of height.
		\brief Getter of class Resolution. We need offering initialized storage.
	*/
	void GetResolution(Size_ui &io_w, Size_ui &io_h) const;
public:
	Resolution& operator=(const Resolution& i_src);
private:
	void CalculateRatioBetweenWidthAndHeight();
protected:
	/* \var Size_ui m_width;
	   \brief Width of Resolution. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(Size_ui, m_width, Width);

	/* \var Size_ui m_width;
	   \brief Width of Resolution. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(Size_ui, m_height, Height);

	/* \var float m_aspect_ratio;
	   \brief Aspect ratio of Resolution. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(float, m_aspect_ratio, Ratio);
};

//--------------------------------- ctor and dtor -------------------------------------------
inline Resolution::Resolution()
: m_width(DEFAULT_WIDTH)
, m_height(DEFAULT_HEIGHT)
, m_aspect_ratio(1.0f)
{
	CalculateRatioBetweenWidthAndHeight();
}

inline Resolution::Resolution(Size_ui i_w, Size_ui i_h)
: m_width(i_w)
, m_height(i_h)
, m_aspect_ratio(1.0f)
{
	CalculateRatioBetweenWidthAndHeight();
}

inline Resolution::Resolution(const Resolution &i_src)
: m_width(i_src.m_width)
, m_height(i_src.m_height)
, m_aspect_ratio(1.0f)
{
	CalculateRatioBetweenWidthAndHeight();
}

inline Resolution::~Resolution()
{
}

//--------------------------------- private working function --------------------------------
inline void Resolution::CalculateRatioBetweenWidthAndHeight()
{
	m_aspect_ratio = static_cast<float>(m_width) / static_cast<float>(m_height);
}

//--------------------------------- public working function ---------------------------------
inline void Resolution::SetResolution(Size_ui i_w, Size_ui i_h)
{
	m_width  = i_w;
	m_height = i_h;
	CalculateRatioBetweenWidthAndHeight();
}

inline void Resolution::GetResolution(Size_ui &io_w, Size_ui &io_h) const
{
	io_w = m_width;
	io_h = m_height;
}

inline Resolution& Resolution::operator =(const Resolution &i_src)
{
	if(this != &i_src){
		m_width  = i_src.m_width;
		m_height = i_src.m_height;
		CalculateRatioBetweenWidthAndHeight();
	}
	return *this;
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}
