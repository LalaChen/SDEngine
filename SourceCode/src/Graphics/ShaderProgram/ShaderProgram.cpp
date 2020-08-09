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
#include "ShaderProgram.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

ShaderProgram::ShaderProgram(const ObjectName &i_name)
: Object(i_name)
, m_descriptor_counts{0}
, m_registered(false)
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::RegisterShaderProgramStructure(
    const RenderPassInfos &i_rp_infos,
    const GraphicsPipelines &i_gp_srefs,
    const CommonDescriptorSetLayouts &i_common_dsl_wrefs,
    const DescriptorSetLayouts &i_dsl_srefs)
{
    if (i_rp_infos.size() == 0) {
        SDLOGE("No render pass group.");
        return;
    }

    for (uint32_t rpg_count = 0; rpg_count < i_rp_infos.size(); ++rpg_count) {
        if (i_rp_infos[rpg_count].m_rp_wref.IsNull() == true) {
            SDLOGE("Render pass in group[%d] is nullptr.", rpg_count);
            return;
        }
    }

    //1. set prepared information.
    m_rp_infos = i_rp_infos;
    m_pipe_srefs = i_gp_srefs;
    m_common_dsl_wrefs = i_common_dsl_wrefs;
    m_dsl_srefs = i_dsl_srefs;

    //2.calculate descriptor set count.
    m_descriptor_counts[UniformBindingType_UNIFORM_BUFFER] = 0;
    m_descriptor_counts[UniformBindingType_COMBINED_IMAGE_SAMPLER] = 0;

    for (uint32_t dsl_count = 0; dsl_count < m_dsl_srefs.size(); ++dsl_count) {
        uint32_t dsl_dcounts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
        m_dsl_srefs[dsl_count].GetRef().GetUniformDescriptorCounts(dsl_dcounts);
        for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
            m_descriptor_counts[count] += dsl_dcounts[count];
        }
    }

    m_registered = true;
}

void ShaderProgram::GetDescriptorCount(uint32_t i_d_counts[UniformBindingType_MAX_DEFINE_VALUE]) const
{
    for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
        i_d_counts[count] = m_descriptor_counts[count];
    }
}

void ShaderProgram::AllocateEssentialObjects(
    std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uv_wrefs,
    std::vector<DescriptorSetWeakReferenceObject> &io_desc_set_wrefs,
    DescriptorPoolWeakReferenceObject &io_pool_wref)
{
    //1. allocate descriptor for each pipeline.
    io_desc_set_wrefs.resize(m_dsl_srefs.size());
    for (uint32_t dsl_count = 0; dsl_count < m_dsl_srefs.size(); ++dsl_count) {
        io_desc_set_wrefs[dsl_count] = io_pool_wref.GetRef().AllocateDescriptorSet(m_dsl_srefs[dsl_count]);
        io_desc_set_wrefs[dsl_count].GetConstRef().LinkUniformVariables(io_uv_wrefs);
    }
}

void ShaderProgram::UseProgramWithTargetDescriptorSet(
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const RenderPassWeakReferenceObject &i_rp_wref,
    uint32_t i_sp_idx,
    const std::vector<DescriptorSetWeakReferenceObject> &i_desc_set_wrefs)
{
    for (uint32_t rp_id = 0; rp_id < m_rp_infos.size(); ++rp_id) {
        if (m_rp_infos[rp_id].m_rp_wref == i_rp_wref) {
            if (i_sp_idx < m_rp_infos[rp_id].m_sp_pipe_infos.size()) {
                uint32_t pipe_idx = m_rp_infos[rp_id].m_sp_pipe_infos[i_sp_idx].m_pipe_id;
                m_pipe_srefs[pipe_idx].GetRef().UseAndBindDescriptorSets(i_cb_wref, i_desc_set_wrefs);
            }
            return;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________