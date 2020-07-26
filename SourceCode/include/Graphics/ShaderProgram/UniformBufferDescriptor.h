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

/*! \file      UniformBufferDescriptor.h
 *  \brief     Introduce of class about UniformBufferDescriptor.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/21
 *  \copyright MIT License.
 */

#pragma once

#include <map>

#include "UniformVariableDescriptor.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(UniformBufferDescriptor);

enum UniformBufferVariableTypeEnum
{
    UniformBufferVariableType_INT = 0,
    UniformBufferVariableType_UINT,
    UniformBufferVariableType_FLOAT,
    UniformBufferVariableType_VECTOR3F,
    UniformBufferVariableType_COLOR4F,
    UniformBufferVariableType_MATRIX4X4F,
    UniformBufferVariableType_MAX_DEFINE_VALUE
};

class SDENGINE_CLASS UniformBufferVariableInfo
{
public:
    static const Size_ui32 TypeSizes[UniformBufferVariableType_MAX_DEFINE_VALUE];
public:
    UniformBufferVariableInfo()
    : m_type_size(0u), m_number(0u), m_offset(0u)
    {
    }

    UniformBufferVariableInfo(UniformBufferVariableTypeEnum i_type, Size_ui32 i_type_size, Size_ui32 i_offset, Size_ui32 i_number)
    : m_type(i_type), m_type_size(i_type_size), m_number(i_number), m_offset(i_offset)
    {
    }

    ~UniformBufferVariableInfo()
    {
    }
public:
    Size_ui32 GetTotalSize() const;
    bool IsValid() const;
public:
    UniformBufferVariableTypeEnum m_type;
    Size_ui32 m_type_size;
    Size_ui32 m_number;
    Size_ui32 m_offset;
};

inline Size_ui32 UniformBufferVariableInfo::GetTotalSize() const
{
    return m_type_size * m_number;
}

inline bool UniformBufferVariableInfo::IsValid() const
{
    return (GetTotalSize() != 0);
}

class SDENGINE_CLASS UniformBufferDescriptor : public UniformVariableDescriptor
{
public:
    explicit UniformBufferDescriptor(const ObjectName &i_name, Size_ui32 i_binding_id, Size_ui32 i_number = 1);
    virtual ~UniformBufferDescriptor();
public:
    void AddVariable(const std::string &i_var_name, UniformBufferVariableTypeEnum i_var_type, Size_ui32 i_var_offset, Size_ui32 i_var_number = 1);
public:
    UniformBufferVariableInfo GetVariableInfo(const std::string &i_var_name) const;
    Size_ui32 GetBufferSize() const;
    bool IsInitialized() const;
public:
    UniformBindingTypeEnum GetType() const override;
    UniformVariableStrongReferenceObject AllocateUniformVariable() override;
protected:
    std::map<std::string, UniformBufferVariableInfo> m_variable_map;
    Size_ui32 m_total_size;
};

inline Size_ui32 UniformBufferDescriptor::GetBufferSize() const
{
    return m_total_size;
}

inline bool UniformBufferDescriptor::IsInitialized() const
{
    return (m_total_size > 0);
}

inline UniformBindingTypeEnum UniformBufferDescriptor::GetType() const
{
    return UniformBindingType_UNIFORM_BUFFER;
}

______________SD_END_GRAPHICS_NAMESPACE______________