#include "SDEnginePlatform.h"
#include "AndroidApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

________________SD_START_APP_NAMESPACE_______________

AndroidApplication::AndroidApplication(const std::string &i_win_title, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, Resolution(2,2), true, i_adopt_library, i_argc, i_argv)
, m_window(nullptr)
, m_asset_mgr(nullptr)
{
}

AndroidApplication::~AndroidApplication()
{
}

void AndroidApplication::Initialize()
{
    new AndroidLogManager();
    SDLOG("Initialize Android Application!!!");

    new AndroidTimer();

    new AndroidAssetResourceManger(m_asset_mgr);

    Timer::GetRef().Start();
    new ImageLoader();

    ImageLoader::GetRef().Initialize();
}

void AndroidApplication::InitializeGraphicsSystem()
{
    if (m_adopt_library == SDE::Graphics::GraphicsLibrary_Vulkan) {
        new VulkanManager();
    }
    else {

    }
}

void AndroidApplication::ReleaseGraphicsSystem()
{
    m_window = nullptr;
}

void AndroidApplication::TerminateApplication()
{
    //destroy Timer.
    Timer::GetRef().End();
    SDLOG("APP Ending at %lf.", Timer::GetRef().GetEndProgramTime());
    Timer::Destroy();
    //destroy File Manager
    AssetResourceManager::Destroy();
    //destroy LogManager
    LogManager::Destroy();
    //
    m_asset_mgr = nullptr;
}

void AndroidApplication::RegisterNativeWindow(ANativeWindow *i_window)
{
    if (m_window == nullptr) {
        m_window = i_window;
    }
    else {
        SDLOGE("Native window isn't nullptr. We can't register.");
    }
}

void AndroidApplication::RegisterAssetManager(AAssetManager *i_asset_mgr)
{
    if (m_asset_mgr == nullptr) {
        m_asset_mgr = i_asset_mgr;
    }
    else {
        SDLOGE("Asset manager isn't nullptr. We can't register.");
    }
}

void AndroidApplication::RunMainLoop()
{
}
_________________SD_END_APP_NAMESPACE________________
