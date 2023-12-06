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

/*! \file      DescriptorSetLayout.h
 *  \brief     Introduce of class about DescriptorSetLayout.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/87/05
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "UniformVariableDescriptor.h"
#include "DescriptorSetLayoutIdentity.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(DescriptorSetLayout);

class SDENGINE_CLASS DescriptorSetLayout : public Object
{
public:
    friend class GraphicsIdentityGetter;
public:
    explicit DescriptorSetLayout(const ObjectName &i_object_name);
    virtual ~DescriptorSetLayout();
public:
    void AddUniformVariableDescriptors(const std::vector<UniformVariableDescriptorStrongReferenceObject> &i_uvds);
public:
    void Initialize();
public:
    /*! \fn void GetUniformDescriptorCounts(uint32_t io_counts[UniformBindingType_MAX_DEFINE_VALUE]) const;
     *  \brief return descriptor count in this descriptor set layout.
     */
    void GetUniformDescriptorCounts(uint32_t io_counts[UniformBindingType_MAX_DEFINE_VALUE]) const;

    int32_t GetUniformVariableIDByName(const std::string &i_uv_name) const;

    Size_ui32 GetUniformBindingAmount() const;

    /*! \fn void AllocateUniformVariables(std::vector<UniformVariableStrongReferenceObject> &io_uvs);
     *  \var [inout] io_uvs container for returning allocated uniform variable.
     *  \brief allocate uniform variables for this descriptor set layout. 
     */
    void AllocateUniformVariables(std::vector<UniformVariableStrongReferenceObject> &io_uvs);
protected:
    /*! \var DescriptorSetLayoutIdentity m_identity;
     *  \brief identity this descriptor set layout.
     */
    DescriptorSetLayoutIdentity m_identity;

    /*! \var std::vector<UniformVariableDescriptorStrongReferenceObject> m_uvds;
     *  \brief all uniform descriptors about this set layout.
     *         Note : one uvd shouldn't in two or more layouts. 
     */
    std::vector<UniformVariableDescriptorStrongReferenceObject> m_uvds;
    
    uint32_t m_descriptor_counts[UniformBindingType_MAX_DEFINE_VALUE];
};

inline void DescriptorSetLayout::GetUniformDescriptorCounts(uint32_t io_counts[UniformBindingType_MAX_DEFINE_VALUE]) const
{
    for (uint32_t count = 0; count < UniformBindingType_MAX_DEFINE_VALUE; ++count) {
        io_counts[count] = m_descriptor_counts[count];
    }
}

inline Size_ui32 DescriptorSetLayout::GetUniformBindingAmount() const
{
    return static_cast<uint32_t>(m_uvds.size());
}

inline int32_t DescriptorSetLayout::GetUniformVariableIDByName(const std::string &i_uv_name) const
{
    for (uint32_t uvd_count = 0; uvd_count < m_uvds.size(); ++uvd_count) {
        if (SD_CONST_SREF(m_uvds[uvd_count]).GetObjectName().compare(i_uv_name) == 0) {
            return uvd_count;
        }
    }
    return -1;
}

______________SD_END_GRAPHICS_NAMESPACE______________