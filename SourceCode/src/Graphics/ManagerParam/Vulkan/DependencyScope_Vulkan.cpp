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

#include "DependencyScope_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkDependencyFlagBits DependencyScope_Vulkan::Dependencies[DependencyScope_MAX_DEFINE_VALUE] =
{
    VK_DEPENDENCY_BY_REGION_BIT,
    VK_DEPENDENCY_DEVICE_GROUP_BIT,
    VK_DEPENDENCY_VIEW_LOCAL_BIT
};

VkDependencyFlags DependencyScope_Vulkan::Convert(const std::vector<DependencyScopeEnum> &i_src)
{
    VkDependencyFlags result = 0;
    for (DependencyScopeEnum scope : i_src) {
        if (SD_ENUM_TO_UINT(scope) < DependencyScope_MAX_DEFINE_VALUE) {
            result |= Dependencies[SD_ENUM_TO_UINT(scope)];
        }
    }
    return result;
}

______________SD_END_GRAPHICS_NAMESPACE______________