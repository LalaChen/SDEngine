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

/*! \file      GLFWApplication.h
 *  \brief     Introduce of class GLFWApplication.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright FreeBSD Public License.
 */

#pragma once

#include <GLFW/glfw3.h>
#include "Application.h"

using SDE::Basic::Application;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace App ----------------------------
namespace App
{
/*! \class GLFWApplication
 *  This class is used to create GLFW window. We use GLFWApplication::LaunchGLFWApplication to launch GLFW.
 */
class SDENGINE_CLASS GLFWApplication : public Application
{
public:
	/*! \fn static void LaunchGLFWApplication(const std::string &i_win_title, int i_argc, char **i_argv, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library);
	 *  \param [in] i_win_title Title of window.
	 *  \param [in] i_argc number of arguments.
	 *  \param [in] i_argv arguments.
	 *  \param [in] i_win_res Resolution of this window.
	 *  \param [in] i_full_window Is full window or not.
	 *  \param [in] i_adopt_library Which graphics library you use.
	 */
	static void LaunchGLFWApplication(const std::string &i_win_title, int i_argc, char **i_argv, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library);
public:
//------------ KeyEvent
	static void KeyEventCallback(GLFWwindow *i_window, int i_key, int i_scancode, int i_action, int i_mod_keys);
//------------ Mouse
	static void CursorMouseButtonCallback(GLFWwindow *i_window, int i_button, int i_action, int i_mod_keys);

	static void CursorEnterCallback(GLFWwindow *i_window, int entered); //Enter or leave

	static void CursorPositionCallback(GLFWwindow *i_window, double i_x, double i_y); //Motion
//------------ JoyStick
	static void JoyStickCallback(int i_jid, int i_event);
//------------ Error
	static void ErrorCallback(int i_error, const char* i_error_str);
//------------ Monitor
	static void MonitorCallback(GLFWmonitor *i_monitor, int i_event);
//------------ Window
	static void WindowCloseCallback(GLFWwindow *i_window);

	static void WindowMaximizeCallback(GLFWwindow *i_window, int i_maximized);

	static void WindowPosCallback(GLFWwindow *i_window, int i_x, int i_y);

	static void WindowSizeCallback(GLFWwindow *i_window, int i_w, int i_h);
	
	static void DropCallback(GLFWwindow *i_window, int i_count, const char **i_paths);

	static void WindowFocusCallback(GLFWwindow *i_window, int i_focused);

	static void ScrollCallback(GLFWwindow *i_window, double i_x, double i_y);
public:
	/*! \fn explicit GLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, int i_argc, char **i_argv);
	 *  \param [in] i_win_res Window resolution.
	 *  \param [in] i_full_window full window screen.
	 *  \param [in] i_adopt_library Which graphics library you use.
	 *  \param [in] i_argc argument count.
	 *  \param [in] i_argv arguments.
	 *  \brief The constructor of GLFWApplication Class.
	 */
	explicit GLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
	
	/*! \fn virtual ~GLFWApplication();
	 *  \brief The destructor of GLFWApplication Class.
	 */
	virtual ~GLFWApplication();
public:
	/*! \fn void RegisterGLFW(GLFWwindow *i_window, GLFWmonitor *i_monitor);
	 *  \param [in] i_window Window instance of GLFW.
	 *  \param [in] i_monitor Monitor instance of GLFW.
	 *  \brief Register window and monitor.
	 */
	void RegisterGLFW(GLFWwindow *i_window, GLFWmonitor *i_monitor);
public:
	/*! \fn void Initialize() override;
	 *  \brief Initialize this app. We will create WindowsLogManager, OpenGL4 API and Manager for GLFW.
	 */
	void Initialize() override;

	/*! \fn void InitializeGraphicsSystem() override;
	 *  \brief Initialize graphics system of this app.
	 */
	void InitializeGraphicsSystem() override;

	/*! \fn void ReleaseGraphicsSystem() override;
	 *  \brief release graphics system of this app.
	 */
	void ReleaseGraphicsSystem() override;

	/*! \fn void TerminateApplication() override;
	 *  \brief release all mamager for terminating app.
	 */
	void TerminateApplication() override;
protected:
	GLFWwindow *m_window;
	GLFWmonitor *m_monitor;
};

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------