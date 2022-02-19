#include "VulkanConfig.h"
#include "LogManager.h"
#include "FileResourceRequester.h"

#include <nlohmann/json.hpp>

using SDE::Basic::FileData;
using SDE::Basic::FileResourceRequester;

_____________SD_START_GRAPHICS_NAMESPACE_____________

VulkanConfig::VulkanConfig()
: m_max_img_acq_time(2000000000) //2s
, m_max_fence_wait_time(17000000) //17ms
{
}

bool VulkanConfig::LoadFromFile(const FilePathString& i_fp)
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

bool VulkanConfig::LoadFromSource(const std::string & i_src_str)
{
    nlohmann::json sp_root = nlohmann::json::parse(i_src_str);
    std::string os_name;
    //------- OS Platform Only -------
#if defined(_WIN32) || defined(_WIN64) 
    os_name = "windows";
#elif defined(__ANDROID__)
    os_name = "android";
#else
    os_name = "default";
#endif
    nlohmann::json setting_root = sp_root.at(os_name);
    m_max_img_acq_time = setting_root.at("ImageAcqNanoTime").get<uint32_t>();
    if (m_max_img_acq_time == 0) {
        m_max_img_acq_time = 0;
    }

    m_max_fence_wait_time = setting_root.at("MaxFenceWaitNanoTime").get<uint32_t>();
    if (m_max_fence_wait_time == 0) {
        m_max_fence_wait_time = 0;
    }

    nlohmann::json valid_layers_root = setting_root.at("DesiredValidLayers");
    for (uint32_t id = 0; id < valid_layers_root.size(); id++) {
        std::string layer_name = valid_layers_root.at(id).get<std::string>();
        if (layer_name.empty() == false) {
            m_desire_valid_layers.push_back(layer_name);
        }
    }

    nlohmann::json dev_exts_root = setting_root.at("DesiredDeviceExtension");
    for (uint32_t id = 0; id < dev_exts_root.size(); id++) {
        std::string ext_name = dev_exts_root.at(id).get<std::string>();
        if (ext_name.empty() == false) {
            m_necessary_extensions.push_back(ext_name);
        }
    }

    nlohmann::json sfmts_root = setting_root.at("DesiredSurfaceFormats");
    for (uint32_t id = 0; id < sfmts_root.size(); id++) {
        nlohmann::json sfmt_root = sfmts_root.at(id);
        VkSurfaceFormatKHR sfmt;
        sfmt.format = static_cast<VkFormat>(sfmt_root.at("format").get<uint32_t>());
        sfmt.colorSpace = static_cast<VkColorSpaceKHR>(sfmt_root.at("colorSpace").get<uint32_t>());
        m_desired_sur_formats.push_back(sfmt);
    }

    nlohmann::json presents_root = setting_root.at("DesiredPresentModes");
    for (uint32_t id = 0; id < presents_root.size(); id++) {
        VkPresentModeKHR pre_mode = static_cast<VkPresentModeKHR>(presents_root.at(id).get<uint32_t>());
        m_desired_pre_modes.push_back(pre_mode);
    }

    return true;
}

VulkanConfig::~VulkanConfig()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________