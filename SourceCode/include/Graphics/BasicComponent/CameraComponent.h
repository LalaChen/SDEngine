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

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderFlow.h"
#include "Transform.h"
#include "Resolution.h"
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

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CameraComponent);

class SDENGINE_CLASS CameraComponent : public Component
{
public:
    friend class GraphicsSystem;
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(CameraComponent, CameraComponent);
public:
    enum WorkspaceType {
        WorkspaceType_Forward = 0,
        WorkspaceType_Deferred,
        WorkspaceType_Other
    };
public:
    explicit CameraComponent(const ObjectName &i_object_name);
    virtual ~CameraComponent();
public:
    void SetPerspective(float i_fov, float i_aspect, float i_near, float i_far);
    void SetClearValues(ClearValue i_color, ClearValue i_d_and_s);
    void SetCameraSize(const Resolution &i_size);
public:
    virtual void Initialize();
    virtual void Resize();
protected:
    void InitializeWorkspaceForForwardPath();
    void InitializeWorkspaceForDeferredPath();
protected:
    virtual void ClearWorkspace();
protected:
    WorkspaceType m_workspace_type;
    RenderFlowStrongReferenceObject m_rf_sref;
    TextureStrongReferenceObject m_color_buf_sref;
    TextureStrongReferenceObject m_depth_buf_sref;
protected:
    //Extra buffer for defer pass.
protected:
    Resolution m_screen_size;
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
    Matrix4X4f m_proj_mat;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
};

inline void CameraComponent::SetClearValues(ClearValue i_color, ClearValue i_d_and_s)
{
    m_clear_color = i_color;
    m_clear_d_and_s = i_d_and_s;
}

inline void CameraComponent::SetPerspective(float i_fov, float i_aspect, float i_near, float i_far)
{
    m_fov = i_fov;
    m_aspect = i_aspect;
    m_near = i_near;
    m_far = i_far;
}

inline void CameraComponent::SetCameraSize(const Resolution &i_screen_size)
{
    m_screen_size = i_screen_size;
}

______________SD_END_GRAPHICS_NAMESPACE______________