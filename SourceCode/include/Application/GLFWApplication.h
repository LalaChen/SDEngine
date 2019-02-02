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

class SDENGINE_CLASS GLFWApplication : public Application
{
public:
	/*! \fn explicit GLFWApplication();
		\brief The constructor of GLFWApplication Class.
	*/
	explicit GLFWApplication();

	/*! \fn GLFWApplication(const Resolution &i_win_res, FullWindowOption i_full_window);
		\param [in] i_win_res Window resolution.
		\param [in] i_full_window full window screen.
		\brief The constructor of GLFWApplication Class.
	*/
	GLFWApplication(const Resolution &i_win_res, FullWindowOption i_full_window);

	/*! \fn virtual ~GLFWApplication();
		\brief The destructor of GLFWApplication Class.
	*/
	virtual ~GLFWApplication();
};

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------