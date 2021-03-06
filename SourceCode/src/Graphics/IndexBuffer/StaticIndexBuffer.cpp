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

#include "SDEngineMacro.h"
#include "LogManager.h"
#include "GraphicsManager.h"
#include "StaticIndexBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

StaticIndexBuffer::StaticIndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format)
: IndexBuffer(i_object_name, i_format, MemoryType_STATIC)
{
}

StaticIndexBuffer::~StaticIndexBuffer()
{
}

void StaticIndexBuffer::RefreshBufferData(const void *i_data_ptr, Size_ui64 i_data_size)
{
    //1. Ckeck CompHandle is null handle or not.
    if (m_identity.m_buffer_handle != SD_NULL_HANDLE) {
        SDLOGW("Static vertex buffer had been initialized. Please refresh data after clear old one.");
        return;
    }
    //2. Create new one.
    GraphicsManager::GetRef().CreateIndexBuffer(m_identity, i_data_size);
    //3. refresh static buffer.(staging)
    if (m_identity.m_buffer_handle != SD_NULL_HANDLE && m_identity.m_memory_handle != SD_NULL_HANDLE) {
        GraphicsManager::GetRef().RefreshStaticIndexBuffer(m_identity, i_data_ptr, i_data_size);
    }
    else {
        SDLOG("Reallocate or initialize buffer failure.");
    }
    //3. calculate index array number.
    CalculateIndexArraySize();
}

______________SD_END_GRAPHICS_NAMESPACE______________