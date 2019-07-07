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

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

//------------------------- enum VertexBufferUsageKindEnum----------------------
/*! \enum VertexBufferUsageKindEnum
 *  The vertex buffer usage kind.
 */
enum VertexBufferUsageEnum
{
    VertexBufferUsage_VERTEX_BUFFER = 0, /*!< Use for storing vertice.*/
    VertexBufferUsage_NORMAL_BUFFER = 1, /*!< Use for storing normals.*/
    VertexBufferUsage_TEX_COORD_BUFFER = 2, /*!< Use for storing texture coordinates.*/
    VertexBufferUsage_COLOR_BUFFER = 3, /*!< Use for storing colors.*/
    VertexBufferUsage_TANGENT_BUFFER = 4, /*!< Use for storing tangents.*/
    VertexBufferUsage_BINORMAL_BUFFER = 5, /*!< Use for storing binormals.*/
    VertexBufferUsage_ELEMENT_BUFFER = 6, /*!< Use for storing elements.*/
    VertexBufferUsage_MAX_DEFINE_VALUE /*!< Bound of basic usage.*/
};

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}