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

/*! \file      PolygonMode.h
 *  \brief     Introduce of enum PolygonModeEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/27
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum PolygonModeEnum
 *  \brief PolygonMode is used to decide how to we fill polygon when we draw. Fill mode
 *  is used to tell gpu that we want to fill polygon. Wireless mode is used to
 *  tell gpu that we just want to draw outline of polygon. Finally, Point mode
 *  is used to tell gpu that we only want to draw vertices of polygon.
 */
enum PolygonModeEnum
{
    PolygonMode_FILL = 0, /*!< Fill polygon.*/
    PolygonMode_WIRELESS, /*!< Only draw outline.*/
    PolygonMode_POINT, /*!< Only draw vertices.*/
    PolygonMode_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________