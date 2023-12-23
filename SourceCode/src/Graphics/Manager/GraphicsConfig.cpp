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

#include "GraphicsConfig.h"

#include <nlohmann/json.hpp>

#include "LogManager.h"
#include "FileResourceRequester.h"

using SDE::Basic::FileData;
using SDE::Basic::FileResourceRequester;

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsConfig::GraphicsConfig()
: m_desired_depth_buffer_formats {
    TextureFormat_D32_SFLOAT_S8_UINT, //Default Depth
    TextureFormat_D24_UNORM_S8_UINT,
    TextureFormat_D32_SFLOAT,
    TextureFormat_D16_UNORM }
, m_desired_color_buffer_formats {
    TextureFormat_R8G8B8A8_UNORM, //Default Color Buffer,
    TextureFormat_R8G8B8A8_SNORM }
{
}

GraphicsConfig::~GraphicsConfig()
{
}

bool GraphicsConfig::LoadFromFile(const FilePathString &i_fp)
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

bool GraphicsConfig::LoadFromSource(const std::string & i_src_str)
{
    nlohmann::json sp_root = nlohmann::json::parse(i_src_str);
    nlohmann::json dc_fmts_root = sp_root.at("DesiredColorTextureFormats");
    for (uint32_t fmt_id = 0; fmt_id < dc_fmts_root.size(); ++fmt_id) {
        uint32_t fmt_int = dc_fmts_root.at(fmt_id);
        TextureFormatEnum fmt = static_cast<TextureFormatEnum>(fmt_int);
        m_desired_color_buffer_formats.push_back(fmt);
    }

    nlohmann::json dd_fmts_root = sp_root.at("DesiredDepthTextureFormats");
    for (uint32_t fmt_id = 0; fmt_id < dd_fmts_root.size(); ++fmt_id) {
        uint32_t fmt_int = dd_fmts_root.at(fmt_id);
        TextureFormatEnum fmt = static_cast<TextureFormatEnum>(fmt_int);
        m_desired_depth_buffer_formats.push_back(fmt);
    }

    return true;
}

______________SD_END_GRAPHICS_NAMESPACE______________