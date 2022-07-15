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
 *  \brief     Introduce of class VertexBuffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/07
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "IndexBufferIdentity.h"
#include "CommandBuffer.h"
#include "Object.h"

using SDE::Basic::UByte;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(IndexBuffer);

/*! \class DynamicVertexBuffer
 *  In our graphic system, IndexBuffer objects are used to keep index datas.
 */
class SDENGINE_CLASS IndexBuffer : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit IndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format, MemoryTypeEnum i_memory_type);
     *  \param [in] i_object_name Name of this buffer.
     *  \param [in] i_format The format.
     *  \param [in] i_memory_type The memory type of this buffer memory.
     *  \brief The constructor of IndexBuffer Class.
     */
    explicit IndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format, MemoryTypeEnum i_memory_type);

    /*! \fn ~IndexBuffer()
	 *  \brief The destructor of IndexBuffer Class.
	 */
	virtual ~IndexBuffer();
public:
    /*! \fn void Bind(const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset);
     *  \param [in] i_cb Target began command buffer.
     *  \param [in] i_offset. Binding start address.
     *  \brief Specify binding channel for this vertex buffer and then bind.
     */
    void Bind(const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset);
public:
    /*! \fn virtual void RefreshBufferData(const void *i_data_ptr, Size_ui64 i_data_size) = 0;
     *  \param [in] i_data_ptr Data pointer.
     *  \param [in] i_data_size Data size.
     *  \brief Refresh data into buffer.
     */
    virtual void RefreshBufferData(const void *i_data_ptr, Size_ui64 i_data_size) = 0;
public:
    /*! \fn Size_ui64 GetBufferSize() const;
     *  \brief return data size of vertex buffer.
     */
    Size_ui64 GetBufferSize() const;

    /*! \fn Size_ui64 GetDeviceSize() const;
     *  \brief return allocated size of vertex buffer.
     */
    Size_ui64 GetDeviceSize() const;
protected:
    /*! \fn void CalculateIndexArraySize();
     *  \brief Calculate index array size.
     */
    void CalculateIndexArraySize();
protected:
    /*! \var IndexBufferIdentity m_identity;
     *  \brief Index buffer identity.
     */
    IndexBufferIdentity m_identity;
};

inline Size_ui64 IndexBuffer::GetBufferSize() const
{
    return m_identity.m_data_size;
}

inline Size_ui64 IndexBuffer::GetDeviceSize() const
{
    return m_identity.m_memory_size;
}

______________SD_END_GRAPHICS_NAMESPACE______________