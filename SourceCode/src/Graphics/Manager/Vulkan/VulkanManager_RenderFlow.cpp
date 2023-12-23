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

#include "ImageAspect_Vulkan.h"
#include "TextureFormat_Vulkan.h"
#include "SampleCount_Vulkan.h"
#include "AttachmentOperator_Vulkan.h"
#include "ImageLayout_Vulkan.h"
#include "PipelineBindPoint_Vulkan.h"
#include "MemoryAccessMask_Vulkan.h"
#include "PipelineStage_Vulkan.h"
#include "DependencyScope_Vulkan.h"
#include "ShaderKind_Vulkan.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________
//-------- RenderPass --------
void VulkanManager::CreateRenderPass(RenderPassIdentity &io_identity)
{
    VkResult result = VK_SUCCESS;
    VkRenderPass &render_pass = reinterpret_cast<VkRenderPass&>(io_identity.m_handle);
    VkDevice     &device      = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;
    std::vector<VkAttachmentDescription> vk_attachment_descs;
    std::vector<VkSubpassDescription> vk_subpass_descs;
    std::vector<VkSubpassDependency> vk_subpass_dependecies;

    std::vector< std::vector<VkAttachmentReference> > vk_sps_color_att_refs;
    std::vector< std::vector<VkAttachmentReference> > vk_sps_res_att_refs;
    std::vector< std::vector<VkAttachmentReference> > vk_sps_input_att_refs;
    std::vector< std::vector<VkAttachmentReference> > vk_sps_depth_refs;
    //---- attachment description.
    for (AttachmentDescription &d : io_identity.m_attachment_descs) {
        VkAttachmentDescription vk_att_desc = {};
        vk_att_desc.flags          = 0;
        vk_att_desc.format         = TextureFormat_Vulkan::Convert(d.m_format);
        vk_att_desc.samples        = SampleCount_Vulkan::Convert(d.m_sample_counts);
        vk_att_desc.loadOp         = AttachmentLoadOperator_Vulkan::Convert(d.m_load_op);
        vk_att_desc.storeOp        = AttachmentStoreOperator_Vulkan::Convert(d.m_store_op);
        vk_att_desc.stencilLoadOp  = AttachmentLoadOperator_Vulkan::Convert(d.m_stencil_load_op);
        vk_att_desc.stencilStoreOp = AttachmentStoreOperator_Vulkan::Convert(d.m_stencil_store_op);
        vk_att_desc.initialLayout  = ImageLayout_Vulkan::Convert(d.m_initial_layout);
        vk_att_desc.finalLayout    = ImageLayout_Vulkan::Convert(d.m_final_layout);
        vk_attachment_descs.push_back(vk_att_desc);
    }
    //---- Subpass description.
    for (SubpassDescription &sp : io_identity.m_subpasses_descs) {
        VkSubpassDescription vk_sp_desc = {};
        vk_sp_desc.flags = 0;
        vk_sp_desc.pipelineBindPoint = PipelineBindPoint_Vulkan::Convert(sp.m_bind_point);

        if (sp.m_color_attachment_refs.size() > 0) {
            std::vector<VkAttachmentReference> vk_sp_color_att_refs;
            for (AttachmentReference &ref : sp.m_color_attachment_refs) {
                VkAttachmentReference vk_ref = {};
                vk_ref.attachment = ref.m_attachment_ID;
                vk_ref.layout = ImageLayout_Vulkan::Convert(ref.m_layout);
                vk_sp_color_att_refs.push_back(vk_ref);
            }
            vk_sps_color_att_refs.push_back(vk_sp_color_att_refs);
            vk_sp_desc.colorAttachmentCount = static_cast<uint32_t>(vk_sps_color_att_refs.back().size());
            vk_sp_desc.pColorAttachments = vk_sps_color_att_refs.back().data();
            //
            if (sp.m_res_attachment_refs.size() == sp.m_color_attachment_refs.size()) {
                std::vector<VkAttachmentReference> vk_sp_res_att_refs;
                for (AttachmentReference &ref : sp.m_res_attachment_refs) {
                    VkAttachmentReference vk_ref = {};
                    vk_ref.attachment = ref.m_attachment_ID;
                    vk_ref.layout = ImageLayout_Vulkan::Convert(ref.m_layout);
                    vk_sp_res_att_refs.push_back(vk_ref);
                }
                vk_sps_res_att_refs.push_back(vk_sp_res_att_refs);
                vk_sp_desc.pResolveAttachments = vk_sps_res_att_refs.back().data(); //size is same with color attachment.
            }
        }
        else {
            vk_sp_desc.colorAttachmentCount = 0;
            vk_sp_desc.pColorAttachments = nullptr;
            vk_sp_desc.pResolveAttachments = nullptr;
        }

        if (sp.m_input_attachment_refs.size() > 0) {
            std::vector<VkAttachmentReference> vk_sp_input_att_refs;
            for (AttachmentReference &ref : sp.m_input_attachment_refs) {
                VkAttachmentReference vk_ref = {};
                vk_ref.attachment = ref.m_attachment_ID;
                vk_ref.layout = ImageLayout_Vulkan::Convert(ref.m_layout);
                vk_sp_input_att_refs.push_back(vk_ref);
            }
            vk_sps_input_att_refs.push_back(vk_sp_input_att_refs);
            vk_sp_desc.inputAttachmentCount = static_cast<uint32_t>(vk_sps_input_att_refs.back().size());
            vk_sp_desc.pInputAttachments = vk_sps_input_att_refs.back().data();
        }
        else {
            vk_sp_desc.inputAttachmentCount = 0;
            vk_sp_desc.pInputAttachments = nullptr;
        }

        if (sp.m_pre_attachment_ref_ids.size() > 0) {
            vk_sp_desc.preserveAttachmentCount = static_cast<uint32_t>(sp.m_pre_attachment_ref_ids.size());
            vk_sp_desc.pPreserveAttachments = sp.m_pre_attachment_ref_ids.data();
        }
        else {
            vk_sp_desc.preserveAttachmentCount = 0;
            vk_sp_desc.pPreserveAttachments = nullptr;
        }

        if (sp.m_depth_attachment_ref.m_attachment_ID != SD_ERROR_ATTACHMENT_REF) {
            VkAttachmentReference dep_att_ref = {};
            std::vector<VkAttachmentReference> vk_sp_depth_att_refs;
            dep_att_ref.attachment = sp.m_depth_attachment_ref.m_attachment_ID;
            dep_att_ref.layout = ImageLayout_Vulkan::Convert(sp.m_depth_attachment_ref.m_layout);
            vk_sp_depth_att_refs.push_back(dep_att_ref);
            vk_sps_depth_refs.push_back(vk_sp_depth_att_refs);
            vk_sp_desc.pDepthStencilAttachment = vk_sps_depth_refs.back().data();
        }
        else {
            vk_sp_desc.pDepthStencilAttachment = nullptr;
        }
        //
        vk_subpass_descs.push_back(vk_sp_desc);
    }

    //---- Subpass dependencies.
    for (SubpassDependency &dp : io_identity.m_sp_dependencies) {
        VkSubpassDependency vk_sp_dep = {};
        vk_sp_dep.dependencyFlags = DependencyScope_Vulkan::Convert(dp.m_dependencies);
        vk_sp_dep.srcSubpass = dp.m_src_spID;
        vk_sp_dep.srcAccessMask = MemoryAccess_Vulkan::Convert(dp.m_src_mem_masks);
        vk_sp_dep.srcStageMask = PipelineStage_Vulkan::Convert(dp.m_src_pipeline_stages);
        vk_sp_dep.dstSubpass = dp.m_dst_spID;
        vk_sp_dep.dstAccessMask = MemoryAccess_Vulkan::Convert(dp.m_dst_mem_masks);
        vk_sp_dep.dstStageMask = PipelineStage_Vulkan::Convert(dp.m_dst_pipeline_stages);
        vk_subpass_dependecies.push_back(vk_sp_dep);
    }

    VkRenderPassCreateInfo rp_c_info = InitializeVkRenderPassCreateInfo(
        vk_attachment_descs, vk_subpass_descs, vk_subpass_dependecies
    );

    if (io_identity.m_multiview_info.m_view_masks.size() > 0) {
        VkRenderPassMultiviewCreateInfo rpm_c_info = InitializeVkRenderPassMultiviewCreateInfo(
            io_identity.m_multiview_info.m_view_masks,
            io_identity.m_multiview_info.m_view_offsets,
            io_identity.m_multiview_info.m_correlation_masks
        );
        rp_c_info.pNext = &rpm_c_info;
    }

    result = CreateVKRenderPass(render_pass, device, rp_c_info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create render pass failure(%d).", result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::DestroyRenderPass(RenderPassIdentity &io_identity)
{
    VkRenderPass &rp_handle = reinterpret_cast<VkRenderPass&>(io_identity.m_handle);
    VkDevice     &device    = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;
    DestroyVKRenderPass(rp_handle, device);
    io_identity.SetInvalid();
    io_identity = RenderPassIdentity();
}

void VulkanManager::BeginRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, const RenderPassWeakReferenceObject &i_rp, const ImageOffset &i_start_pos, const ImageSize &i_render_size)
{
    const FrameBufferIdentity &fb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_fb);
    const CommandBufferIdentity &cmd_buf_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
    const RenderPassIdentity &rp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_rp);
    VkCommandBuffer cmd_buffer  = reinterpret_cast<VkCommandBuffer>(cmd_buf_identity.m_handle);
    VkRenderPass    render_pass = reinterpret_cast<VkRenderPass>(rp_identity.m_handle);
    VkFramebuffer   framebuffer = reinterpret_cast<VkFramebuffer>(fb_identity.m_handle);
    VkRect2D render_area = {};
    Size_ui32 final_w = i_render_size.m_width, final_h = i_render_size.m_height;

    if (final_w + i_start_pos.m_x > fb_identity.m_size.m_width) {
        final_w = fb_identity.m_size.m_width - i_start_pos.m_x;
    }
    if (final_h + i_start_pos.m_y > fb_identity.m_size.m_height) {
        final_h = fb_identity.m_size.m_height - i_start_pos.m_y;
    }

    render_area.offset.x = i_start_pos.m_x;
    render_area.offset.y = i_start_pos.m_y;
    render_area.extent.width = final_w;
    render_area.extent.height = final_h;

    std::vector<VkClearValue> clear_values;
    clear_values.resize(fb_identity.m_clear_values.size());
    for (uint32_t i = 0; i < clear_values.size(); ++i) {
        const UBytePtr dst_ptr = reinterpret_cast<const UBytePtr>(&clear_values[i]);
        const UBytePtr src_ptr = (const UBytePtr)(&fb_identity.m_clear_values[i]);
        size_t dst_size = sizeof(VkClearValue);
        std::memcpy(dst_ptr, src_ptr, dst_size);
    }

    BeginVkRenderPass(cmd_buffer, render_pass, framebuffer, render_area, clear_values);
}

void VulkanManager::GoToNextStepOfRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, uint32_t i_sp_id)
{
    const CommandBufferIdentity &cmd_buf_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
    VkCommandBuffer cmd_buffer = reinterpret_cast<VkCommandBuffer>(cmd_buf_identity.m_handle);
    GotoNextStepInVKRenderPass(cmd_buffer);
}

void VulkanManager::EndRenderPass(const CommandBufferWeakReferenceObject &i_cb)
{
    const CommandBufferIdentity& cb_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_cb);
    VkCommandBuffer cmd_buffer = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    EndVkRenderPass(cmd_buffer);
}

//-------- FrameBuffer --------
void VulkanManager::CreateFrameBuffer(FrameBufferIdentity &io_identity, const RenderPassWeakReferenceObject &i_rp, const std::vector<TextureWeakReferenceObject> &i_bufs)
{
    VkResult result = VK_SUCCESS;
    //1. get all image views of this framebuffer
    std::vector<VkImageView> ivs;

    for (const TextureWeakReferenceObject &buf : i_bufs) {
        const TextureIdentity &tex_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(buf);
        ivs.push_back(reinterpret_cast<VkImageView>(tex_identity.m_image_view));
    }

    //2. create framebuffer.
    const RenderPassIdentity &rp_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_rp);
    VkFramebuffer &framebuffer = reinterpret_cast<VkFramebuffer&>(io_identity.m_handle);
    VkRenderPass  render_pass  = reinterpret_cast<VkRenderPass>(rp_identity.m_handle);
    VkDevice      &device      = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;

    result = CreateVKFrameBuffer(framebuffer, device, render_pass, ivs, io_identity.m_size.m_width, io_identity.m_size.m_height, io_identity.m_size.m_length);
    if (result != VK_SUCCESS) {
        SDLOGE("Create Framebuffer failure(%d)!!!.", result);
    }
    else {
        io_identity.SetValid();
    }
}

void VulkanManager::CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity)
{
    //Don't do anything.
}

void VulkanManager::DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity)
{
    //Don't do anything.
}

void VulkanManager::DestroyFrameBuffer(FrameBufferIdentity &io_identity)
{
    VkFramebuffer &framebuffer = reinterpret_cast<VkFramebuffer&>(io_identity.m_handle);
    VkDevice      &device      = reinterpret_cast<VkDevice&>(io_identity.m_device);
    device = m_device;
    DestroyVkFrameBuffer(framebuffer, device);
    io_identity.SetInvalid();
    io_identity = FrameBufferIdentity();
}

______________SD_END_GRAPHICS_NAMESPACE______________