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

/*! \file      VertexBufferMemoryType.h
 *  \brief     Introduce of class VertexBufferMemoryTypeEnum.
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

//------------------------- enum VertexBufferMemoryTypeEnum----------------------
/*! \enum VertexBufferMemoryTypeEnum
 *  The memory type of this buffer.
 *  The dynamic memory means the content in this buffer will be modified in future using.
 *  The static memory means the content is immutable after initializing.
 */
enum VertexBufferMemoryTypeEnum
{
    VertexBufferMemoryType_DYNAMIC = 0, /*!< Dynamic buffer.*/
    VertexBufferMemoryType_STATIC = 1, /*!< Static buffer.*/
    VertexBufferMemoryType_MAX_DEFINE_VALUE /*!< Use for return error.*/
};


//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}