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

/*! \file      CameraComponentBase.h
 *  \brief     The class CameraComponentBase is the interface.
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/10
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderFlow.h"
#include "Transform.h"
#include "Resolution.h"
#include "Area.h"
#include "LightComponent.h"
#include "MeshRenderComponent.h"

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

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CameraComponentBase);

enum CameraWorkspaceType {
    CameraWorkspaceType_Forward = 0,
    CameraWorkspaceType_Deferred,
    CameraWorkspaceType_Other
};

class SDENGINE_CLASS CameraComponentBase : public Component
{
public:
    static const std::string sCameraResizedEventName;
public:
    explicit CameraComponentBase(const ObjectName &i_object_name);
    virtual ~CameraComponentBase();
public:
    virtual bool OnGeometryChanged(const EventArg &i_arg) = 0;
public:
    void SetClearValues(ClearValue i_color, ClearValue i_d_and_s);
public:
    TextureWeakReferenceObject GetColorBuffer() const;
    TextureWeakReferenceObject GetDepthBuffer() const;
    DepthArea2D ConvertScreenAreaToWorldArea(AreaAlignOrientationEnum i_orientation, const Area2D &i_screen_area) const;
public:
    virtual DepthArea2D ConvertNCPAreaToWorldArea(const Area2D &i_ncp_area) const = 0;
    virtual void RecordCommand(
        const CommandBufferWeakReferenceObject &i_cb,
        const std::list<LightComponentWeakReferenceObject> &i_light_list,
        const std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> > &i_mr_groups) = 0;
protected:
    void ResizeImpl() override;
protected:
    virtual void InitializeDescriptorSetAndPool() = 0;
    virtual void InitializeWorkspaceForForwardPass() = 0;
    virtual void InitializeWorkspaceForDeferredPass() = 0;
protected:
    virtual void ClearWorkspace() = 0;
protected:
    TransformComponentWeakReferenceObject m_xform;
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
    Resolution m_buffer_size;
    bool m_follow_resolution;
    bool m_ws_initialized;
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
};

______________SD_END_GRAPHICS_NAMESPACE______________