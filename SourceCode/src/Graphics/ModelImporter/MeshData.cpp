#include <sstream>

#include "SDEngineCommonFunction.h"
#include "MeshData.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshData::MeshData()
: m_material_ID(-1)
{
}

MeshData::~MeshData()
{
}

std::string MeshData::ToString(uint32_t i_level) const
{
    std::stringstream ss;
    std::string tabs;
    for (uint32_t i = 0; i < i_level; ++i) {
        tabs += "---";
    }
    ss << tabs << StringFormat("M[%s]:\n", m_name.c_str());
    for (uint32_t i = 0; i < VertexBufferUsage_BUFFER_GROUP; ++i) {
        if (m_vertex_attribs[i].size() > 0) {
            ss << tabs << tabs << StringFormat("VA[%d](%llu)\n", i, m_vertex_attribs[i].size());
        }
    }
    ss << tabs << tabs << StringFormat("Color(%llu)\n", m_vertex_colors.size());
    ss << tabs << tabs << StringFormat("Face(%llu)\n", m_face_indices.size() / 3);
    return ss.str();
}

______________SD_END_GRAPHICS_NAMESPACE______________