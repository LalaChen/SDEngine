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
#include "TransformComponent.h"

using SDE::Basic::Event;
using SDE::Basic::EventStrongReferenceObject;
using SDE::Basic::EventWeakReferenceObject;

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectStrongReferenceObject;
using SDE::Basic::EventObjectWeakReferenceObject;

using SDE::Basic::EventArg;

_____________SD_START_GRAPHICS_NAMESPACE_____________

const std::string TransformComponent::sTransformChangedEventName = "TransformChanged";

TransformComponent::TransformComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
    RegisterEvent(new Event(sTransformChangedEventName));
}

TransformComponent::~TransformComponent()
{
    for (std::list<TransformComponentWeakReferenceObject>::iterator t_iter = m_children.begin(); t_iter != m_children.end();) {
        SD_WREF((*t_iter)).SetParent(m_parent);
        t_iter = m_children.erase(t_iter);
    }
    if (m_parent.IsNull() == false) {
        SD_WREF(m_parent).EraseChild(GetThisWeakPtrByType<TransformComponent>());
    }
}

void TransformComponent::Initialize()
{
}

void TransformComponent::SetLocalPosition(const Vector3f &i_position)
{
    m_local_trans.m_position = i_position;
    UpdateWorldTransform();
}

void TransformComponent::SetLocalRotation(const Quaternion &i_rotation)
{
    m_local_trans.m_rotation = i_rotation;
    UpdateWorldTransform();
}

void TransformComponent::SetLocalScale(const Vector3f &i_scale)
{
    m_local_trans.m_scale = i_scale;
    UpdateWorldTransform();
}

void TransformComponent::SetLocalTransform(const Transform &i_transform)
{
    m_local_trans = i_transform;
    UpdateWorldTransform();
}

void TransformComponent::SetWorldPosition(const Vector3f &i_position)
{
    if (m_parent.IsNull() == false) {
        //T = Tp * Rp * Sp * Tx.
        //Tp-1 * T = Tp-1 * Tp * Rp * Sp * Tx
        //Rp-1 * Tp-1 * T = Rp-1 * (Tp-1 * Tp) * Rp * Sp * Tx
        //Sp-1 * Rp-1 * Tp-1 * T = Sp-1 * (Rp-1 * Rp) * Sp * Tx
        //Sp-1 * Rp-1 * Tp-1 * T = (Sp-1 * Sp) * Tx
        //Sp-1 * Rp-1 * Tp-1 * T =  Tx
        m_world_trans.m_position = i_position;
        m_local_trans.m_position = SD_WREF(m_parent).m_world_trans.InverseVector3fToLocalSpace(m_world_trans.m_position);
    }
    else {
        m_world_trans.m_position = i_position;
        m_local_trans.m_position = m_world_trans.m_position;
    }
    NotifyEvent(sTransformChangedEventName, EventArg());
    UpdateChildrenWorldTransform();
}

void TransformComponent::SetWorldRotation(const Quaternion &i_rotation)
{
    if (m_parent.IsNull() == false) {
        m_world_trans.m_rotation = i_rotation;
        m_local_trans.m_rotation = SD_WREF(m_parent).m_world_trans.m_rotation.inverse() * m_world_trans.m_rotation;
    }
    else {
        m_world_trans.m_rotation = i_rotation;
        m_local_trans.m_rotation = m_world_trans.m_rotation;
    }

    NotifyEvent(sTransformChangedEventName, EventArg());
    UpdateChildrenWorldTransform();
}

void TransformComponent::SetWorldTransform(const Transform &i_transform)
{
    if (m_parent.IsNull() == false) {
        m_world_trans = i_transform;
        m_local_trans = Transform::DecomposeMatrixToTransform(
            SD_WREF(m_parent).m_world_trans.MakeWorldMatrix().inverse() * m_world_trans.MakeWorldMatrix());
    }
    else {
        m_world_trans = i_transform;
        m_local_trans = m_world_trans;
    }

    NotifyEvent(sTransformChangedEventName, EventArg());
    UpdateChildrenWorldTransform();
}

void TransformComponent::AddChild(const TransformComponentWeakReferenceObject &i_target_so)
{
    if (i_target_so.IsNull() == false) {
        SD_WREF(i_target_so).SetParent(GetThisWeakPtrByType<TransformComponent>());
        m_children.push_back(i_target_so);
    }
    else {
        SDLOGW("Set nullptr child.");
    }
}

bool TransformComponent::RemoveChild(const TransformComponentWeakReferenceObject &i_child)
{
    if (i_child != GetThisWeakPtrByType<TransformComponent>()) {
        std::list<TransformComponentWeakReferenceObject>::iterator child_iter;
        for (child_iter = m_children.begin(); child_iter != m_children.end();) {
            if ((*child_iter) == i_child) {
                child_iter = m_children.erase(child_iter);
                return true;
            }
            else {
                bool result = SD_WREF((*child_iter)).RemoveChild(i_child);
                if (result == false) {
                    child_iter++;
                }
                else {
                    return true;
                }
            }
        }
        return false;
    }
    else {
        SDLOGE("Delete self.");
        return false;
    }
}

//------------- protected function --------------
void TransformComponent::SetParent(const TransformComponentWeakReferenceObject &i_parent)
{
    if (i_parent.IsNull() == false) {
        m_parent = i_parent;
        UpdateWorldTransform();
    }
    else {
        SDLOGW("Set nullptr parent.");
    }
}

void TransformComponent::EraseChild(const TransformComponentWeakReferenceObject &i_child)
{
    for (std::list<TransformComponentWeakReferenceObject>::iterator iter = m_children.begin(); iter != m_children.end();) {
        if ((*iter) != i_child) {
            iter++;
        }
        else {
            iter = m_children.erase(iter);
            break;
        }
    }
}

void TransformComponent::UpdateWorldTransform()
{
    if (m_parent.IsNull() == false) {
        m_world_trans = SD_WREF(m_parent).m_world_trans * m_local_trans;
    }
    else {
        m_world_trans = m_local_trans;
    }
    NotifyEvent(sTransformChangedEventName, EventArg());
    UpdateChildrenWorldTransform();
}

void TransformComponent::UpdateChildrenWorldTransform()
{
    for (TransformComponentWeakReferenceObject &so : m_children) {
        SD_WREF(so).UpdateWorldTransform();
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________