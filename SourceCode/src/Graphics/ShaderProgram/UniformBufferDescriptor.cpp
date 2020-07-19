#include "LogManager.h"
#include "UniformBuffer.h"
#include "UniformBufferDescriptor.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

UniformBufferDescriptor::UniformBufferDescriptor(const ObjectName &i_name, Size_ui32 i_binding_id, Size_ui32 i_number)
: UniformVariableDescriptor(i_name, i_binding_id, i_number)
, m_total_size(0)
{
}

UniformBufferDescriptor::~UniformBufferDescriptor()
{
}

void UniformBufferDescriptor::AddVariable(const std::string &i_var_name, Size_ui32 i_var_type_size, Size_ui32 i_var_offset, Size_ui32 i_var_number)
{
    std::map<std::string, UniformBufferVariableInfo>::iterator var_iter;
    
    if (IsInitialized() == true) {
        SDLOGE("Add variable[%s] to UB[%s] failure because UBD is initialized !!!",
            i_var_name.c_str(), m_object_name.c_str());
    }

    if (i_var_type_size == 0) {
        SDLOGE("Add variable[%s] to UBD[%s] failure because variable type size is zero!!!",
            i_var_name.c_str(), m_object_name.c_str());
    }

    if (i_var_number == 0) {
        SDLOGE("Add variable[%s] to UBD[%s] failure because variable number is zero!!!",
            i_var_name.c_str(), m_object_name.c_str());
    }

    var_iter = m_variable_map.find(i_var_name);
    if (var_iter != m_variable_map.end() ){
        SDLOGE("Add variable[%s] to UBD[%s] failure because variable name is exist!!!",
            i_var_name.c_str(), m_object_name.c_str());
    }
    else {
        m_variable_map[i_var_name] = UniformBufferVariableInfo(i_var_type_size, i_var_offset, i_var_number);
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
    UniformBufferStrongReferenceObject ub_sref = new UniformBuffer(m_object_name, m_binding_id);
    UniformBufferDescriptorWeakReferenceObject this_wref = GetThisWeakPtrByType<UniformBufferDescriptor>();
    ub_sref.GetRef().Initialize(this_wref);
    return ub_sref.StaticCastTo<UniformVariable>();
}

______________SD_END_GRAPHICS_NAMESPACE______________