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

/*! \file      StaticIndexBuffer.h
 *  \brief     Introduce of class StaticIndexBuffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/08
 *  \copyright MIT License.
 */

#pragma once

#include "IndexBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(StaticIndexBuffer);

/*! \class StaticVertexBuffer
 *  In our graphic system, StaticIndexBuffer objects are used to keep index datas in device.
 */
class SDENGINE_CLASS StaticIndexBuffer : public IndexBuffer
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit StaticIndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format, MemoryTypeEnum i_memory_type);
     *  \param [in] i_object_name Name of this buffer.
     *  \param [in] i_format The format.
     *  \brief The constructor of StaticIndexBuffer Class.
     */
    explicit StaticIndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format);

    /*! \fn ~StaticIndexBuffer()
     *  \brief The destructor of StaticIndexBuffer Class.
     */
    virtual ~StaticIndexBuffer();
public:
    /* \fn void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) override;
     * \param [in] i_data_ptr Data pointer.
     * \param [in] i_data_size Data size.
     * \brief Refresh data into buffer.
     */
    void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) override;
};

______________SD_END_GRAPHICS_NAMESPACE______________