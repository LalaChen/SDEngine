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

/*! \file      MeshRenderComponent.h
 *  \brief     The class MeshRenderComponent is used to render mesh at location of owner(Entity).
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/10
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderFlow.h"
#include "Material.h"
#include "Mesh.h"
#include "TransformComponent.h"
#include "Component.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

using SDE::Basic::Component;
using SDE::Basic::ComponentStrongReferenceObject;
using SDE::Basic::ComponentWeakReferenceObject;

using SDE::Math::Transform;
using SDE::Math::Vector3f;
using SDE::Math::Quaternion;
using SDE::Math::Matrix4X4f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(MeshRenderComponent);

class SDENGINE_CLASS MeshRenderComponent : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(MeshRenderComponent, MeshRenderComponent);
public:
    explicit MeshRenderComponent(const ObjectName &i_object_name);
    virtual ~MeshRenderComponent();
public:
    void Initialize();
    bool AppendMesh(const MeshWeakReferenceObject &i_mesh_wref, const MaterialWeakReferenceObject &i_mat_wref);
    void RenderMesh(
        const RenderPassWeakReferenceObject &i_rp_wref,
        const CommandBufferWeakReferenceObject &i_cb_wref,
        const DescriptorSetWeakReferenceObject &i_camera_ds_wref,
        const DescriptorSetWeakReferenceObject &i_light_ds_wref,
        uint32_t i_sp_id);

    bool OnGeometryChanged(const EventArg &i_arg);
protected:
    MeshWeakReferenceObject m_mesh_wref;
    MaterialWeakReferenceObject m_mat_wref;
protected:
    //Create common descriptorset and buffer for meshes.
    DescriptorPoolStrongReferenceObject m_geo_pool_sref;
    UniformBufferWeakReferenceObject m_geo_ub_wrefs;
    DescriptorSetWeakReferenceObject m_geo_set_wref;
protected:
    TransformComponentWeakReferenceObject m_geo_comp_wref;
};


______________SD_END_GRAPHICS_NAMESPACE______________