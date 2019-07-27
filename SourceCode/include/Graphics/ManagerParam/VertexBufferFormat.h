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

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \enum VertexBufferFormatEnum
 *  VertexBufferFormatTypeEnum is enum for defining format type about vertex buffer.
 */
enum VertexBufferFormatEnum
{
//------ unsigned byte.
    VertexBufferFormat_X8_UINT = 0, /*!< x unsigned byte.*/
    VertexBufferFormat_X8Y8_UINT = 1, /*!< xy unsigned byte.*/
    VertexBufferFormat_X8Y8Z8_UINT = 2, /*!< xyz unsigned byte.*/
    VertexBufferFormat_X8Y8Z8W8_UINT = 3, /*!< xyzw unsigned byte.*/
//------ signed byte.
    VertexBufferFormat_X8_SINT = 4, /*!< x signed byte.*/
    VertexBufferFormat_X8Y8_SINT = 5, /*!< xy signed byte.*/
    VertexBufferFormat_X8Y8Z8_SINT = 6, /*!< xyz signed byte.*/
    VertexBufferFormat_X8Y8Z8W8_SINT = 7, /*!< xyzw signed byte.*/
//------ unsigned short.
    VertexBufferFormat_X16_UINT = 8, /*!< x unsigned short.*/
    VertexBufferFormat_X16Y16_UINT = 9, /*!< xy unsigned short.*/
    VertexBufferFormat_X16Y16Z16_UINT = 10, /*!< xyz unsigned short.*/
    VertexBufferFormat_X16Y16Z16W16_UINT = 11, /*!< xyzw unsigned short.*/
//------ signed short.
    VertexBufferFormat_X16_SINT = 12, /*!< x signed short.*/
    VertexBufferFormat_X16Y16_SINT = 13, /*!< xy signed short.*/
    VertexBufferFormat_X16Y16Z16_SINT = 14, /*!< xyz signed short.*/
    VertexBufferFormat_X16Y16Z16W16_SINT = 15, /*!< xyzw signed short.*/
//------ unsigned int32.
    VertexBufferFormat_X32_UINT = 16, /*!< x signed int32.*/
    VertexBufferFormat_X32Y32_UINT = 17, /*!< xy signed int32.*/
    VertexBufferFormat_X32Y32Z32_UINT = 18, /*!< xyz signed int32.*/
    VertexBufferFormat_X32Y32Z32W32_UINT = 19, /*!< xyzw signed int32.*/
//------ signed int32
    VertexBufferFormat_X32_SINT = 20, /*!< x signed int32.*/
    VertexBufferFormat_X32Y32_SINT = 21, /*!< xy signed int32.*/
    VertexBufferFormat_X32Y32Z32_SINT = 22, /*!< xyz signed int32.*/
    VertexBufferFormat_X32Y32Z32W32_SINT = 23, /*!< xyzw signed int32.*/
//------ float
    VertexBufferFormat_X32_SFLOAT = 24, /*!< x float32.*/
    VertexBufferFormat_X32Y32_SFLOAT = 25, /*!< xy float32.*/
    VertexBufferFormat_X32Y32Z32_SFLOAT = 26, /*!< xyz float32.*/
    VertexBufferFormat_X32Y32Z32W32_SFLOAT = 27, /*!< xyzw float32.*/
//------ unsigned int64
    VertexBufferFormat_X64_UINT = 28, /*!< x signed int64.*/
    VertexBufferFormat_X64Y64_UINT = 29, /*!< xy signed int64.*/
    VertexBufferFormat_X64Y64Z64_UINT = 30, /*!< xyz signed int64.*/
    VertexBufferFormat_X64Y64Z64W64_UINT = 31, /*!< xyzw signed int64.*/
//------ signed int64
    VertexBufferFormat_X64_SINT = 32, /*!< x signed int64.*/
    VertexBufferFormat_X64Y64_SINT = 33, /*!< xy signed int64.*/
    VertexBufferFormat_X64Y64Z64_SINT = 34, /*!< xyz signed int64.*/
    VertexBufferFormat_X64Y64Z64W64_SINT = 35, /*!< xyzw signed int64.*/
//------ double
    VertexBufferFormat_X64_SFLOAT = 36, /*!< x double.*/
    VertexBufferFormat_X64Y64_SFLOAT = 37, /*!< xy double.*/
    VertexBufferFormat_X64Y64Z64_SFLOAT = 38, /*!< xyz double.*/
    VertexBufferFormat_X64Y64Z64W64_SFLOAT = 39, /*!< xyzw double.*/
//------ MaxDefineValue
    VertexBufferFormat_MAX_DEFINE_VALUE /*!< max define value for return error.*/
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}