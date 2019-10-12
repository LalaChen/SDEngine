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

#include <sstream>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "SDEngine.h"
#include "MaterialData.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

const uint32_t MaterialData::AI_TEXTURE_ID[MaterialTextureType_MAX_DEFINE_VALUE] = {
    aiTextureType_DIFFUSE,
    aiTextureType_SPECULAR,
    aiTextureType_AMBIENT,
    aiTextureType_EMISSIVE,
    aiTextureType_HEIGHT,
    aiTextureType_NORMALS,
    aiTextureType_SHININESS,
    aiTextureType_OPACITY,
    aiTextureType_DISPLACEMENT,
    aiTextureType_LIGHTMAP,
    aiTextureType_REFLECTION
};

MaterialData::MaterialData()
: m_ambient(0.2f, 0.2f, 0.2f, 1.0f)
, m_diffuse(0.6f, 0.6f, 0.6f, 1.0f)
, m_specular(0.2f, 0.2f, 0.2f, 1.0f)
, m_emission(0.0f, 0.0f, 0.0f, 1.0f)
, m_shininess(1.0f)
{
}

MaterialData::~MaterialData()
{
}

std::string MaterialData::ToString(uint32_t i_level) const
{
    std::stringstream ss;
    std::string tabs;
    for (uint32_t i = 0; i < i_level; ++i) {
        tabs += "---";
    }
    ss << tabs << StringFormat("M[%s]\n", m_name.c_str());
    ss << tabs << tabs << "Ambient" << m_ambient.ToString() << "\n";
    ss << tabs << tabs << "Diffuse" << m_diffuse.ToString() << "\n";
    ss << tabs << tabs << "Specular" << m_specular.ToString() << "\n";
    ss << tabs << tabs << "Emission" << m_emission.ToString() << "\n";
    ss << tabs << tabs << "Shineness:" << m_shininess << "\n";
    ss << tabs << tabs << "Texture :\n";
    for (uint32_t i = 0; i < MaterialTextureType_MAX_DEFINE_VALUE; ++i) {
        if (m_textures_fns[i].empty() == false) {
            ss << tabs << tabs << tabs << StringFormat("%d:%s\n", i, m_textures_fns[i].c_str());
        }
    }

    return ss.str();
}

______________SD_END_GRAPHICS_NAMESPACE______________