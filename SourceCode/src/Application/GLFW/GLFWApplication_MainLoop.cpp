#include "LogManager.h"
#include "GLFWApplication.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace App ----------------------------
namespace App
{

void GLFWApplication::LaunchGLFWApplication(const std::string &i_win_title, int i_argc, char **i_argv, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library)
{
    GLFWwindow* window = nullptr;
    GLFWmonitor *monitor = nullptr;
    Resolution final_win_res = i_win_res;
    //1. set error callback.
    glfwSetErrorCallback(GLFWApplication::ErrorCallback);
    //2. create glfw window.
    //--- i. initialize glfw.
    if (!glfwInit())
        exit(EXIT_FAILURE);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    if (glfwVulkanSupported() && i_adopt_library == Graphics::GraphicsLibrary_Vulkan) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    else {
        i_adopt_library = Graphics::GraphicsLibrary_OpenGL4;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    }

    //glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    //--- ii. create window.
    if (i_full_window == true) {
        monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

        final_win_res.SetResolution(mode->width, mode->height);
    }

    window = glfwCreateWindow(final_win_res.GetWidth(), final_win_res.GetHeight(), i_win_title.c_str(), monitor, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //--- iii. register key cbk.
    //------ keyboard
    glfwSetKeyCallback(window, GLFWApplication::KeyEventCallback);
    //------ Mouse
    glfwSetCursorPosCallback(window, GLFWApplication::CursorPositionCallback);
    glfwSetMouseButtonCallback(window, GLFWApplication::CursorMouseButtonCallback);
    glfwSetCursorPosCallback(window, GLFWApplication::CursorPositionCallback);
    glfwSetCursorEnterCallback(window, GLFWApplication::CursorEnterCallback);
    //------ Windows
    glfwSetWindowCloseCallback(window, GLFWApplication::WindowCloseCallback);
    glfwSetWindowMaximizeCallback(window, GLFWApplication::WindowMaximizeCallback);
    glfwSetFramebufferSizeCallback(window, GLFWApplication::WindowSizeCallback);
    glfwSetWindowPosCallback(window, GLFWApplication::WindowPosCallback);
    glfwSetWindowFocusCallback(window, GLFWApplication::WindowFocusCallback);
    glfwSetDropCallback(window, GLFWApplication::DropCallback);
    glfwSetScrollCallback(window, GLFWApplication::ScrollCallback);

    //3. new application and initialize without graphics.
    new GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv);
    Application::GetDynamicCastPtr<GLFWApplication>()->RegisterGLFW(window, monitor);
    Application::GetRef().Initialize();
    
    //--- v. initialize glew.
    Application::GetRef().InitializeGraphicsSystem();
    
    //3. launch main loop.
    if (i_adopt_library == Graphics::GraphicsLibrary_OpenGL4) {
        while (!glfwWindowShouldClose(window)){
            try {
                // Update Game.
                Application::GetRef().Update();
            }
            catch (std::exception &e)
            {
                SDLOGE("Execption occur !!! %s.", e.what());
            }

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    else {
        while (!glfwWindowShouldClose(window)){
            try {
                // Update Game.
                Application::GetRef().Update();
            }
            catch (std::exception &e)
            {
                SDLOGE("Execption occur !!! %s.", e.what());
            }
            glfwPollEvents();
        }
    }

    Application::GetRef().ReleaseGraphicsSystem();
    // Terminate GLFW
    glfwTerminate();

    //Destroy App.
    Application::GetRef().TerminateApplication();
    Application::Destroy();
}

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------