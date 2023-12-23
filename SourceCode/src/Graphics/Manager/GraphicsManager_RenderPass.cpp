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
#include "GraphicsManager.h"
#include "UniformBufferDescriptor.h"
#include "UniformImagesDescriptor.h"
#include "DescriptorSetLayout.h"
#include "LogManager.h"

//#define __SD_HARD_CODE__

_____________SD_START_GRAPHICS_NAMESPACE_____________

void GraphicsManager::InitializeDefaultRenderPasses()
{
    SDLOG("Create default render pass");

#if defined(__SD_HARD_CODE__)
    //1. Initialize render pass.
    //--- ForwardPass
    RenderPassStrongReferenceObject forward_pass = new RenderPass(sRenderPass_Forward);
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

    SD_SREF(forward_pass).AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    SD_SREF(forward_pass).Initialize();

    RegisterRenderPass(forward_pass);

    //--- ForwardPass (VR Version)
    RenderPassStrongReferenceObject vr_forward_pass = new RenderPass(sRenderPass_VRForward);
    MultiviewInfo mv_info;
    mv_info.m_view_masks.resize(sp_descs.size());
    for (uint32_t count = 0; count < mv_info.m_view_masks.size(); ++count) {
        mv_info.m_view_masks[count] = 0b00000011;
    }
    mv_info.m_correlation_masks.resize(sp_descs.size());
    for (uint32_t count = 0; count < mv_info.m_correlation_masks.size(); ++count) {
        mv_info.m_correlation_masks[count] = 0b00000011;
    }

    SD_SREF(vr_forward_pass).AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    SD_SREF(vr_forward_pass).SetMultiviewInfo(mv_info);
    SD_SREF(vr_forward_pass).Initialize();

    RegisterRenderPass(vr_forward_pass);

    att_descs.clear();
    sp_descs.clear();
    sp_denps.clear();
    //--- GUIRenderPass
    RenderPassStrongReferenceObject gui_pass = new RenderPass(sRenderPass_GUI);
    //--- Color Attachment for sp0(GUI).
    att_desc.m_format = GetDefaultColorBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp0(GUI).
    att_desc.m_format = GetDefaultDepthBufferFormat();
    att_desc.m_initial_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //1.2. prepare attachment references data.
    //--- sp0 FirstOrNoLight
    sp_desc = SubpassDescription();
    sp_desc.m_name = "GUI";
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

    SD_SREF(gui_pass).AddRenderPassDescription(att_descs, sp_descs, sp_denps);
    SD_SREF(gui_pass).Initialize();
    RegisterRenderPass(gui_pass);
#else
    RenderPassStrongReferenceObject rp;
    rp = new RenderPass(sRenderPass_Forward);
    SD_SREF(rp).AddRenderPassDescriptionFromFile("Common/RenderPass/ForwardPass.json");
    SD_SREF(rp).Initialize();
    RegisterRenderPass(rp);

    rp = new RenderPass(sRenderPass_VRForward);
    SD_SREF(rp).AddRenderPassDescriptionFromFile("Common/RenderPass/VRForwardPass.json");
    SD_SREF(rp).Initialize();
    RegisterRenderPass(rp);

    rp = new RenderPass(sRenderPass_GUI);
    SD_SREF(rp).AddRenderPassDescriptionFromFile("Common/RenderPass/GUIPass.json");
    SD_SREF(rp).Initialize();
    RegisterRenderPass(rp);

    rp = new RenderPass(sRenderPass_Composing);
    SD_SREF(rp).AddRenderPassDescriptionFromFile("Common/RenderPass/CompositorPass.json");
    SD_SREF(rp).Initialize();
    RegisterRenderPass(rp);

    rp = new RenderPass(sRenderPass_VRComposing);
    SD_SREF(rp).AddRenderPassDescriptionFromFile("Common/RenderPass/VRCompositorPass.json");
    SD_SREF(rp).Initialize();
    RegisterRenderPass(rp);
#endif
    InitializeDefaultRenderPassesImpl();
}

void GraphicsManager::InitializeDefaultRenderPassesImpl()
{
}

void GraphicsManager::ReleaseRenderPasses()
{
    SDLOG("Release render passes");
    for (std::map<ObjectName, RenderPassStrongReferenceObject>::iterator rp_iter = m_rp_map.begin(); rp_iter != m_rp_map.end(); ) {
        rp_iter = m_rp_map.erase(rp_iter);
    }
}

void GraphicsManager::RegisterRenderPass(const RenderPassStrongReferenceObject &i_rp)
{
    if (i_rp.IsNull() == false) {
        std::map<ObjectName, RenderPassStrongReferenceObject>::iterator rp_iter = m_rp_map.find(SD_SREF(i_rp).GetObjectName());
          
        if (rp_iter == m_rp_map.end()) {
            SDLOG("Register render pass[%s] to renderpass list.", SD_SREF(i_rp).GetObjectName().c_str());
            m_rp_map[SD_SREF(i_rp).GetObjectName()] = i_rp;
        }
        else {
            SDLOGE("Register render pass[%s] to renderpass map failure. It already exist.", SD_SREF(i_rp).GetObjectName().c_str());
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

RenderPassWeakReferenceObject GraphicsManager::GetRenderPass(const ObjectName &i_target_rp_name) const
{
    RenderPassWeakReferenceObject target_rp;
    for (std::map<ObjectName, RenderPassStrongReferenceObject>::const_iterator rp_iter = m_rp_map.begin(); rp_iter != m_rp_map.end(); ) {
        if (SD_SREF((*rp_iter).second).GetObjectName().compare(i_target_rp_name) == 0) {
            target_rp = (*rp_iter).second;
            break;
        }
        else {
            rp_iter++;
        }
    }
    return target_rp;
}

______________SD_END_GRAPHICS_NAMESPACE______________