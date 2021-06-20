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

/*! \file      VRCameraComponent.h
 *  \brief     The class CameraComponent is used to perform rendering for VR at location of owner(Entity).
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/06/06
 *  \copyright MIT License.
 */

#pragma once

#include "CameraComponentBase.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum VREyeEnum
{
    VREye_Left = 0,
    VREye_Right,
    VREye_Both
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(VRCameraComponent);

class SDENGINE_CLASS VRCameraComponent : public CameraComponentBase
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(VRCameraComponent, CameraComponentBase);
public:
    explicit VRCameraComponent(const ObjectName &i_object_name);
    virtual ~VRCameraComponent();
public:
    void SetEyeCenters(Vector3f i_eye_centers[VREye_Both]);
    void SetProjectionMatrices(Matrix4X4f i_proj_mats[VREye_Both]);
public:
    TextureWeakReferenceObject GetColorBuffer() const;
    TextureWeakReferenceObject GetDepthBuffer() const;
public:
    bool OnGeometryChanged(const EventArg &i_arg) override;
public:
    void Initialize() override;
    void Resize() override;
    void RecordCommand(
        const CommandBufferWeakReferenceObject &i_cb,
        const std::list<LightComponentWeakReferenceObject> &i_light_list,
        const std::list<MeshRenderComponentWeakReferenceObject> &i_mesh_render_list) override;
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
    TransformComponentWeakReferenceObject m_geo_comp;
protected:
    bool m_initialized;
    bool m_follow_resolution;
    Resolution m_buffer_size;
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
    Vector3f m_eye_centers[VREye_Both];
    Matrix4X4f m_proj_mats[VREye_Both];
};


______________SD_END_GRAPHICS_NAMESPACE______________