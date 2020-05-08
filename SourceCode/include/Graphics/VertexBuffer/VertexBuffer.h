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
 *  \date      2019/06/30
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "MemoryType.h"
#include "VertexBufferIdentity.h"
#include "CommandBuffer.h"
#include "Object.h"

using SDE::Basic::UByte;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(VertexBuffer);

/*! \class VertexBuffer
 *  In our graphic system, VertexBuffer objects are used to keep vertex datas.
 */
class SDENGINE_CLASS VertexBuffer : public Object
{
public:
    friend class GraphicsManager;
public:
	/*! \fn explicit VertexBuffer(const ObjectName &i_object_name, uint32_t i_va_location, VertexBufferFormatEnum i_format);
	 *  \param [in] i_object_name Name of this buffer.
	 *  \param [in] i_va_location Attribute location of this buffer.
	 *  \param [in] i_value_type The value type of this buffer.
     *  \param [in] i_memory_type The memory type of this buffer memory.
	 *  \brief The constructor of VertexBuffer Class.
	 */
	explicit VertexBuffer(const ObjectName &i_object_name, uint32_t i_va_location, VertexBufferFormatEnum i_format, MemoryTypeEnum i_memory_type);

	/*! \fn ~VertexBuffer()
	 *  \brief The destructor of VertexBuffer Class.
	 */
	virtual ~VertexBuffer();
public:
    /*! \fn Size_ui64 GetBufferSize() const;
     *  \brief return data size of vertex buffer.
     */
    Size_ui64 GetBufferSize() const;

    /*! \fn Size_ui64 GetDeviceSize() const;
     *  \brief return allocated size of vertex buffer.
     */
    Size_ui64 GetDeviceSize() const;

    /*! \fn uint32_t GetAttribLocation() const;
     *  \brief return attribute location of vertex buffer.
     */
    uint32_t GetAttribLocation() const;

    /*! \fn MemoryTypeEnum GetMemoryType() const;
     *  \brief return memory type of vertex buffer.
     */
    MemoryTypeEnum GetMemoryType() const;

    /*! \fn VertexBufferFormatEnum GetFormat() const;
     *  \brief return buffer type of vertex buffer.
     */
    VertexBufferFormatEnum GetFormat() const;
public:
    /*! \fn virtual void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) = 0;
     *  \param [in] i_data_ptr Data pointer.
     *  \param [in] i_data_size Data size.
     *  \brief Refresh data into buffer.
     */
    virtual void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) = 0;
public:
    /*! \fn void Bind(const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_binding_id, Size_ui64 i_offset); 
     *  \param [in] i_cb_wref Target began command buffer.
     *  \param [in] i_binding_id Binding channel we want.
     *  \param [in] i_offset. Binding start address.
     *  \brief Specify binding channel for this vertex buffer and then bind.
     */
    void Bind(const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_binding_id, Size_ui64 i_offset);
protected:
	/*! \var VertexBufferIdentity m_identity;
     *  \brief The identity. We keep all handles or ids from graphics API.
	 */
    VertexBufferIdentity m_identity;
};

inline Size_ui64 VertexBuffer::GetBufferSize() const
{
    return m_identity.m_data_size;
}

inline Size_ui64 VertexBuffer::GetDeviceSize() const
{
    return m_identity.m_memory_size;
}

inline uint32_t VertexBuffer::GetAttribLocation() const
{
    return m_identity.m_location;
}

inline MemoryTypeEnum VertexBuffer::GetMemoryType() const
{
    return m_identity.m_memory_type;
}

inline VertexBufferFormatEnum VertexBuffer::GetFormat() const
{
    return m_identity.m_format;
}

______________SD_END_GRAPHICS_NAMESPACE______________