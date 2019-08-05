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

#include "TextureIdentity.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics -------------------------
namespace Graphics
{

TextureIdentity::TextureIdentity()
: m_image_handle(SD_NULL_HANDLE)
, m_memory_handle(SD_NULL_HANDLE)
, m_texture_type(TextureType_MAX_DEFINE_VALUE)
, m_texture_format(TextureFormat_MAX_DEFINE_VALUE)
, m_texture_mag_filter_type(TextureFilterType_NEAREST)
, m_texture_min_filter_type(TextureFilterType_NEAREST)
, m_texture_wrap_mode_s(TextureWrapMode_CLAMP_TO_EDGE)
, m_texture_wrap_mode_t(TextureWrapMode_CLAMP_TO_EDGE)
, m_texture_wrap_mode_r(TextureWrapMode_CLAMP_TO_EDGE)
, m_width(0)
, m_height(0)
, m_length(0)
{
}

TextureIdentity::~TextureIdentity()
{
}

//-------------------------- end of namespace Graphics --------------------------
}
//---------------------------- end of namespace SDE -----------------------------
}