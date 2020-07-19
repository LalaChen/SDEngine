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

#include "SDEngineCommonFunction.h"
#include "LogManager.h"
#include "GraphicsManager.h"
#include "DescriptorPool.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

DescriptorPool::DescriptorPool(const ObjectName &i_object_name)
: Object(i_object_name)
, m_current_set(0)
{
}

DescriptorPool::~DescriptorPool()
{
    GraphicsManager::GetRef().DestroyDescriptorPool(m_identity);
}

void DescriptorPool::Initialize(uint32_t i_d_counts[UniformBindingType_MAX_DEFINE_VALUE], uint32_t i_max_set, bool i_individual_flag)
{
    m_identity.m_max_set = i_max_set;
    m_identity.m_individual_op_flag = i_individual_flag;
    for (uint32_t type_id = 0; type_id < UniformBindingType_MAX_DEFINE_VALUE; ++type_id) {
        m_identity.m_descriptor_counts[type_id] = i_d_counts[type_id];
    }
    //
    GraphicsManager::GetRef().CreateDescriptorPool(m_identity);
}

DescriptorSetWeakReferenceObject DescriptorPool::AllocateDescriptorSet(const GraphicsPipelineWeakReferenceObject &i_pipe_wref)
{
    DescriptorSetWeakReferenceObject ds_wref;
    if (m_current_set < m_identity.m_max_set) {
        DescriptorPoolWeakReferenceObject this_dp_wref = GetThisWeakPtrByType<DescriptorPool>();
        DescriptorSetStrongReferenceObject new_set_sref = new DescriptorSet(StringFormat("%s_dset_%d", m_object_name.c_str(), m_current_set));
        new_set_sref.GetRef().Initialize(
            this_dp_wref.StaticCastTo<Object>(),
            i_pipe_wref);
        m_set_srefs.push_back(new_set_sref);
        ds_wref = new_set_sref;
    }
    else {
        SDLOGE("Descriptor Pool(%s) is full.", m_object_name.c_str());
    }
    return ds_wref;
}

______________SD_END_GRAPHICS_NAMESPACE______________