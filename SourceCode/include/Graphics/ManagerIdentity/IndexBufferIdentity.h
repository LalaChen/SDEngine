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

/*! \file      IndexBufferIdentity.h
 *  \brief     Introduce of class IndexBufferIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/08
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "MemoryType.h"
#include "IndexBufferFormat.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class IndexBufferIdentity
 *  \brief Keep all graphics handle or ID in this struct.
 */
class SDENGINE_CLASS IndexBufferIdentity
{
public:
    /*! \fn explicit IndexBufferIdentity();
     *  \brief The constructor of IndexBufferIdentity Class.
     */
    IndexBufferIdentity();

    /*! \fn explicit ~IndexBufferIdentity();
     *  \brief The destructor of IndexBufferIdentity Class.
     */
    ~IndexBufferIdentity();
public:
    /*! \var CompHandle m_buffer_handle;
     *  \brief The buffer handle. It is valid while the value is not equal 0.
     */
    CompHandle m_buffer_handle;

    /*! \var CompHandle m_memory_handle;
     *  \brief The memory handle. This value is nullptr in opengl system.
     */
    CompHandle m_memory_handle;

    /*! \var Size_ui64 m_data_size;
     *  \brief current buffer data size.
     */
    Size_ui64 m_data_size;

    /*! \var Size_ui64 m_memory_size;
     *  \brief current allocated memory size.
     */
    Size_ui64 m_memory_size;

    /*! \var IndexBufferFormatEnum m_format;
     *  \brief Current buffer format. (ushort or uint32) Default is ushort.
     */
    IndexBufferFormatEnum m_format;

    /*! \var MemoryTypeEnum m_memory_type;
     *  \brief Keep memory type.
     */
    MemoryTypeEnum m_memory_type;
};

______________SD_END_GRAPHICS_NAMESPACE______________