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

/*! \file      UniformBuffer.h
 *  \brief     Introduce of class about UniformBuffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/26
 *  \copyright MIT License.
 */

#pragma once

#include "UniformBufferDescriptor.h"
#include "UniformBufferIdentity.h"
#include "UniformVariable.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(UniformBuffer);

class UniformBuffer : public UniformVariable
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit UniformBuffer(const ObjectName &i_object_name, uint32_t i_binding_id);
     *  \param [in] i_object_name Name of this object.
     *  \param [in] i_binding_id ID of shader binding.
     *  \brief Constructor of UniformBuffer.
     */
    explicit UniformBuffer(const ObjectName &i_object_name, uint32_t i_binding_id);

    /*! \fn virtual ~UniformBuffer();
     *  \brief Destructor of UniformBuffer.
     */
    virtual ~UniformBuffer();

public:
    void Initialize(const UniformBufferDescriptorWeakReferenceObject &i_desc_wref);
public:
    inline UniformBindingTypeEnum GetType() const override;

public:
    template<class Type> bool SetVariable(const std::string &i_var_name, const Type &i_data, Size_ui64 i_idx = 0);

    void SetBufferData(const std::vector<uint8_t> &i_data);

public:
    // call set function and then update.
    void Update();
protected:

    UniformBufferIdentity m_identity;

    UniformBufferDescriptorWeakReferenceObject m_ub_desc_wref;

    std::vector<uint8_t> m_buffer;

    bool m_modified;
};

inline UniformBindingTypeEnum UniformBuffer::GetType() const
{
    return UniformBindingType_UNIFORM_BUFFER;
}

template<class Type>
inline bool UniformBuffer::SetVariable(const std::string &i_var_name, const Type &i_data, Size_ui64 i_idx)
{
    if (m_ub_desc_wref.IsNull() == false) {
        UniformBufferVariableInfo ub_var_info = m_ub_desc_wref.GetConstRef().GetVariableInfo(i_var_name);
        if (ub_var_info.IsValid() == true) {
            if (i_idx < ub_var_info.m_number) {
                Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
                std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
                m_modified = true;
                return true;
            }
        }
    }

    return false;
}

______________SD_END_GRAPHICS_NAMESPACE______________