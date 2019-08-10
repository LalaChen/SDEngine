#include "VertexBufferIdentity.h"

//-------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics -----------------------
namespace Graphics
{

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

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}