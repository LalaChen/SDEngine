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

#include "QueryFlags_Vulkan.h"
#include "CommandBufferLevel_Vulkan.h"
#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateCommandPool(CommandPoolIdentity &io_identity)
{
    VkCommandPool &cmd_pool_handle = reinterpret_cast<VkCommandPool&>(io_identity.m_pool_handle);
    CreateVkCommandPool(cmd_pool_handle);
}

void VulkanManager::DestroyCommandPool(CommandPoolIdentity &io_identity)
{
    VkCommandPool &cmd_pool_handle = reinterpret_cast<VkCommandPool&>(io_identity.m_pool_handle);
    DestroyVkCommandPool(cmd_pool_handle);
}

void VulkanManager::AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool_wref)
{
    VkCommandBuffer &cmd_buffer_handle = reinterpret_cast<VkCommandBuffer&>(io_identity.m_handle);
    VkCommandPool cmd_pool_handle = reinterpret_cast<VkCommandPool>(i_pool_wref.GetConstRef().GetHandle());
    VkCommandBufferLevel cmd_level = CommandBufferLevel_Vulkan::Convert(io_identity.m_cmd_buffer_level);
    AllocateVkCommandBuffer(cmd_buffer_handle, cmd_pool_handle, cmd_level);
}

void VulkanManager::BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info)
{
    VkCommandBufferBeginInfo info = InitializeVKCommandBufferBeginInfo();
    VkCommandBufferInheritanceInfo inheritance_info = InitializeVkCommandBufferInheritanceInfo();
    const VkCommandBuffer cmd_buffer_handle = reinterpret_cast<const VkCommandBuffer>(i_identity.m_handle);
    if (i_inheritance_info.IsValid() == true) {
        inheritance_info.renderPass = reinterpret_cast<VkRenderPass>(i_inheritance_info.m_rp_wref.GetConstRef().GetHandle());
        inheritance_info.framebuffer = reinterpret_cast<VkFramebuffer>(i_inheritance_info.m_fb_wref.GetConstRef().GetHandle());
        inheritance_info.occlusionQueryEnable = ConvertBoolean(i_inheritance_info.m_occusion_query_enable);
        inheritance_info.occlusionQueryEnable = ConvertBoolean(i_inheritance_info.m_occusion_query_enable);
        inheritance_info.queryFlags = QueryControlFlag_Vulkan::Convert(i_inheritance_info.m_ctrl_mask);
        inheritance_info.pipelineStatistics = QueryPipelineStatisticFlag_Vulkan::Convert(i_inheritance_info.m_pipe_stat_mask);
        info.pInheritanceInfo = &inheritance_info;
    }
    BeginVkCommandBuffer(cmd_buffer_handle, info);
}

void VulkanManager::EndCommandBuffer(const CommandBufferIdentity &i_identity)
{
    const VkCommandBuffer cmd_buffer_handle = reinterpret_cast<const VkCommandBuffer>(i_identity.m_handle);
    EndVkCommandBuffer(cmd_buffer_handle);
}

void VulkanManager::FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool_wref)
{
    VkCommandBuffer &cmd_buffer_handle = reinterpret_cast<VkCommandBuffer&>(io_identity.m_handle);
    VkCommandPool cmd_pool_handle = reinterpret_cast<VkCommandPool>(i_pool_wref.GetConstRef().GetHandle());
    FreeVkCommandBuffer(cmd_buffer_handle, cmd_pool_handle);
}

void VulkanManager::SubmitCommandBufferToQueue(const std::vector<CommandBufferWeakReferenceObject> &i_cmd_bufs)
{
    std::vector<VkCommandBuffer> cb_handles;
    for (const CommandBufferWeakReferenceObject &cb : i_cmd_bufs) {
        if (cb.IsNull() == false) {
            const CommandBufferIdentity &cb_indentity = GetIdentity(cb);
            cb_handles.push_back(reinterpret_cast<VkCommandBuffer>(cb_indentity.m_handle));
        }
    }

    if (cb_handles.size() > 0) {
        SubmitVkCommandBuffers(cb_handles);
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________