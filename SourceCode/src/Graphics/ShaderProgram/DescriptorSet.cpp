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
    //We don't need to destroy graphics resource because real memory will keep at pool.
}

void DescriptorSet::Initialize(
    const WeakReferenceObject<Object> &i_pool_wref,
    const DescriptorSetLayoutWeakReferenceObject &i_layout_wref)
{
    if (i_pool_wref.IsNull() == true) {
        SDLOGE("Initialize null pool in ds[%s].", m_object_name.c_str());
    }

    DescriptorPoolWeakReferenceObject pool_wref = i_pool_wref.DynamicCastTo<DescriptorPool>();

    if (pool_wref.IsNull() == true) {
        SDLOGE("cast pool object[%s] failure in ds[%s].", SD_WREF(i_pool_wref).GetObjectName().c_str(), m_object_name.c_str());
        return;
    }

    if (i_layout_wref.IsNull() == true) {
        SDLOGE("input null layout into ds[%s].", m_object_name.c_str());
        return;
    }

    //1. set data.
    m_layout_wref = i_layout_wref;
    m_pool_wref = i_pool_wref;
    //2. allocate identity.
    GraphicsManager::GetRef().AllocateDescriptorSet(m_identity, pool_wref, m_layout_wref);
    //3. allocate uniform variables.
    m_layout_wref.GetRef().AllocateUniformVariables(m_uv_srefs);
}

void DescriptorSet::WriteDescriptor()
{
    std::vector<UniformVariableWeakReferenceObject> uv_wrefs;
    uv_wrefs.resize(m_uv_srefs.size());
    for (uint32_t uv_count = 0; uv_count < uv_wrefs.size(); ++uv_count) {
        uv_wrefs[uv_count] = m_uv_srefs[uv_count];
    }
    GraphicsManager::GetRef().WriteUniformVariablesToDescriptorSet(m_identity, uv_wrefs);
}

void DescriptorSet::GetAllocatedUniformVariables(
    std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uv_wrefs) const
{
    for (const UniformVariableStrongReferenceObject &uv_sref : m_uv_srefs) {
        std::string uv_name = uv_sref.GetRef().GetObjectName();
        std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator iter;
        iter = io_uv_wrefs.find(uv_name);
        if (iter == io_uv_wrefs.end()) {
            io_uv_wrefs[uv_name] = uv_sref;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________