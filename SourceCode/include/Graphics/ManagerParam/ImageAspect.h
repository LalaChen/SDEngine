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

/*! \file      ImageAspect.h
 *  \brief     Introduce of enum ImageAspectEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/01/31
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum ImageAspectEnum
 *  \brief ImageAspectEnum is used to represent image aspect.(Vulkan Only)
 */
enum ImageAspectEnum
{
    ImageAspect_ASPECT_COLOR = 0, /*!<Aspect color.*/
    ImageAspect_ASPECT_DEPTH, /*!<Aspect depth.*/
    ImageAspect_ASPECT_STENCIL, /*!<Aspec Stencil.*/
    ImageAspect_ASPECT_METADATA, /*!<Aspec Depth Metadata.*/
    ImageAspect_ASPECT_PLANE_0, /*!<Aspec Plane 0.*/
    ImageAspect_ASPECT_PLANE_1,/*!<Aspec Plane 1.*/
    ImageAspect_ASPECT_PLANE_2, /*!<Aspec Plane 2.*/
    ImageAspect_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};
______________SD_END_GRAPHICS_NAMESPACE______________