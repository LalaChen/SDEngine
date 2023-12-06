/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "GraphicsManager.h"

#include "Vec.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void GraphicsManager::InitializeDefaultPipelineInfos()
{
    uint32_t va_id;
    m_default_va_binding_descs.resize(SD_ENUM_TO_UINT(VertexBufferUsage_MAX_DEFINE_VALUE));
    m_default_va_location_descs.resize(SD_ENUM_TO_UINT(VertexBufferUsage_MAX_DEFINE_VALUE));

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_VERTEX_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_id = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_id = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_NORMAL_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_id = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_id = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_id = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec2);

    m_default_va_location_descs[va_id].m_binding_id = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TANGENT_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_id = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_id = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_BINORMAL_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_id = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(vec3);

    m_default_va_location_descs[va_id].m_binding_id = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;

    va_id = SD_ENUM_TO_UINT(VertexBufferUsage_COLOR_BUFFER);
    m_default_va_binding_descs[va_id].m_binding_id = va_id;
    m_default_va_binding_descs[va_id].m_input_rate = VertexInputRate_VERTEX;
    m_default_va_binding_descs[va_id].m_stride = sizeof(Color4f);

    m_default_va_location_descs[va_id].m_binding_id = va_id;
    m_default_va_location_descs[va_id].m_location = va_id;
    m_default_va_location_descs[va_id].m_format = VertexBufferFormat_X32Y32Z32W32_SFLOAT;
    m_default_va_location_descs[va_id].m_offset = 0;
}


void GraphicsManager::GetBasicVertexAttribInfos(
    std::vector<VertexAttribBindingDescription> &io_binds,
    std::vector<VertexAttribLocationDescription> &io_locations,
    VertexLocationKindEnum i_vl_kind) const
{
    io_binds = m_default_va_binding_descs;
    io_locations = m_default_va_location_descs;

    if (i_vl_kind == VertexLocationKind_GENERAL) {
        uint32_t va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
        io_binds[va_id].m_binding_id = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(vec2);
        io_locations[va_id].m_binding_id = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32Y32_SFLOAT;
        io_locations[va_id].m_offset = 0;
    }
    else if (i_vl_kind == VertexLocationKind_LOOKUP_TABLE) {
        uint32_t va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
        io_binds[va_id].m_binding_id = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(float);
        io_locations[va_id].m_binding_id = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32_SFLOAT;
        io_locations[va_id].m_offset = 0;
    }
    else if (i_vl_kind == VertexLocationKind_VOL_DATA) {
        uint32_t va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
        io_binds[va_id].m_binding_id = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(vec3);
        io_locations[va_id].m_binding_id = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32Y32Z32_SFLOAT;
        io_locations[va_id].m_offset = 0;
    }
    else if (i_vl_kind == VertexLocationKind_GUI) {
        uint32_t va_id;
        va_id = SD_ENUM_TO_UINT(VertexBufferUsage_TEX_COORD_BUFFER);
        io_binds[va_id].m_binding_id = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(vec2);
        io_locations[va_id].m_binding_id = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32Y32_SFLOAT;
        io_locations[va_id].m_offset = 0;
        va_id = SD_ENUM_TO_UINT(VertexBufferUsage_VERTEX_BUFFER);
        io_binds[va_id].m_binding_id = va_id;
        io_binds[va_id].m_input_rate = VertexInputRate_VERTEX;
        io_binds[va_id].m_stride = sizeof(vec2);
        io_locations[va_id].m_binding_id = va_id;
        io_locations[va_id].m_location = va_id;
        io_locations[va_id].m_format = VertexBufferFormat_X32Y32_SFLOAT;
        io_locations[va_id].m_offset = 0;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________