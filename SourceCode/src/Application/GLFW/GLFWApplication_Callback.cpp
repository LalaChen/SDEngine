#include "LogManager.h"
#include "GLFWApplication.h"

________________SD_START_APP_NAMESPACE_______________

//----- Keyboard ------
void GLFWApplication::KeyEventCallback(GLFWwindow* i_window, int i_key, int i_scancode, int i_action, int i_mod_keys)
{
    if (i_key == GLFW_KEY_ESCAPE && i_action == GLFW_PRESS) {
        glfwSetWindowShouldClose(i_window, GLFW_TRUE);
    }
    else if (i_key >= 0 && i_key < GLFW_KEY_ESCAPE) {
        uint32_t key_id = static_cast<uint32_t>(i_key);
        if (i_action == GLFW_PRESS) {
            Application::GetRef().SetKeyboardStatus(key_id, true);
        }
        else if (i_action == GLFW_RELEASE){
            Application::GetRef().SetKeyboardStatus(key_id, false);
        }
    }
    else {
        //Handle special key.
    }
}

//----- error return ------
void GLFWApplication::ErrorCallback(int i_error, const char* i_error_str)
{
    SDLOGE("GLFW Error : %s(%d)", i_error_str, i_error);
}

//----- Mouse ------
void GLFWApplication::CursorMouseButtonCallback(GLFWwindow *i_window, int i_button, int i_action, int i_mod_keys)
{
    TouchButtonEnum button;
    TouchButtonStateEnum state;

    button = static_cast<TouchButtonEnum>(i_button);

    switch (i_action) {
        case GLFW_PRESS:
            state = SDE::Basic::TOUCH_BUTTON_STATE_PRESSED;
            break;
        case GLFW_RELEASE:
            state = SDE::Basic::TOUCH_BUTTON_STATE_RELEASED;
            break;
        default:
            state = SDE::Basic::TOUCH_BUTTON_STATE_MAX_DEFINE_VALUE;
            break;
    }
}

void GLFWApplication::CursorEnterCallback(GLFWwindow *i_window, int entered)
{
    //Enter or leave
}

void GLFWApplication::CursorPositionCallback(GLFWwindow *i_window, double i_x, double i_y)
{
    //Motion
    //SDLOGD("Mouse Motion (%lf,%lf)", i_x, i_y);
    Application::GetRef().SetTouchPosition(SDE::Basic::TOUCH_BUTTON_LEFT, i_x, i_y);
    Application::GetRef().SetTouchPosition(SDE::Basic::TOUCH_BUTTON_RIGHT, i_x, i_y);
}

//----- JoyStick ------
void GLFWApplication::JoyStickCallback(int i_jid, int i_event)
{

}

//----- DropItem ------
void GLFWApplication::DropCallback(GLFWwindow *i_window, int i_count, const char **i_paths)
{

}

//----- Window ------
void GLFWApplication::WindowCloseCallback(GLFWwindow *i_window)
{

}

void GLFWApplication::WindowMaximizeCallback(GLFWwindow *i_window, int i_maximized)
{

}

void GLFWApplication::WindowPosCallback(GLFWwindow *i_window, int i_x, int i_y)
{

}

void GLFWApplication::WindowSizeCallback(GLFWwindow *i_window, int i_w, int i_h)
{
    SDLOG("Window size change to (%d, %d)", i_w, i_h);
    Application::GetRef().Resize(SD_NULL_HANDLE, i_w, i_h);
}

void GLFWApplication::WindowFocusCallback(GLFWwindow *i_window, int i_focused)
{

}

//----- monitor ------
void GLFWApplication::MonitorCallback(GLFWmonitor *i_monitor, int i_event)
{

}

void GLFWApplication::ScrollCallback(GLFWwindow *i_window, double i_x, double i_y)
{

}

_________________SD_END_APP_NAMESPACE________________