#include "LogManager.h"
#include "UniformBuffer.h"
#include "UniformBufferDescriptor.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

const Size_ui32 UniformBufferVariableInfo::TypeSizes[UniformBufferVariableType_MAX_DEFINE_VALUE] = {
    sizeof(int32_t),
    sizeof(uint32_t),
    sizeof(float),
    sizeof(Vector3f),
    sizeof(Color4f),
    sizeof(Matrix4X4f)
};

UniformBufferDescriptor::UniformBufferDescriptor(const ObjectName &i_name, Size_ui32 i_binding_id, Size_ui32 i_number, const std::vector<ShaderStageEnum> &i_stages)
: UniformVariableDescriptor(i_name, i_binding_id, UniformBindingType_UNIFORM_BUFFER, i_number, i_stages)
, m_total_size(0)
{
}

UniformBufferDescriptor::~UniformBufferDescriptor()
{
}

void UniformBufferDescriptor::AddVariable(const std::string &i_var_name, UniformBufferVariableTypeEnum i_var_type, Size_ui32 i_var_offset, Size_ui32 i_var_number)
{
    std::map<std::string, UniformBufferVariableInfo>::iterator var_iter;
    
    if (IsInitialized() == true) {
        SDLOGE("Add variable[%s] to UB[%s] failure because UBD is initialized !!!",
            i_var_name.c_str(), m_object_name.c_str());
        return;
    }

    if (i_var_type == UniformBufferVariableType_MAX_DEFINE_VALUE) {
        SDLOGE("Add variable[%s] to UBD[%s] failure because variable is invalid type!!!",
            i_var_name.c_str(), m_object_name.c_str());
        return;
    }

    if (i_var_number == 0) {
        SDLOGE("Add variable[%s] to UBD[%s] failure because variable number is zero!!!",
            i_var_name.c_str(), m_object_name.c_str());
        return;
    }

    var_iter = m_variable_map.find(i_var_name);
    if (var_iter != m_variable_map.end()) {
        SDLOGE("Add variable[%s] to UBD[%s] failure because variable name is exist!!!",
            i_var_name.c_str(), m_object_name.c_str());
        return;
    }
    else {
        m_variable_map[i_var_name] = UniformBufferVariableInfo(i_var_type, UniformBufferVariableInfo::TypeSizes[i_var_type], i_var_offset, i_var_number);
    }
}

void UniformBufferDescriptor::AddVariableDone()
{
    m_total_size = 0;
    std::map<std::string, UniformBufferVariableInfo>::iterator iter;
    for (iter = m_variable_map.begin(); iter != m_variable_map.end(); ++iter) {
        m_total_size += (*iter).second.GetTotalSize();
    }
}

UniformBufferVariableInfo UniformBufferDescriptor::GetVariableInfo(const std::string &i_var_name) const
{
    std::map<std::string, UniformBufferVariableInfo>::const_iterator var_iter = m_variable_map.find(i_var_name);
    if (var_iter != m_variable_map.end()) {
        return (*var_iter).second;
    }
    else {
        return UniformBufferVariableInfo();
    }
}

UniformVariableStrongReferenceObject UniformBufferDescriptor::AllocateUniformVariable()
{
    UniformBufferStrongReferenceObject ub = new UniformBuffer(m_object_name, m_binding_id);
    UniformBufferDescriptorWeakReferenceObject this_obj = GetThisWeakPtrByType<UniformBufferDescriptor>();
    ub.GetRef().Initialize(this_obj);
    return ub.StaticCastTo<UniformVariable>();
}

______________SD_END_GRAPHICS_NAMESPACE______________