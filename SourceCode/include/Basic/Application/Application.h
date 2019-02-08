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

/*! \file      Application.h
 *  \brief     Introduce of class Application
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/02
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "Resolution.h"

using SDE::Graphics::Resolution;
using SDE::Graphics::Size_ui;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \class Application
	In our system, Application is used to manager application lifecycle. We also record \n
	the resolution in this class.
*/
class SDENGINE_CLASS Application
{
public:
	/*! \typedef bool FullWindowOption;
	    \brief full window option. 
	*/
	typedef bool FullWindowOption;

public:
	SINGLETON_DECLARATION(Application);

public:
	/*! \fn explicit Application(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, int i_argc, char **i_argv);
	    \param [in] i_win_title Window title
	    \param [in] i_win_res Window resolution.
		\param [in] i_full_window full window screen.
		\param [in] i_argc argument count.
		\param [in] i_argv arguments.
		\brief The constructor of Application Class.
	*/
	explicit Application(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, int i_argc, char **i_argv);

	/*! \fn virtual ~Application();
		\brief The destructor of Application Class.
	*/
	virtual ~Application();
public:
	/*! \fn virtual void Initialize();
		\brief Initialize this game app. Please call this function once at executable \n
		       starting. Please implement this function by derived class.
	*/
	virtual void Initialize() = 0;

	/*! \fn virtual void InitializeGraphicsSystem() = 0;
		\brief Initialize graphics system of this app. Please implement this function by \n
		       derived class.
	*/
	virtual void InitializeGraphicsSystem() = 0;

	/*! \fn virtual void TerminateApplication() = 0;
		\brief release all mamager for terminating app.
	*/
	virtual void TerminateApplication() = 0;
public:
	/*! \fn virtual void Update();
		\brief Update app. Please call this function each frame.
	*/
	virtual void Update();

	/*! \fn virtual void Pause();
		\brief Pause app. Please call this function when app doesn't focus.
	*/
	virtual void Pause();

	/*! \fn virtual void Resume();
		\brief Resume app. Please call this function when app re-focus.
	*/
	virtual void Resume();
public:
	/*! \fn void SetWindowResolution(Size_ui i_width, Size_ui i_height);
	    \param [in] i_width Width of app.
		\param [in] i_height Height of app.
		\brief Set resolution of this app.
	*/
	void SetWindowResolution(Size_ui i_width, Size_ui i_height);

protected:
	/*! \var Resolution m_win_res; [VarGet Attribute]
	    \brief resolution of this app.
	*/
	DECLARE_ATTRIBUTE_VAR_GET(Resolution, m_win_res, WindowResolution);

	/*! \var WindowSize m_full_window; [VarGet Attribute]
		\brief is full screen or not.
	*/
	DECLARE_ATTRIBUTE_VAR_GET(FullWindowOption, m_full_window, FullScreenSignal);

	/*! \var std::string m_win_title;
		\brief is full screen or not. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(std::string, m_win_title, WinTitle);
};

inline void Application::SetWindowResolution(Size_ui i_width, Size_ui i_height)
{
	m_win_res.SetResolution(i_width, i_height);
}

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}
