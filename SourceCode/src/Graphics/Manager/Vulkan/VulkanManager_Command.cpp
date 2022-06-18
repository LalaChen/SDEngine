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
    VkCommandPool &cp_handle = reinterpret_cast<VkCommandPool&>(io_identity.m_pool_handle);
    VkResult result = CreateVkCommandPool(cp_handle);
    if (result != VK_SUCCESS) {

    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::DestroyCommandPool(CommandPoolIdentity &io_identity)
{
    VkCommandPool &cp_handle = reinterpret_cast<VkCommandPool&>(io_identity.m_pool_handle);
    DestroyVkCommandPool(cp_handle);
    io_identity.SetInvalid();
    io_identity = CommandPoolIdentity();
}

void VulkanManager::AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool)
{
    const CommandPoolIdentity &cp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_pool);
    VkCommandBuffer &cb_handle = reinterpret_cast<VkCommandBuffer&>(io_identity.m_handle);
    VkCommandPool cp_handle = reinterpret_cast<VkCommandPool>(cp_identity.m_pool_handle);
    VkCommandBufferLevel cb_level = CommandBufferLevel_Vulkan::Convert(io_identity.m_cmd_buffer_level);
    VkResult result = AllocateVkCommandBuffer(cb_handle, cp_handle, cb_level);
    if (result != VK_SUCCESS) {

    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info)
{
    VkCommandBufferBeginInfo info = InitializeVKCommandBufferBeginInfo();
    VkCommandBufferInheritanceInfo inheritance_info = InitializeVkCommandBufferInheritanceInfo();
    const VkCommandBuffer cb_handle = reinterpret_cast<const VkCommandBuffer>(i_identity.m_handle);
    if (i_inheritance_info.IsValid() == true) {
        const RenderPassIdentity &rp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_inheritance_info.m_rp);
        const FrameBufferIdentity &fb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_inheritance_info.m_fb);
        inheritance_info.renderPass = reinterpret_cast<VkRenderPass>(rp_identity.m_handle);
        inheritance_info.framebuffer = reinterpret_cast<VkFramebuffer>(fb_identity.m_fb_handle);
        inheritance_info.occlusionQueryEnable = ConvertBoolean(i_inheritance_info.m_occusion_query_enable);
        inheritance_info.queryFlags = QueryControlFlag_Vulkan::Convert(i_inheritance_info.m_ctrl_mask);
        inheritance_info.pipelineStatistics = QueryPipelineStatisticFlag_Vulkan::Convert(i_inheritance_info.m_pipe_stat_mask);
        info.pInheritanceInfo = &inheritance_info;
        info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    BeginVkCommandBuffer(cb_handle, info);
}

void VulkanManager::EndCommandBuffer(const CommandBufferIdentity &i_identity)
{
    const VkCommandBuffer cb_handle = reinterpret_cast<const VkCommandBuffer>(i_identity.m_handle);
    EndVkCommandBuffer(cb_handle);
}

void VulkanManager::FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool)
{
    VkCommandBuffer &cb_handle = reinterpret_cast<VkCommandBuffer&>(io_identity.m_handle);
    const CommandPoolIdentity cp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_pool);
    VkCommandPool cp_handle = reinterpret_cast<VkCommandPool>(cp_identity.m_pool_handle);
    FreeVkCommandBuffer(cb_handle, cp_handle);
    io_identity.SetInvalid();
    io_identity = CommandBufferIdentity();
}

void VulkanManager::SubmitCommandBuffersToQueue(const std::vector<CommandBufferWeakReferenceObject> &i_cbs)
{
    std::vector<VkCommandBuffer> cb_handles;
    for (const CommandBufferWeakReferenceObject &cb : i_cbs) {
        if (cb.IsNull() == false) {
            const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(cb);
            cb_handles.push_back(reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle));
        }
    }

    if (cb_handles.size() > 0) {
        SubmitVkCommandBuffers(cb_handles);
    }
}

void VulkanManager::SubmitCommandBufferToQueue(const CommandBufferWeakReferenceObject &i_cb)
{
    std::vector<VkCommandBuffer> cb_handles;

    if (i_cb.IsNull() == false) {
        const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
        cb_handles.push_back(reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle));
    }

    if (cb_handles.size() > 0) {
        SubmitVkCommandBuffers(cb_handles);
    }
}

void VulkanManager::ExecuteCommandsToPrimaryCommandBuffer(const CommandBufferWeakReferenceObject &i_primary_cb, const std::list<CommandBufferWeakReferenceObject> &i_secondary_cbs)
{
    std::vector<VkCommandBuffer> sec_cb_handles;
    for (const CommandBufferWeakReferenceObject &cb : i_secondary_cbs) {
        if (cb.IsNull() == false) {
            const CommandBufferIdentity &sec_cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(cb);
            sec_cb_handles.push_back(reinterpret_cast<VkCommandBuffer>(sec_cb_identity.m_handle));
        }
    }

    const CommandBufferIdentity &pri_cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_primary_cb);
    VkCommandBuffer pri_cb_handle = reinterpret_cast<VkCommandBuffer>(pri_cb_identity.m_handle);

    if (sec_cb_handles.size() > 0 && pri_cb_handle != VK_NULL_HANDLE) {
        ExecuteVkSecondaryCommandBuffersToPrimaryVkCommandBuffer(pri_cb_handle, sec_cb_handles);
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________