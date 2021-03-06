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

/*! \file      StencilOperator.h
 *  \brief     Introduce of class StencilOperator.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/01
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum StencilOperatorEnum
 *  \brief StencilOperator is used to represent action after operating(pass, fail, ...etc.)
 */
enum StencilOperatorEnum
{
    StencilOperator_KEEP = 0, /*!<Keep value.*/
    StencilOperator_ZERO, /*!<Clear value.*/
    StencilOperator_REPLACE, /*!<Replace by reference.*/
    StencilOperator_INCREMENT_AND_CLAMP, /*!<Increment one and clamp to maximum if it surpass maximum.*/
    StencilOperator_DECREMENT_AND_CLAMP, /*!<Decrement one and clamp to zero if it's small than zero.*/
    StencilOperator_INVERT, /*!< Make bit inverse.*/
    StencilOperator_INCREMENT_AND_WRAP, /*!< Increment one and restart to zero if it surpass maximum.*/
    StencilOperator_DECREMENT_AND_WRAP, /*!< Decrement one and restart to zero if it's small than zero.*/
    StencilOperator_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________