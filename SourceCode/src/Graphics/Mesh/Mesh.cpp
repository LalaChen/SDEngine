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
#include "Mesh.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

Mesh::Mesh(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

Mesh::~Mesh()
{
}

void Mesh::RegisterVertexBuffer(const VertexBufferUsageEnum &i_usage, const VertexBufferStrongReferenceObject &i_va_sref)
{
    if (m_vertex_attribs[i_usage].IsNull() == false) {
        m_vertex_attribs[i_usage] = nullptr;
    }
    m_vertex_attribs[i_usage] = i_va_sref;
}

void Mesh::BindVertexBuffers(const CommandBufferWeakReferenceObject &i_cmd_buf_wref)
{
    for (uint32_t va_idx = 0; va_idx < VertexBufferUsage_BUFFER_GROUP; ++va_idx) {
        if (m_vertex_attribs[va_idx].IsNull() == false) {
            m_vertex_attribs[va_idx].GetRef().BindVertexBuffer(i_cmd_buf_wref, va_idx, 0);
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________