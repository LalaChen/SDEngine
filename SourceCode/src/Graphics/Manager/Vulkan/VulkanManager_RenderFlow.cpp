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

void VulkanManager::CreateRenderPass(RenderPassIdentity &io_identity)
{
    VkResult result = VK_SUCCESS;
    VkRenderPass &rp_handle = reinterpret_cast<VkRenderPass&>(io_identity.m_rp_handle);
    std::vector<VkAttachmentDescription> vk_attachment_descs;
    std::vector<VkSubpassDescription> vk_subpass_descs;
    std::vector<VkSubpassDependency> vk_subpass_dependecies;

    std::vector < std::vector<VkAttachmentReference> > vk_sps_color_att_refs;
    std::vector < std::vector<VkAttachmentReference> > vk_sps_res_att_refs;
    std::vector < std::vector<VkAttachmentReference> > vk_sps_input_att_refs;
    std::vector<VkAttachmentReference> vk_sps_depth_refs;
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
            dep_att_ref.attachment = sp.m_depth_attachment_ref.m_attachment_ID;
            dep_att_ref.layout = ImageLayout_Vulkan::Convert(sp.m_depth_attachment_ref.m_layout);
            vk_sps_depth_refs.push_back(dep_att_ref);
            vk_sp_desc.pDepthStencilAttachment = &(vk_sps_depth_refs.back());
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

______________SD_END_GRAPHICS_NAMESPACE______________