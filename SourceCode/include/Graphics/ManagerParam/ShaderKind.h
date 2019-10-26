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

/*! \file      ShaderKind.h
 *  \brief     Introduce of enum ShaderKind.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/26
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum ShaderKindEnum
 *  Enum ShaderKindEnum is used to list all kind of shader.
 */
enum ShaderKindEnum
{
    ShaderKind_VERTEX = 0, /*!< Vertex shader.*/
    ShaderKind_TESS_CTRL, /*!< Tesselation controller shader.*/
    ShaderKind_TESS_EVA, /*!< Tesselation evaluation shader.*/
    ShdaerKind_GEOMETRY, /*!< Geometry shader.*/
    ShaderKind_FRAGMENT, /*!< Fragment shader.*/
    ShaderKind_COMPUTE, /*!< Compute shader.*/
    ShaderKind_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________
