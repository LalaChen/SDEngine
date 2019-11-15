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


/*! \file      LogicOperator.h
 *  \brief     Introduce of enum LogicOperator.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum LogicOperatorEnum
 *  \brief Logic op is used to decide with bitwise operator we want to use.
 */
enum LogicOperatorEnum
{
    LogicOperator_CLEAR = 0, /*!< clear to 0.*/
    LogicOperator_AND, /*!< S and D.*/
    LogicOperator_AND_REVERSE, /*!< S and not D.*/
    LogicOperator_COPY,  /*!< S.*/
    LogicOperator_AND_INVERTED, /*!< not S and D.*/
    LogicOperator_XOR, /*!< S xor D.*/
    LogicOperator_OR, /*!< S or D.*/
    LogicOperator_NOR, /*!< not (S or D).*/
    LogicOperator_EQUIVALENT, /*!< not (S xor D). */
    LogicOperator_INVERT, /*!< not D. */
    LogicOperator_OR_REVERSE, /*!< s or not D. */
    LogicOperator_COPY_INVERTED,  /*!< not S. */
    LogicOperator_OR_INVERTED, /*!< not S or D. */
    LogicOperator_NAND, /*!< ~(S and D)*/
    LogicOperator_SET, /*!< clear to 1.*/
    LogicOperator_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________