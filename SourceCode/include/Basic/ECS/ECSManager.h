/*==============  SD Engine License ==============
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

/*! \file      Entity.h
 *  \brief     Introduce of class EntityManager
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/02
 *  \copyright MIT License.
 */

#pragma once

#include <map>
#include <functional>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "LogManager.h"
#include "System.h"
#include "ComponentPool.h"
#include "ComponentBase.h"
#include "EntitiyGroup.h"
#include "Entity.h"

______________SD_START_BASIC_NAMESPACE_______________

class SDENGINE_CLASS ECSManager
{
public:
    SD_SINGLETON_DECLARATION(ECSManager);
public:
    ECSManager();
    ~ECSManager();
public:
    EntityWeakReferenceObject CreateEntity(const ObjectName &i_object_name);
    bool DeleteEntity(const EntityWeakReferenceObject &i_entity_wref);
public:
    EntityGroupWeakReferenceObject AddEntityGroup(const ObjectName &i_group_name, const std::vector<std::type_index> &i_conditions);
public:
    template<typename T, typename... TArgs> ComponentBaseWeakReferenceObject AddComponentForEntity(const EntityWeakReferenceObject &i_entity_wref, TArgs &&...i_args);
    bool RemoveComponentFromEntity(const EntityWeakReferenceObject &i_entity_wref, const std::type_index &i_type_index);
public:
    SystemWeakReferenceObject RegisterSystem(const SystemStrongReferenceObject &i_system_sref);
    bool UnregisterSystem(const SystemWeakReferenceObject &i_system_wref);
public:
    void NotifyEntityChanged(const EntityWeakReferenceObject &i_entity_wref);
protected:
    void LinkComponentAndEntity(const EntityWeakReferenceObject &i_entity_wref, const ComponentBaseWeakReferenceObject &i_comp_wref, const std::type_index &i_type);
    bool DeleteComponent(const std::type_index &i_type_index, const ComponentBaseWeakReferenceObject &i_comp_wref);
protected:
    std::map<std::type_index, ComponentPoolStrongReferenceObject> m_comp_pool_srefs;
    std::list<EntityStrongReferenceObject> m_entity_srefs;
    std::list<EntityGroupStrongReferenceObject> m_entity_group_srefs;
    std::list<SystemStrongReferenceObject> m_system_srefs;
};

template<typename T, typename... TArgs>
ComponentBaseWeakReferenceObject ECSManager::AddComponentForEntity(const EntityWeakReferenceObject &i_entity_wref, TArgs &&...i_args)
{
    ComponentBaseWeakReferenceObject comp_wref;
    std::type_index target_type = T::GetComponentPoolType();
    if (i_entity_wref.IsNull() == false) {
        if (i_entity_wref.GetRef().IsComponentExisted(target_type) == false) {
            //find component pool and then new compoennt.
            std::map<std::type_index, ComponentPoolStrongReferenceObject>::iterator pool_iter =
                m_comp_pool_srefs.find(target_type);
            if (pool_iter != m_comp_pool_srefs.end()) {
                comp_wref = m_comp_pool_srefs[target_type].GetRef().NewComponent<T>(std::forward<TArgs>(i_args)...);
            }
            else {
                SDLOGD("Add new compoent pool with type(%s)", target_type.name());
                m_comp_pool_srefs[target_type] = new ComponentPool(StringFormat("%sComponentPool", target_type.name()));
                comp_wref = m_comp_pool_srefs[target_type].GetRef().NewComponent<T>(std::forward<TArgs>(i_args)...);
            }
            LinkComponentAndEntity(i_entity_wref, comp_wref, target_type);
            NotifyEntityChanged(i_entity_wref);
        }
        else {
            SDLOGE("Target component type(%s) is existed at Entity(%s)!!!", target_type.name(), i_entity_wref.GetRef().GetObjectName().c_str());
        }
    }
    else {
        SDLOGE("Add compoent type(%s) to null entity!!!", target_type.name());
    }
    return comp_wref;
}

_______________SD_END_BASIC_NAMESPACE________________