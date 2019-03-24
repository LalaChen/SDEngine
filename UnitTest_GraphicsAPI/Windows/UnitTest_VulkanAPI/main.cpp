#include <windows.h>

#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanTestGLFWApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::App;

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    VulkanTestGLFWApplication app(
        std::string("UnitTest_VulkanAPITest"),
        Resolution(1200, 800),
        false, //full screen can't debug.
        GraphicsLibrary_Vulkan,
        0,
        nullptr
    );

    app.Initialize();

    app.RunMainLoop();
}