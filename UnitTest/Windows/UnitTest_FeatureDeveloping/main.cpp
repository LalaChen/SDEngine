#include <windows.h>

#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "FeatureApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::App;

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    FeatureApplication app(
        std::string("UnitTest_FeatureDeveloping"),
        Resolution(1200, 800),
        false, //full screen can't debug.
        GraphicsLibrary_Vulkan,
        0,
        nullptr
    );

    app.Initialize();

    app.RunMainLoop();
}