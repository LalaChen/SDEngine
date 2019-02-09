#include "SDEngine.h"
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

}

GLFWApplication::~GLFWApplication()
{

}

void GLFWApplication::Initialize()
{
	SDLOG("Initialize Application.");
	//new LogManager.
	new WindowsLogManager();
	//new WindowsFileSystemManager.
	new WindowsFileSystemManager();
	FileSystemManager::GetRef().Initialize();
	//new Timer.
	new WindowsTimer();
	Timer::GetRef().Start();
	SDLOG("APP Starting at %lf.", Timer::GetRef().GetProgramStartTime());
	//new Graphics API.
	new OpenGL4();
	//new Graphics Manager

	//Initialize KeyBoard Mapping.
}

void GLFWApplication::InitializeGraphicsSystem()
{
	SDLOG("Initialize Graphics System of Application.");
	Graphics::GetRef().InitializeGraphicsSystem();
}

void GLFWApplication::TerminateApplication()
{
	SDLOG("Terminate Application.");
	//destroy Graphics Manager.

	//destroy Graphics API.
	Graphics::GetRef().ReleaseGraphicsSystem();
	Graphics::Destroy();
	//destroy Timer.
	Timer::GetRef().End();
	SDLOG("APP Ending at %lf.", Timer::GetRef().GetEndProgramTime());
	//destroy File Manager

	//destroy LogManager
	LogManager::Destroy();
}

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------