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

void LightComponent::InitializeImpl()
{
    InitializeDescriptorSetAndPool();

    m_geo_comp = SD_GET_COMP_WREF(m_entity, TransformComponent);

    SD_WREF(m_geo_comp).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<LightComponent>(
            "LightComponent::OnGeometryChanged",
            GetThisWeakPtrByType<LightComponent>(),
            &LightComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());
}

void LightComponent::InitializeDescriptorSetAndPool()
{
    m_dp = new DescriptorPool("LightPool");
    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    //1. Allocated descriptor set for .
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };
    DescriptorSetLayoutWeakReferenceObject light_dsl = GraphicsManager::GetRef().GetBasicDescriptorSetLayout(sUniformDescriptorSetLayout_Light);
    SD_SREF(light_dsl).GetUniformDescriptorCounts(desc_counts);
    SD_SREF(m_dp).Initialize(desc_counts, 1, false);

    m_ds = SD_SREF(m_dp).AllocateDescriptorSet(light_dsl);
    SD_SREF(m_ds).GetAllocatedUniformVariables(uvs);
    if (m_ds.IsNull() == false) {
        SD_SREF(m_ds).WriteDescriptor();
    }

    if (uvs.find(sUniformBuffer_Light) != uvs.end()) {
        m_ub = uvs[sUniformBuffer_Light].DynamicCastTo<UniformBuffer>();
    }

    if (m_ub.IsNull() == true) {
        SDLOGE("We can't find light uniform buffer.");
    }
}

bool LightComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_ub.IsNull() == false) {
        Transform node_trans = SD_WREF(m_geo_comp).GetWorldTransform();
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

        SD_WREF(m_ub).SetBufferData(&m_light_params, sizeof(LightUniforms));
        SD_WREF(m_ub).Update();
    }

    return true;
}

______________SD_END_GRAPHICS_NAMESPACE______________