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

#include <nlohmann/json.hpp>

#include "GraphicsManager.h"
#include "LogManager.h"
#include "FileResourceRequester.h"
#include "RenderPass.h"

using SDE::Basic::FileData;
using SDE::Basic::FileResourceRequester;

_____________SD_START_GRAPHICS_NAMESPACE_____________

RenderPass::RenderPass(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

RenderPass::~RenderPass()
{
}


void RenderPass::AddRenderPassDescriptionFromFile(const FilePathString &i_file_url)
{
    FileData file_data;
    bool result = FileResourceRequester::GetRef().AskFile(i_file_url, file_data);
    if (result == true) {
        SDLOG("Read rp file[%s] success. Start to initialize rp descs.", i_file_url.c_str());
        AddRenderPassDescriptionFromText(file_data.GetDataString());
    }
    else {
        SDLOGE("fp[%s] isn't exist. Please check!!!", i_file_url.c_str());
    }
}

void RenderPass::AddRenderPassDescriptionFromText(const std::string &i_rp_file)
{
    if (i_rp_file.empty() == false) {
        nlohmann::json rp_root = nlohmann::json::parse(i_rp_file);
        nlohmann::json &rp = rp_root.at("RenderPass");
        //1. load attachment descriptions.
        nlohmann::json &ads_root = rp.at("AttachmentDescriptions");
        for (uint32_t ad_id = 0; ad_id < ads_root.size(); ++ad_id) {
            AttachmentDescription ad_desc;
            nlohmann::json &ad_root = ads_root.at(ad_id);
            bool is_color_attachment = ad_root.at("IsColorAttachment").get<bool>();
            
            int32_t format_type = ad_root.at("FormatType").get<int32_t>();
            
            if (format_type < 0) {
                if (is_color_attachment == true) {
                    ad_desc.m_format = GraphicsManager::GetRef().GetDefaultColorBufferFormat();
                }
                else {
                    ad_desc.m_format = GraphicsManager::GetRef().GetDefaultDepthBufferFormat();
                }
            }
            else {
                ad_desc.m_format = static_cast<TextureFormatEnum>(format_type);
            }

            ad_desc.m_load_op = static_cast<AttachmentLoadOperatorEnum>(ad_root.at("LoadOpType").get<int32_t>());
            if (ad_desc.m_load_op == AttachmentLoadOperator_MAX_DEFINE_VALUE) {
                ad_desc.m_load_op = AttachmentLoadOperator_DONT_CARE;
                SDLOGW("RP(%s): AD[%d] ad_desc.m_load_op is max define value. Mark it dontcare.", m_object_name.c_str(), ad_id);
            }

            ad_desc.m_store_op = static_cast<AttachmentStoreOperatorEnum>(ad_root.at("StoreOpType").get<int32_t>());
            if (ad_desc.m_store_op == AttachmentStoreOperator_MAX_DEFINE_VALUE) {
                ad_desc.m_store_op = AttachmentStoreOperator_DONT_CARE;
                SDLOGW("RP(%s): AD[%d] ad_desc.m_store_op is max define value. Mark it dontcare.", m_object_name.c_str(), ad_id);
            }

            ad_desc.m_stencil_load_op = static_cast<AttachmentLoadOperatorEnum>(ad_root.at("StencilLoadOpType").get<int32_t>());
            if (ad_desc.m_stencil_load_op == AttachmentLoadOperator_MAX_DEFINE_VALUE) {
                ad_desc.m_stencil_load_op = AttachmentLoadOperator_DONT_CARE;
                SDLOGW("RP(%s): AD[%d] ad_desc.m_stencil_load_op is max define value. Mark it dontcare.", m_object_name.c_str(), ad_id);
            }

            ad_desc.m_stencil_store_op = static_cast<AttachmentStoreOperatorEnum>(ad_root.at("StencilStoreOpType").get<int32_t>());
            if (ad_desc.m_stencil_store_op == AttachmentStoreOperator_MAX_DEFINE_VALUE) {
                ad_desc.m_stencil_store_op = AttachmentStoreOperator_DONT_CARE;
                SDLOGW("RP(%s): AD[%d] ad_desc.m_stencil_store_op is max define value. Mark it dontcare.", m_object_name.c_str(), ad_id);
            }

            ad_desc.m_initial_layout = static_cast<ImageLayoutEnum>(ad_root.at("InitialLayoutType").get<int32_t>());
            if (ad_desc.m_initial_layout == ImageLayout_MAX_DEFINE_VALUE) {
                ad_desc.m_initial_layout = ImageLayout_UNDEFINED;
                SDLOGW("RP(%s): AD[%d] ad_desc.m_initial_layout is max define value. Mark it undefined.", m_object_name.c_str(), ad_id);
            }

            ad_desc.m_final_layout = static_cast<ImageLayoutEnum>(ad_root.at("FinalLayoutType").get<int32_t>());
            if (ad_desc.m_final_layout == ImageLayout_MAX_DEFINE_VALUE) {
                ad_desc.m_final_layout = ImageLayout_UNDEFINED;
                SDLOGW("RP(%s): AD[%d] ad_desc.m_final_layout is max define value. Mark it undefined.", m_object_name.c_str(), ad_id);
            }

            m_identity.m_attachment_descs.push_back(ad_desc);
        }
        //2. subpass descriptions.
        nlohmann::json &spds_root = rp.at("SubpassDescriptions");
        for (uint32_t spd_id = 0; spd_id < spds_root.size(); ++spd_id) {
            SubpassDescription sp_desc;
            nlohmann::json &spd_root = spds_root.at(spd_id);
            sp_desc.m_name = spd_root.at("Name").get<std::string>();
            sp_desc.m_bind_point = static_cast<PipelineBindPointEnum>(spd_root.at("BindPoint").get<int32_t>());
            if (sp_desc.m_bind_point == PipelineBindPoint_MAX_DEFINE_VALUE) {
                SDLOGW("RP(%s): SP[%d] sp_desc.m_bind_point is max define value. Mark it undefined.", m_object_name.c_str(), spd_id);
            }
            nlohmann::json &cars_root = spd_root.at("ColorAttachmentRefs");
            for (uint32_t ar_id = 0; ar_id < cars_root.size(); ++ar_id) {
                AttachmentReference att_ref;
                nlohmann::json &ar_root = cars_root.at(ar_id);
                att_ref.m_attachment_ID = ar_root.at("AttachmentID").get<uint32_t>();
                att_ref.m_layout = static_cast<ImageLayoutEnum>(ar_root.at("LayoutType").get<int32_t>());
                if (att_ref.m_layout == ImageLayout_MAX_DEFINE_VALUE) {
                    att_ref.m_layout = ImageLayout_UNDEFINED;
                    SDLOGW("RP(%s): SP[%d]->Color Ar[%d] att_ref.m_layout is max define value. Mark it undefined.", m_object_name.c_str(), spd_id, ar_id);
                }
                sp_desc.m_color_attachment_refs.push_back(att_ref);
            }

            if (spds_root.find("InputAttachmentRefs") != spds_root.end()) {
                nlohmann::json& iars_root = spd_root.at("InputAttachmentRefs");
                for (uint32_t ar_id = 0; ar_id < iars_root.size(); ++ar_id) {
                    AttachmentReference att_ref;
                    nlohmann::json& ar_root = iars_root.at(ar_id);
                    att_ref.m_attachment_ID = ar_root.at("AttachmentID").get<uint32_t>();
                    att_ref.m_layout = static_cast<ImageLayoutEnum>(ar_root.at("LayoutType").get<int32_t>());
                    if (att_ref.m_layout == ImageLayout_MAX_DEFINE_VALUE) {
                        att_ref.m_layout = ImageLayout_UNDEFINED;
                        SDLOGW("RP(%s): SP[%d]->Input Ar[%d] att_ref.m_layout is max define value. Mark it undefined.", m_object_name.c_str(), spd_id, ar_id);
                    }
                    sp_desc.m_input_attachment_refs.push_back(att_ref);
                }
            }

            if (spds_root.find("ResAttachmentRefs") != spds_root.end()) {
                nlohmann::json& rars_root = spd_root.at("ResAttachmentRefs");
                for (uint32_t ar_id = 0; ar_id < rars_root.size(); ++ar_id) {
                    AttachmentReference att_ref;
                    nlohmann::json& ar_root = rars_root.at(ar_id);
                    att_ref.m_attachment_ID = ar_root.at("AttachmentID").get<uint32_t>();
                    att_ref.m_layout = static_cast<ImageLayoutEnum>(ar_root.at("LayoutType").get<int32_t>());
                    if (att_ref.m_layout == ImageLayout_MAX_DEFINE_VALUE) {
                        att_ref.m_layout = ImageLayout_UNDEFINED;
                        SDLOGW("RP(%s): SP[%d]->Res Ar[%d] att_ref.m_layout is max define value. Mark it undefined.", m_object_name.c_str(), spd_id, ar_id);
                    }
                    sp_desc.m_res_attachment_refs.push_back(att_ref);
                }
            }

            if (spd_root.find("DepthAttachmentRef") != spd_root.end()) {
                nlohmann::json& dar_root = spd_root.at("DepthAttachmentRef");
                sp_desc.m_depth_attachment_ref.m_attachment_ID = dar_root.at("AttachmentID").get<uint32_t>();
                sp_desc.m_depth_attachment_ref.m_layout = static_cast<ImageLayoutEnum>(dar_root.at("LayoutType").get<int32_t>());
                if (sp_desc.m_depth_attachment_ref.m_layout == ImageLayout_MAX_DEFINE_VALUE) {
                    sp_desc.m_depth_attachment_ref.m_layout = ImageLayout_UNDEFINED;
                    SDLOGW("RP(%s): SP[%d]->Depth att_ref.m_layout is max define value. Mark it undefined.", m_object_name.c_str(), spd_id);
                }
            }

            m_identity.m_subpasses_descs.push_back(sp_desc);
        }

        //3. subpass dependency.
        nlohmann::json &sp_denps_root = rp.at("SubpassDependencies");
        for (uint32_t spd_id = 0; spd_id < sp_denps_root.size(); ++spd_id) {
            SubpassDependency sp_dependency;
            nlohmann::json &sp_denp_root = sp_denps_root.at(spd_id);
            int32_t spID = SD_SUBPASS_EXTERNAL;
            spID = sp_denp_root.at("SrcSPID").get<int32_t>();
            if (spID < 0) {
                sp_dependency.m_src_spID = SD_SUBPASS_EXTERNAL;
            }
            else {
                sp_dependency.m_src_spID = spID;
            }

            spID = sp_denp_root.at("DstSPID").get<int32_t>();
            if (spID < 0) {
            }
            else {
                sp_dependency.m_dst_spID = spID;
            }

            nlohmann::json &src_pss_root = sp_denp_root.at("SrcPipeStages");
            for (uint32_t ps_id = 0; ps_id < src_pss_root.size(); ++ps_id) {
                nlohmann::json &ps_root = src_pss_root.at(ps_id);
                PipelineStageEnum ps = static_cast<PipelineStageEnum>(ps_root.get<int32_t>());
                if (ps == PipelineStage_MAX_DEFINE_VALUE) {
                    ps = PipelineStage_ALL_COMMANDS;
                    SDLOGW("RP(%s): SPD[%d]->SrcPs[%d] is max define value. Mark it all commands.", m_object_name.c_str(), spd_id, ps_id);
                }
                sp_dependency.m_src_pipeline_stages.push_back(ps);
            }

            nlohmann::json &dst_pss_root = sp_denp_root.at("DstPipeStages");
            for (uint32_t ps_id = 0; ps_id < dst_pss_root.size(); ++ps_id) {
                nlohmann::json& ps_root = dst_pss_root.at(ps_id);
                PipelineStageEnum ps = static_cast<PipelineStageEnum>(ps_root.get<int32_t>());
                if (ps == PipelineStage_MAX_DEFINE_VALUE) {
                    ps = PipelineStage_ALL_COMMANDS;
                    SDLOGW("RP(%s): SPD[%d]->DstPs[%d] is max define value. Mark it all commands.", m_object_name.c_str(), spd_id, ps_id);
                }
                sp_dependency.m_dst_pipeline_stages.push_back(ps);
            }

            nlohmann::json &src_mem_msks_root = sp_denp_root.at("SrcMemMasks");
            for (uint32_t mmk_id = 0; mmk_id < src_mem_msks_root.size(); ++mmk_id) {
                nlohmann::json &mmk_root = src_mem_msks_root.at(mmk_id);
                MemoryAccessMaskEnum mmk = static_cast<MemoryAccessMaskEnum>(mmk_root.get<int32_t>());
                if (mmk == MemoryAccessMask_MAX_DEFINE_VALUE) {
                    SDLOGW("RP(%s): SPD[%d]->SrcMMK[%d] is max define value", m_object_name.c_str(), spd_id, mmk_id);
                }
                sp_dependency.m_src_mem_masks.push_back(mmk);
            }

            nlohmann::json &dst_mem_msk_root = sp_denp_root.at("DstMemMasks");
            for (uint32_t mmk_id = 0; mmk_id < dst_mem_msk_root.size(); ++mmk_id) {
                nlohmann::json& mmk_root = dst_mem_msk_root.at(mmk_id);
                MemoryAccessMaskEnum mmk = static_cast<MemoryAccessMaskEnum>(mmk_root.get<int32_t>());
                if (mmk == MemoryAccessMask_MAX_DEFINE_VALUE) {
                    SDLOGW("RP(%s): SPD[%d]->DstMMK[%d] is max define value.", m_object_name.c_str(), spd_id, mmk_id);
                }
                sp_dependency.m_dst_mem_masks.push_back(mmk);
            }

            nlohmann::json &dep_scopes_root = sp_denp_root.at("DepScopes");
            for (uint32_t ds_id = 0; ds_id < dep_scopes_root.size(); ++ds_id) {
                nlohmann::json &dep_scope_root = dep_scopes_root.at(ds_id);
                DependencyScopeEnum dep_scope = static_cast<DependencyScopeEnum>(dep_scope_root.get<int32_t>());
                if (dep_scope == DependencyScope_MAX_DEFINE_VALUE) {
                    SDLOGW("RP(%s): SPD[%d]->DepScope[%d] is max define value.", m_object_name.c_str(), spd_id, ds_id);
                }
                sp_dependency.m_dependencies.push_back(dep_scope);
            }
            m_identity.m_sp_dependencies.push_back(sp_dependency);
        }

        //4. External Params.
        if (rp.find("ExternalParams") != rp.end()) {
            nlohmann::json &eps_root = rp.at("ExternalParams");
            for (uint32_t ep_id = 0; ep_id < eps_root.size(); ++ep_id) {
                nlohmann::json &ep_root = eps_root.at(ep_id);
                std::string param_name = ep_root.at("Name").get<std::string>();
                if (param_name.compare("Multiview") == 0) {
                    if (ep_root.find("ViewMasks") != ep_root.end()) {
                        nlohmann::json& view_msks_root = ep_root.at("ViewMasks");
                        for (uint32_t vmk_id = 0; vmk_id < view_msks_root.size(); ++vmk_id) {
                            nlohmann::json& view_msk_root = view_msks_root.at(vmk_id);
                            uint32_t view_msk = view_msk_root.get<uint32_t>();
                            m_identity.m_multiview_info.m_view_masks.push_back(view_msk);
                        }
                    }

                    if (ep_root.find("ViewOffests") != ep_root.end()) {
                        nlohmann::json& view_offsets_root = ep_root.at("ViewOffests");
                        for (uint32_t voffset_id = 0; voffset_id < view_offsets_root.size(); ++voffset_id) {
                            nlohmann::json& view_offset_root = view_offsets_root.at(voffset_id);
                            int32_t view_offset = view_offset_root.get<int32_t>();
                            m_identity.m_multiview_info.m_view_offsets.push_back(view_offset);
                        }
                    }

                    if (ep_root.find("CorreletionMasks") != ep_root.end()) {
                        nlohmann::json& cor_msks_root = ep_root.at("CorreletionMasks");
                        for (uint32_t cmk_id = 0; cmk_id < cor_msks_root.size(); ++cmk_id) {
                            nlohmann::json& view_msk_root = cor_msks_root.at(cmk_id);
                            uint32_t view_msk = view_msk_root.get<uint32_t>();
                            m_identity.m_multiview_info.m_correlation_masks.push_back(view_msk);
                        }
                    }
                }
            }
        }
    }
    else {
        SDLOGE("RP file is empty. We can't initialize it.");
    }
}

void RenderPass::AddRenderPassDescription(const std::vector<AttachmentDescription> &i_att_descs, const std::vector<SubpassDescription> &i_sp_descs, const std::vector<SubpassDependency> &i_sp_deps)
{
    m_identity.m_attachment_descs = i_att_descs;
    m_identity.m_subpasses_descs = i_sp_descs;
    m_identity.m_sp_dependencies = i_sp_deps;
}

void RenderPass::SetMultiviewInfo(const MultiviewInfo &i_multiview_info)
{
    m_identity.m_multiview_info = i_multiview_info;
}

void RenderPass::Initialize()
{
    GraphicsManager::GetRef().CreateRenderPass(m_identity);
}

std::vector<TextureFormatEnum> RenderPass::CreateImageViewFormats() const
{
    std::vector<TextureFormatEnum> formats;
    formats.resize(m_identity.m_attachment_descs.size());
    for (uint32_t id = 0; id < formats.size(); ++id) {
        formats[id] = m_identity.m_attachment_descs[id].m_format;
    }
    return formats;
}

______________SD_END_GRAPHICS_NAMESPACE______________