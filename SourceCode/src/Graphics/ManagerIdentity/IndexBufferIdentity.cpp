#include "IndexBufferIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

IndexBufferIdentity::IndexBufferIdentity()
: m_buffer_handle(SD_NULL_HANDLE)
, m_memory_handle(SD_NULL_HANDLE)
, m_data_size(0)
, m_index_array_size(0)
, m_memory_size(0)
, m_format(IndexBufferFormat_X32_UINT)
, m_memory_type(MemoryType_MAX_DEFINE_VALUE)
{
}

IndexBufferIdentity::~IndexBufferIdentity()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________