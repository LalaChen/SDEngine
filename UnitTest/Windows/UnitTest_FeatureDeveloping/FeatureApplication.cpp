#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "GameSystem.h"
#include "SampleDrawObjects.h"
#include "FeatureApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::App;

FeatureApplication::FeatureApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
{
}

FeatureApplication::~FeatureApplication()
{
}

void FeatureApplication::Initialize()
{
    GLFWApplication::Initialize();
    //
    ECSManager::GetRef().RegisterSystem<GameSystem>("GameSystem");
}