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

/*! \file      OpenXRConfig.h
 *  \brief     Introduce of class OpenXRConfig
 *  \author    Kuan-Chih, Chen
 *  \date      2022/09/04
 *  \copyright MIT License.
 */

#pragma once

#include <vector>
#include <string>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

using SDE::Basic::FilePathString;

______________SD_START_OPENXR_NAMESPACE______________

enum OpenXRWindowSizeStrategyEnum {
	OpenXRWindowSize_RECOMMAND_SIZE = 0,
	OpenXRWindowSize_MAXIMUM_SIZE = 1,
	OpenXRWindowSize_CUSTOM_SIZE
};

class SDENGINE_CLASS OpenXRConfig
{
public:
	OpenXRConfig();
	~OpenXRConfig();
public:
	bool LoadFromFile(const FilePathString &i_fp);
	bool LoadFromSource(const std::string &i_src_str);
	void CollectExtensionCstr(std::vector<const char*> &io_extensions);
public:
	std::vector<std::string> m_extensions;
public:
	OpenXRWindowSizeStrategyEnum m_wm_strategy;
	uint32_t m_wm_width;
	uint32_t m_wm_height;
};


_______________SD_END_OPENXR_NAMESPACE_______________