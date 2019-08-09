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
public://Sampler Part.
    /*! \var CompHandle m_sampler;
     *  \brief Tell us how to sample this texture.
     */
    CompHandle m_sampler;

    /*! \var TextureFilterTypeEnum m_texture_mag_filter_type;
     *  \brief The texture mag filter type. Default is nearest.
     */
    TextureFilterTypeEnum m_texture_mag_filter_type;

    /*! \var TextureFilterTypeEnum m_texture_min_filter_type;
     *  \brief The texture mag filter type.
     */
    TextureFilterTypeEnum m_texture_min_filter_type;

    /*! \var TextureMipmapModeEnum m_texture_mipmap_mode;
     *  \brief The texture mipmap mode.
     */
    TextureMipmapModeEnum m_texture_mipmap_mode;

    /*! \var TextureWrapModeEnum m_texture_wrap_mode_s;
     *  \brief The texture wrap type about coordinate s axis.
     */
    TextureWrapModeEnum m_texture_wrap_mode_s;

    /*! \var TextureWrapModeEnum m_texture_wrap_mode_t;
     *  \brief The texture wrap type about coordinate t axis.
     */
    TextureWrapModeEnum m_texture_wrap_mode_t;

    /*! \var TextureWrapModeEnum m_texture_wrap_mode_r;
     *  \brief The texture wrap type about coordinate r(z) axis.
     */
    TextureWrapModeEnum m_texture_wrap_mode_r;

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

//-------------------------- end of namespace Graphics --------------------------
}
//---------------------------- end of namespace SDE -----------------------------
}