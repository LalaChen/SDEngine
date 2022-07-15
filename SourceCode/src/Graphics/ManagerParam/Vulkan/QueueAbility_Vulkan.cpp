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

#define ENABLE_ABI_AND(func, mask) \
    (mask.m_enable_masks[SD_ENUM_TO_UINT(func)] & QueueAbilityFlags[SD_ENUM_TO_UINT(func)])


#include "QueueAbility_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkQueueFlagBits QueueAbility_Vulkan::QueueAbilityFlags[QueueAbility_MAX_DEFINE_VALUE] = {
    VK_QUEUE_GRAPHICS_BIT,
    VK_QUEUE_COMPUTE_BIT,
    VK_QUEUE_TRANSFER_BIT,
    VK_QUEUE_SPARSE_BINDING_BIT,
    VK_QUEUE_PROTECTED_BIT
};

VkQueueFlags QueueAbility_Vulkan::Convert(const QueueAbility& i_flag_mask)
{
    VkQueueFlags result =
        ENABLE_ABI_AND(QueueAbility_GRAPHICS, i_flag_mask) |
        ENABLE_ABI_AND(QueueAbility_COMPUTE, i_flag_mask) |
        ENABLE_ABI_AND(QueueAbility_TRANSFER, i_flag_mask) |
        ENABLE_ABI_AND(QueueAbility_SPARSE_BINDING, i_flag_mask) |
        ENABLE_ABI_AND(QueueAbility_PROTECTED, i_flag_mask);

    return result;
}

______________SD_END_GRAPHICS_NAMESPACE______________