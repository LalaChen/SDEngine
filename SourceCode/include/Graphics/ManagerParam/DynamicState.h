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


/*! \file      DynamicState.h
 *  \brief     Introduce of enum DynamicStateEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum DynamicStateEnum
 *  \brief Dynamic state is used to decide which state can be set by cmd dynamically.
 */
enum DynamicStateEnum
{
    DynamicState_VIEWPORT = 0, /*!< Allow set viewport region dynamically.*/
    DynamicState_SCISSOR, /*!< Allow set scissor dynamically.*/
    DynamicState_LINE_WIDTH, /*!< Allow set line width dynamically.*/
    DynamicState_DEPTH_BIAS, /*!< Allow set depth bias dynamically.*/
    DynamicState_BLEND_CONSTANTS, /*!< Allow set blend constant dynamically.*/
    DynamicState_DEPTH_BOUNDS, /*!< Allow set depth bounds dynamically.*/
    DynamicState_STENCIL_COMPARE_MASK, /*!< Allow set stencil compare mask dynamically.*/
    DynamicState_STENCIL_WRITE_MASK, /*!< Allow set stencil write dynamically.*/
    DynamicState_STENCIL_REFERENCE, /*!< Allow set stencil reference dynamically.*/
    DynamicState_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________