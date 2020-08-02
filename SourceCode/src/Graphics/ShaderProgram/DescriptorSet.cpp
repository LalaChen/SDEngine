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
#include "DescriptorPool.h"
#include "DescriptorSet.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

DescriptorSet::DescriptorSet(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

DescriptorSet::~DescriptorSet()
{
}

void DescriptorSet::Initialize(
    const WeakReferenceObject<Object> &i_pool_wref,
    const GraphicsPipelineWeakReferenceObject &i_pipe_wref)
{
    DescriptorPoolWeakReferenceObject pool_wref = i_pool_wref.DynamicCastTo<DescriptorPool>();

    if (pool_wref.IsNull() == false && i_pipe_wref.IsNull() == false) {
        m_target_pipe_wref = i_pipe_wref;
        m_pool_wref = i_pool_wref;
        Size_ui32 ub_amount = i_pipe_wref.GetConstRef().GetUniformBindingAmount();
        m_uv_wrefs.resize(ub_amount);
        GraphicsManager::GetRef().AllocateDescriptorSet(m_identity, pool_wref, i_pipe_wref);
    }
}

void DescriptorSet::RegisterUniformVariable(const UniformVariableWeakReferenceObject &i_uv_wref, uint32_t i_binding_id)
{
    for (UniformVariableWeakReferenceObject &uv : m_uv_wrefs) {
        if (uv.IsNull() == false) {
            if (uv.GetConstRef().GetObjectName().compare(i_uv_wref.GetConstRef().GetObjectName()) == 0) {
                SDLOGW("Uniform variable[%s]", i_uv_wref.GetConstRef().GetObjectName().c_str());
                return;
            }
        }
    }

    if (i_binding_id < m_uv_wrefs.size()) {
        m_uv_wrefs[i_binding_id] = i_uv_wref;
    }
    else {
        SDLOGW("Try to bind target uv to ");
    }
}

void DescriptorSet::WriteDescriptor()
{
    GraphicsManager::GetRef().WriteUniformVariablesToDescriptorSet(m_identity, m_uv_wrefs);
}

void DescriptorSet::Bind(const CommandBufferWeakReferenceObject &i_cb_wref) const
{
    GraphicsManager::GetRef().BindDescriptorSet(m_identity, i_cb_wref, m_target_pipe_wref);
}

______________SD_END_GRAPHICS_NAMESPACE______________