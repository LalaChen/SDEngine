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
#include <ImGui/imgui.h>

#include "IMGUIWindow.h"

________________SD_START_GUI_NAMESPACE_______________

IMGUIWindow::IMGUIWindow(const ObjectName &i_name, const std::string &i_title)
: IMGUINode(i_name)
, m_title(i_title)
{
}

IMGUIWindow::~IMGUIWindow()
{
}

void IMGUIWindow::RecordCommand()
{
    ImGui::SetNextWindowPos(m_region.m_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(m_region.m_size, ImGuiCond_Always);
    ImGui::Begin(m_title.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Text("ABOUT THIS DEMO:");

    std::list<IMGUINodeStrongReferenceObject>::iterator node_iter;
    for (node_iter = m_children.begin(); node_iter != m_children.end(); ++node_iter) {
        SD_SREF((*node_iter)).RecordCommand();
    }

    ImGui::End();
}

_________________SD_END_GUI_NAMESPACE________________