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

/*! \file      BitmapPixelDataType.h
 *  \brief     Introduce of enum BitmapPixelDataTypeEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/22
 *  \copyright MIT License.
 */
#pragma once

//---------------------------- start of namespace SDE ------------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics ---------------------------
namespace Graphics
{

enum BitmapPixelDataTypeEnum
{
    BitmapPixelDataType_UNSIGNED_BYTE = 0,
    BitmapPixelDataType_BYTE = 1,
    BitmapPixelDataType_UNSIGNED_SHORT = 2,
    BitmapPixelDataType_SHORT = 3,
    BitmapPixelDataType_UNSIGNED_INT = 4,
    BitmapPixelDataType_INT = 5,
    BitmapPixelDataType_FLOAT = 6,
    BitmapPixelDataType_UNSIGNED_SHORT_5_6_5 = 7,
    BitmapPixelDataType_UNSIGNED_SHORT_4_4_4_4 = 8,
    BitmapPixelDataType_MAX_DEFINE_VALUE
};

//-------------------------- end of namespace Graphics ---------------------------
}
//---------------------------- end of namespace SDE ------------------------------
}