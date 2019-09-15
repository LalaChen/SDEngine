#include "VertexBufferIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VertexBufferIdentity::VertexBufferIdentity()
: m_buffer_handle(SD_NULL_HANDLE)
, m_memory_handle(SD_NULL_HANDLE)
, m_data_size(0)
, m_memory_size(0)
{
}

VertexBufferIdentity::~VertexBufferIdentity()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________