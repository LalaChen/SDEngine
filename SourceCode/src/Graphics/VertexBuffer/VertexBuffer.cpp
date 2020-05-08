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

_____________SD_START_GRAPHICS_NAMESPACE_____________

//------------------- class VertexBuffer -----------------
VertexBuffer::VertexBuffer(const ObjectName &i_object_name, uint32_t i_va_location, VertexBufferFormatEnum i_format, MemoryTypeEnum i_memory_type)
: Object(i_object_name)
{
}

VertexBuffer::~VertexBuffer()
{
    if (m_identity.m_buffer_handle != SD_NULL_HANDLE && m_identity.m_memory_handle != SD_NULL_HANDLE) {
        GraphicsManager::GetRef().DeleteVertexBuffer(m_identity);
    }
}

void VertexBuffer::Bind(const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_binding_id, Size_ui64 i_offset)
{
    GraphicsManager::GetRef().BindVertexBuffer(m_identity, i_cb_wref, i_binding_id, i_offset);
}

______________SD_END_GRAPHICS_NAMESPACE______________