/*==============  SD Engine License ==============
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

/*! \file      ImageParam.h
 *  \brief     Introduce of class ImageOffset and ImageSize.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class ImageSize
 *  \brief Record size of this image.
 */
class SDENGINE_CLASS ImageSize
{
public:
    /*! \fn ImageSize();
     *  \brief The constructor of ImageSize Class.
     */
    ImageSize();

    /*! \fn ImageSize(Size_ui32 i_width, Size_ui32 i_height, Size_ui32 i_length);
     *  \brief The constructor of ImageSize Class.
     *  \param [in] i_width Width.
     *  \param [in] i_height Height.
     *  \param [in] i_length Length.
     */
    ImageSize(Size_ui32 i_width, Size_ui32 i_height, Size_ui32 i_length);

    /*! \fn ~ImageSize();
     *  \brief The destructor of ImageSize Class.
     */
    ~ImageSize();
public:
    /*! \var Size_ui32 m_width;
     *  \brief The width of texture.
     */
    Size_ui32 m_width;

    /*! \var Size_ui32 m_height;
     *  \brief The height of texture.
     */
    Size_ui32 m_height;

    /*! \var Size_ui32 m_length;
     *  \brief The length of texture.
     */
    Size_ui32 m_length;
};

inline ImageSize::ImageSize()
: m_width(0u) ,m_height(0u) ,m_length(0u) 
{
}

inline ImageSize::ImageSize(Size_ui32 i_width, Size_ui32 i_height, Size_ui32 i_length)
: m_width(i_width), m_height(i_height), m_length(i_length)
{
}

inline ImageSize::~ImageSize()
{
}

/*! \class ImageOffset
 *  Record starting position in an image.
 */
class SDENGINE_CLASS ImageOffset
{
public:
    /*! \fn ImageOffset();
     *  \brief The constructor of ImageOffset Class.
     */
    ImageOffset();

    /*! \fn ImageOffset(int32_t i_x, int32_t i_y, int32_t i_z);
     *  \param [in] i_x x.
     *  \param [in] i_y y.
     *  \param [in] i_z z.
     *  \brief The constructor of ImageOffset Class.
     */
    ImageOffset(int32_t i_x, int32_t i_y, int32_t i_z);

    /*! \fn ~ImageOffset();
     *  \brief The destructor of ImageOffset Class.
     */
    ~ImageOffset();
public:
    /*! \var int32_t m_x;
     *  \brief The start index at width of texture.
     */
    int32_t m_x;

    /*! \var int32_t m_y;
     *  \brief The start index at height of texture.
     */
    int32_t m_y;

    /*! \var int32_t m_z;
     *  \brief The start index at length of texture.
     */
    int32_t m_z;
};

inline ImageOffset::ImageOffset()
: m_x(0u), m_y(0u), m_z(0u)
{
}

inline ImageOffset::ImageOffset(int32_t i_x, int32_t i_y, int32_t i_z)
: m_x(i_x), m_y(i_y), m_z(i_z)
{
}

inline ImageOffset::~ImageOffset()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________