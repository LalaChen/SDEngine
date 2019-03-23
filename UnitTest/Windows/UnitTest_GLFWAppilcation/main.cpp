#include "SDEngine.h"
#include "GLFWApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::App;

int main(int argc, char **argv)
{
    GLFWApplication app(
        std::string("UnitTest_GLFWApplication"),
        Resolution(1200, 800),
        false, //full screen can't debug.
        GraphicsLibrary_Vulkan,
        argc,
        argv
    );

    app.Initialize();

    app.RunMainLoop();
}