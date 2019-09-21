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

/*! \file      VertexBufferUsage.h
 *  \brief     Introduce of class VertexBufferUsageEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/05
 *  \copyright MIT License.
 */

#pragma once

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \enum VertexBufferUsageKindEnum
 *  The vertex buffer usage kind.
 */
enum VertexBufferUsageEnum
{
    VertexBufferUsage_VERTEX_BUFFER = 0, /*!< Use for storing vertice.(vec3)*/
    VertexBufferUsage_NORMAL_BUFFER, /*!< Use for storing normals.(vec3)*/
    VertexBufferUsage_TEX_COORD_BUFFER, /*!< Use for storing texture coordinates.(vec3)*/
    VertexBufferUsage_TANGENT_BUFFER, /*!< Use for storing tangents.(vec3)*/
    VertexBufferUsage_BINORMAL_BUFFER , /*!< Use for storing binormals.(vec3)*/
    VertexBufferUsage_COLOR_BUFFER, /*!< Use for storing colors.(Color4f)*/
    VertexBufferUsage_ELEMENT_BUFFER, /*!< Use for storing elements.(uint32_t)*/
    VertexBufferUsage_MAX_DEFINE_VALUE, /*!< Bound of enum.*/
    VertexBufferUsage_BUFFER_GROUP = VertexBufferUsage_BINORMAL_BUFFER + 1
};

______________SD_END_GRAPHICS_NAMESPACE______________