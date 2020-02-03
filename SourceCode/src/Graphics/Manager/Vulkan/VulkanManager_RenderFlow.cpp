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

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content)
{
    VkShaderModule &shader_module = reinterpret_cast<VkShaderModule&>(io_identity.m_prog_handle);
    VkShaderStageFlagBits shader_stage = ShaderKind_Vulkan::Convert(io_identity.m_shader_kind);
    CreateVKShaderModule(shader_module, shader_stage, i_content.data(), i_content.size());
}

void VulkanManager::DeleteShaderModule(ShaderModuleIdentity &io_identity)
{
    VkShaderModule &shader_module = reinterpret_cast<VkShaderModule&>(io_identity.m_prog_handle);
    DestroyVKShaderModule(shader_module);
}

//-------- RenderPass --------
void VulkanManager::CreateRenderPass(RenderPassIdentity &io_identity)
{
    VkResult result = VK_SUCCESS;
    VkRenderPass &rp_handle = reinterpret_cast<VkRenderPass&>(io_identity.m_rp_handle);
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

    result = CreateVKRenderPass(rp_handle, vk_attachment_descs, vk_subpass_descs, vk_subpass_dependecies);
    if (result != VK_SUCCESS) {
        SDLOGE("Create render pass failure(%d).", result);
    }
}

void VulkanManager::DestroyRenderPass(RenderPassIdentity &io_identity)
{
    VkRenderPass &rp_handle = reinterpret_cast<VkRenderPass&>(io_identity.m_rp_handle);
    DestroyVKRenderPass(rp_handle);
}

void VulkanManager::BeginRenderPass(const CompHandle i_cmd_buffer_handle, const FrameBufferIdentity &i_fb_identity, const ImageOffset &i_start_pos, const ImageSize &i_render_size, const std::vector<ClearValue> &i_clear_values)
{
    VkCommandBuffer cmd_handle = reinterpret_cast<VkCommandBuffer>(i_cmd_buffer_handle);
    VkRenderPass rp_handle = reinterpret_cast<VkRenderPass>(i_fb_identity.m_rp_handle);
    VkFramebuffer fb_handle = reinterpret_cast<VkFramebuffer>(i_fb_identity.m_fb_handle);
    VkRect2D render_area = {};
    Size_ui32 final_w = i_render_size.m_width, final_h = i_render_size.m_height;

    if (final_w + i_start_pos.m_x > i_fb_identity.m_size.m_width) {
        final_w = i_fb_identity.m_size.m_width - i_start_pos.m_x;
    }
    if (final_h + i_start_pos.m_y > i_fb_identity.m_size.m_height) {
        final_h = i_fb_identity.m_size.m_height - i_start_pos.m_y;
    }

    render_area.offset.x = i_start_pos.m_x;
    render_area.offset.y = i_start_pos.m_y;
    render_area.extent.width = final_w;
    render_area.extent.height = final_h;

    std::vector<VkClearValue> clear_values;
    clear_values.resize(i_clear_values.size());
    for (uint32_t i = 0; i < clear_values.size(); ++i) {
        const UBytePtr dst_ptr = reinterpret_cast<const UBytePtr>(&clear_values[i]);
        const UBytePtr src_ptr = (const UBytePtr)(&i_clear_values[i]);
        size_t dst_size = sizeof(VkClearValue);
        std::memcpy(dst_ptr, src_ptr, dst_size);
    }

    BeginVkRenderPass(cmd_handle, rp_handle, fb_handle, render_area, clear_values);
}

void VulkanManager::GoToNextStepOfRenderPass(const CompHandle i_cmd_buffer_handle, const FrameBufferGroupIdentity &i_target_fbg_identity)
{
    VkCommandBuffer cmd_handle = reinterpret_cast<VkCommandBuffer>(i_cmd_buffer_handle);
    GotoNextStepInVKRenderPass(cmd_handle);
}

void VulkanManager::EndRenderPass(const CompHandle i_cmd_buffer_handle)
{
    VkCommandBuffer cmd_handle = reinterpret_cast<VkCommandBuffer>(i_cmd_buffer_handle);
    EndVkRenderPass(cmd_handle);
}

//-------- FrameBuffer --------
void VulkanManager::CreateFrameBuffer(FrameBufferIdentity &io_identity, const std::vector<TextureWeakReferenceObject> &i_buf_wrefs)
{
    VkResult result = VK_SUCCESS;
    //1. create all image view of this framebuffer
    std::vector<VkImageView> ivs;
    for (uint32_t iv_ID = 0; iv_ID < io_identity.m_iv_identities.size(); ++iv_ID) {
        ImageViewIdentity& iv_identity = io_identity.m_iv_identities[iv_ID];
        VkImageView &iv_handle = reinterpret_cast<VkImageView&>(iv_identity.m_iv_handle);
        const VkImage img_handle = reinterpret_cast<const VkImage>(i_buf_wrefs[iv_ID].GetConstRef().GetHandle());
        
        TextureTypeEnum tex_type = i_buf_wrefs[iv_ID].GetConstRef().GetTextureType();
        VkImageViewType vk_iv_type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
        if (tex_type == TextureType_TEXTURE_1D) {
            if (iv_identity.m_layer_count == 1) {
                vk_iv_type = VK_IMAGE_VIEW_TYPE_1D;
            }
            else {
                vk_iv_type = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
            }
        }
        else if (tex_type == TextureType_TEXTURE_2D) {
            if (iv_identity.m_layer_count == 1) {
                vk_iv_type = VK_IMAGE_VIEW_TYPE_2D;
            }
            else {
                vk_iv_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            }
        }
        else if (tex_type == TextureType_TEXTURE_CUBE_MAP) {
            if (iv_identity.m_layer_count == 1) {
                vk_iv_type = VK_IMAGE_VIEW_TYPE_CUBE;
            }
            else {
                vk_iv_type = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
            }
        }
        else {
            SDLOGE("We can't create image view for tex[%s](%d)", i_buf_wrefs[iv_ID].GetConstRef().GetObjectName().c_str(), tex_type);
            return;
        }

        VkFormat vk_format = TextureFormat_Vulkan::Convert(iv_identity.m_format);
        VkImageSubresourceRange img_sub_range = {};
        img_sub_range.aspectMask = ImageAspect_Vulkan::Convert(iv_identity.m_aspect);
        img_sub_range.baseMipLevel = iv_identity.m_base_mip_level;
        img_sub_range.levelCount = iv_identity.m_level_count;
        img_sub_range.baseArrayLayer = iv_identity.m_base_array_level;
        img_sub_range.layerCount = iv_identity.m_layer_count;

        VkComponentMapping comp_mapping = {};
        comp_mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        comp_mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        comp_mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        comp_mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        
        result = CreateVkImageView(iv_handle, img_handle, vk_iv_type, vk_format, comp_mapping, img_sub_range);
        if (result != VK_SUCCESS) {
            SDLOGE("Create image view for texture[%s] failure(%d).", i_buf_wrefs[iv_ID].GetConstRef().GetObjectName().c_str(), result);
            return;
        }
        else {
            ivs.push_back(iv_handle);
        }
    }
    //2. create framebuffer.
    VkFramebuffer &fb_handle = reinterpret_cast<VkFramebuffer&>(io_identity.m_fb_handle);
    VkRenderPass &rp_handle = reinterpret_cast<VkRenderPass&>(io_identity.m_rp_handle);
    result = CreateVKFrameBuffer(fb_handle, rp_handle, ivs, io_identity.m_size.m_width, io_identity.m_size.m_height, io_identity.m_size.m_length);
    if (result != VK_SUCCESS) {
        SDLOGE("Create Framebuffer failure(%d)!!!.",  result);
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
    for (ImageViewIdentity &iv_identity : io_identity.m_iv_identities) {
        VkImageView &iv_handle = reinterpret_cast<VkImageView&>(iv_identity.m_iv_handle);
        DestroyVkImageView(iv_handle);
        iv_identity = ImageViewIdentity();
    }
    VkFramebuffer &fb_handle = reinterpret_cast<VkFramebuffer&>(io_identity.m_fb_handle);
    DestroyVkFrameBuffer(fb_handle);
    io_identity = FrameBufferIdentity();
}

______________SD_END_GRAPHICS_NAMESPACE______________