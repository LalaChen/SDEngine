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

/*! \file      VertexBuffer.h
 *  \brief     Introduce of class VertexBufferIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/03
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "VBufferFormat.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class VertexBufferIdentity
 *  Keep all graphics handle or ID in this struct.
 */
class SDENGINE_CLASS VertexBufferIdentity
{
public:
    /*! \fn explicit VertexBufferIdentity();
     *  \brief The constructor of VertexBufferIdentity Class.
     */
    VertexBufferIdentity();

    /*! \fn explicit ~VertexBufferIdentity();
     *  \brief The destructor of VertexBufferIdentity Class.
     */
    ~VertexBufferIdentity();
public:
    /*! \var BufferHandle m_buffer_handle;
     *  \brief The buffer handle. It is valid while the value is not equal 0.
     */
    BufferHandle m_buffer_handle;

    /*! \var DeviceMemoryHandle m_memory_handle;
     *  \brief The memory handle. This value is nullptr in opengl system.
     */
    DeviceMemoryHandle m_memory_handle;
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}