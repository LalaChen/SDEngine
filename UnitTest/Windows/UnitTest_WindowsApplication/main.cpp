#include <string>

#include "WindowsApplication.h"

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    WindowsApplication app(
        std::string("UnitTest_WindowsApp"), 
        SDE::Graphics::Resolution(1280,960), false, 
        SDE::Graphics::GraphicsLibrary_Vulkan, 0, nullptr);

    app.Initialize();

    app.RunMainLoop();

    return 0;
}