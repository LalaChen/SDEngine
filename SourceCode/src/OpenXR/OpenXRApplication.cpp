/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "OpenXRApplication.h"

#include "OpenXRAPIManager.h"
#include "LogManager.h"
#include "Timer.h"
#include "ECSManager.h"
#include "ImageLoader.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "FileResourceRequester.h"

using SDE::Basic::KEY_STATUS_NOT_SUPPORT;

______________SD_START_OPENXR_NAMESPACE______________

OpenXRApplication::OpenXRApplication(
	const std::string &i_win_title,
	const Resolution &i_win_res,
	FullWindowOption i_full_window,
	GraphicsLibraryEnum i_adopt_library,
	int i_argc, char **i_argv)
: Application(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
{
}

OpenXRApplication::~OpenXRApplication()
{
}

void OpenXRApplication::Initialize()
{
	new OpenXRAPIManager("Common/Configs/OpenXRConfig.json");
	OpenXRAPIManager::GetRef().Initialize(m_win_title);
}

void OpenXRApplication::TerminateApplication()
{
	OpenXRAPIManager::GetRef().Terminate();
	OpenXRAPIManager::Destroy();
}

KeyStatusEnum OpenXRApplication::GetKeyStateByCode(KeyCodeEnum i_code)
{
	return KEY_STATUS_NOT_SUPPORT;
}

_______________SD_END_OPENXR_NAMESPACE_______________