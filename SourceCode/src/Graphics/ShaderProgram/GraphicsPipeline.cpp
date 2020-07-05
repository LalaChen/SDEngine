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
#include "GraphicsManager.h"
#include "GraphicsPipeline.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsPipeline::GraphicsPipeline(const ObjectName &i_object_name)
: Object(i_object_name)
, m_initialized(false)
, m_descriptor_counts{0}
{
}

GraphicsPipeline::~GraphicsPipeline()
{
}

void GraphicsPipeline::SetGraphicsPipelineParams(const GraphicsPipelineParam &i_params, const RenderPassWeakReferenceObject &i_rp_wref, uint32_t i_sp_id)
{
    m_identity.m_params = i_params;
    m_target_rp_wref = i_rp_wref;
    m_uv_descriptor_wrefs.clear();
    m_uv_descriptor_wrefs.resize(m_identity.m_params.m_uniform_binding_infos.size());
}

void GraphicsPipeline::RegisterUniformVariableDescriptor(const UniformVariableDescriptorWeakReferenceObject &i_uvd_wref, uint32_t i_uvd_id)
{
    if (i_uvd_id < m_uv_descriptor_wrefs.size()) {
        m_uv_descriptor_wrefs[i_uvd_id] = i_uvd_wref;
    }
    else {
        SDLOGE("Wrong ID(%d).", i_uvd_id);
    }
}

void GraphicsPipeline::Initialize(const ShaderModules &i_shaders)
{
    SDLOG("Initialize graphics pipeline (%s).", m_object_name.c_str());
    GraphicsManager::GetRef().CreateGraphicsPipeline(m_identity, i_shaders, m_target_rp_wref);
    for (UniformBinding &ub : m_identity.m_params.m_uniform_binding_infos) {
        m_descriptor_counts[ub.m_binding_type]++;
    }
    m_initialized = true;
}

void GraphicsPipeline::Use(const CommandBufferWeakReferenceObject &i_cb_wref)
{
    GraphicsManager::GetRef().BindGraphicsPipeline(m_identity, i_cb_wref);
}

______________SD_END_GRAPHICS_NAMESPACE______________