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

/*! \file      TextureIdentity.h
 *  \brief     Introduce of class TextureIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/08/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ManagerParam.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//--------------------------- start of namespace Graphics ------------------------
namespace Graphics
{
/*! \class ImageSize
 *  Record size of this image.
 */
class SDENGINE_CLASS ImageSize
{
public:
    /*! \fn ImageSize();
     *  \brief The constructor of ImageSize Class.
     */
    ImageSize() :m_width(0u) ,m_height(0u) ,m_length(0u) {}

    /*! \fn ~ImageSize();
     *  \brief The destructor of ImageSize Class.
     */
    ~ImageSize() {}
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

/*! \class ImageOffset
 *  Record starting position in an image.
 */
class SDENGINE_CLASS ImageOffset
{
public:
    /*! \fn ImageOffset();
     *  \brief The constructor of ImageOffset Class.
     */
    ImageOffset() :m_x(0u), m_y(0u), m_z(0u) {}
    /*! \fn ~ImageOffset();
     *  \brief The destructor of ImageOffset Class.
     */
    ~ImageOffset() {}
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

/*! \class TextureIdentity
 *  Keep all graphics handle or ID in this struct.
 */
class SDENGINE_CLASS TextureIdentity
{
public:
    /*! \fn explicit TextureIdentity();
     *  \brief The constructor of TextureIdentity Class.
     */
    TextureIdentity();

    /*! \fn explicit ~TextureIdentity();
     *  \brief The destructor of TextureIdentity Class.
     */
    ~TextureIdentity();
public://Raw Data Part.
    /*! \var CompHandle m_image_handle;
     *  \brief The image handle. It is valid while the value is not equal 0.
     */
    CompHandle m_image_handle;

    /*! \var CompHandle m_memory_handle;
     *  \brief The memory handle. This value is nullptr in opengl system.
     */
    CompHandle m_memory_handle;

    /*! \var TextureTypeEnum m_texture_type;
     *  \brief The texture type.
     */
    TextureTypeEnum m_texture_type;

    /*! \var TextureFormatEnum m_texture_format;
     *  \brief The texture format.
     */
    TextureFormatEnum m_texture_format;

    /*! \var Size_ui64 m_allocated_size;
     *  \brief The size of memory handle.
     */
    Size_ui64 m_allocated_size;

    /*! \var ImageSize m_image_size;
     *  \brief The size of image size.
     */
    ImageSize m_image_size;

    /*! \var Size_ui32 m_mipmap_levels;
     *  \brief The total mipmap levels of this texture.
     */
    Size_ui32 m_mipmap_levels;

    /*! \var Size_ui32 m_array_layers;
     *  \brief The array layer size of this texture.
     */
    Size_ui32 m_array_layers;
};

//-------------------------- end of namespace Graphics --------------------------
}
//---------------------------- end of namespace SDE -----------------------------
}