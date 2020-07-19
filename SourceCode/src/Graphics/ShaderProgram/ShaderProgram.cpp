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
        if (i_rp_groups[rpg_count].m_rp_wref.IsNull() == false) {
            SDLOGE("Render pass in group[%d] is nullptr.", rpg_count);
            return;
        }
    }

    //1. tell us what pipeline we need to use.
    for (uint32_t rpg_count = 0; rpg_count < i_rp_groups.size(); ++rpg_count) {
        ObjectName rp_name = i_rp_groups[rpg_count].m_rp_wref.GetConstRef().GetObjectName();
        std::map<ObjectName, RenderPassGroup>::iterator pipe_iter = m_rp_groups.find(rp_name);
        if (pipe_iter == m_rp_groups.end()) {
            m_rp_groups[rp_name] = i_rp_groups[rpg_count];
        }
        else {
            SDLOGE("Render Pass[%s] is existed!!!", rp_name.c_str());
        }
    }
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
}

void ShaderProgram::Initialize()
{
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
        io_desc_set_wrefs.push_back(io_pool_wref.GetRef().AllocateDescriptorSet(m_pipe_srefs[pipe_count]));
    }

    //3. bind descriptor set to uniform variables.
    for (uint32_t pipe_set_count = 0; pipe_set_count < m_pipe_srefs.size(); ++pipe_set_count) {
        std::map<ObjectName, UniformVariableStrongReferenceObject>::iterator uv_iter;
        for (uv_iter = io_uv_srefs.begin(); uv_iter != io_uv_srefs.end(); ++uv_iter) {
            //Check all uniform variables is one of variable in this pipeline.
            //If it's one of uniform variables for this pipeline, we register 
            //this uniform variable to this descriptor set.
            if (m_pipe_srefs[pipe_set_count].GetRef().IsThisUniformVariableUsed((*uv_iter).second) == true) {
                io_desc_set_wrefs[pipe_set_count].GetRef().RegisterUniformVariable((*uv_iter).second);
            }
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________