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
#include "VertexBufferMemoryType.h"
#include "VertexBufferIdentity.h"
#include "Object.h"

using SDE::Basic::UByte;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;

//------------------------------ start of namespace SDE -------------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class VertexBuffer
 *  In our graphic system, VertexBuffer objects are used to keep vertex datas.
 */
SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(VertexBuffer);

class SDENGINE_API VertexBuffer : public Object
{
public:
	/*! \type  typedef std::vector< UByte > VertexDatas;
	 *  \brief type of ubyte-buffer.
	 */
	typedef std::vector<UByte> VertexDatas;
public:
	/*! \fn explicit VertexBuffer(const ObjectName &i_object_name, VertexAttribLocation i_va_location, VertexBufferFormatEnum i_format);
	 *  \param [in] i_object_name Name of this buffer.
	 *  \param [in] i_va_location Attribute location of this buffer.
	 *  \param [in] i_value_type The value type of this buffer.
     *  \param [in] i_memory_type The memory type of this buffer memory.
	 *  \brief The constructor of VertexBuffer Class.
	 */
	explicit VertexBuffer(const ObjectName &i_object_name, VertexAttribLocation i_va_location, VertexBufferFormatEnum i_format, VertexBufferMemoryTypeEnum i_memory_type);

	/*! \fn ~VertexBuffer()
	 *  \brief The destructor of VertexBuffer Class.
	 */
	virtual ~VertexBuffer();
public:
    /* \fn virtual void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) = 0;
     * \param [in] i_data_ptr Data pointer.
     * \param [in] i_data_size Data size.
     * \brief Refresh data into buffer.
     */
    virtual void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) = 0;
protected:
	/*! \var VertexBufferIdentity m_identity;
     *  \brief The identity. We keep all handles or ids from graphics API.
	 */
    VertexBufferIdentity m_identity;
protected:
	/*! \var VertexAttribLocation m_location;
	 *  \brief Record the input location of this buffer.
     */
	SD_DECLARE_ATTRIBUTE_VAR_GET(VertexAttribLocation, m_location, AttribLocation);

    /*! \var VertexBufferKindEnum m_memory_type;
     *  \brief Keep memory type.
     */
    SD_DECLARE_ATTRIBUTE_VAR_GET(VertexBufferMemoryTypeEnum, m_memory_type, MemoryType);

    /*! \var Size_ui64 m_size;
     *  \brief current buffer size.
     */
    SD_DECLARE_ATTRIBUTE_VAR_GET(Size_ui64, m_size, Size);

    /*! \var ValueTypeEnum m_format;
     *  \brief Record the buffer format.
     */
    SD_DECLARE_ATTRIBUTE_VAR_GET(VertexBufferFormatEnum, m_format, Format);
};

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}