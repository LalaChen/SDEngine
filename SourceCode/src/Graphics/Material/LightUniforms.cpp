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

#include "LightUniforms.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

LightUniforms::LightUniforms()
: m_ambient(0.2f, 0.2f, 0.2f, 1.0f)
, m_diffuse(0.7f, 0.7f, 0.7f, 1.0f)
, m_specular(0.1f, 0.1f, 0.1f, 1.0f)
, m_spot_exp(0.0f)
, m_spot_cos_cutoff(1.0f)
, m_constant_attenuation(1.0f)
, m_linear_attenuation(1.0f)
, m_quadratic_attenuation(1.0f)
, m_kind(0)
{
}

LightUniforms::~LightUniforms()
{
}
______________SD_END_GRAPHICS_NAMESPACE______________