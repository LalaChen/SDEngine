/*==============  SD Engine License ==============
MIT License

Copyright(c) 2019 Kuan - Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <SDEngine.h>
#include <SDEnginePlatform.h>
#include "VulkanAPITestManager.h"
#include "TestAndroidApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

TestAndroidApplication::TestAndroidApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: AndroidApplication(i_win_title, i_asset_mgr, i_adopt_library, i_argc, i_argv)
{
}

TestAndroidApplication::~TestAndroidApplication()
{
}


void TestAndroidApplication::Initialize()
{
    m_current_state = AppState_INITIALIZE;

    new AndroidLogManager();
    SDLOG("Initialize Android Application!!!");

    new Timer();
    Timer::GetRef().Start();

    new AndroidAssetResourceManger(m_asset_mgr);

    new ImageLoader();

    ImageLoader::GetRef().Initialize();

    if (m_adopt_library == SDE::Graphics::GraphicsLibrary_Vulkan) {
        new VulkanAPITestManager();
    }
    else {
    }
}