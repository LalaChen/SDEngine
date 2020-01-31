#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanAPITestManager.h"
#include "VulkanTestGLFWApplication.h"

using namespace::SDE::App;
using namespace::SDE::Basic;
using namespace::SDE::Graphics;

VulkanTestGLFWApplication::VulkanTestGLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
{

}

VulkanTestGLFWApplication::~VulkanTestGLFWApplication()
{

}

void VulkanTestGLFWApplication::Initialize()
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
    //new ImageWindowsLoader
    new ImageLoader();
    ImageLoader::GetRef().Initialize();
    //new graphics engine.
    if (m_adopt_library == GraphicsLibrary_OpenGL4) {
        new OpenGL4Manager();
    }
    else {
        new VulkanAPITestManager();
    }
}