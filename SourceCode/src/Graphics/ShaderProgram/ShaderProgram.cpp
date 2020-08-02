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
    const RenderPassGroups &i_rp_groups,
    const GraphicsPipelines &i_gp_srefs,
    const UniformVariableDescriptors &i_uvd_srefs)
{
    if (i_rp_groups.size() == 0) {
        SDLOGE("No render pass group.");
        return;
    }

    for (uint32_t rpg_count = 0; rpg_count < i_rp_groups.size(); ++rpg_count) {
        if (i_rp_groups[rpg_count].m_rp_wref.IsNull() == true) {
            SDLOGE("Render pass in group[%d] is nullptr.", rpg_count);
            return;
        }
    }

    //1. tell us what pipeline we need to use in target render pass.
    m_rp_groups = i_rp_groups;

    //2. cache used pipeline.
    m_pipe_srefs = i_gp_srefs;
    m_uvd_srefs = i_uvd_srefs;

    //3.calculate descriptor set count.
    m_descriptor_counts[UniformBindingType_UNIFORM_BUFFER] = 0;
    m_descriptor_counts[UniformBindingType_COMBINED_IMAGE_SAMPLER] = 0;
    std::map<ObjectName, GraphicsPipelineStrongReferenceObject>::iterator p_iter;
    for (uint32_t pipe_count = 0; pipe_count < m_pipe_srefs.size(); ++pipe_count) {
        uint32_t pipe_dcounts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
        m_pipe_srefs[pipe_count].GetRef().GetUniformDescriptorCounts(pipe_dcounts);
        for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
            m_descriptor_counts[count] += pipe_dcounts[count];
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
    std::map<ObjectName, UniformVariableStrongReferenceObject> &io_uv_srefs,
    std::vector<DescriptorSetWeakReferenceObject> &io_desc_set_wrefs,
    DescriptorPoolWeakReferenceObject &io_pool_wref)
{
    //1. allocate uniform variable.
    for (uint32_t uvd_count = 0; uvd_count < m_uvd_srefs.size(); ++uvd_count) {
        ObjectName uv_name = m_uvd_srefs[uvd_count].GetRef().GetObjectName();
        UniformVariableStrongReferenceObject uv_sref = m_uvd_srefs[uvd_count].GetRef().AllocateUniformVariable();
        if (uv_sref.IsNull() == false) {
            io_uv_srefs[uv_name] = uv_sref;
        }
        else {
            SDLOGE("Allocate uniform variable[%s,%d] failure.", uv_name.c_str(), m_uvd_srefs[uvd_count].GetRef().GetType());
        }
    }
    //2. allocate descriptor for each pipeline.
    io_desc_set_wrefs.resize(m_pipe_srefs.size());
    for (uint32_t pipe_count = 0; pipe_count < m_pipe_srefs.size(); ++pipe_count) {
        io_desc_set_wrefs[pipe_count] = io_pool_wref.GetRef().AllocateDescriptorSet(m_pipe_srefs[pipe_count]);
    }

    //3. bind descriptor set to uniform variables.
    for (uint32_t pipe_set_count = 0; pipe_set_count < m_pipe_srefs.size(); ++pipe_set_count) {
        std::map<ObjectName, UniformVariableStrongReferenceObject>::iterator uv_iter;
        for (uv_iter = io_uv_srefs.begin(); uv_iter != io_uv_srefs.end(); ++uv_iter) {
            //Check all uniform variables is one of variable in this pipeline.
            //If it's one of uniform variables for this pipeline, we register 
            //this uniform variable to this descriptor set.
            UniformVariableWeakReferenceObject uv_wref = (*uv_iter).second;
            if (m_pipe_srefs[pipe_set_count].GetRef().IsThisUniformVariableUsed(uv_wref) == true) {
                io_desc_set_wrefs[pipe_set_count].GetRef().RegisterUniformVariable(uv_wref, uv_wref.GetRef().GetBindingID());
            }
        }
    }
}

void ShaderProgram::UseProgramWithTargetDescriptorSet(
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const RenderPassWeakReferenceObject &i_rp_wref,
    uint32_t i_sp_idx,
    const std::vector<DescriptorSetWeakReferenceObject> &i_desc_set_wrefs)
{
    for (uint32_t rpg_id = 0; rpg_id < m_rp_groups.size(); ++rpg_id) {
        if (m_rp_groups[rpg_id].m_rp_wref == i_rp_wref) {
            if (i_sp_idx < m_rp_groups[rpg_id].m_pipe_orders.size()) {
                uint32_t pipe_idx = m_rp_groups[rpg_id].m_pipe_orders[i_sp_idx];
                if (pipe_idx < i_desc_set_wrefs.size() && pipe_idx < m_pipe_srefs.size()) {
                    m_pipe_srefs[pipe_idx].GetRef().Use(i_cb_wref);
                    i_desc_set_wrefs[pipe_idx].GetRef().Bind(i_cb_wref);
                }
            }
            return;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________