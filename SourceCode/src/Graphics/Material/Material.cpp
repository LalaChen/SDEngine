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

void Material::BindShaderProgram(const ShaderProgramWeakReferenceObject &i_sp)
{
    if (i_sp.IsNull() == true) {
        SDLOGE("Bind null sp into material[%s]", m_object_name.c_str());
        return;
    }

    SDLOG("Bind SP[%s] into Mat[%s]", SD_WREF(i_sp).GetObjectName().c_str(), m_object_name.c_str());

    m_is_linked = false;
    m_sp = i_sp;
    //1. prepare descriptor pool.
    m_dsp = new DescriptorPool(StringFormat("%s_DescriptorPool", m_object_name.c_str()));
    uint32_t descriptor_counts[UniformBindingType_MAX_DEFINE_VALUE] = {0};
    uint32_t max_set = SD_WREF(m_sp).GetPipelineAmount();
    SD_WREF(m_sp).GetDescriptorCount(descriptor_counts);
    SD_SREF(m_dsp).Initialize(descriptor_counts, max_set, false);
    //2. allocate uniform variables for uniform descriptors and descriptor sets.
    DescriptorPoolWeakReferenceObject dsp = m_dsp.StaticCastToWeakPtr<DescriptorPool>();
    SD_WREF(m_sp).AllocateEssentialObjects(m_uvs, m_dsets, dsp);
}

void Material::LinkWithShaderProgram()
{
    if (m_sp.IsNull() == false) {
        for (DescriptorSetWeakReferenceObject &ds : m_dsets) {
            ds.GetRef().WriteDescriptor();
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetInt(i_var_name, i_value, i_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetUint(i_var_name, i_value, i_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetFloat(i_var_name, i_value, i_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetVector3f(i_var_name, i_value, i_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetColor4f(i_var_name, i_value, i_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetMatrix4X4f(i_var_name, i_value, i_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetIntArray(i_var_name, i_datas, i_start_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetUintArray(i_var_name, i_datas, i_start_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetFloatArray(i_var_name, i_datas, i_start_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetVector3fArray(i_var_name, i_datas, i_start_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetColor4fArray(i_var_name, i_datas, i_start_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetMatrix4X4fArray(i_var_name, i_datas, i_start_idx);
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
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_ub_name);
    if (uv_iter != m_uvs.end()) {
        UniformBufferWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformBuffer>();
        if (target_uv.IsNull() == false) {
            return target_uv.GetRef().SetBufferData(i_data, i_data_size);
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

bool Material::SetTexture(const ObjectName &i_uv_name, const TextureWeakReferenceObject &i_tex, int32_t i_idx)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter = m_uvs.find(i_uv_name);
    if (uv_iter != m_uvs.end()) {
        UniformImagesWeakReferenceObject target_uv = (*uv_iter).second.DynamicCastTo<UniformImages>();
        if (target_uv.IsNull() == false) {
            bool result = target_uv.GetRef().SetTexture(i_tex, i_idx);
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
    const CommandBufferWeakReferenceObject &i_cb,
    const RenderPassWeakReferenceObject &i_rp,
    const std::vector<DescriptorSetWeakReferenceObject> &i_common_dss,
    uint32_t i_sp_id,
    uint32_t i_step_id)
{
    std::vector<DescriptorSetWeakReferenceObject> dss = i_common_dss;
    dss.insert(std::end(dss), std::begin(m_dsets), std::end(m_dsets));
    if (m_sp.IsNull() == false) {
        m_sp.GetRef().UseProgramWithTargetDescriptorSet(i_cb, i_rp, i_sp_id, i_step_id, dss);
    }
}

void Material::Update()
{
    std::map<ObjectName, UniformVariableWeakReferenceObject>::iterator uv_iter;
    for (uv_iter = m_uvs.begin(); uv_iter != m_uvs.end(); ++uv_iter) {
        (*uv_iter).second.GetRef().Update();
    }
}

uint32_t Material::GetStepAmount(const RenderPassWeakReferenceObject &i_rp, uint32_t i_sp_id) const
{
    return SD_WREF(m_sp).GetStepAmount(i_rp, i_sp_id);
}

______________SD_END_GRAPHICS_NAMESPACE______________
