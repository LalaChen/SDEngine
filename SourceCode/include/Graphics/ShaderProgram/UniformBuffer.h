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

#include "Color4f.h"
#include "Vector3f.h"
#include "Matrix4X4f.h"
#include "UniformBufferDescriptor.h"
#include "UniformBufferIdentity.h"
#include "UniformVariable.h"

using SDE::Graphics::Color4f;
using SDE::Math::Vector3f;
using SDE::Math::Matrix4X4f;

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
    bool SetInt(const std::string &i_var_name, int32_t i_data, Size_ui32 i_idx = 0);

    bool SetUint(const std::string &i_var_name, uint32_t i_data, Size_ui32 i_idx = 0);

    bool SetFloat(const std::string &i_var_name, float i_data, Size_ui32 i_idx = 0);

    bool SetVector3f(const std::string &i_var_name, const Vector3f &i_data, Size_ui32 i_idx = 0);

    bool SetColor4f(const std::string &i_var_name, const Color4f &i_data, Size_ui32 i_idx = 0);

    bool SetMatrix4X4f(const std::string &i_var_name, const Matrix4X4f &i_data, Size_ui32 i_idx = 0);
public:
    bool SetIntArray(const std::string &i_var_name, const std::vector<int32_t> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetUintArray(const std::string &i_var_name, const std::vector<uint32_t> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetFloatArray(const std::string &i_var_name, const std::vector<float> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetVector3fArray(const std::string &i_var_name, const std::vector<Vector3f> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetColor4fArray(const std::string &i_var_name, const std::vector<Color4f> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetMatrix4X4fArray(const std::string &i_var_name, const std::vector<Matrix4X4f> &i_datas, Size_ui32 i_start_idx = 0);
public:
    //
    bool SetBufferData(const void *i_data, Size_ui32 i_data_size);

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

______________SD_END_GRAPHICS_NAMESPACE______________