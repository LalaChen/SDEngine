#include "VulkanAPITestManager.h"
#include "Sample.h"

LightUniformBuffer::LightUniformBuffer()
: m_ambient(1.0f, 1.0f, 1.0f, 1.0f)
, m_diffuse(1.0f, 1.0f, 1.0f, 1.0f)
, m_specular(1.0f, 1.0f, 1.0f, 1.0f)
, m_spot_exp(0.0f)
, m_spot_cos_cutoff(1.0f)
, m_constant_attenuation(1.0f)
, m_linear_attenuation(1.0f)
, m_quadratic_attenuation(1.0f)
, m_shininess(1.0f)
, m_kind(0)
{
}

LightUniformBuffer::~LightUniformBuffer()
{
}

Sample::Sample(const ObjectName &i_object_name, VulkanAPITestManager *i_mgr)
: Object(i_object_name)
, m_mgr(i_mgr)
{
    m_current_res = m_mgr->GetScreenResolution();
}

Sample::~Sample()
{
}