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

/*! \file      TextureType.h
 *  \brief     Introduce of enum TextureTypeEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/08/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum TextureTypeEnum
 *  TextureTypeEnum is enum for defining texture type.
 */
enum TextureTypeEnum
{
    TextureType_TEXTURE_1D = 0, /*!< One dimension texture. It can be used to make lookuptable.*/
    TextureType_TEXTURE_2D, /*!< Two dimension texture. It can be used to make general texture, color or depth buffer, ...etc.*/
    TextureType_TEXTURE_2D_ARRAY, /*!< Two dimension texture array.*/
    TextureType_TEXTURE_CUBE_MAP, /*!< Cube map. It will be used to simulate reflection of an object.*/
    TextureType_TEXTURE_3D, /*!< Three dimension texture. It will be used to make volume rendering.*/
    TextureType_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________