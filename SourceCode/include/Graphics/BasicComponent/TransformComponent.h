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

/*! \file      TransformComponent.h
 *  \brief     The class TransformComponent is used to describe geometry information and entity of each object.
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/01
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "Component.h"
#include "Transform.h"

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

class SDENGINE_CLASS TransformComponent : public Component
{
public:
    static const std::string sTransformChangedEventName;
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(TransformComponent, TransformComponent);
public:
    explicit TransformComponent(const ObjectName &i_object_name);
    virtual ~TransformComponent();
public:
    void Initialize();
public:
    void SetLocalPosition(const Vector3f &i_position);
    void SetLocalRotation(const Quaternion &i_rotation);
    void SetLocalScale(const Vector3f &i_scale);
    void SetLocalTransform(const Transform &i_transform);
    void SetWorldPosition(const Vector3f &i_position);
    void SetWorldRotation(const Quaternion &i_rotation);
    void SetWorldTransform(const Transform &i_transform);
    const Transform& GetWorldTransform() const;
    const Transform& GetLocalTransform() const;
public:
    void AddChild(const TransformComponentWeakReferenceObject &i_target);
    bool RemoveChild(const TransformComponentWeakReferenceObject &i_child);
protected:
    void SetParent(const TransformComponentWeakReferenceObject &i_parent);
    void UpdateChildrenWorldTransform();
    void UpdateWorldTransform();
protected:
    TransformComponentWeakReferenceObject m_parent;
    std::list<TransformComponentWeakReferenceObject> m_childs;
protected:
    Transform m_world_trans;
    Transform m_local_trans;
};

inline const Transform& TransformComponent::GetWorldTransform() const
{
    return m_world_trans;
}

inline const Transform& TransformComponent::GetLocalTransform() const
{
    return m_local_trans;
}

______________SD_END_GRAPHICS_NAMESPACE______________