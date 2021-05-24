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

#include "LogManager.h"
#include "GraphicsManager.h"
#include "IndexBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

IndexBuffer::IndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format, MemoryTypeEnum i_memory_type)
: Object(i_object_name)
{
    m_identity.m_memory_type = i_memory_type;
    m_identity.m_format = i_format;
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Bind(const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset)
{
    GraphicsManager::GetRef().BindIndexBuffer(m_identity, i_cb, i_offset);
}

void IndexBuffer::CalculateIndexArraySize()
{
    if (m_identity.m_format == IndexBufferFormat_X16_UINT) {
        m_identity.m_index_array_size = static_cast<uint32_t>(m_identity.m_data_size / 2);
    }
    else if (m_identity.m_format == IndexBufferFormat_X32_UINT) {
        m_identity.m_index_array_size = static_cast<uint32_t>(m_identity.m_data_size / 4);
    }
    else {
        SDLOGE("No corrent format of index buffer.");
        m_identity.m_index_array_size = 0;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________
