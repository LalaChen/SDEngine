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

#include "VertexAttribDescription.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VertexAttribBindingDesc::VertexAttribBindingDesc()
: m_binding_ID(0)
, m_stride(0)
, m_input_rate(VertexInputRate_MAX_DEFINE_VALUE)
{
}

VertexAttribBindingDesc::~VertexAttribBindingDesc()
{
}

VertexAttribLocationDesc::VertexAttribLocationDesc()
: m_binding_ID(0)
, m_location(0)
, m_format(VertexBufferFormat_MAX_DEFINE_VALUE)
, m_offset(0)
{
}

VertexAttribLocationDesc::~VertexAttribLocationDesc()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________