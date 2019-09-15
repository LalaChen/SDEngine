#include "LogManager.h"
#include "GLFWApplication.h"

________________SD_START_APP_NAMESPACE_______________

//----- Keyboard ------
void GLFWApplication::KeyEventCallback(GLFWwindow* i_window, int i_key, int i_scancode, int i_action, int i_mod_keys)
{
    if (i_key == GLFW_KEY_ESCAPE && i_action == GLFW_PRESS) {
        glfwSetWindowShouldClose(i_window, GLFW_TRUE);
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

}

void GLFWApplication::CursorEnterCallback(GLFWwindow *i_window, int entered)
{
    //Enter or leave
}

void GLFWApplication::CursorPositionCallback(GLFWwindow *i_window, double i_x, double i_y)
{
    //Motion
    //SDLOGD(" Mouse Motion (%lf,%lf)", i_x, i_y);
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
    Application::GetRef().Resize(i_w, i_h);
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