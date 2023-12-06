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
{
}

GraphicsPipeline::~GraphicsPipeline()
{
}

void GraphicsPipeline::SetGraphicsPipelineParams(
    const GraphicsPipelineParam &i_params,
    const RenderPassWeakReferenceObject &i_rp,
    const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_dsls,
    uint32_t i_sp_id)
{
    m_identity.m_params = i_params;
    m_target_rp = i_rp;
    m_dsls = i_dsls;
    if (m_dsls.size() > 4) {
        SDLOGW("Min maxBoundDescriptorSets is 4. m_dsls.size() is %u", static_cast<uint32_t>(m_dsls.size()));
    }
}

void GraphicsPipeline::Initialize(const ShaderModules &i_shaders)
{
    SDLOG("Initialize graphics pipeline (%s).", m_object_name.c_str());
    if (m_target_rp.IsNull() == false) {
        GraphicsManager::GetRef().CreateGraphicsPipeline(m_identity, i_shaders, m_target_rp, m_dsls);
        m_initialized = true;
    }
    else {
        SDLOGE("RenderPass is null. We can't initialize graphics pipeline (%s).", m_object_name.c_str());
        m_initialized = false;
    }
}

void GraphicsPipeline::UseAndBindDescriptorSets(const CommandBufferWeakReferenceObject &i_cb, const std::vector<DescriptorSetWeakReferenceObject> &i_dss) const
{
    //1. collect necessary descriptor sets for binding.
    std::vector<DescriptorSetWeakReferenceObject> pipe_dss;
    for (const DescriptorSetWeakReferenceObject &ds : i_dss) {
        for (uint32_t dsl_count = 0; dsl_count < m_dsls.size(); ++dsl_count) {
            if (SD_WREF(ds).IsThisLayout(m_dsls[dsl_count]) == true) {
                pipe_dss.push_back(ds);
                break;
            }
        }
    }
    //2. Bind graphics pipeline with necenssary descriptor sets.
    GraphicsManager::GetRef().BindGraphicsPipeline(m_identity, i_cb, pipe_dss);
}

______________SD_END_GRAPHICS_NAMESPACE______________