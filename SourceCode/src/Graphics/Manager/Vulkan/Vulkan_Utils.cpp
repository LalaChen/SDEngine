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

#include "LogManager.h"
#include "Vulkan_Utils.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugReportFlagsEXT i_flags,
    VkDebugReportObjectTypeEXT i_object_type,
    uint64_t i_obj,
    size_t i_location,
    int32_t i_code,
    const char* i_layer_prefix,
    const char* i_msg,
    void* i_use_data)
{
    if (i_flags == VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        SDLOG("Vulkan Verbose : %s", i_msg);
    }
    else if (i_flags == VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        SDLOGD("Vulkan Info : %s", i_msg);
    }
    else if (i_flags == VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        SDLOGW("Vulkan Warning : %s", i_msg);
    }
    else if (i_flags == VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        SDLOGW("Vulkan Performance Warning : %s", i_msg);
    }
    else {
        SDLOGE("Vulkan Error : %s", i_msg);
    }

    return VK_FALSE;
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}