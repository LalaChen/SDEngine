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

/*! \file      VertexBufferFormat.h
 *  \brief     Introduce of enum VertexBufferFormat.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/06/30
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum VertexBufferFormatEnum
 *  VertexBufferFormatTypeEnum is enum for defining format type about vertex buffer.
 */
enum VertexBufferFormatEnum
{
//------ unsigned byte.
    VertexBufferFormat_X8_UINT = 0, /*!< x unsigned byte.*/
    VertexBufferFormat_X8Y8_UINT, /*!< xy unsigned byte.*/
    VertexBufferFormat_X8Y8Z8_UINT, /*!< xyz unsigned byte.*/
    VertexBufferFormat_X8Y8Z8W8_UINT, /*!< xyzw unsigned byte.*/
//------ signed byte.
    VertexBufferFormat_X8_SINT, /*!< x signed byte.*/
    VertexBufferFormat_X8Y8_SINT, /*!< xy signed byte.*/
    VertexBufferFormat_X8Y8Z8_SINT, /*!< xyz signed byte.*/
    VertexBufferFormat_X8Y8Z8W8_SINT, /*!< xyzw signed byte.*/
//------ unsigned short.
    VertexBufferFormat_X16_UINT, /*!< x unsigned short.*/
    VertexBufferFormat_X16Y16_UINT, /*!< xy unsigned short.*/
    VertexBufferFormat_X16Y16Z16_UINT, /*!< xyz unsigned short.*/
    VertexBufferFormat_X16Y16Z16W16_UINT, /*!< xyzw unsigned short.*/
//------ signed short.
    VertexBufferFormat_X16_SINT, /*!< x signed short.*/
    VertexBufferFormat_X16Y16_SINT, /*!< xy signed short.*/
    VertexBufferFormat_X16Y16Z16_SINT, /*!< xyz signed short.*/
    VertexBufferFormat_X16Y16Z16W16_SINT, /*!< xyzw signed short.*/
//------ unsigned int32.
    VertexBufferFormat_X32_UINT, /*!< x signed int32.*/
    VertexBufferFormat_X32Y32_UINT, /*!< xy signed int32.*/
    VertexBufferFormat_X32Y32Z32_UINT, /*!< xyz signed int32.*/
    VertexBufferFormat_X32Y32Z32W32_UINT, /*!< xyzw signed int32.*/
//------ signed int32
    VertexBufferFormat_X32_SINT, /*!< x signed int32.*/
    VertexBufferFormat_X32Y32_SINT, /*!< xy signed int32.*/
    VertexBufferFormat_X32Y32Z32_SINT, /*!< xyz signed int32.*/
    VertexBufferFormat_X32Y32Z32W32_SINT, /*!< xyzw signed int32.*/
//------ float
    VertexBufferFormat_X32_SFLOAT, /*!< x float32.*/
    VertexBufferFormat_X32Y32_SFLOAT, /*!< xy float32.*/
    VertexBufferFormat_X32Y32Z32_SFLOAT, /*!< xyz float32.*/
    VertexBufferFormat_X32Y32Z32W32_SFLOAT, /*!< xyzw float32.*/
//------ unsigned int64
    VertexBufferFormat_X64_UINT, /*!< x signed int64.*/
    VertexBufferFormat_X64Y64_UINT, /*!< xy signed int64.*/
    VertexBufferFormat_X64Y64Z64_UINT, /*!< xyz signed int64.*/
    VertexBufferFormat_X64Y64Z64W64_UINT, /*!< xyzw signed int64.*/
//------ signed int64
    VertexBufferFormat_X64_SINT, /*!< x signed int64.*/
    VertexBufferFormat_X64Y64_SINT, /*!< xy signed int64.*/
    VertexBufferFormat_X64Y64Z64_SINT, /*!< xyz signed int64.*/
    VertexBufferFormat_X64Y64Z64W64_SINT, /*!< xyzw signed int64.*/
//------ double
    VertexBufferFormat_X64_SFLOAT, /*!< x double.*/
    VertexBufferFormat_X64Y64_SFLOAT, /*!< xy double.*/
    VertexBufferFormat_X64Y64Z64_SFLOAT, /*!< xyz double.*/
    VertexBufferFormat_X64Y64Z64W64_SFLOAT, /*!< xyzw double.*/
//------ MaxDefineValue
    VertexBufferFormat_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________