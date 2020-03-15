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

#include "SamplerIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SamplerIdentity::SamplerIdentity()
: m_sampler_handle(SD_NULL_HANDLE)
, m_mag_filter_type(SamplerFilterType_NEAREST)
, m_min_filter_type(SamplerFilterType_NEAREST)
, m_mipmap_mode(SamplerMipmapMode_NEAREST)
, m_wrap_mode_s(SamplerWrapMode_CLAMP_TO_EDGE)
, m_wrap_mode_t(SamplerWrapMode_CLAMP_TO_EDGE)
, m_wrap_mode_r(SamplerWrapMode_CLAMP_TO_EDGE)
, m_sampler_b_color_type(SamplerBorderColorType_FLOAT_TRANSPARENT_BLACK)
, m_use_compare(false)
, m_compare_op(CompareOperator_LESS)
, m_use_anisotropy(false)
, m_max_anisotropy(1.0f)
{
}

SamplerIdentity::~SamplerIdentity()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________