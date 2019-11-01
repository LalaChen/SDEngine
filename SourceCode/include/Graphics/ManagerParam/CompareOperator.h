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

/*! \file      CompareOperator.h
 *  \brief     Introduce of enum CompareOperator.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/20
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum CompareOperatorEnum
 *  \brief CompareOperatorEnum is enum for comparing operator.
 */
enum CompareOperatorEnum
{
    CompareOperator_NEVER = 0, /*!> Never pass compare.*/
    CompareOperator_LESS, /*!> Pass if src < dst.*/
    CompareOperator_EQUAL, /*!> Pass if src == dst.*/
    CompareOperator_LESS_OR_EQUAL, /*!> Pass if src <= dst.*/
    CompareOperator_GREATER, /*!> Pass if src > dst.*/
    CompareOperator_NOT_EQUAL, /*!> Pass if src != dst.*/
    CompareOperator_GREATER_OR_EQUAL, /*!> Pass if src >= dst.*/
    CompareOperator_ALWAYS, /*!> Pass always.*/
    CompareOperator_MAX_DEFINE_VALUE /*!> Maximum define value.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________