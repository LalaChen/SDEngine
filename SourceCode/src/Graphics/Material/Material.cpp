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

#include "Material.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

Material::Material(const ObjectName &i_name)
: Object(i_name)
{
}

Material::~Material()
{
}

void Material::Initialize(const ShaderProgramWeakReferenceObject &i_sp_wref)
{
    m_sp_wref = i_sp_wref;
    //1. prepare descriptor pool.
    m_dsp_sref = new DescriptorPool(StringFormat("%s_DescriptorPool", m_object_name.c_str()));
    uint32_t descriptor_counts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
    uint32_t max_set = m_sp_wref.GetRef().GetPipelineAmount();
    m_sp_wref.GetRef().GetDescriptorCount(descriptor_counts);
    m_dsp_sref.GetRef().Initialize(descriptor_counts, max_set, false);
    //2. allocate uniform variables for uniform descriptors.
    DescriptorPoolWeakReferenceObject dp_wref = m_dsp_sref.StaticCastToWeakPtr<DescriptorPool>();
    m_sp_wref.GetRef().AllocateEssentialObjects(m_uv_srefs, dp_wref);
    //3. allocate descriptor sets.

    //4. bind uniform variable to descriptor set.
}

______________SD_END_GRAPHICS_NAMESPACE______________
