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

/*! \file      VertexBufferIdentity.h
 *  \brief     Introduce of class VertexBufferIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/03
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsElementIdentity.h"
#include "MemoryType.h"
#include "VertexBufferFormat.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class VertexBufferIdentity
 *  \brief Keep all graphics handle or ID about vertex buffer in this structure.
 */
class SDENGINE_CLASS VertexBufferIdentity : public GraphicsElementIdentity
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
    /*! \var CompHandle m_buffer;
     *  \brief The buffer handle. It is valid while the value is not equal 0.
     */
    CompHandle m_handle;

    /*! \var CompHandle m_device;
     *  \brief devices handle. (Original Context in OpenGL, VkDevice in Vulkan)
     */
    CompHandle m_device;

    /*! \var CompHandle m_memory;
     *  \brief The memory handle. This value is nullptr in opengl system.
     */
    CompHandle m_memory;

    /*! \var Size_ui64 m_data_size;
     *  \brief current buffer data size.
     */
    Size_ui64 m_data_size;

    /*! \var Size_ui64 m_memory_size;
     *  \brief current allocated memory size.
     */
    Size_ui64 m_memory_size;

    /*! \var VertexBufferFormatEnum m_format;
     *  \brief Current buffer format.
     */
    VertexBufferFormatEnum m_format;

    /*! \var MemoryTypeEnum m_memory_type;
     *  \brief Keep memory type.
     */
    MemoryTypeEnum m_memory_type;

    /*! \var uint32_t m_location;
     *  \brief Record the input location of this buffer.
     */
    uint32_t m_location;
};

______________SD_END_GRAPHICS_NAMESPACE______________