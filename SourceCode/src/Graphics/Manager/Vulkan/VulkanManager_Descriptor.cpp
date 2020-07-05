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
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,/*==============  SD Engine License ==============
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

#include "VulkanStructureInitializer.h"
#include "UniformBindingType_Vulkan.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateDescriptorPool(DescriptorPoolIdentity &io_identity)
{
    VkDescriptorPool &dp_handle = reinterpret_cast<VkDescriptorPool&>(io_identity.m_handle);
    VkDescriptorPoolCreateInfo c_info = InitializeVkDescriptorPoolCreateInfo();
    if (io_identity.m_individual_op_flag == true) {
        c_info.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    }
    c_info.maxSets = io_identity.m_max_set;

    std::vector<VkDescriptorPoolSize> type_sizes;
    type_sizes.resize(UniformBindingType_MAX_DEFINE_VALUE);

    for (uint32_t type_id = 0; type_id < UniformBindingType_MAX_DEFINE_VALUE; ++type_id) {
        type_sizes[type_id] = {};
        type_sizes[type_id].type = UniformBindingType_Vulkan::Convert(static_cast<UniformBindingTypeEnum>(type_id));
        type_sizes[type_id].descriptorCount = io_identity.m_descriptor_counts[type_id];
    }

    c_info.pPoolSizes = type_sizes.data();
    c_info.poolSizeCount = static_cast<uint32_t>(type_sizes.size());
    CreateVkDescriptorPool(dp_handle, c_info);
}

void VulkanManager::DestroyDescriptorPool(DescriptorPoolIdentity &io_identity)
{
    VkDescriptorPool& dp_handle = reinterpret_cast<VkDescriptorPool&>(io_identity.m_handle);
    DestroyVkDescriptorPool(dp_handle);
}

______________SD_END_GRAPHICS_NAMESPACE______________ 
