#include "GraphicsConfig.h"
#include "LogManager.h"
#include "FileResourceRequester.h"

#include <nlohmann/json.hpp>

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