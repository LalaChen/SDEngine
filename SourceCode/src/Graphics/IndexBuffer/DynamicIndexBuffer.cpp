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
#include "DynamicIndexBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

DynamicIndexBuffer::DynamicIndexBuffer(const ObjectName &i_object_name, IndexBufferFormatEnum i_format)
: IndexBuffer(i_object_name, i_format, MemoryType_DYNAMIC)
{
}

DynamicIndexBuffer::~DynamicIndexBuffer()
{
}

void DynamicIndexBuffer::RefreshBufferData(const void *i_data_ptr, Size_ui64 i_data_size)
{
    if (i_data_ptr == nullptr || i_data_size == 0) {
        return;
    }
    //1. Ckeck CompHandle is null handle or not.
    if (m_identity.m_buffer != SD_NULL_HANDLE) {
        //--- No, compare current buffer size with new one.
        if (m_identity.m_memory_size < i_data_size) {
            SDLOG("Dynamic buffer size is small than input datas. Delete old and allocate new one!!!");
            //----- Smaller than new one, delete old buffer.
            GraphicsManager::GetRef().DeleteIndexBuffer(m_identity);
            //----- Create new one.
            GraphicsManager::GetRef().CreateIndexBuffer(m_identity, i_data_size);
        }
    }
    else {
        //----- Create new one.
        SDLOG("Dynamic buffer is initialized first time. Allocate new one!!!");
        GraphicsManager::GetRef().CreateIndexBuffer(m_identity, i_data_size);
    }

    //2. refresh dynamic buffer.(host)
    if (m_identity.m_buffer != SD_NULL_HANDLE && m_identity.m_memory != SD_NULL_HANDLE) {
        GraphicsManager::GetRef().RefreshDynamicIndexBuffer(m_identity, i_data_ptr, i_data_size);
    }
    else {
        SDLOG("Reallocate or initialize buffer failure.");
    }
    //3. calculate index array number.
    CalculateIndexArraySize();
}


void DynamicIndexBuffer::Resize(Size_ui64 i_data_size)
{
    if (i_data_size == 0) {
        return;
    }
    //1. Ckeck CompHandle is null handle or not.
    if (m_identity.m_buffer != SD_NULL_HANDLE) {
        //--- No, compare current buffer size with new one.
        if (m_identity.m_memory_size < i_data_size) {
            SDLOG("Dynamic buffer size is small than input datas. Delete old and allocate new one!!!");
            //----- Cache old data.
            std::vector<uint8_t> tmp_data;
            tmp_data.resize(m_identity.m_memory_size);
            VoidPtr old_data = MapMemory();
            std::memcpy(tmp_data.data(), old_data, tmp_data.size());
            UnmapMemory();
            //----- Smaller than new one, delete old buffer.
            GraphicsManager::GetRef().DeleteIndexBuffer(m_identity);
            //----- Create new one.
            GraphicsManager::GetRef().CreateIndexBuffer(m_identity, i_data_size);
            //----- Copy old data to new buffer.
            RefreshBufferData(tmp_data.data(), tmp_data.size());
        }
    }
    else {
        //----- Create new one.
        SDLOG("Dynamic buffer is initialized first time. Allocate new one!!!");
        GraphicsManager::GetRef().CreateIndexBuffer(m_identity, i_data_size);
        CalculateIndexArraySize();
    }
}

VoidPtr DynamicIndexBuffer::MapMemory()
{
    VoidPtr local_ptr = nullptr;
    GraphicsManager::GetRef().MapIndexBuffer(m_identity, local_ptr);
    return local_ptr;
}

void DynamicIndexBuffer::UnmapMemory()
{
    GraphicsManager::GetRef().UnmapIndexBuffer(m_identity);
}

______________SD_END_GRAPHICS_NAMESPACE______________