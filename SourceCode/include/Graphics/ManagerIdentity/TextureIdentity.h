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

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ImageParam.h"
#include "ManagerParam.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

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

    /*! \var CompHandle m_view_handle;
     *  \brief The view handle. We bind view to shader or framebufer in Vulkan system. 
     *         This value is nullptr in opengl system.
     */
    CompHandle m_view_handle;

    /*! \var TextureTypeEnum m_texture_type;
     *  \brief The texture type.
     */
    TextureTypeEnum m_texture_type;

    /*! \var TextureViewTypeEnum m_texture_view_type;
     *  \brief The texture view type.
     */
    TextureViewTypeEnum m_texture_view_type;

    /*! \var TextureFormatEnum m_texture_format;
     *  \brief The texture format.
     */
    TextureFormatEnum m_texture_format;

    /*! \var ImageUsageEnum m_image_usage;
     *  \brief The image usage.
     */
    std::vector<ImageUsageEnum> m_image_usages;

    /*! \var ImageLayout m_init_layout;
     *  \brief The initial layout.
     */
    ImageLayoutEnum m_init_layout;

    /*! \var ImageAspectEnum m_aspect;
     *  \brief The image usage.
     */
    ImageAspectEnum m_aspect;

    /*! \var ImageTiling m_tiling;
     *  \brief The image usage.
     */
    ImageTilingEnum m_tiling;

    /*! \var SampleCountEnum m_sample_count;
     *  \brief The sample count.
     */
    SampleCountEnum m_sample_count;

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

______________SD_END_GRAPHICS_NAMESPACE______________