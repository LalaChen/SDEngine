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

#include "LogManager.h"
#include "Application.h"
#include "Timer.h"
#include "IMGUIBatch.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;

________________SD_START_GUI_NAMESPACE_______________

IMGUIBatch::IMGUIBatch(const ObjectName &i_name, const ImVec2& i_buffer_size)
: EventObject(i_name)
, m_buffer_size(i_buffer_size)
{
}

IMGUIBatch::~IMGUIBatch()
{
}

bool IMGUIBatch::LoadGUI(const IMGUIBatchLoadingCallback &i_load_func)
{
    if (i_load_func != nullptr) {
        return i_load_func(GetThisWeakPtrByType<IMGUIBatch>());
    }
    else {
        SDLOGE("Loading function is null.");
        return false;
    }
}
void IMGUIBatch::AddWindow(const IMGUIWindowStrongReferenceObject &i_window)
{
    if (i_window.IsNull() == false) {
        m_windows.push_back(i_window);
    }
}

void IMGUIBatch::Initialize()
{
    DynamicVertexBufferStrongReferenceObject vertices = new DynamicVertexBuffer("GUIBatchVBuffer", VertexBufferUsage_VERTEX_BUFFER, VertexBufferFormat_X32Y32_SFLOAT);
    DynamicVertexBufferStrongReferenceObject texcoords = new DynamicVertexBuffer("GUIBatchTCBuffer", VertexBufferUsage_TEX_COORD_BUFFER, VertexBufferFormat_X32Y32_SFLOAT);
    DynamicVertexBufferStrongReferenceObject colors = new DynamicVertexBuffer("GUIBatchCBuffer", VertexBufferUsage_COLOR_BUFFER, VertexBufferFormat_X32Y32Z32W32_SFLOAT);
    DynamicIndexBufferStrongReferenceObject indices = new DynamicIndexBuffer("GUIBatchIBuffer", IndexBufferFormat_X16_UINT);

    m_GUI_meshes = new Mesh("GUIBatchMesh");
    SD_SREF(m_GUI_meshes).RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, vertices.StaticCastTo<VertexBuffer>());
    SD_SREF(m_GUI_meshes).RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, texcoords.StaticCastTo<VertexBuffer>());
    SD_SREF(m_GUI_meshes).RegisterVertexBuffer(VertexBufferUsage_COLOR_BUFFER, colors.StaticCastTo<VertexBuffer>());
    SD_SREF(m_GUI_meshes).RegisterIndexBuffer(indices.StaticCastTo<IndexBuffer>());

    m_GUI_vertices = vertices;
    m_GUI_texcoords = texcoords;
    m_GUI_colors = colors;
    m_GUI_indices = indices;
}

void IMGUIBatch::RecordBatchCommand()
{
    ImGuiIO &io = ImGui::GetIO();
    io.DeltaTime = static_cast<float>(Timer::GetRef().GetProgramDeltaTime());
    io.DisplaySize = m_buffer_size;
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    io.MousePos.x = m_touch_btn.m_x;
    io.MousePos.y = m_touch_btn.m_y;
    io.MouseDown[0] = (m_touch_btn.m_state == SDE::Basic::TouchButtonState_PRESSED);

    ImGui::NewFrame(); //Need to execute after calling io.Fonts->GetTexDataAsRGBA32(). 

    std::list<IMGUIWindowStrongReferenceObject>::iterator window_iter;
    for (window_iter = m_windows.begin(); window_iter != m_windows.end(); ++window_iter) {
        SD_SREF((*window_iter)).RecordCommand();
    }

    ImGui::Render();
}

void IMGUIBatch::GetBuffers(
    DynamicVertexBufferWeakReferenceObject &io_GUI_vertices,
    DynamicVertexBufferWeakReferenceObject &io_GUI_texcoords,
    DynamicVertexBufferWeakReferenceObject &io_GUI_colors,
    DynamicIndexBufferWeakReferenceObject &io_GUI_indices)
{
    io_GUI_vertices = m_GUI_vertices;
    io_GUI_texcoords = m_GUI_texcoords;
    io_GUI_colors = m_GUI_colors;
    io_GUI_indices = m_GUI_indices;
}

_________________SD_END_GUI_NAMESPACE________________