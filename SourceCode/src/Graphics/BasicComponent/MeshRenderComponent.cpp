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

#include "GraphicsManager.h"
#include "LogManager.h"
#include "MeshRenderComponent.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshRenderComponent::MeshRenderComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
    std::map<ObjectName, UniformVariableWeakReferenceObject> uv_wrefs;
    uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };

    //1. Get DescriptorSet for MeshRenderer and count number of uniforms.
    DescriptorSetLayoutWeakReferenceObject basic_dsl_wref = GraphicsManager::GetRef().GetBasicDescriptorSetLayout("MeshRender");
    basic_dsl_wref.GetRef().GetUniformDescriptorCounts(desc_counts);

    //2. Allocate descriptor pool and set.
    m_basic_pool_sref = new DescriptorPool("DescriptorSetPool");
    m_basic_pool_sref.GetRef().Initialize(desc_counts, 1, false);
    m_basic_set_wref = m_basic_pool_sref.GetRef().AllocateDescriptorSet(basic_dsl_wref);
    m_basic_set_wref.GetRef().LinkUniformVariables(uv_wrefs);

    //3. Write descriptor to GPU for this set.
    if (m_basic_set_wref.IsNull() == false) {
        m_basic_set_wref.GetRef().WriteDescriptor();
    }

    //4. Get basic uniform buffer for update MVP.
    if (uv_wrefs.find("basic") != uv_wrefs.end()) {
        m_basic_wrefs = uv_wrefs["basic"].DynamicCastTo<UniformBuffer>();
    }

    if (m_basic_wrefs.IsNull() == true) {
        SDLOGE("We can find basic uniform buffer.");
    }

}

MeshRenderComponent::~MeshRenderComponent()
{
}

bool MeshRenderComponent::AppendMesh(const MeshWeakReferenceObject &i_mesh_wref, const MaterialWeakReferenceObject &i_mat_wref)
{
    if (i_mat_wref.IsNull() == false && i_mesh_wref.IsNull() == false) {
        m_mesh_wref = i_mesh_wref;
        m_mat_wref = i_mat_wref;
        return true;
    }
    else {
        SDLOGE("Add null data. (Mat, Mesh)(%d, %d)", i_mat_wref.IsNull(), i_mesh_wref.IsNull());
        return false;
    }
}

void MeshRenderComponent::RenderMesh(
    const RenderPassWeakReferenceObject &i_rp_wref,
    const CommandBufferWeakReferenceObject &i_cb_wref,
    const DescriptorSetWeakReferenceObject &i_light_ds_wref,
    const DescriptorSetWeakReferenceObject &i_camera_ds_wref,
    uint32_t i_sp_id)
{
    if (m_mesh_wref.IsNull() == false) {
        if (m_mat_wref.IsNull() == false) {
            std::vector<DescriptorSetWeakReferenceObject> common_set_wrefs = {
                m_basic_set_wref,
                i_camera_ds_wref,
                i_light_ds_wref
            };
            //1. use material.
            m_mat_wref.GetRef().UseMaterial(
                i_cb_wref, i_rp_wref,
                common_set_wrefs,
                i_sp_id);
            //2. bind mesh vertex attributes.
            m_mesh_wref.GetRef().BindVertexBuffers(i_cb_wref);
            //3. draw mesh.
            m_mesh_wref.GetRef().BindIndexBuffer(i_cb_wref);
            m_mesh_wref.GetRef().Render(i_cb_wref);
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________