#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void GraphicsManager::InitializeDefaultRenderPasses()
{
    SDLOG("Create default render pass");
    //1. Initialize Forward render pass.
    RenderPassStrongReferenceObject forward_pass = new RenderPass("ForwardPath");
    //1.1. prepare attachment references data.
    std::vector<AttachmentDescription> att_descs;
    AttachmentDescription att_desc;
    //--- Color Attachment for sp0(FirstLight).
    att_desc.m_format = TextureFormat_R8G8B8A8_UNORM;
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp0(FirstLight).
    att_desc.m_format = TextureFormat_D32_SFLOAT_S8_UINT;
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_STORE;
    att_descs.push_back(att_desc);

    //--- Color Attachment for sp1(SecondLight).
    att_desc.m_format = TextureFormat_R8G8B8A8_UNORM;
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp1(SecondLight).
    att_desc.m_format = TextureFormat_D32_SFLOAT_S8_UINT;
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_STORE;
    att_descs.push_back(att_desc);

    //--- Color Attachment for sp2(TransparentObject).
    att_desc.m_format = TextureFormat_R8G8B8A8_UNORM;
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp2(TransparentObject).
    att_desc.m_format = TextureFormat_D32_SFLOAT_S8_UINT;
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_STORE;
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

    //--- sp1 SecondLight
    sp_desc = SubpassDescription();
    sp_desc.m_name = "SecondLight";
    sp_desc.m_bind_point = PipelineBindPoint_GRAPHICS;
    //--- color attachment reference.
    atta_ref.m_attachment_ID = 2;
    atta_ref.m_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    sp_desc.m_color_attachment_refs.push_back(atta_ref);
    //--- depth attachment reference.
    sp_desc.m_depth_attachment_ref.m_attachment_ID = 3;
    sp_desc.m_depth_attachment_ref.m_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    sp_descs.push_back(sp_desc);
    
    //--- sp2 Transparent.
    sp_desc = SubpassDescription();
    sp_desc.m_name = "Transparent";
    sp_desc.m_bind_point = PipelineBindPoint_GRAPHICS;
    //--- color attachment reference.
    atta_ref.m_attachment_ID = 4;
    atta_ref.m_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    sp_desc.m_color_attachment_refs.push_back(atta_ref);
    //--- depth attachment reference.
    sp_desc.m_depth_attachment_ref.m_attachment_ID = 5;
    sp_desc.m_depth_attachment_ref.m_layout = ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    sp_descs.push_back(sp_desc);

    //1.3 SubpassDependency.
    std::vector<SubpassDependency> sp_denps;
    SubpassDependency sp_denp;
    //sp external to 0
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = SD_SUBPASS_EXTERNAL;
    sp_denp.m_dst_spID = 0;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);
    //sp0 and 1
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = 0;
    sp_denp.m_dst_spID = 1;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);
    //sp1 and 2
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = 1;
    sp_denp.m_dst_spID = 2;
    sp_denp.m_src_pipeline_stages.push_back(PipelineStage_BOTTOM_OF_PIPE);
    sp_denp.m_dst_pipeline_stages.push_back(PipelineStage_TOP_OF_PIPE);
    sp_denp.m_src_mem_masks.push_back(MemoryAccessMask_MEMORY_WRITE);
    sp_denp.m_dst_mem_masks.push_back(MemoryAccessMask_MEMORY_READ);
    sp_denp.m_dependencies.push_back(DependencyScope_REGION);
    sp_denps.push_back(sp_denp);
    //sp2 and external
    sp_denp = SubpassDependency();
    sp_denp.m_src_spID = 2;
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
        bool is_exist = false;
        for (RenderPassStrongReferenceObject& rp_sref : m_rp_list) {
            if (rp_sref.GetRef().GetObjectName().compare(i_rp_sref.GetRef().GetObjectName()) == 0) {
                is_exist = true;
                break;
            }
        }
        if (is_exist == false) {
            SDLOG("Register render pass[%s] to renderpass list.", i_rp_sref.GetRef().GetObjectName().c_str());
            m_rp_list.push_back(i_rp_sref);
        }
    }
    else {
        SDLOGE("Null render pass.");
    }
}

void GraphicsManager::UnregisterRenderPass(const ObjectName &i_target_rp_name)
{
    for (std::list<RenderPassStrongReferenceObject>::iterator rp_sref_iter = m_rp_list.begin(); rp_sref_iter != m_rp_list.end(); ) {
        if ((*rp_sref_iter).GetRef().GetObjectName().compare(i_target_rp_name) == 0) {
            rp_sref_iter = m_rp_list.erase(rp_sref_iter);
            break;
        }
        else {
            rp_sref_iter++;
        }
    }
}

void GraphicsManager::ReleaseRenderPasses()
{
    SDLOG("Release render passes");
    for (std::list<RenderPassStrongReferenceObject>::iterator rp_sref_iter = m_rp_list.begin(); rp_sref_iter != m_rp_list.end(); ) {
        rp_sref_iter = m_rp_list.erase(rp_sref_iter);
    }
}

RenderPassWeakReferenceObject GraphicsManager::GetRenderPass(const ObjectName& i_target_rp_name) const
{
    RenderPassWeakReferenceObject target_rp_wref;
    for (std::list<RenderPassStrongReferenceObject>::const_iterator rp_sref_iter = m_rp_list.begin(); rp_sref_iter != m_rp_list.end(); ) {
        if ((*rp_sref_iter).GetRef().GetObjectName().compare(i_target_rp_name) == 0) {
            target_rp_wref = (*rp_sref_iter);
            break;
        }
        else {
            rp_sref_iter++;
        }
    }
    return target_rp_wref;
}

______________SD_END_GRAPHICS_NAMESPACE______________