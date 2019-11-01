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

/*! \file      VertexInputRate.h
 *  \brief     Introduce of enum VertexInputRate.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/26
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum VertexInputRateEnum
 *  \brief VertexInputRate is used to describe what kind of index we use for getting data from
 *  target vertex attribute. It also means that this vertex attribute record datas for vertex
 *  or instance. If this VertexInputRate of this vertex buffer is VERTEX, we will get new data
 *  when vertex shader is executing for drawing each primitive in this drawcall. Otherwise,
 *  we will get new data when vertex shader is executing for drawing each instance in this drawcall.
 */
enum VertexInputRateEnum
{
    VertexInputRate_VERTEX = 0, /*!< Vertex input for vertex.*/
    VertexInputRate_INSTANCE, /*!< Vertex input for instance.*/
    VertexInputRate_MAX_DEFINE_VALUE /*!< Bound of enum.*/
};

______________SD_END_GRAPHICS_NAMESPACE______________