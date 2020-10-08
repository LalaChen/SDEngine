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

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_COMPONENT_POOL_TYPE_IMPLEMENTATION(TransformComponent, TransformComponent);

TransformComponent::TransformComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
}

TransformComponent::~TransformComponent()
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
    if (m_parent_wref.IsNull() == false) {
        //T = Tp * Rp * Sp * Tx.
        //Tp-1 * T = Tp-1 * Tp * Rp * Sp * Tx
        //Rp-1 * Tp-1 * T = Rp-1 * (Tp-1 * Tp) * Rp * Sp * Tx
        //Sp-1 * Rp-1 * Tp-1 * T = Sp-1 * (Rp-1 * Rp) * Sp * Tx
        //Sp-1 * Rp-1 * Tp-1 * T = (Sp-1 * Sp) * Tx
        //Sp-1 * Rp-1 * Tp-1 * T =  Tx
        m_world_trans.m_position = i_position;
        m_local_trans.m_position = m_parent_wref.GetRef().m_world_trans.InverseVector3fToLocalSpace(m_world_trans.m_position);
    }
    else {
        m_world_trans.m_position = i_position;
        m_local_trans.m_position = m_world_trans.m_position;
    }

    UpdateChildrenWorldTransform();
}

void TransformComponent::SetWorldRotation(const Quaternion &i_rotation)
{
    if (m_parent_wref.IsNull() == false) {
        m_world_trans.m_rotation = i_rotation;
        m_local_trans.m_rotation = m_parent_wref.GetRef().m_world_trans.m_rotation.inverse() * m_world_trans.m_rotation;
    }
    else {
        m_world_trans.m_rotation = i_rotation;
        m_local_trans.m_rotation = m_world_trans.m_rotation;
    }

    UpdateChildrenWorldTransform();
}

void TransformComponent::SetWorldTransform(const Transform &i_transform)
{
    if (m_parent_wref.IsNull() == false) {
        m_world_trans = i_transform;
        m_local_trans = Transform::DecomposeMatrixToTransform(
            m_parent_wref.GetRef().m_world_trans.MakeWorldMatrix().inverse() * m_world_trans.MakeWorldMatrix());
    }
    else {
        m_world_trans = i_transform;
        m_local_trans = m_world_trans;
    }
    UpdateChildrenWorldTransform();
}

void TransformComponent::AddChild(TransformComponentWeakReferenceObject&i_target_so_sref)
{
    if (i_target_so_sref.IsNull() == false) {
        i_target_so_sref.GetRef().SetParent(GetThisWeakPtrByType<TransformComponent>());
    }
    else {
        SDLOGW("Set nullptr child.");
    }
}

bool TransformComponent::RemoveChild(const TransformComponentWeakReferenceObject &i_child_wref)
{
    if (i_child_wref != GetThisWeakPtrByType<TransformComponent>()) {
        for (std::list<TransformComponentWeakReferenceObject>::iterator child_iter = m_child_wrefs.begin();
             child_iter != m_child_wrefs.end();) {
            if ((*child_iter) == i_child_wref) {
                child_iter = m_child_wrefs.erase(child_iter);
                return true;
            }
            else {
                bool result = (*child_iter).GetRef().RemoveChild(i_child_wref);
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
void TransformComponent::SetParent(const TransformComponentWeakReferenceObject &i_parent_wref)
{
    if (i_parent_wref.IsNull() == false) {
        m_parent_wref = i_parent_wref;
        UpdateWorldTransform();
    }
    else {
        SDLOGW("Set nullptr parent.");
    }
}

void TransformComponent::UpdateWorldTransform()
{
    if (m_parent_wref.IsNull() == false) {
        m_world_trans = m_parent_wref.GetRef().m_world_trans * m_local_trans;
    }
    else {
        m_world_trans = m_local_trans;
    }
    UpdateChildrenWorldTransform();
}

void TransformComponent::UpdateChildrenWorldTransform()
{
    for (TransformComponentWeakReferenceObject so_wref : m_child_wrefs) {
        so_wref.GetRef().UpdateWorldTransform();
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________