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
, m_shineness(1.0f)
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
    ss << tabs << tabs << "Shineness:" << m_shineness << "\n";
    ss << tabs << tabs << "Texture :\n";
    for (uint32_t i = 0; i < MaterialTextureType_MAX_DEFINE_VALUE; ++i) {
        if (m_textures_fns[i].empty() == false) {
            ss << tabs << tabs << tabs << StringFormat("%d:%s\n", i, m_textures_fns[i].c_str());
        }
    }

    return ss.str();
}

______________SD_END_GRAPHICS_NAMESPACE______________