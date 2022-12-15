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
#include "OpenXRConfig.h"

#include <nlohmann/json.hpp>

#include "LogManager.h"
#include "FileResourceRequester.h"

using SDE::Basic::FileData;
using SDE::Basic::FileResourceRequester;

______________SD_START_OPENXR_NAMESPACE______________

OpenXRConfig::OpenXRConfig()
: m_wm_strategy(OpenXRWindowSize_RECOMMAND_SIZE)
, m_wm_width(0u)
, m_wm_height(0u)
{
}

OpenXRConfig::~OpenXRConfig()
{
}

bool OpenXRConfig::LoadFromFile(const FilePathString &i_fp)
{
    FileData fd;
    bool is_exist = FileResourceRequester::GetRef().AskFile(i_fp, fd);
    if (is_exist == true) {
        return LoadFromSource(fd.GetDataString());
    }
    else {
        return false;
    }
}

bool OpenXRConfig::LoadFromSource(const std::string &i_src_str)
{
    nlohmann::json sp_root = nlohmann::json::parse(i_src_str);
    nlohmann::json exts_group_root = sp_root.at("Extensions");
    nlohmann::json window_size_root = sp_root.at("WindowSize");
    if (exts_group_root.is_null() == false) {
        nlohmann::json exts_root;
#if defined(_WIN32) || defined(_WIN64)
        exts_root = sp_root.at("Windows");
#elif defined(__ANDROID__)
        exts_root = sp_root.at("Android");
#else
        exts_root = sp_root.at("Default");
#endif
        for (uint32_t id = 0; id < exts_root.size(); ++id) {
            m_extensions.push_back(sp_root.at(id));
        }
    }
    
    if (window_size_root.is_null() == false) {
        m_wm_strategy = window_size_root["Strategy"];
        m_wm_width = window_size_root["Width"];
        m_wm_height = window_size_root["Height"];
    }

    return true;
}

void OpenXRConfig::CollectExtensionCstr(std::vector<const char*> &io_extensions)
{
    io_extensions.clear();
    for (uint32_t i = 0; i < m_extensions.size(); ++i) {
        io_extensions.push_back(m_extensions[i].c_str());
    }
}

_______________SD_END_OPENXR_NAMESPACE_______________