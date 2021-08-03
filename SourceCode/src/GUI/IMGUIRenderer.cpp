/* ==============  SD Engine License ==============
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

//#define SD_IMGUI_DEBUG

#include <ImGui/imgui.h>

#include "LogManager.h"
#include "IMGUIRenderer.h"

using namespace SDE::Graphics;

________________SD_START_GUI_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(IMGUIRenderer);

IMGUIRenderer::IMGUIRenderer()
{
    SD_SINGLETON_DECLARATION_REGISTER;
    ImGui::CreateContext();
}

IMGUIRenderer::~IMGUIRenderer()
{
    ImGui::DestroyContext();
}

void IMGUIRenderer::InitializeGraphicsSystem()
{
    // Color scheme
    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

    ImGuiIO &io = ImGui::GetIO();
    unsigned char* font = nullptr;
    int tex_width, tex_height;
    io.Fonts->GetTexDataAsRGBA32(&font, &tex_width, &tex_height);
    BitmapStrongReferenceObject font_bitmap = new Bitmap("FontAltasBitmap", BitmapConfig_RGBA_8888, tex_width, tex_height, font, tex_width * tex_height * 4, -1, BitmapPixelDataType_UNSIGNED_BYTE);
    m_font_atlas = new Texture("IMGUIDefaultFontAtlas");
    SD_SREF(m_font_atlas).InitializeFromBitmap(font_bitmap);
    //
    m_GUI_shader = GraphicsManager::GetRef().GetShaderProgram("GUIShader");

    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };

    //1. Get DescriptorSet for MeshRenderer and count number of uniforms.
    DescriptorSetLayoutWeakReferenceObject basic_ds_layouts = GraphicsManager::GetRef().GetBasicDescriptorSetLayout(sUniformDescriptorSetLayout_GUI);
    SD_WREF(basic_ds_layouts).GetUniformDescriptorCounts(desc_counts);

    //2. Allocate descriptor pool and set.
    m_dp = new DescriptorPool("DescriptorSetPool");
    SD_SREF(m_dp).Initialize(desc_counts, 1, false);
    m_ds = SD_SREF(m_dp).AllocateDescriptorSet(basic_ds_layouts);
    SD_WREF(m_ds).GetAllocatedUniformVariables(uvs);

    //4. Get basic uniform buffer for update MVP.
    if (uvs.find(sUniformBuffer_GUI_Offset) != uvs.end()) {
        m_gui_ub = uvs[sUniformBuffer_GUI_Offset].DynamicCastTo<UniformBuffer>();
    }
    if (m_gui_ub.IsNull() == true) {
        SDLOGE("We can't find GUI uniform buffer.");
    }

    if (uvs.find(sUniformImages_GUI_Font) != uvs.end()) {
        m_atlas_ui = uvs[sUniformImages_GUI_Font].DynamicCastTo<UniformImages>();
    }
    if (m_atlas_ui.IsNull() == true) {
        SDLOGE("We can't find atlas uniform images.");
    }

    SD_WREF(m_atlas_ui).SetTexture(m_font_atlas, 0);

    //3. Write descriptor to GPU for this set.
    if (m_ds.IsNull() == false) {
        SD_WREF(m_ds).WriteDescriptor();
    }
}

void IMGUIRenderer::ReleaseGraphicsSystem()
{
    m_font_atlas.Reset();
}

bool IMGUIRenderer::PrepareVerticesData(
    DynamicVertexBufferWeakReferenceObject &io_vertices,
    DynamicVertexBufferWeakReferenceObject &io_texcoords,
    DynamicVertexBufferWeakReferenceObject &io_colors,
    DynamicIndexBufferWeakReferenceObject &io_indices)
{

    ImDrawData* draw_data = ImGui::GetDrawData();
    Size_ui64 vtx_count = draw_data->TotalVtxCount;
    Size_ui64 idx_count = draw_data->TotalIdxCount;

    if (vtx_count == 0 || idx_count == 0) {
        return false;
    }
#if !defined(SD_IMGUI_DEBUG)
    SD_WREF(io_vertices).Resize(vtx_count * sizeof(vec2));
    SD_WREF(io_texcoords).Resize(vtx_count * sizeof(vec2));
    SD_WREF(io_colors).Resize(vtx_count * sizeof(Color4f));
    SD_WREF(io_indices).Resize(idx_count * sizeof(uint16_t));

    vec2 *vertices_ptr = reinterpret_cast<vec2*>(SD_WREF(io_vertices).MapMemory());
    vec2 *texcoords_ptr = reinterpret_cast<vec2*>(SD_WREF(io_texcoords).MapMemory());
    Color4f *colors_ptr = reinterpret_cast<Color4f*>(SD_WREF(io_colors).MapMemory());
    uint16_t *indices_ptr = reinterpret_cast<uint16_t*>(SD_WREF(io_indices).MapMemory());

    uint32_t v_number = 0, i_number = 0;

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        //vertices.
        for (int v_id = 0; v_id < cmd_list->VtxBuffer.Size; ++v_id) {
            vertices_ptr[v_number].x = cmd_list->VtxBuffer.Data[v_id].pos.x;
            vertices_ptr[v_number].y = cmd_list->VtxBuffer.Data[v_id].pos.y;
            texcoords_ptr[v_number].x = cmd_list->VtxBuffer.Data[v_id].uv.x;
            texcoords_ptr[v_number].y = cmd_list->VtxBuffer.Data[v_id].uv.y;
            Color4B color_b(reinterpret_cast<uint8_t*>(&cmd_list->VtxBuffer.Data[v_id].col));
            colors_ptr[v_number].r = color_b.GetFloatR();
            colors_ptr[v_number].g = color_b.GetFloatG();
            colors_ptr[v_number].b = color_b.GetFloatB();
            colors_ptr[v_number].a = color_b.GetFloatA();
            v_number++;
        }
        //indices.
        for (int i_id = 0; i_id < cmd_list->IdxBuffer.Size; ++i_id) {
            indices_ptr[i_number] = cmd_list->IdxBuffer.Data[i_id];
            i_number++;
        }
    }

    SD_WREF(io_vertices).UnmapMemory();
    SD_WREF(io_texcoords).UnmapMemory();
    SD_WREF(io_colors).UnmapMemory();
    SD_WREF(io_indices).UnmapMemory();
#else
    SD_WREF(io_vertices).Resize(4 * sizeof(vec2));
    SD_WREF(io_texcoords).Resize(4 * sizeof(vec2));
    SD_WREF(io_colors).Resize(4 * sizeof(Color4f));
    SD_WREF(io_indices).Resize(6 * sizeof(uint16_t));

    vec2* vertices_ptr = reinterpret_cast<vec2*>(SD_WREF(io_vertices).MapMemory());
    vec2* texcoords_ptr = reinterpret_cast<vec2*>(SD_WREF(io_texcoords).MapMemory());
    Color4f* colors_ptr = reinterpret_cast<Color4f*>(SD_WREF(io_colors).MapMemory());
    uint16_t* indices_ptr = reinterpret_cast<uint16_t*>(SD_WREF(io_indices).MapMemory());

    vertices_ptr[0].x = -1.0f; vertices_ptr[0].y = -1.0f;
    vertices_ptr[1].x = -1.0f; vertices_ptr[1].y = 1.0f;
    vertices_ptr[2].x =  1.0f; vertices_ptr[2].y = 1.0f;
    vertices_ptr[3].x =  1.0f; vertices_ptr[3].y = -1.0f;

    texcoords_ptr[0].x = 0.0f; texcoords_ptr[0].y = 0.0f;
    texcoords_ptr[1].x = 0.0f; texcoords_ptr[1].y = 1.0f;
    texcoords_ptr[2].x = 1.0f; texcoords_ptr[2].y = 1.0f;
    texcoords_ptr[3].x = 1.0f; texcoords_ptr[3].y = 0.0f;

    colors_ptr[0].r = 0.0f; colors_ptr[0].g = 0.0f; colors_ptr[0].b = 0.0f; colors_ptr[0].a = 1.0f;
    colors_ptr[1].r = 0.0f; colors_ptr[1].g = 1.0f; colors_ptr[1].b = 0.0f; colors_ptr[1].a = 1.0f;
    colors_ptr[2].r = 0.0f; colors_ptr[2].g = 0.0f; colors_ptr[2].b = 1.0f; colors_ptr[2].a = 1.0f;
    colors_ptr[3].r = 1.0f; colors_ptr[3].g = 1.0f; colors_ptr[3].b = 1.0f; colors_ptr[3].a = 1.0f;

    indices_ptr[0] = 0; indices_ptr[1] = 1; indices_ptr[2] = 2;
    indices_ptr[3] = 0; indices_ptr[4] = 2; indices_ptr[5] = 3;

    SD_WREF(io_vertices).UnmapMemory();
    SD_WREF(io_texcoords).UnmapMemory();
    SD_WREF(io_colors).UnmapMemory();
    SD_WREF(io_indices).UnmapMemory();
#endif

    return true;
}

void IMGUIRenderer::RecordGUICommands(
    const RenderFlowWeakReferenceObject &i_flow,
    const CommandBufferWeakReferenceObject &i_cmd_buffer,
    const IMGUIBatchWeakReferenceObject &i_gui_batch)
{
    //1. Record Imgui commands.
    SD_WREF(i_gui_batch).RecordBatchCommand();
    //2. Update vertices.
    DynamicVertexBufferWeakReferenceObject vertices;
    DynamicVertexBufferWeakReferenceObject texcoords;
    DynamicVertexBufferWeakReferenceObject colors;
    DynamicIndexBufferWeakReferenceObject indices;
    SD_WREF(i_gui_batch).GetBuffers(
        vertices, texcoords, colors, indices);
    ImVec2 buffer_size = SD_WREF(i_gui_batch).GetBufferSize();
    if (PrepareVerticesData(vertices, texcoords, colors, indices) == true) {
        //3. update ub.
        ImGuiIO& io = ImGui::GetIO();
        float offset[4] = { 0 };
        offset[0] = 2.0f / io.DisplaySize.x;
        offset[1] = 2.0f / io.DisplaySize.y;
        offset[2] = -1.0f;
        offset[3] = -1.0f;
        SD_WREF(m_gui_ub).SetBufferData(offset, sizeof(offset));
        SD_WREF(m_gui_ub).Update();
        //4. record command.
        SD_WREF(i_cmd_buffer).Begin();
        SD_WREF(i_flow).BeginRenderFlow(i_cmd_buffer);
        Viewport vp;
        vp.m_x = 0; vp.m_y = 0; vp.m_width = buffer_size[0]; vp.m_height = buffer_size[1]; vp.m_min_depth = -1.0; vp.m_max_depth = 1.0f;
        ScissorRegion sr;
        sr.m_x = 0; sr.m_y = 0; sr.m_width = buffer_size[0]; sr.m_height = buffer_size[1];

        GraphicsManager::GetRef().SetViewport(i_cmd_buffer, vp);
        GraphicsManager::GetRef().SetScissor(i_cmd_buffer, sr);

        CommandBufferInheritanceInfo cb_inher_info = SD_WREF(i_flow).GetCurrentInheritanceInfo();

        SD_WREF(m_GUI_shader).UseProgramWithTargetDescriptorSet(
            i_cmd_buffer, cb_inher_info.m_rp,
            0, 0, { m_ds });

        SD_WREF(texcoords).Bind(i_cmd_buffer, VertexBufferUsage_TEX_COORD_BUFFER, 0);
        SD_WREF(colors).Bind(i_cmd_buffer, VertexBufferUsage_COLOR_BUFFER, 0);
        SD_WREF(vertices).Bind(i_cmd_buffer, VertexBufferUsage_VERTEX_BUFFER, 0);
        SD_WREF(indices).Bind(i_cmd_buffer, 0);
        GraphicsManager::GetRef().DrawByIndices(i_cmd_buffer, indices.StaticCastTo<IndexBuffer>(), 0, 0, 0, 1);

        SD_WREF(i_flow).EndRenderFlow(i_cmd_buffer);
        SD_WREF(i_cmd_buffer).End();
    }
}

_________________SD_END_GUI_NAMESPACE________________
