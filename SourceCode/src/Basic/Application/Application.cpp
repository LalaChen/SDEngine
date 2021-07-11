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
#include "Timer.h"
#include "ECSManager.h"
#include "ImageLoader.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "FileResourceRequester.h"
#include "Application.h"

using SDE::Basic::FileResourceRequester;
using SDE::Graphics::ImageLoader;
using SDE::Graphics::GraphicsManager;
using SDE::Graphics::SceneManager;

______________SD_START_BASIC_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(Application);

Application::Application(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: m_win_title(i_win_title)
, m_win_res(i_win_res)
, m_full_window(i_full_window)
, m_adopt_library(i_adopt_library)
, m_graphics_app_instance(SD_NULL_HANDLE)
{
    //Register instance.
    SD_SINGLETON_DECLARATION_REGISTER;
    //Initialize events of application.
    m_key_map_manager = new KeyMapManager(i_win_title + "_KeyMapManager");
    //Initialize events.
    m_app_event_notifier = new EventObject("AppEventNotifier");
    SD_WREF(m_app_event_notifier).RegisterEvent(new Event(sAppEventName));
    //Initialize ECSManager
    new ECSManager();
    ECSManager::GetRef().Initialize();
    //Initialize Timer.
    new Timer();
    Timer::GetRef().Start();
    //ImageLoader.
    new ImageLoader();
    ImageLoader::GetRef().Initialize();
    //new FileResourceRequester
    new FileResourceRequester();
    //
    new SceneManager();
    SceneManager::GetRef().Initialize();
}

Application::~Application()
{ 
    FileResourceRequester::Destroy();
    ImageLoader::Destroy();
    Timer::Destroy();
    ECSManager::Destroy();
    SceneManager::Destroy();
    m_key_map_manager.Reset();
}

void Application::Resume()
{
}

void Application::Update()
{
    //1. Update Timer.
    Timer::GetRef().Update();
    //2. Update System.
    ECSManager::GetRef().Update();
}

void Application::Pause()
{

}

void Application::Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h)
{
    GraphicsManager::GetRef().Resize(i_ns_handle, i_w, i_h);

    ECSManager::GetRef().Resize();

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_SCREEN_RESIZED));
}

void Application::SetKeyboardStatus(int32_t i_key_id, bool i_is_pressed)
{
    if (m_key_map_manager.IsNull() == false) {
        SD_SREF(m_key_map_manager).SetKeyboardStatus(i_key_id, i_is_pressed);
    }
    else {
        SDLOGE("No key map manager.");
    }
}

void Application::SetTouchStatus(TouchButtonEnum i_touch_id, TouchButtonStateEnum i_touch_state)
{
    if (i_touch_id != TOUCH_BUTTON_MAX_DEFINE_VALUE) {
        m_buttons[SD_ENUM_TO_UINT(i_touch_id)].m_state = i_touch_state;
    }
}

void Application::SetTouchPosition(TouchButtonEnum i_touch_id, float i_x, float i_y)
{
    if (i_touch_id != TOUCH_BUTTON_MAX_DEFINE_VALUE) {
        m_buttons[SD_ENUM_TO_UINT(i_touch_id)].m_x = i_x;
        m_buttons[SD_ENUM_TO_UINT(i_touch_id)].m_y = i_y;
    }
}

TouchButton Application::GetTouchButton(TouchButtonEnum i_button_id) const
{
    if (i_button_id != TOUCH_BUTTON_MAX_DEFINE_VALUE) {
        return m_buttons[i_button_id];
    }
    else {
        SDLOGE("Error btn id.");
        return TouchButton();
    }
}

bool Application::RegisterSlotFunctionIntoKeyEvent(const FunctionSlotBaseStrongReferenceObject &i_function_slot)
{
    if (m_key_map_manager.IsNull() == false) {
        return SD_SREF(m_key_map_manager).RegisterSlotFunctionIntoEvent("KeyEvent", i_function_slot);
    }
    else {
        SDLOGE("No key map manager.");
        return false;
    }
}

bool Application::UnregisterSlotFunctionFromKeyEvent(const FunctionSlotBaseStrongReferenceObject &i_function_slot)
{
    if (m_key_map_manager.IsNull() == false) {
        return SD_SREF(m_key_map_manager).UnregisterSlotFunctionFromEvent("KeyEvent", i_function_slot);
    }
    else {
        SDLOGE("No key map manager.");
        return false;
    }
}

_______________SD_END_BASIC_NAMESPACE________________