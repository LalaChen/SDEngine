#include "GLFWApplication.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace App ----------------------------
namespace App
{

GLFWApplication::GLFWApplication()
{

}

GLFWApplication::GLFWApplication(const Resolution &i_win_res, FullWindowOption i_full_window)
: Application(i_win_res, i_full_window)
{

}

GLFWApplication::~GLFWApplication()
{

}

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------