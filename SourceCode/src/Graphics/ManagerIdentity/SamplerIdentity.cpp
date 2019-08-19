#include "SamplerIdentity.h"

//---------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics ------------------------
namespace Graphics
{

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

//-------------------------- end of namespace Graphics --------------------------
}
//---------------------------- end of namespace SDE -----------------------------
}