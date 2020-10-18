#include "BasicUniforms.h"
#include "LightUniforms.h"
#include "MaterialUniforms.h"
#include "ModelData.h"
#include "UniformBufferDescriptor.h"
#include "UniformImagesDescriptor.h"
#include "DescriptorSetLayout.h"
#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

DescriptorSetLayoutWeakReferenceObject GraphicsManager::GetBasicDescriptorSetLayout(const ObjectName &i_dsl_name) const
{
    DescriptorSetLayoutWeakReferenceObject result;
    std::map<ObjectName, DescriptorSetLayoutStrongReferenceObject>::const_iterator dsl_iter = m_basic_dsl_maps.find(i_dsl_name);
    if (dsl_iter != m_basic_dsl_maps.end()) {
        return (*dsl_iter).second;
    }
    else {
        return DescriptorSetLayoutWeakReferenceObject();
    }
}

void GraphicsManager::InitializeBasicDescriptorSetLayout()
{
    //1. For Cameras.
    UniformBufferDescriptorStrongReferenceObject camera_ubd_sref = new UniformBufferDescriptor("camera", 0);
    camera_ubd_sref.GetRef().AddVariable("proj", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_proj));
    camera_ubd_sref.GetRef().AddVariable("view", UniformBufferVariableType_MATRIX4X4F, offsetof(CameraUniforms, m_view));
    camera_ubd_sref.GetRef().AddVariable("viewEye", UniformBufferVariableType_VECTOR3F, offsetof(CameraUniforms, m_view_eye));
    camera_ubd_sref.GetRef().AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject camera_dsl_sref = new DescriptorSetLayout("Camera");
    camera_dsl_sref.GetRef().AddUniformVariableDescriptors({ camera_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    camera_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["Camera"] = camera_dsl_sref;

    //2. For Meshes.
    //Use for MVP matrices at all subpasses at Forward Pass and Defered Pass.
    UniformBufferDescriptorStrongReferenceObject basic_ubd_sref = new UniformBufferDescriptor("geometry", 0);
    basic_ubd_sref.GetRef().AddVariable("world", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_worid));
    basic_ubd_sref.GetRef().AddVariable("normal", UniformBufferVariableType_MATRIX4X4F, offsetof(WorldUniforms, m_normal));
    basic_ubd_sref.GetRef().AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject basic_dsl_sref = new DescriptorSetLayout("MeshRender");
    basic_dsl_sref.GetRef().AddUniformVariableDescriptors({ basic_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    basic_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["MeshRender"] = basic_dsl_sref;

    //3. For light.
    //Use for Record Light Parameter.
    UniformBufferDescriptorStrongReferenceObject light_ubd_sref = new UniformBufferDescriptor("light", 0);
    light_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_ambient));
    light_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_diffuse));
    light_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(LightUniforms, m_specular));
    light_ubd_sref.GetRef().AddVariable("position", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_position));
    light_ubd_sref.GetRef().AddVariable("direction", UniformBufferVariableType_VECTOR3F, offsetof(LightUniforms, m_direction));
    light_ubd_sref.GetRef().AddVariable("spotExponent", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_exp));
    light_ubd_sref.GetRef().AddVariable("spotCosCutoff", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_spot_cos_cutoff));
    light_ubd_sref.GetRef().AddVariable("constantAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_constant_attenuation));
    light_ubd_sref.GetRef().AddVariable("linearAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_linear_attenuation));
    light_ubd_sref.GetRef().AddVariable("quadraticAttenuation", UniformBufferVariableType_FLOAT, offsetof(LightUniforms, m_quadratic_attenuation));
    light_ubd_sref.GetRef().AddVariable("kind", UniformBufferVariableType_INT, offsetof(LightUniforms, m_kind));
    light_ubd_sref.GetRef().AddVariableDone();
    DescriptorSetLayoutStrongReferenceObject light_dsl_sref = new DescriptorSetLayout("Light");
    light_dsl_sref.GetRef().AddUniformVariableDescriptors({ light_ubd_sref.StaticCastTo<UniformVariableDescriptor>() });
    light_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["Light"] = light_dsl_sref;

    //4. For Material.
    UniformBufferDescriptorStrongReferenceObject mat_ubd_sref = new UniformBufferDescriptor("material", 0);
    mat_ubd_sref.GetRef().AddVariable("ambient", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_ambient));
    mat_ubd_sref.GetRef().AddVariable("diffuse", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_diffuse));
    mat_ubd_sref.GetRef().AddVariable("specular", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_specular));
    mat_ubd_sref.GetRef().AddVariable("emission", UniformBufferVariableType_COLOR4F, offsetof(MaterialUniforms, m_emission));
    mat_ubd_sref.GetRef().AddVariable("shininess", UniformBufferVariableType_FLOAT, offsetof(MaterialUniforms, m_shininess));
    mat_ubd_sref.GetRef().AddVariableDone();
    UniformImagesDescriptorStrongReferenceObject mt_imgd_sref = new UniformImagesDescriptor("textures", 1, MaterialTextureType_MAX_DEFINE_VALUE);
    DescriptorSetLayoutStrongReferenceObject gen_dsl_sref = new DescriptorSetLayout("Material");
    gen_dsl_sref.GetRef().AddUniformVariableDescriptors({ mat_ubd_sref.StaticCastTo<UniformVariableDescriptor>(), mt_imgd_sref.StaticCastTo<UniformVariableDescriptor>() });
    gen_dsl_sref.GetRef().Initialize();
    m_basic_dsl_maps["Material"] = gen_dsl_sref;
}

void GraphicsManager::InitializeDefaultRenderPasses()
{
    SDLOG("Create default render pass");
    //1. Initialize Forward render pass.
    RenderPassStrongReferenceObject forward_pass = new RenderPass("ForwardPath");
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
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_CLEAR;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_STORE;
    att_descs.push_back(att_desc);

    //--- Color Attachment for sp1(SecondLight).
    att_desc.m_format = GetDefaultColorBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp1(SecondLight).
    att_desc.m_format = GetDefaultDepthBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_STORE;
    att_descs.push_back(att_desc);

    //--- Color Attachment for sp2(TransparentObject).
    att_desc.m_format = GetDefaultColorBufferFormat();
    att_desc.m_initial_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_final_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
    att_desc.m_sample_counts = SampleCount_1;
    att_desc.m_load_op = AttachmentLoadOperator_LOAD;
    att_desc.m_store_op = AttachmentStoreOperator_STORE;
    att_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
    att_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
    att_descs.push_back(att_desc);
    //--- Depth Attachment for sp2(TransparentObject).
    att_desc.m_format = GetDefaultDepthBufferFormat();
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
    sp_desc.m_name = "MainLight";
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
    sp_desc.m_name = "SecondaryLight";
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