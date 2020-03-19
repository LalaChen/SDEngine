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

#if defined(__ANDROID__)

#include <unistd.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "LogManager.h"
#include "AndroidAssetResourceManager.h"

______________SD_START_BASIC_NAMESPACE_______________

AndroidAssetResourceManger::AndroidAssetResourceManger(AAssetManager* i_mgr)
: m_mgr(i_mgr)
{
}

AndroidAssetResourceManger::~AndroidAssetResourceManger()
{
}

void AndroidAssetResourceManger::Initialize()
{
	
}

bool AndroidAssetResourceManger::LoadAsset(const FilePathString &i_asset_fn, FileData &io_file)
{
	SDLOG("Load Asset[%s].", i_asset_fn.c_str());
	FileData result;
	unsigned int file_length = 0;
	if (m_mgr != nullptr) {
		AAsset* asset = AAssetManager_open(m_mgr, i_asset_fn.c_str(), AASSET_MODE_UNKNOWN);
		if (asset != nullptr) {
			//-- calculate file size
			file_length = AAsset_getLength(asset);
			//-- create buffer
			io_file.m_file_content.resize(file_length);
			//-- load file
			AAsset_read(asset, (void*)io_file.m_file_content.data(), file_length);
			AAsset_close(asset);
			return true;
		}
		else {
			SDLOGW("Asset[%s] isn't in asset.", i_asset_fn.c_str());
			return false;
		}
	}
	else {
		SDLOGE("Asset manager is nullptr.");
		return false;
	}
}

_______________SD_END_BASIC_NAMESPACE________________

#endif