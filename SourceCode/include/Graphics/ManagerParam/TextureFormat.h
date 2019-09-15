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

/*! \file      TextureFormat.h
 *  \brief     Introduce of enum TextureFormat.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/30
 *  \copyright MIT License.
 */

#pragma once

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum TextureFormatEnum
 *  TextureFormatEnum is enum for defining format type about texture format.
 */
enum TextureFormatEnum
{
    //Byte
    //--- R
    TextureFormat_R8_UNORM = 0, /*!< read value to [0,1]. data is byte. channel number is 1.*/ 
    TextureFormat_R8_SNORM, /*!< read value to [-1,1]. data is byte. channel number is 1.*/ 
    TextureFormat_R8_UINT, /*!< read value to [0,255]. data is byte. channel number is 1.*/ 
    TextureFormat_R8_SINT, /*!< read value to [-128,127]. data is byte. channel number is 1.*/
    //--- RG
    TextureFormat_R8G8_UNORM, /*!< read value to [0,1]. data is byte. channel number is 2.*/
    TextureFormat_R8G8_SNORM, /*!< read value to [-1,1]. data is byte. channel number is 2.*/
    TextureFormat_R8G8_UINT, /*!< read value to [0,255]. data is byte. channel number is 2.*/
    TextureFormat_R8G8_SINT, /*!< read value to [-128,127]. data is byte. channel number is 2.*/
    //--- RGBA
    TextureFormat_R8G8B8A8_UNORM, /*!< read value to [0,1]. data is byte. channel number is 4.*/
    TextureFormat_R8G8B8A8_SNORM, /*!< read value to [-1,1]. data is byte. channel number is 4.*/
    TextureFormat_R8G8B8A8_UINT, /*!< read value to [0,255]. data is byte. channel number is 4.*/
    TextureFormat_R8G8B8A8_SINT, /*!< read value to [-128,127]. data is byte. channel number is 4.*/
    //Short
    //--- R
    TextureFormat_R16_UNORM, /*!< read value to [0,1]. data is short. channel number is 1.*/
    TextureFormat_R16_SNORM, /*!< read value to [-1,1]. data is short. channel number is 1.*/
    TextureFormat_R16_UINT, /*!< read value to [0,65535]. data is short. channel number is 1.*/
    TextureFormat_R16_SINT, /*!< read value to [-32768,32767]. data is short. channel number is 1.*/
    //--- RG
    TextureFormat_R16G16_UNORM, /*!< read value to [0,1]. data is short. channel number is 2.*/
    TextureFormat_R16G16_SNORM, /*!< read value to [-1,1]. data is short. channel number is 2.*/
    TextureFormat_R16G16_UINT, /*!< read value to [0,65535]. data is short. channel number is 2.*/
    TextureFormat_R16G16_SINT, /*!< read value to [-32768,32767]. data is short. channel number is 2.*/
    //--- RGBA
    TextureFormat_R16G16B16A16_UNORM, /*!< read value to [0,1]. data is short. channel number is 4.*/
    TextureFormat_R16G16B16A16_SNORM, /*!< read value to [-1,1]. data is short. channel number is 4.*/
    TextureFormat_R16G16B16A16_UINT, /*!< read value to [0,65535]. data is short. channel number is 4.*/
    TextureFormat_R16G16B16A16_SINT, /*!< read value to [-32768,32767]. data is short. channel number is 4.*/
    //Int
    //--- R
    TextureFormat_R32_UINT, /*!< read value to [0,65535]. data is 4-byte. channel number is 1.*/
    TextureFormat_R32_SINT, /*!< read value to [-32768,32767]. data is 4-byte. channel number is 1.*/
    //--- RG
    TextureFormat_R32G32_UINT, /*!< read value to [0,MAX_UINT]. data is 4-byte. channel number is 2.*/
    TextureFormat_R32G32_SINT, /*!< read value to [-32768,32767]. data is 4-byte. channel number is 2.*/
    //--- RGBA
    TextureFormat_R32G32B32A32_UINT, /*!< read value to [0,MAX_UINT]. data is 4-byte. channel number is 4.*/
    TextureFormat_R32G32B32A32_SINT, /*!< read value to [MIN_INT,MAX_INT]. data is 4-byte. channel number is 4.*/
    //Float
    //--- R
    TextureFormat_R32_SFLOAT, /*!< read value by float. channel number is 1.*/
    //--- RG
    TextureFormat_R32G32_SFLOAT, /*!< read value by float. channel number is 2.*/
    //--- RGBA
    TextureFormat_R32G32B32A32_SFLOAT, /*!< read value by float. channel number is 4.*/
    //Special
    //--- DEPTH and STENCIL
    TextureFormat_D16_UNORM, /*!< depth texture.*/
    TextureFormat_D32_SFLOAT, /*!< depth texture.*/
    TextureFormat_D24_UNORM_S8_UINT, /*!< depth and stencil texture.*/
    TextureFormat_D32_SFLOAT_S8_UINT, /*!< depth and stencil texture.*/
    //--- other
    TextureFormat_RGBA_4444, /*!< 4 bits RGBA texture.*/
    TextureFormat_RGB565, /*!< 4 bit-5 bit-4 bit RGB texture.*/
    TextureFormat_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________