#include "SamplerIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SamplerIdentity::SamplerIdentity()
: m_sampler(SD_NULL_HANDLE)
, m_texture_mag_filter_type(TextureFilterType_NEAREST)
, m_texture_min_filter_type(TextureFilterType_NEAREST)
, m_texture_wrap_mode_s(TextureWrapMode_CLAMP_TO_EDGE)
, m_texture_wrap_mode_t(TextureWrapMode_CLAMP_TO_EDGE)
, m_texture_wrap_mode_r(TextureWrapMode_CLAMP_TO_EDGE)
{
}

SamplerIdentity::~SamplerIdentity()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________