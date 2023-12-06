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
    const WeakReferenceObject<Object> &i_pool,
    const DescriptorSetLayoutWeakReferenceObject &i_layout)
{
    if (i_pool.IsNull() == true) {
        SDLOGE("Initialize null pool in ds[%s].", m_object_name.c_str());
    }

    DescriptorPoolWeakReferenceObject pool = i_pool.DynamicCastTo<DescriptorPool>();

    if (pool.IsNull() == true) {
        SDLOGE("cast pool object[%s] failure in ds[%s].", SD_WREF(i_pool).GetObjectName().c_str(), m_object_name.c_str());
        return;
    }

    if (i_layout.IsNull() == true) {
        SDLOGE("input null layout into ds[%s].", m_object_name.c_str());
        return;
    }

    //1. set data.
    m_layout = i_layout;
    m_pool = i_pool;
    //2. allocate identity.
    GraphicsManager::GetRef().AllocateDescriptorSet(m_identity, pool, m_layout);
    //3. allocate uniform variables.
    SD_WREF(m_layout).AllocateUniformVariables(m_uvs);
}

void DescriptorSet::WriteDescriptor()
{
    std::vector<UniformVariableWeakReferenceObject> uvs;
    uvs.resize(m_uvs.size());
    for (uint32_t uv_count = 0; uv_count < uvs.size(); ++uv_count) {
        uvs[uv_count] = m_uvs[uv_count];
    }
    GraphicsManager::GetRef().WriteUniformVariablesToDescriptorSet(m_identity, uvs);
}

void DescriptorSet::GetAllocatedUniformVariables(
    std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uvs) const
{
    for (const UniformVariableStrongReferenceObject &uv : m_uvs) {
        std::string uv_name = SD_SREF(uv).GetObjectName();
        std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator iter;
        iter = io_uvs.find(uv_name);
        if (iter == io_uvs.end()) {
            io_uvs[uv_name] = uv;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________