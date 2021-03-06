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

/*! \file      ECSManager.h
 *  \brief     Introduce of class ECSManager
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/02
 *  \copyright MIT License.
 */

#pragma once

#include <map>
#include <list>
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
    bool DeleteEntity(const EntityWeakReferenceObject &i_entity);
public:
    EntityGroupWeakReferenceObject AddEntityGroup(const ObjectName &i_group_name, const std::vector<std::type_index> &i_conditions);
public:
    template<typename T, typename... TArgs> ComponentBaseWeakReferenceObject AddComponentForEntity(const EntityWeakReferenceObject &i_entity, TArgs &&...i_args);
    bool RemoveComponentFromEntity(const EntityWeakReferenceObject &i_entity, const std::type_index &i_type_index);
public:
    template<typename T, typename... TArgs> SystemWeakReferenceObject RegisterSystem(TArgs &&...i_args);
    SystemWeakReferenceObject GetSystem(const std::type_index &i_type_index);
    void OverrideSystemUpdatingOrder(const std::list<std::type_index> &i_orders);
    bool UnregisterSystem(const std::type_index &i_type_index);
public:
    void NotifyEntityChanged(const EntityWeakReferenceObject &i_entity);
public:
    void Initialize();
    void Update();
    void Terminate();
    void Resize();
protected:
    void LinkComponentAndEntity(const EntityWeakReferenceObject &i_entity, const ComponentBaseWeakReferenceObject &i_comp, const std::type_index &i_type);
    bool DeleteComponent(const std::type_index &i_type_index, const ComponentBaseWeakReferenceObject &i_comp);
protected:
    void AddNewSystemIntoOrder(const std::type_index &i_type_index);
    void UpdateSystemOrders();
protected:
    std::map<std::type_index, ComponentPoolStrongReferenceObject> m_comp_pools;
    std::map<std::type_index, SystemStrongReferenceObject> m_system_map;
    std::list<EntityStrongReferenceObject> m_entity_list;
    std::list<EntityGroupStrongReferenceObject> m_entity_group_list;
    std::list<SystemWeakReferenceObject> m_system_updating_list;
    std::list<std::type_index> m_system_updating_orders;
};

template<typename T, typename... TArgs>
ComponentBaseWeakReferenceObject ECSManager::AddComponentForEntity(const EntityWeakReferenceObject &i_entity, TArgs &&...i_args)
{
    ComponentBaseWeakReferenceObject comp;
    std::type_index target_type = T::GetComponentPoolType();
    if (i_entity.IsNull() == false) {
        if (SD_WREF(i_entity).IsComponentExisted(target_type) == false) {
            //find component pool and then new compoennt.
            std::map<std::type_index, ComponentPoolStrongReferenceObject>::iterator pool_iter =
                m_comp_pools.find(target_type);
            if (pool_iter != m_comp_pools.end()) {
                comp = SD_SREF(m_comp_pools[target_type]).NewComponent<T>(std::forward<TArgs>(i_args)...);
            }
            else {
                SDLOGD("Add new compoent pool with type(%s)", target_type.name());
                m_comp_pools[target_type] = new ComponentPool(StringFormat("%sComponentPool", target_type.name()));
                comp = SD_SREF(m_comp_pools[target_type]).NewComponent<T>(std::forward<TArgs>(i_args)...);
            }
            LinkComponentAndEntity(i_entity, comp, target_type);
            NotifyEntityChanged(i_entity);
        }
        else {
            SDLOGE("Target component type(%s) is existed at Entity(%s)!!!", target_type.name(), SD_WREF(i_entity).GetObjectName().c_str());
        }
    }
    else {
        SDLOGE("Add compoent type(%s) to null entity!!!", target_type.name());
    }
    return comp;
}

template<typename T, typename... TArgs>
SystemWeakReferenceObject ECSManager::RegisterSystem(TArgs &&...i_args)
{
    std::type_index target_type = typeid(T);
    std::map<std::type_index, SystemStrongReferenceObject>::iterator sys_iter = m_system_map.find(target_type);
    if (sys_iter == m_system_map.end()) {
        SystemStrongReferenceObject system = new T(std::forward<TArgs>(i_args)...);
        m_system_map[target_type] = system;
        SD_SREF(m_system_map[target_type]).Initialize();
        AddNewSystemIntoOrder(target_type);
        UpdateSystemOrders();
        return m_system_map[target_type];
    }
    else {
        SDLOGE("Register system (type %s) repeatly.", target_type.name());
        return SystemWeakReferenceObject();
    }
}

_______________SD_END_BASIC_NAMESPACE________________