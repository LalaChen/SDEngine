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

#include "BasicUniforms.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "MeshRenderComponent.h"

using SDE::Basic::MemberFunctionSlot;

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshRenderComponent::MeshRenderComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
}

MeshRenderComponent::~MeshRenderComponent()
{
}

void MeshRenderComponent::Initialize()
{
    std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };

    //1. Get DescriptorSet for MeshRenderer and count number of uniforms.
    DescriptorSetLayoutWeakReferenceObject basic_ds_layouts = GraphicsManager::GetRef().GetBasicDescriptorSetLayout("MeshRender");
    basic_ds_layouts.GetRef().GetUniformDescriptorCounts(desc_counts);

    //2. Allocate descriptor pool and set.
    m_geo_dp = new DescriptorPool("DescriptorSetPool");
    SD_SREF(m_geo_dp).Initialize(desc_counts, 1, false);
    m_geo_ds = SD_SREF(m_geo_dp).AllocateDescriptorSet(basic_ds_layouts);
    SD_WREF(m_geo_ds).GetAllocatedUniformVariables(uvs);

    //3. Write descriptor to GPU for this set.
    if (m_geo_ds.IsNull() == false) {
        SD_WREF(m_geo_ds).WriteDescriptor();
    }

    //4. Get basic uniform buffer for update MVP.
    if (uvs.find("geometry") != uvs.end()) {
        m_geo_ub = uvs["geometry"].DynamicCastTo<UniformBuffer>();
    }

    if (m_geo_ub.IsNull() == true) {
        SDLOGE("We can find geometry uniform buffer.");
    }

    m_geo_comp = SD_GET_COMP_WREF(m_entity, TransformComponent);
    SD_WREF(m_geo_comp).RegisterSlotFunctionIntoEvent(
        TransformComponent::sTransformChangedEventName,
        new MemberFunctionSlot<MeshRenderComponent>(
            "MeshRenderComponent::OnGeometryChanged",
            GetThisWeakPtrByType<MeshRenderComponent>(),
            &MeshRenderComponent::OnGeometryChanged));

    OnGeometryChanged(EventArg());
}

bool MeshRenderComponent::AppendMesh(const MeshWeakReferenceObject &i_mesh, const MaterialWeakReferenceObject &i_material)
{
    if (i_material.IsNull() == false && i_mesh.IsNull() == false) {
        m_mesh = i_mesh;
        m_material = i_material;
        return true;
    }
    else {
        SDLOGE("Add null data. (Mat, Mesh)(%d, %d)", i_material.IsNull(), m_mesh.IsNull());
        return false;
    }
}

void MeshRenderComponent::RenderMesh(
    const RenderPassWeakReferenceObject &i_rp,
    const CommandBufferWeakReferenceObject &i_cb,
    const DescriptorSetWeakReferenceObject &i_camera_ds,
    const DescriptorSetWeakReferenceObject & i_light_ds,
    uint32_t i_sp_id)
{
    if (m_mesh.IsNull() == false) {
        if (m_material.IsNull() == false) {
            std::vector<DescriptorSetWeakReferenceObject> common_d_sets = {
                i_camera_ds,
                m_geo_ds,
                i_light_ds
            };
            uint32_t step_amount = SD_WREF(m_material).GetStepAmount(i_rp, i_sp_id);
            for (uint32_t step_id = 0; step_id < step_amount; ++step_id) {
                //1. use material.
                SD_WREF(m_material).UseMaterial(
                    i_cb, i_rp,
                    common_d_sets,
                    i_sp_id, step_id);
                //2. bind mesh vertex attributes.
                SD_WREF(m_mesh).BindVertexBuffers(i_cb);
                //3. draw mesh.
                SD_WREF(m_mesh).BindIndexBuffer(i_cb);
                SD_WREF(m_mesh).Render(i_cb);
            }
        }
    }
}

bool MeshRenderComponent::OnGeometryChanged(const EventArg &i_arg)
{
    if (m_geo_ub.IsNull() == false) {
        SD_WREF(m_geo_ub).SetMatrix4X4f("world",
            SD_WREF(m_geo_comp).GetWorldTransform().MakeWorldMatrix());
        SD_WREF(m_geo_ub).SetMatrix4X4f("normal",
            SD_WREF(m_geo_comp).GetWorldTransform().MakeNormalMatrix());
        SD_WREF(m_geo_ub).Update();
    }

    return true;
}

______________SD_END_GRAPHICS_NAMESPACE______________