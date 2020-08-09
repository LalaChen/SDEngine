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
#include "DescriptorSetLayout.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

DescriptorSetLayout::DescriptorSetLayout(const ObjectName &i_object_name)
: Object(i_object_name)
, m_descriptor_counts{0}
{
    GraphicsManager::GetRef().DestroyDescriptorSetLayout(m_identity);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
}

void DescriptorSetLayout::AddUniformVariableDescriptors(const std::vector<UniformVariableDescriptorStrongReferenceObject> &i_uvd_srefs)
{
    m_uvd_srefs = i_uvd_srefs;
    for (UniformVariableDescriptorStrongReferenceObject &uvd_sref : m_uvd_srefs) {
        UniformBindingTypeEnum ub_type = uvd_sref.GetRef().GetBindingType();
        if (ub_type != UniformBindingType_MAX_DEFINE_VALUE) {
            m_descriptor_counts[SD_ENUM_TO_UINT(ub_type)]++;
        }
        else {
            SDLOGE("Error Uniform Binding in uniform variable descriptor(%s).", uvd_sref.GetRef().GetObjectName().c_str());
        }
    }
}

void DescriptorSetLayout::Initialize()
{
    std::vector<UniformVariableDescriptorWeakReferenceObject> uvd_wrefs;
    for (UniformVariableDescriptorStrongReferenceObject &uvd_sref : m_uvd_srefs) {
        uvd_wrefs.push_back(uvd_sref);
    }
    GraphicsManager::GetRef().CreateDescriptorSetLayout(m_identity, uvd_wrefs);
}

void DescriptorSetLayout::AllocateUniformVariables(std::vector<UniformVariableStrongReferenceObject> &io_uv_srefs)
{
    for (uint32_t uvd_count = 0; uvd_count < m_uvd_srefs.size(); ++uvd_count) {
        io_uv_srefs.push_back(m_uvd_srefs[uvd_count].GetRef().AllocateUniformVariable());
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________