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

/*! \file      UniformBufferIdentity.h
 *  \brief     Introduce of class UniformBufferIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/06/26
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class UniformBufferIdentity
 *  \brief Keep all graphics handle or ID about uniform buffer in this structure.
 */
class SDENGINE_CLASS UniformBufferIdentity
{
public:
    /*! \fn explicit UniformBufferIdentity();
     *  \brief The constructor of UniformBufferIdentity Class.
     */
    UniformBufferIdentity();

    /*! \fn explicit ~UniformBufferIdentity();
     *  \brief The destructor of UniformBufferIdentity Class.
     */
    ~UniformBufferIdentity();
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
};

______________SD_END_GRAPHICS_NAMESPACE______________