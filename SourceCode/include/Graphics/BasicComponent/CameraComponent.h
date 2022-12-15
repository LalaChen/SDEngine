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

/*! \file      CameraComponent.h
 *  \brief     The class CameraComponent is used to perform rendering at location of owner(Entity).
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/10
 *  \copyright MIT License.
 */

#pragma once

#include "Ray.h"
#include "CameraComponentBase.h"

using SDE::Basic::TouchButton;

using SDE::Physics::Ray;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CameraComponent);

class SDENGINE_CLASS CameraComponent : public CameraComponentBase
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(CameraComponent, CameraComponentBase);

public:
    explicit CameraComponent(const ObjectName &i_object_name);
    virtual ~CameraComponent();
public:
    void SetPerspective(float i_fov, float i_near, float i_far);
    void SetClearValues(ClearValue i_color, ClearValue i_d_and_s);
    void SetCameraSize(const Resolution &i_size);
    TextureWeakReferenceObject GetColorBuffer() const override;
    TextureWeakReferenceObject GetDepthBuffer() const override;
    Ray CalculateRay(const TouchButton &i_tb) const;
public:
    bool OnGeometryChanged(const EventArg &i_arg) override;
    void Resize() override;
    void RecordCommand(
        const CommandBufferWeakReferenceObject &i_cb,
        const std::list<LightComponentWeakReferenceObject> &i_light_list,
        const std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> > &i_mr_groups) override;
protected:
    void InitializeImpl() override;
protected:
    void InitializeDescriptorSetAndPool();
    void InitializeWorkspaceForForwardPass();
    void InitializeWorkspaceForDeferredPass();
protected:
    virtual void ClearWorkspace();
protected:
    CameraWorkspaceType m_workspace_type;
    RenderFlowStrongReferenceObject m_render_flow;
    TextureStrongReferenceObject m_color_buffer;
    TextureStrongReferenceObject m_depth_buffer;
protected:
    DescriptorPoolStrongReferenceObject m_dp;
    UniformBufferWeakReferenceObject m_ub;
    DescriptorSetWeakReferenceObject m_ds;
protected:
    //Extra buffer for defer pass.
protected:
    TransformComponentWeakReferenceObject m_xform_comp;
protected:
    bool m_ws_initialized;
    bool m_follow_resolution;
    Resolution m_buffer_size;
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
    Matrix4X4f m_proj_mat;
    float m_fov;
    float m_near;
    float m_far;
};

inline void CameraComponent::SetClearValues(ClearValue i_color, ClearValue i_d_and_s)
{
    m_clear_color = i_color;
    m_clear_d_and_s = i_d_and_s;
}

inline void CameraComponent::SetPerspective(float i_fov, float i_near, float i_far)
{
    m_fov = i_fov;
    m_near = i_near;
    m_far = i_far;
    m_proj_mat.perspective(m_fov, m_buffer_size.GetRatio(), i_near, i_far);
}

inline TextureWeakReferenceObject CameraComponent::GetColorBuffer() const
{
    if (IsInitialized() == true && m_ws_initialized == true) {
        return m_color_buffer;
    }
    else {
        return TextureWeakReferenceObject();
    }
}

inline TextureWeakReferenceObject CameraComponent::GetDepthBuffer() const
{
    if (IsInitialized() == true && m_ws_initialized == true) {
        return m_depth_buffer;
    }
    else {
        return TextureWeakReferenceObject();
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________