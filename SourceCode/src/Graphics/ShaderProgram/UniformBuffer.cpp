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
#include "UniformBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

UniformBuffer::UniformBuffer(const ObjectName &i_object_name, uint32_t i_binding_id)
: UniformVariable(i_object_name, i_binding_id)
, m_modified(false)
{
}

UniformBuffer::~UniformBuffer()
{
}

//Initialize part. 
void UniformBuffer::Initialize(const UniformBufferDescriptorWeakReferenceObject &i_desc)
{
    if (m_ub_desc.IsNull() == false) {
        SDLOGE("UniformBuffer[%s] is initialized.", m_object_name.c_str());
    }
    m_ub_desc = i_desc;
    m_identity.m_data_size = i_desc.GetConstRef().GetBufferSize();
    m_buffer.resize(m_identity.m_data_size);
    m_buffer.shrink_to_fit();
    //create uniform buffer.
    GraphicsManager::GetRef().CreateUniformBuffer(m_identity);
}

//Set uniform variable part.
bool UniformBuffer::SetInt(const std::string &i_var_name, int32_t i_data, Size_ui32 i_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_INT) {
        if (i_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
            std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetUint(const std::string &i_var_name, uint32_t i_data, Size_ui32 i_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_UINT) {
        if (i_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
            std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetFloat(const std::string &i_var_name, float i_data, Size_ui32 i_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_FLOAT) {
        if (i_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
            std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetVector3f(const std::string &i_var_name, const Vector3f &i_data, Size_ui32 i_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_VECTOR3F) {
        if (i_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
            std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetColor4f(const std::string &i_var_name, const Color4f &i_data, Size_ui32 i_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_COLOR4F) {
        if (i_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
            std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetMatrix4X4f(const std::string &i_var_name, const Matrix4X4f &i_data, Size_ui32 i_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_MATRIX4X4F) {
        if (i_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_idx;
            std::memcpy(m_buffer.data() + final_offset, &i_data, ub_var_info.m_type_size);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

//Set uniform variable with array
bool UniformBuffer::SetIntArray(const std::string &i_var_name, const std::vector<int32_t> &i_datas, Size_ui32 i_start_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_INT) {
        if (i_start_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_start_idx;
            Size_ui32 final_amount = static_cast<Size_ui32>(i_datas.size());
            if ((final_amount + i_start_idx) > ub_var_info.m_number) {
                final_amount = ub_var_info.m_number - i_start_idx;
                SDLOGW("Variable[%s]'s amount is (%d). But updating range is (%d, %d).", i_var_name.c_str(), ub_var_info.m_number, i_start_idx, (final_amount + i_start_idx));
            }
            std::memcpy(m_buffer.data() + final_offset, i_datas.data(), ub_var_info.m_type_size * final_amount);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetUintArray(const std::string &i_var_name, const std::vector<uint32_t> &i_datas, Size_ui32 i_start_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_UINT) {
        if (i_start_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_start_idx;
            Size_ui32 final_amount = static_cast<Size_ui32>(i_datas.size());
            if ((final_amount + i_start_idx) > ub_var_info.m_number) {
                final_amount = ub_var_info.m_number - i_start_idx;
                SDLOGW("Variable[%s]'s amount is (%d). But updating range is (%d, %d).", i_var_name.c_str(), ub_var_info.m_number, i_start_idx, (final_amount + i_start_idx));
            }
            std::memcpy(m_buffer.data() + final_offset, i_datas.data(), ub_var_info.m_type_size * final_amount);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetFloatArray(const std::string &i_var_name, const std::vector<float> &i_datas, Size_ui32 i_start_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_FLOAT) {
        if (i_start_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_start_idx;
            Size_ui32 final_amount = static_cast<Size_ui32>(i_datas.size());
            if ((final_amount + i_start_idx) > ub_var_info.m_number) {
                final_amount = ub_var_info.m_number - i_start_idx;
                SDLOGW("Variable[%s]'s amount is (%d). But updating range is (%d, %d).", i_var_name.c_str(), ub_var_info.m_number, i_start_idx, (final_amount + i_start_idx));
            }
            std::memcpy(m_buffer.data() + final_offset, i_datas.data(), ub_var_info.m_type_size * final_amount);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

bool UniformBuffer::SetVector3fArray(const std::string &i_var_name, const std::vector<Vector3f> &i_datas, Size_ui32 i_start_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_VECTOR3F) {
        if (i_start_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_start_idx;
            Size_ui32 final_amount = static_cast<Size_ui32>(i_datas.size());
            if ((final_amount + i_start_idx) > ub_var_info.m_number) {
                final_amount = ub_var_info.m_number - i_start_idx;
                SDLOGW("Variable[%s]'s amount is (%d). But updating range is (%d, %d).", i_var_name.c_str(), ub_var_info.m_number, i_start_idx, (final_amount + i_start_idx));
            }
            std::memcpy(m_buffer.data() + final_offset, i_datas.data(), ub_var_info.m_type_size * final_amount);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}


bool UniformBuffer::SetColor4fArray(const std::string &i_var_name, const std::vector<Color4f> &i_datas, Size_ui32 i_start_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_COLOR4F) {
        if (i_start_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_start_idx;
            Size_ui32 final_amount = static_cast<Size_ui32>(i_datas.size());
            if ((final_amount + i_start_idx) > ub_var_info.m_number) {
                final_amount = ub_var_info.m_number - i_start_idx;
                SDLOGW("Variable[%s]'s amount is (%d). But updating range is (%d, %d).", i_var_name.c_str(), ub_var_info.m_number, i_start_idx, (final_amount + i_start_idx));
            }
            std::memcpy(m_buffer.data() + final_offset, i_datas.data(), ub_var_info.m_type_size * final_amount);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}


bool UniformBuffer::SetMatrix4X4fArray(const std::string &i_var_name, const std::vector<Matrix4X4f> &i_datas, Size_ui32 i_start_idx)
{
    if (m_ub_desc.IsNull() == true) {
        SDLOGW("UniformBuffer[%s]'s is invalid. Please check!!!", m_object_name.c_str());
        return false;
    }

    UniformBufferVariableInfo ub_var_info = m_ub_desc.GetConstRef().GetVariableInfo(i_var_name);
    if (ub_var_info.IsValid() == false) {
        SDLOGW("Variable[%s] is not in UniformBuffer(%s). Please check!!!", i_var_name.c_str(), m_object_name.c_str());
        return false;
    }

    if (ub_var_info.m_type == UniformBufferVariableType_MATRIX4X4F) {
        if (i_start_idx < ub_var_info.m_number) {
            Size_ui32 final_offset = ub_var_info.m_offset + ub_var_info.m_type_size * i_start_idx;
            Size_ui32 final_amount = static_cast<Size_ui32>(i_datas.size());
            if ((final_amount + i_start_idx) > ub_var_info.m_number) {
                final_amount = ub_var_info.m_number - i_start_idx;
                SDLOGW("Variable[%s]'s amount is (%d). But updating range is (%d, %d).", i_var_name.c_str(), ub_var_info.m_number, i_start_idx, (final_amount + i_start_idx));
            }
            std::memcpy(m_buffer.data() + final_offset, i_datas.data(), ub_var_info.m_type_size * final_amount);
            m_modified = true;
            return true;
        }
        else {
            SDLOGW("Variable[%s]'s amount is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_number);
        }
    }
    else {
        SDLOGW("Variable[%s]'s type is (%d). Please check!!!", i_var_name.c_str(), ub_var_info.m_type);
    }

    return false;
}

//Set uniform variables.
bool UniformBuffer::SetBufferData(const void *i_data, Size_ui32 i_data_size)
{
    if (i_data_size != m_buffer.size()) {
        SDLOGW("Buffer[%s]'s size is (%d). Please check!!!", m_object_name.c_str(), m_buffer.size());
        return false;
    }

    std::memcpy(m_buffer.data(), i_data, m_buffer.size());
    m_modified = true;
    return true;
}

//Update uniform variable data to buffers.
void UniformBuffer::Update()
{
    if (m_modified == true) {
        void* mem_ptr = nullptr;
        UniformBufferWeakReferenceObject this_obj = GetThisWeakPtrByType<UniformBuffer>();
        GraphicsManager::GetRef().MapUniformBuffer(this_obj, mem_ptr);
        std::memcpy(mem_ptr, m_buffer.data(), m_buffer.size());
        GraphicsManager::GetRef().UnmapUniformBuffer(this_obj);
        m_modified = false;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________