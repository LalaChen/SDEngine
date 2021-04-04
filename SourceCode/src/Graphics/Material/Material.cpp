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

#include <algorithm>

#include "LogManager.h"
#include "Material.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

Material::Material(const ObjectName &i_name)
: Object(i_name)
, m_is_linked(false)
{
}

Material::~Material()
{
}

void Material::BindShaderProgram(const ShaderProgramWeakReferenceObject &i_sp_wref)
{
    m_is_linked = false;
    m_sp_wref = i_sp_wref;
    //1. prepare descriptor pool.
    m_dsp_sref = new DescriptorPool(StringFormat("%s_DescriptorPool", m_object_name.c_str()));
    uint32_t descriptor_counts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
    uint32_t max_set = m_sp_wref.GetRef().GetPipelineAmount();
    m_sp_wref.GetRef().GetDescriptorCount(descriptor_counts);
    m_dsp_sref.GetRef().Initialize(descriptor_counts, max_set, false);
    //2. allocate uniform variables for uniform descriptors and descriptor sets.
    DescriptorPoolWeakReferenceObject dp_wref = m_dsp_sref.StaticCastToWeakPtr<DescriptorPool>();
    m_sp_wref.GetRef().AllocateEssentialObjects(m_uv_wrefs, m_ds_wrefs, dp_wref);
}

void Material::LinkWithShaderProgram()
{
    if (m_sp_wref.IsNull() == false) {
        for (DescriptorSetWeakReferenceObject &ds_wref : m_ds_wrefs) {
            ds_wref.GetRef().WriteDescriptor();
        }
        m_is_linked = true;
    }
    else {
        SDLOGE("No target shader.");
    }
}

void Material::RefreshLinkingWithShaderProgram()
{
    if (m_is_linked == false) {
        return;
    }
    LinkWithShaderProgram();
}

bool Material::SetInt(const ObjectName &i_ub_name, const ObjectName &i_var_name, int32_t i_value, Size_ui32 i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetInt(i_var_name, i_value, i_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetUint(const ObjectName &i_ub_name, const ObjectName &i_var_name, uint32_t i_value, Size_ui32 i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetUint(i_var_name, i_value, i_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetFloat(const ObjectName &i_ub_name, const ObjectName &i_var_name, float i_value, Size_ui32 i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetFloat(i_var_name, i_value, i_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetVector3f(const ObjectName &i_ub_name, const ObjectName &i_var_name, const Vector3f &i_value, Size_ui32 i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetVector3f(i_var_name, i_value, i_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetColor4f(const ObjectName &i_ub_name, const ObjectName &i_var_name, const Color4f &i_value, Size_ui32 i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetColor4f(i_var_name, i_value, i_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetMatrix4X4f(const ObjectName &i_ub_name, const ObjectName &i_var_name, const Matrix4X4f &i_value, Size_ui32 i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetMatrix4X4f(i_var_name, i_value, i_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetIntArray(const ObjectName &i_ub_name, const ObjectName &i_var_name, const std::vector<int32_t> &i_datas, Size_ui32 i_start_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetIntArray(i_var_name, i_datas, i_start_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetUintArray(const ObjectName &i_ub_name, const ObjectName &i_var_name, const std::vector<uint32_t> &i_datas, Size_ui32 i_start_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetUintArray(i_var_name, i_datas, i_start_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetFloatArray(const ObjectName &i_ub_name, const ObjectName &i_var_name, const std::vector<float> &i_datas, Size_ui32 i_start_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetFloatArray(i_var_name, i_datas, i_start_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}
bool Material::SetVector3fArray(const ObjectName &i_ub_name, const ObjectName &i_var_name, const std::vector<Vector3f> &i_datas, Size_ui32 i_start_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetVector3fArray(i_var_name, i_datas, i_start_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetColor4fArray(const ObjectName &i_ub_name, const ObjectName &i_var_name, const std::vector<Color4f> &i_datas, Size_ui32 i_start_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetColor4fArray(i_var_name, i_datas, i_start_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}
bool Material::SetMatrix4X4fArray(const ObjectName &i_ub_name, const ObjectName &i_var_name, const std::vector<Matrix4X4f> &i_datas, Size_ui32 i_start_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetMatrix4X4fArray(i_var_name, i_datas, i_start_idx);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetDataToUniformBuffer(const ObjectName &i_ub_name, const void *i_data, Size_ui32 i_data_size)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_ub_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformBufferWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv_wref.IsNull() == false) {
            return target_uv_wref.GetRef().SetBufferData(i_data, i_data_size);
        }
        else {
            SDLOGW("UB[%s] isn't uniform buffer in this materal.", i_ub_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UB[%s] in this materal.", i_ub_name.c_str());
        return false;
    }
}

bool Material::SetTexture(const ObjectName &i_uv_name, const TextureWeakReferenceObject &i_tex_wref, int32_t i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uv_wrefs.find(i_uv_name);
    if (uv_iter != m_uv_wrefs.end()) {
        UniformImagesWeakReferenceObject target_uv_wref = (*uv_iter).second.DynamicCastTo<UniformImages>();
        if (target_uv_wref.IsNull() == false) {
            bool result = target_uv_wref.GetRef().SetTexture(i_tex_wref, i_idx);
            //rebind texture and update write descriptor.
            RefreshLinkingWithShaderProgram();
            //
            return result;
        }
        else {
            SDLOGW("UImages[%s] isn't uniform image in this materal.", i_uv_name.c_str());
            return false;
        }
    }
    else {
        SDLOGW("We don't find UImages[%s] in this materal.", i_uv_name.c_str());
        return false;
    }
}

void Material::UseMaterial(
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const RenderPassWeakReferenceObject &i_rp_wref,
    const std::vector<DescriptorSetWeakReferenceObject> &i_common_ds_wrefs,
    uint32_t i_sp_id,
    uint32_t i_step_id)
{
    std::vector<DescriptorSetWeakReferenceObject> ds_wrefs = i_common_ds_wrefs;
    ds_wrefs.insert(std::end(ds_wrefs), std::begin(m_ds_wrefs), std::end(m_ds_wrefs));
    if (m_sp_wref.IsNull() == false) {
        m_sp_wref.GetRef().UseProgramWithTargetDescriptorSet(i_cb_wref, i_rp_wref, i_sp_id, i_step_id, ds_wrefs);
    }
}

void Material::Update()
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter;
    for (uv_iter = m_uv_wrefs.begin(); uv_iter != m_uv_wrefs.end(); ++uv_iter) {
        (*uv_iter).second.GetRef().Update();
    }
}

uint32_t Material::GetStepAmount(const RenderPassWeakReferenceObject &i_rp_wref, uint32_t i_sp_id) const
{
    return SD_WREF(m_sp_wref).GetStepAmount(i_rp_wref, i_sp_id);
}

______________SD_END_GRAPHICS_NAMESPACE______________
