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

#if defined(_WIN32) || defined(_WIN64)

#include "OpenGL4Manager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

//----------- Vertex Buffer Function ------------
void OpenGL4Manager::CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size)
{
}

void OpenGL4Manager::RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
}

void OpenGL4Manager::RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
}

void OpenGL4Manager::DeleteVertexBuffer(VertexBufferIdentity &io_identity)
{
}

void OpenGL4Manager::MapVertexBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle)
{
}

void OpenGL4Manager::UnmapVertexBuffer(const VertexBufferIdentity &i_identity)
{
}

void OpenGL4Manager::BindVertexBuffer(const VertexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_binding_id, Size_ui64 i_offset)
{
}

void OpenGL4Manager::BindIndexBuffer(const IndexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref, Size_ui64 i_offset)
{
}

//----------- Index Buffer Function ------------
void OpenGL4Manager::CreateIndexBuffer(IndexBufferIdentity &io_identity, Size_ui64 i_data_size)
{
}

void OpenGL4Manager::RefreshStaticIndexBuffer(const IndexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
}

void OpenGL4Manager::RefreshDynamicIndexBuffer(const IndexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size)
{
}

void OpenGL4Manager::DeleteIndexBuffer(IndexBufferIdentity &io_identity)
{
}

void OpenGL4Manager::MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr &io_buffer_handle)
{
}

void OpenGL4Manager::UnmapIndexBuffer(const IndexBufferIdentity &i_identity)
{
}

______________SD_END_GRAPHICS_NAMESPACE______________

#endif