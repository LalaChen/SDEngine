#include "WindowsLogManager.h"
#include "OpenGL4API.h"
#include "GLFWApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace App ----------------------------
namespace App
{

GLFWApplication::GLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, int i_argc, char **i_argv)
: Application(i_win_title, i_win_res, i_full_window, i_argc, i_argv)
{
	//new LogManager.
	new WindowsLogManager();
	//new FileManager.

	//new Timer.

	//new Graphics API.
	new OpenGL4API();
	//new Graphics Manager

}

GLFWApplication::~GLFWApplication()
{
	SDLOG("Release Application.");
	//destroy Graphics Manager.

	//destroy Graphics API.
	GraphicsAPI::GetRef().ReleaseGraphicsSystem();
	GraphicsAPI::Destroy();
	//destroy Timer.

	//destroy File Manager

	//destroy LogManager
	LogManager::Destroy();
}

void GLFWApplication::Initialize()
{
	SDLOG("Initialize Application.");
	//Initialize KeyBoard Mapping.
}

void GLFWApplication::InitializeGraphicsSystem()
{
	SDLOG("Initialize Graphics System of Application.");
	GraphicsAPI::GetRef().InitializeGraphicsSystem();
}

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------