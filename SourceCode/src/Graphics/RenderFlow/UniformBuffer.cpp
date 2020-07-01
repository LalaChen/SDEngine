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
#include "UniformBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

UniformBuffer::UniformBuffer(const ObjectName &i_object_name)
: UniformVariable(i_object_name)
, m_modified(false)
{
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::Initialize(const UniformBufferDescriptorWeakReferenceObject &i_desc_wref)
{
    if (m_ub_desc_wref.IsNull() == false) {
        SDLOGE("UniformBuffer[%s] is initialized.", m_object_name.c_str());
    }
    m_ub_desc_wref = i_desc_wref;
    m_identity.m_data_size = i_desc_wref.GetConstRef().GetBufferSize();
    m_buffer.resize(m_identity.m_data_size);
    m_buffer.shrink_to_fit();
    //create uniform buffer.
    GraphicsManager::GetRef().CreateUniformBuffer(m_identity);
}

bool UniformBuffer::SetBufferData(const std::vector<uint8_t> &i_data)
{
    m_buffer = i_data;
    m_modified = true;
}

void UniformBuffer::Update()
{
    if (m_modified == true) {
        void* mem_ptr = nullptr;
        UniformBufferWeakReferenceObject this_wref = GetThisWeakPtrByType<UniformBuffer>();
        GraphicsManager::GetRef().MapUniformBuffer(this_wref, mem_ptr);
        std::memcpy(mem_ptr, m_buffer.data(), m_buffer.size());
        GraphicsManager::GetRef().UnmapUniformBuffer(this_wref);
        m_modified = false;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________