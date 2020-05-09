#include "Vec.h"
#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void GraphicsManager::InitializeDefaultPipelineInfos()
{
    uint32_t va_id;
    m_default_va_binding_descs.resize(SD_ENUM_TO_UINT(VertexBufferUsage_MAX_DEFINE_VALUE));
    m_default_va_location_descs.resize(SD_ENUM_TO_UINT(VertexBufferUsage_MAX_DEFINE_VALUE));

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_VERTEX_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_ID = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_ID = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_NORMAL_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_ID = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_ID = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_ID = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec2);

    m_default_va_location_descs[va_id].m_binding_ID = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TANGENT_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_ID = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_ID = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_BINORMAL_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_ID = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_ID = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_COLOR_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_ID = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(Color4f);

    m_default_va_location_descs[va_id].m_binding_ID = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32W32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;
}


void GraphicsManager::GetBasicVertexAttribInfos(
    std::vector<VertexAttribBindingDescription> &io_binds,
    std::vector<VertexAttribLocationDescription> &io_locations,
    uint32_t i_tex_dimension) const
{
    io_binds = m_default_va_binding_descs;
    io_locations = m_default_va_location_descs;

    uint32_t va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
    if (i_tex_dimension == 1) {
        io_binds[va_id].m_binding_ID = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(float);

        io_locations[va_id].m_binding_ID = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32_SFLOAT;
        io_locations[va_id].m_offset = 0;
    }
    else if (i_tex_dimension == 2) {
        io_binds[va_id].m_binding_ID = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(vec2);

        io_locations[va_id].m_binding_ID = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32Y32_SFLOAT;
        io_locations[va_id].m_offset = 0;

    }
    else {
        io_binds[va_id].m_binding_ID = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(vec3);

        io_locations[va_id].m_binding_ID = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
        io_locations[va_id].m_offset = 0;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________