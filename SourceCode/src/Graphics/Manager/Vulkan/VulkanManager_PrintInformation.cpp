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
#include "VulkanManager.h"

//---------------------------- start of namespace SDE -----------------------------
namespace SDE
{
//------------------------- start of namespace Graphics ---------------------------
namespace Graphics
{

void VulkanManager::PrintSystemInformation()
{
    SDLOG("----------------- Vulkan Graphics Information -----------------");
    SDLOG("Memory Information :");
    VkPhysicalDeviceMemoryProperties phy_dev_memory_props;
    //1. Get memory property.
    vkGetPhysicalDeviceMemoryProperties(m_VK_physical_device, &phy_dev_memory_props);
    for (uint32_t type = 0; type < phy_dev_memory_props.memoryTypeCount; ++type) {
        SDLOG("Type[%d] : Flags(%u) HeadID(%u)", type,
            phy_dev_memory_props.memoryTypes[type].propertyFlags,
            phy_dev_memory_props.memoryTypes[type].heapIndex);
    }

    //2. Get memory info. We will get memory size of graphics card and main memory.
    for (uint32_t heap_ID = 0; heap_ID < phy_dev_memory_props.memoryHeapCount; ++heap_ID) {
        SDLOG("Heap[%d] : Flag(%u) Size(%llu)", heap_ID,
            phy_dev_memory_props.memoryHeaps[heap_ID].flags,
            phy_dev_memory_props.memoryHeaps[heap_ID].size);
    }
}

//-------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE -------------------------------
}