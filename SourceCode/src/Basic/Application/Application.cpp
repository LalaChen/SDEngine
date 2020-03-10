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

#include "LogManager.h"
#include "GraphicsManager.h"
#include "Application.h"

using SDE::Graphics::GraphicsManager;

______________SD_START_BASIC_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(Application);

Application::Application(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: m_win_title(i_win_title)
, m_win_res(i_win_res)
, m_full_window(i_full_window)
, m_adopt_library(i_adopt_library)
{
    //Register instance.
    SD_SINGLETON_DECLARATION_REGISTER;
    //Initialize events of application.
    m_key_map_manager = new KeyMapManager(i_win_title + "_KeyMapManager");
}

Application::~Application()
{

}

void Application::Resume()
{

}

void Application::Update()
{
    //Rendering.
    GraphicsManager::GetRef().Render();
}

void Application::Pause()
{

}

void Application::Resize(Size_ui32 i_w, Size_ui32 i_h)
{
    GraphicsManager::GetRef().Resize(i_w, i_h);
}


void Application::SetKeyboardStatus(int32_t i_key_id, bool i_is_pressed)
{
    if (m_key_map_manager.IsNull() == false) {
        m_key_map_manager.GetRef().SetKeyboardStatus(i_key_id, i_is_pressed);
    }
    else {
        SDLOGE("No key map manager.");
    }
}

bool Application::RegisterSlotFunctionIntoKeyEvent(const FunctionSlotBaseStrongReferenceObject &i_function_slot_ref_obj)
{
    if (m_key_map_manager.IsNull() == false) {
        return m_key_map_manager.GetRef().RegisterSlotFunctionIntoEvent("KeyEvent", i_function_slot_ref_obj);
    }
    else {
        SDLOGE("No key map manager.");
        return false;
    }
}

bool Application::UnregisterSlotFunctionFromKeyEvent(const FunctionSlotBaseStrongReferenceObject &i_function_slot_ref_obj)
{
    if (m_key_map_manager.IsNull() == false) {
        return m_key_map_manager.GetRef().UnregisterSlotFunctionFromEvent("KeyEvent", i_function_slot_ref_obj);
    }
    else {
        SDLOGE("No key map manager.");
        return false;
    }
}

_______________SD_END_BASIC_NAMESPACE________________