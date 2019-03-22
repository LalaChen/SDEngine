#include "SDEngine.h"
#include "GLFWApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::App;

int main(int argc, char **argv)
{
    GLFWApplication::LaunchGLFWApplication(
        std::string("UnitTest_GLFWApplication"),
        argc,
        argv,
        Resolution(1200, 800),
        false, //full screen can't debug.
        GraphicsLibrary_Vulkan
    );
}