#include "SamplerIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SamplerIdentity::SamplerIdentity()
: m_sampler(SD_NULL_HANDLE)
, m_mag_filter_type(SamplerFilterType_NEAREST)
, m_min_filter_type(SamplerFilterType_NEAREST)
, m_wrap_mode_s(SamplerWrapMode_CLAMP_TO_EDGE)
, m_wrap_mode_t(SamplerWrapMode_CLAMP_TO_EDGE)
, m_wrap_mode_r(SamplerWrapMode_CLAMP_TO_EDGE)
, m_mipmap_mode(SamplerMipmapMode_NEAREST)
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