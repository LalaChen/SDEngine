/* ==============  SD Engine License ==============
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
#include "LightComponent.h"

using SDE::Basic::MemberFunctionSlot;

_____________SD_START_GRAPHICS_NAMESPACE_____________

LightComponent::LightComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
}

LightComponent::~LightComponent()
{
}

void LightComponent::Initialize()
{
    InitializeDescriptorSetAndPool();

    m_geo_comp_wref = SD_GET_COMP_WREF(m_entity_wref, TransformComponent);

    SD_WREF(m_geo_comp_wref).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<LightComponent>(
            "LightComponent::OnGeometryChanged",
            GetThisWeakPtrByType<LightComponent>(),
            &LightComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());
}

void LightComponent::InitializeDescriptorSetAndPool()
{
    m_dp_sref = new DescriptorPool("LightPool");
    std::map<ObjectName, UniformVariableWeakReferenceObject> uv_wrefs;
    //1. Allocated descriptor set for .
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    DescriptorSetLayoutWeakReferenceObject dsl_wref = GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Light");
    SD_SREF(dsl_wref).GetUniformDescriptorCounts(desc_counts);
    SD_SREF(m_dp_sref).Initialize(desc_counts, 1, false);

    m_ds_wref = SD_SREF(m_dp_sref).AllocateDescriptorSet(dsl_wref);
    SD_SREF(m_ds_wref).GetAllocatedUniformVariables(uv_wrefs);
    if (m_ds_wref.IsNull() == false) {
        SD_SREF(m_ds_wref).WriteDescriptor();
    }

    if (uv_wrefs.find("light") != uv_wrefs.end()) {
        m_buffer_wref = uv_wrefs["light"].DynamicCastTo<UniformBuffer>();
    }

    if (m_buffer_wref.IsNull() == true) {
        SDLOGE("We can't find light uniform buffer.");
    }
}

bool LightComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_buffer_wref.IsNull() == false) {
        Transform node_trans = SD_WREF(m_geo_comp_wref).GetWorldTransform();
        if (m_light_params.m_kind == 0) {
            m_light_params.m_direction = node_trans.GetForward().negative();
        }
        else if (m_light_params.m_kind == 1) {
            m_light_params.m_position = node_trans.m_position;
            m_light_params.m_direction = node_trans.GetForward().negative();
        }
        else if (m_light_params.m_kind == 2) {
            m_light_params.m_position = node_trans.m_position;
        }

        SD_WREF(m_buffer_wref).SetBufferData(&m_light_params, sizeof(LightUniforms));
        SD_WREF(m_buffer_wref).Update();
    }

    return true;
}

______________SD_END_GRAPHICS_NAMESPACE______________