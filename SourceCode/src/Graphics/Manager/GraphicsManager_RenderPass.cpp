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
#include "UniformBufferDescriptor.h"
#include "UniformImagesDescriptor.h"
#include "DescriptorSetLayout.h"
#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void GraphicsManager::InitializeDefaultRenderPasses()
{
    SDLOG("Create default render pass");

    //1. Initialize Forward render pass.
    RenderPassStrongReferenceObject forward_pass = new RenderPass("ForwardPass");
    //1.1. prepare attachment references data.
    std::vector<AttachmentDescription> att_descs;
    AttachmentDescription att_desc;
    //--- Color Attachment for sp0(FirstLight).
    att_desc.m_format = GetDefaultColorBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp0(FirstLight).
    att_desc.m_format = GetDefaultDepthBufferFormat();
    att_desc.m_initial_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);

    //1.2. prepare attachment references data.
    std::vector<SubpassDescription> sp_descs;
    SubpassDescription sp_desc;
    AttachmentReference atta_ref;
    //--- sp0 FirstOrNoLight
    sp_desc = SubpassDescription();
    sp_desc.m_name = "FirstOrNoLight";
    sp_desc.m_bind_point = PipelineBindPoint_GRAPHICS;
    //--- color attachment reference.
    atta_ref.m_attachment_ID = 0;
    atta_ref.m_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    sp_desc.m_color_attachment_refs.push_back(atta_ref);
    //--- depth attachment reference.
    sp_desc.m_depth_attachment_ref.m_attachment_ID = 1;
    sp_desc.m_depth_attachment_ref.m_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    sp_descs.push_back(sp_desc);

    //1.3 SubpassDependency.
    std::vector<SubpassDependency> sp_denps;
    SubpassDependency sp_denp;
    //sp external to sp0
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_dst_spID = 0;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);
    //sp0 and sp external
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = 0;
    sp_denp.m_dst_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);

    forward_pass.GetRef().AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    forward_pass.GetRef().Initialize();

    RegisterRenderPass(forward_pass);
}

void GraphicsManager::RegisterRenderPass(const RenderPassStrongReferenceObject &i_rp_sref)
{
    if (i_rp_sref.IsNull() == false) {
        std::map<ObjectName, RenderPassStrongReferenceObject>::iterator rp_iter = m_rp_map.find(i_rp_sref.GetRef().GetObjectName());
          
        if (rp_iter == m_rp_map.end()) {
            SDLOG("Register render pass[%s] to renderpass list.", i_rp_sref.GetRef().GetObjectName().c_str());
            m_rp_map[i_rp_sref.GetRef().GetObjectName()] = i_rp_sref;
        }
        else {
            SDLOGE("Register render pass[%s] to renderpass map failure. It already exist.", i_rp_sref.GetRef().GetObjectName().c_str());
        }
    }
    else {
        SDLOGE("Null render pass.");
    }
}

void GraphicsManager::UnregisterRenderPass(const ObjectName &i_target_rp_name)
{
    std::map<ObjectName, RenderPassStrongReferenceObject>::iterator rp_iter = m_rp_map.find(i_target_rp_name);
    if (rp_iter != m_rp_map.end()) {
        m_rp_map.erase(rp_iter);
    }
    else {
        SDLOGW("RenderPass[%s] isn't in render pass map.", i_target_rp_name.c_str());
    }
}

void GraphicsManager::ReleaseRenderPasses()
{
    SDLOG("Release render passes");
    for (std::map<ObjectName, RenderPassStrongReferenceObject>::iterator rp_sref_iter = m_rp_map.begin(); rp_sref_iter != m_rp_map.end(); ) {
        rp_sref_iter = m_rp_map.erase(rp_sref_iter);
    }
}

RenderPassWeakReferenceObject GraphicsManager::GetRenderPass(const ObjectName &i_target_rp_name) const
{
    RenderPassWeakReferenceObject target_rp_wref;
    for (std::map<ObjectName, RenderPassStrongReferenceObject>::const_iterator rp_sref_iter = m_rp_map.begin(); rp_sref_iter != m_rp_map.end(); ) {
        if ((*rp_sref_iter).second.GetRef().GetObjectName().compare(i_target_rp_name) == 0) {
            target_rp_wref = (*rp_sref_iter).second;
            break;
        }
        else {
            rp_sref_iter++;
        }
    }
    return target_rp_wref;
}

______________SD_END_GRAPHICS_NAMESPACE______________