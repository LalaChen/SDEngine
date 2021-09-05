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

#include "IMGUIButton.h"

using SDE::Basic::EventArg;
using SDE::Basic::Event;
using SDE::Basic::EventStrongReferenceObject;
using SDE::Basic::EventWeakReferenceObject;

________________SD_START_GUI_NAMESPACE_______________

const std::string IMGUIButton::sButtonEventName_Clicked = "Clicked";

IMGUIButton::IMGUIButton(const ObjectName &i_name, const std::string &i_text)
: IMGUINode(i_name)
, m_text(i_text)
{
    EventStrongReferenceObject click_event = new Event("Clicked");
    RegisterEvent(click_event);
}

IMGUIButton::~IMGUIButton()
{
}

void IMGUIButton::SetText(const std::string &i_text)
{
    m_text = i_text;
}


void IMGUIButton::RecordCommand()
{
    bool result = ImGui::Button(m_text.c_str());
    if (result == true) {
        NotifyEvent(sButtonEventName_Clicked, EventArg());
    }
}

void IMGUIButton::Append(const IMGUINodeStrongReferenceObject &i_child)
{
}

_________________SD_END_GUI_NAMESPACE________________