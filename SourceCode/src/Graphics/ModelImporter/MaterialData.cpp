#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MaterialData.h"

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

______________SD_END_GRAPHICS_NAMESPACE______________