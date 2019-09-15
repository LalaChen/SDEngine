#include "MeshData.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

ModelMeshData::ModelMeshData(const ObjectName &i_mesh_name)
: m_mesh_name(i_mesh_name)
, m_material_id(-1)
{
}

ModelMeshData::~ModelMeshData()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________