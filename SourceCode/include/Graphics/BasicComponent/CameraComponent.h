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

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(TransformComponent);

class SDENGINE_CLASS CameraComponent : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION;
public:
    explicit CameraComponent(const ObjectName &i_object_name);
    virtual ~CameraComponent();
public:
    void SetPerspective(float i_angle, float i_aspect, float i_near, float i_far);
    void SetRenderPass(const RenderPassWeakReferenceObject &i_rp_wref);
    void SetClearValues(ClearValue i_color, ClearValue i_d_and_s);
public:
    void Initialize();
    void Resize();
    void Render();
protected:
    RenderFlowStrongReferenceObject m_rf_sref;
    RenderPassWeakReferenceObject m_custom_rp_wref;
    TextureStrongReferenceObject m_color_buf_sref;
    TextureStrongReferenceObject m_depth_buf_sref;
protected:
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
    Matrix4X4f m_proj_mat;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
};

inline void CameraComponent::SetRenderPass(const RenderPassWeakReferenceObject &i_rp_wref)
{
    m_custom_rp_wref = i_rp_wref;
}

inline void CameraComponent::SetClearValues(ClearValue i_color, ClearValue i_d_and_s)
{
    m_clear_color = i_color;
    m_clear_d_and_s = i_d_and_s;
}

______________SD_END_GRAPHICS_NAMESPACE______________