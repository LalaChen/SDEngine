#include "Sample.h"

Sample::Sample(const ObjectName &i_object_name, VulkanAPITestManager *i_mgr)
: Object(i_object_name)
, m_mgr(i_mgr)
{
}

Sample::~Sample()
{
}