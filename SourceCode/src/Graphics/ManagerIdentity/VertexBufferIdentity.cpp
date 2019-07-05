#include "VertexBufferIdentity.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

VertexBufferIdentity::VertexBufferIdentity()
: m_buffer_handle(0)
, m_memory_handle(0)
, m_format(VBufferFormat_MAX_DEFINE_VALUE)
{
}

VertexBufferIdentity::~VertexBufferIdentity()
{
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}