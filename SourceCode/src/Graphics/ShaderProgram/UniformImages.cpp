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
#include "UniformImages.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

UniformImages::UniformImages(const ObjectName &i_object_name, uint32_t i_binding_id)
: UniformVariable(i_object_name, i_binding_id)
{
}

UniformImages::~UniformImages()
{
}

void UniformImages::Initialize(const UniformImagesDescriptorWeakReferenceObject &i_uid)
{
    m_texs.resize(SD_CONST_WREF(i_uid).GetNumber());
}

bool UniformImages::SetTexture(const TextureWeakReferenceObject &i_tex, uint32_t i_idx)
{
    if (i_tex.IsNull() == true) {
        SDLOGW("Input null texture for UImages[%d].", i_idx);
        return false;
    }

    if (i_idx < m_texs.size()) {
        m_texs[i_idx] = i_tex;
        return true;
    }
    else {
        SDLOGW("Out of range. Idx(%d). Size(%u)", i_idx, static_cast<uint32_t>(m_texs.size()));
        return false;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________