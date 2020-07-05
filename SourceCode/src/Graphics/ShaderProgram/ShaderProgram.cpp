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

void ShaderProgram::RegisterPipelinesForRenderPass(const RenderPassWeakReferenceObject &i_rp_wref, const GraphicsPipelines &i_gp_srefs, const UniformVariableDescriptors &i_uvd_srefs)
{
    if (i_rp_wref.IsNull() == true) {
        SDLOGE("Render pass is null.");
        return;
    }

    ObjectName rp_name = i_rp_wref.GetConstRef().GetObjectName();
    std::map<ObjectName, RenderPassGroup>::iterator pipe_iter = m_rp_pipelines.find(rp_name);
    if (pipe_iter == m_rp_pipelines.end()) {
        m_rp_pipelines[rp_name] = RenderPassGroup();
        m_rp_pipelines[rp_name].m_rp_wref = i_rp_wref;
        m_rp_pipelines[rp_name].m_gp_wrefs.resize(i_rp_wref.GetRef().GetSubpassAmount());
        //
        for (const GraphicsPipelineStrongReferenceObject &pipe_sref : i_gp_srefs) {
            ObjectName pipe_name = pipe_sref.GetConstRef().GetObjectName();
            uint32_t sp_id = pipe_sref.GetRef().GetSubpassID();
            std::map<ObjectName, GraphicsPipelineStrongReferenceObject>::iterator p_iter = m_pipe_srefs.find(pipe_name);
            if (p_iter == m_pipe_srefs.end()) {
                m_pipe_srefs[pipe_name] = pipe_sref;
            }
            m_rp_pipelines[rp_name].m_gp_wrefs[sp_id] = pipe_sref;
        }
        //
        for (const UniformVariableDescriptorStrongReferenceObject &uvd_sref : i_uvd_srefs) {
            ObjectName uv_name = uvd_sref.GetConstRef().GetObjectName();
            std::map<ObjectName, UniformVariableDescriptorStrongReferenceObject>::iterator uvd_iter = m_uvd_srefs.find(uv_name);
            if (uvd_iter == m_uvd_srefs.end()) {
                m_uvd_srefs[uv_name] = uvd_sref;
            }
            m_rp_pipelines[rp_name].m_uvd_wrefs.push_back(uvd_sref);
        }
    }
    else {
        SDLOGE("Render Pass[%s] is existed!!!", rp_name.c_str());
    }
}

void ShaderProgram::Initialize()
{
    //1. calculate descriptor count.
    m_descriptor_counts[UniformBindingType_UNIFORM_BUFFER] = 0;
    m_descriptor_counts[UniformBindingType_COMBINED_IMAGE_SAMPLER] = 0;
    std::map<ObjectName, GraphicsPipelineStrongReferenceObject>::iterator p_iter;
    for (p_iter = m_pipe_srefs.begin(); p_iter != m_pipe_srefs.end(); ++p_iter) {
        uint32_t pipe_dcounts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
        (*p_iter).second.GetRef().GetUniformDescriptorCounts(pipe_dcounts);
        for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
            m_descriptor_counts[count] += pipe_dcounts[count];
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________