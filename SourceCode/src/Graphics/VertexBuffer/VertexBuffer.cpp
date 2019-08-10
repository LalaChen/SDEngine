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

#include "GraphicsManager.h"
#include "VertexBuffer.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

//------------------- class VertexBuffer -----------------
VertexBuffer::VertexBuffer(const ObjectName &i_object_name, VertexAttribLocation i_va_location, VertexBufferFormatEnum i_format, VertexBufferMemoryTypeEnum i_memory_type)
: Object(i_object_name)
, m_location(i_va_location)
, m_memory_type(i_memory_type)
, m_format(i_format)
{
}

VertexBuffer::~VertexBuffer()
{
    if (m_identity.m_buffer_handle != SD_NULL_HANDLE && m_identity.m_memory_handle != SD_NULL_HANDLE) {
        GraphicsManager::GetRef().DeleteVertexBuffer(m_identity);
    }
}

Size_ui64 VertexBuffer::GetBufferSize() const
{
    return m_identity.m_data_size;
}

Size_ui64 VertexBuffer::GetDeviceSize() const
{
    return m_identity.m_memory_size;
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}