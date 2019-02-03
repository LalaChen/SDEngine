#include "LogManager.h"
#include "GLFWApplication.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace App ----------------------------
namespace App
{

void GLFWApplication::LaunchGLFWApplication(const std::string &i_win_title, int i_argc, char **i_argv, const Resolution &i_win_res, FullWindowOption i_full_window)
{
	GLFWwindow* window = nullptr;
	GLFWmonitor *monitor = nullptr;
	Resolution final_win_res = i_win_res;
	//1. new application and initialize without graphics.
	new GLFWApplication(i_win_title, i_win_res, i_full_window, i_argc, i_argv);
	Application::GetRef().Initialize();
	//2. set error callback.
	glfwSetErrorCallback(GLFWApplication::ErrorCallback);
	//3. create glfw window.
	//--- i. initialize glfw.
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
	glfwSetWindowSizeCallback(window, GLFWApplication::WindowSizeCallback);
	glfwSetWindowPosCallback(window, GLFWApplication::WindowPosCallback);
	glfwSetWindowFocusCallback(window, GLFWApplication::WindowFocusCallback);
	glfwSetDropCallback(window, GLFWApplication::DropCallback);
	glfwSetScrollCallback(window, GLFWApplication::ScrollCallback);
	//--- iv. make current.
	glfwMakeContextCurrent(window);
	//--- v. initialize glew.
	Application::GetRef().InitializeGraphicsSystem();
	glfwSwapInterval(1);

	//3. launch main loop.
	while (!glfwWindowShouldClose(window))
	{
		// Draw gears
		Application::GetRef().UpdateTimer();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate GLFW
	glfwTerminate();
}

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------