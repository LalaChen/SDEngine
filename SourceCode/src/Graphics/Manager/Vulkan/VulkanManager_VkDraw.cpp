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

#include "VulkanManager.h"

#include "LogManager.h"


_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::SetVkViewport(
    VkCommandBuffer i_cmd_buffer,
    const VkViewport &i_viewport)
{
    vkCmdSetViewport(i_cmd_buffer, 0, 1, &i_viewport);
}

void VulkanManager::SetVkViewports(
    VkCommandBuffer i_cmd_buffer,
    const std::vector<VkViewport> &i_viewports)
{
    vkCmdSetViewport(i_cmd_buffer, 0, static_cast<uint32_t>(i_viewports.size()), i_viewports.data());
}

void VulkanManager::SetVkScissors(
    VkCommandBuffer i_cmd_buffer,
    const std::vector<VkRect2D> &i_rects)
{
    vkCmdSetScissor(i_cmd_buffer, 0, static_cast<uint32_t>(i_rects.size()), i_rects.data());
}

void VulkanManager::SetVkScissor(
    VkCommandBuffer i_cmd_buffer,
    const VkRect2D &i_rect)
{
    vkCmdSetScissor(i_cmd_buffer, 0, 1, &i_rect);
}

void VulkanManager::DrawByVkIndexBuffer(
    VkCommandBuffer i_cmd_buffer,
    uint32_t i_indice_size,
    uint32_t i_instance_count,
    uint32_t i_first_index,
    int32_t i_index_offest,
    uint32_t i_first_instance)
{
    vkCmdDrawIndexed(i_cmd_buffer, i_indice_size, i_instance_count, i_first_index, i_index_offest, i_first_instance);
}

______________SD_END_GRAPHICS_NAMESPACE______________