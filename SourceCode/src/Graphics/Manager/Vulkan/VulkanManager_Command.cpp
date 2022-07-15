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

#include "QueryFlags_Vulkan.h"
#include "QueueAbility_Vulkan.h"
#include "CommandBufferLevel_Vulkan.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateQueue(GraphicsQueueIdentity &io_identity)
{
    VkQueue          &queue      = reinterpret_cast<VkQueue&>(io_identity.m_handle);
    VkPhysicalDevice &phy_device = reinterpret_cast<VkPhysicalDevice&>(io_identity.m_phy_device);
    VkDevice         &device     = reinterpret_cast<VkDevice&>(io_identity.m_device);
    phy_device = m_phy_device;
    device     = m_device;
    //
    VkQueueFlags flags = QueueAbility_Vulkan::Convert(io_identity.m_abilities);
    if (m_queue_family.IsSuitable(flags) == true) {
        GetVkQueue(device, io_identity.m_family_id, io_identity.m_queue_id, queue);
        io_identity.SetValid();
    }
    else {
        SDLOGE("Get queue(%d,%d) failure !!!", io_identity.m_family_id, io_identity.m_queue_id);
    }
}

void VulkanManager::SubmitCommandBuffersToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsFenceWeakReferenceObject &i_fence, const std::vector<CommandBufferWeakReferenceObject> &i_cbs)
{
    std::vector<VkCommandBuffer> cmd_buffers;
    for (const CommandBufferWeakReferenceObject &cb : i_cbs) {
        if (cb.IsNull() == false) {
            const CommandBufferIdentity &cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(cb);
            cmd_buffers.push_back(reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle));
        }
    }

    const GraphicsFenceIdentity &fence_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_fence);

    const VkDevice &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    const VkQueue  &queue  = reinterpret_cast<const VkQueue&>(i_identity.m_handle);
    const VkFence  &fence  = reinterpret_cast<const VkFence&>(fence_identity.m_handle);

    if (cmd_buffers.size() > 0) {
        SubmitVkCommandBuffersToQueue(device, queue, fence, cmd_buffers);
    }
}

void VulkanManager::PresentToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsSwapchainIdentity &i_sw_identity, uint32_t i_img_id, const std::vector<GraphicsSemaphoreWeakReferenceObject> &i_semas)
{
    const VkQueue &queue = reinterpret_cast<const VkQueue&>(i_identity.m_handle);

    std::vector<VkSemaphore> semas;
    for (const GraphicsSemaphoreWeakReferenceObject &sema : i_semas) {
        const GraphicsSemaphoreIdentity &sema_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(sema);
        semas.push_back(reinterpret_cast<VkSemaphore>(sema_identity.m_handle));
    }

    const VkSwapchainKHR &swapchain = reinterpret_cast<const VkSwapchainKHR&>(i_sw_identity.m_handle);

    std::vector<VkSwapchainKHR> swapchains = { swapchain };
    std::vector<uint32_t> ids = { i_img_id };

    VkPresentInfoKHR info = InitializeVkPresentInfoKHR(
        semas, swapchains, ids);

    PresentVkSwapchainToVkQueue(queue, info);
}

void VulkanManager::DestroyQueue(GraphicsQueueIdentity &io_identity)
{
    io_identity.SetInvalid();
    io_identity.m_handle = SD_NULL_HANDLE;
    io_identity = GraphicsQueueIdentity();
}

void VulkanManager::CreateFence(GraphicsFenceIdentity &io_identity)
{
    VkFence &fence = reinterpret_cast<VkFence&>(io_identity.m_handle);
    VkDevice &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkFenceCreateInfo info = InitializeVkFenceCreateInfo();

    VkResult result = CreateVkFence(fence, device, info);

    if (result != VK_SUCCESS) {
        SDLOGE("Create fence failure(%d) !!!", result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::ResetFence(const GraphicsFenceIdentity &i_identity)
{
    const VkFence  &fence  = reinterpret_cast<const VkFence&>(i_identity.m_handle);
    const VkDevice &device = reinterpret_cast<const VkDevice&>(i_identity.m_device);
    ResetVkFence(fence, device);
}

void VulkanManager::DestroyFence(GraphicsFenceIdentity &io_identity)
{
    io_identity.SetInvalid();
    io_identity = GraphicsFenceIdentity();
}

void VulkanManager::CreateSemaphoreObject(GraphicsSemaphoreIdentity &io_identity)
{
    VkSemaphore &semaphore = reinterpret_cast<VkSemaphore&>(io_identity.m_handle);
    const VkDevice &device = reinterpret_cast<const VkDevice&>(io_identity.m_device);
    VkSemaphoreCreateInfo info = InitializeVkSemaphoreCreateInfo();
    VkResult result = CreateVkSemaphore(semaphore, device, info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create semaphore failure(%d) !!!", result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::DestroySemaphoreObject(GraphicsSemaphoreIdentity &io_identity)
{
    VkSemaphore &semaphore = reinterpret_cast<VkSemaphore&>(io_identity.m_handle);
    const VkDevice &device = reinterpret_cast<const VkDevice&>(io_identity.m_device);
    DestroyVkSemaphore(semaphore, device);
    io_identity.SetInvalid();
    io_identity = GraphicsSemaphoreIdentity();
}

void VulkanManager::CreateCommandPool(CommandPoolIdentity &io_identity)
{
    VkCommandPool &cmd_pool = reinterpret_cast<VkCommandPool&>(io_identity.m_handle);
    VkDevice      &device   = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;
    VkResult result = CreateVkCommandPool(cmd_pool, device);
    if (result != VK_SUCCESS) {
        SDLOGE("Create command pool failure(%d) !!!", result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::DestroyCommandPool(CommandPoolIdentity &io_identity)
{
    VkCommandPool  &cmd_pool = reinterpret_cast<VkCommandPool&>(io_identity.m_handle);
    const VkDevice &device   = reinterpret_cast<VkDevice&>(io_identity.m_device);
    DestroyVkCommandPool(cmd_pool, device);
    io_identity.SetInvalid();
    io_identity = CommandPoolIdentity();
}

void VulkanManager::AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool)
{
    const CommandPoolIdentity &cp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_pool);
    VkCommandBuffer &cmd_buffer = reinterpret_cast<VkCommandBuffer&>(io_identity.m_handle);
    VkDevice        &device     = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkCommandPool    cmd_pool   = reinterpret_cast<VkCommandPool>(cp_identity.m_handle);
    VkCommandBufferLevel cb_level = CommandBufferLevel_Vulkan::Convert(io_identity.m_cmd_buffer_level);

    io_identity.m_device = cp_identity.m_device;

    VkResult result = AllocateVkCommandBuffer(cmd_buffer, device, cmd_pool, cb_level);
    if (result != VK_SUCCESS) {
        SDLOGE("Allocate command buffer from pool(%s) failure(%d) !!!", SD_WREF(i_pool).GetObjectName().c_str(), result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info)
{
    VkCommandBufferBeginInfo info = InitializeVKCommandBufferBeginInfo();
    VkCommandBufferInheritanceInfo inheritance_info = InitializeVkCommandBufferInheritanceInfo();
    const VkCommandBuffer cmd_buffer = reinterpret_cast<const VkCommandBuffer>(i_identity.m_handle);
    if (i_inheritance_info.IsValid() == true) {
        const RenderPassIdentity &rp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_inheritance_info.m_rp);
        const FrameBufferIdentity &fb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_inheritance_info.m_fb);
        inheritance_info.renderPass = reinterpret_cast<VkRenderPass>(rp_identity.m_handle);
        inheritance_info.framebuffer = reinterpret_cast<VkFramebuffer>(fb_identity.m_handle);
        inheritance_info.occlusionQueryEnable = ConvertBoolean(i_inheritance_info.m_occusion_query_enable);
        inheritance_info.queryFlags = QueryControlFlag_Vulkan::Convert(i_inheritance_info.m_ctrl_mask);
        inheritance_info.pipelineStatistics = QueryPipelineStatisticFlag_Vulkan::Convert(i_inheritance_info.m_pipe_stat_mask);
        info.pInheritanceInfo = &inheritance_info;
        info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    BeginVkCommandBuffer(cmd_buffer, info);
}

void VulkanManager::EndCommandBuffer(const CommandBufferIdentity &i_identity)
{
    const VkCommandBuffer &cmd_buffer = reinterpret_cast<const VkCommandBuffer&>(i_identity.m_handle);
    EndVkCommandBuffer(cmd_buffer);
}

void VulkanManager::FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool)
{
    VkCommandBuffer &cmd_buffer = reinterpret_cast<VkCommandBuffer&>(io_identity.m_handle);
    const VkDevice  &device = reinterpret_cast<VkDevice&>(io_identity.m_device);
    const CommandPoolIdentity cp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_pool);
    VkCommandPool cmd_pool = reinterpret_cast<VkCommandPool>(cp_identity.m_handle);
    FreeVkCommandBuffer(cmd_buffer, device, cmd_pool);
    io_identity.SetInvalid();
    io_identity = CommandBufferIdentity();
}

void VulkanManager::ExecuteCommandsToPrimaryCommandBuffer(const CommandBufferWeakReferenceObject &i_primary_cb, const std::list<CommandBufferWeakReferenceObject> &i_secondary_cbs)
{
    std::vector<VkCommandBuffer> sec_cmd_buffers;
    for (const CommandBufferWeakReferenceObject &cb : i_secondary_cbs) {
        if (cb.IsNull() == false) {
            const CommandBufferIdentity &sec_cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(cb);
            sec_cmd_buffers.push_back(reinterpret_cast<VkCommandBuffer>(sec_cb_identity.m_handle));
        }
    }

    const CommandBufferIdentity &pri_cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_primary_cb);
    VkCommandBuffer pri_cmd_buffer = reinterpret_cast<VkCommandBuffer>(pri_cb_identity.m_handle);

    if (sec_cmd_buffers.size() > 0 && pri_cmd_buffer != VK_NULL_HANDLE) {
        ExecuteVkSecondaryCommandBuffersToPrimaryVkCommandBuffer(pri_cmd_buffer, sec_cmd_buffers);
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________