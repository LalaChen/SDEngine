#include "Sample.h"

Sample::Sample(const ObjectName &i_object_name, VulkanAPITestManager *i_mgr)
: Object(i_object_name)
, m_mgr(i_mgr)
, m_VK_cmd_pool(VK_NULL_HANDLE)
, m_VK_cmd_buffer(VK_NULL_HANDLE)
, m_VK_frame_buffer(VK_NULL_HANDLE)
{
}

Sample::~Sample()
{
}