#include "VulkanAPITestManager.h"
#include "Sample.h"

BasicUniformBuffer::BasicUniformBuffer()
{
}

BasicUniformBuffer::~BasicUniformBuffer()
{
}

LightUniformBuffer::LightUniformBuffer()
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

LightUniformBuffer::~LightUniformBuffer()
{
}

MaterialUniformBuffer::MaterialUniformBuffer()
: m_ambient(0.2f, 0.2f, 0.2f, 1.0f)
, m_diffuse(0.6f, 0.6f, 0.6f, 1.0f)
, m_specular(0.2f, 0.2f, 0.2f, 1.0f)
, m_emission(0.0f, 0.0f, 0.0f, 1.0f)
, m_shineness(1.0f)
{
}

MaterialUniformBuffer::~MaterialUniformBuffer()
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