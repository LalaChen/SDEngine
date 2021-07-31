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

#include "GraphicsSystem.h"
#include "Component.h"
#include "LogManager.h"
#include "ECSManager.h"

using SDE::Basic::ECSManager;
using SDE::Graphics::GraphicsSystem;

SD_SINGLETON_DECLARATION_IMPL(ECSManager);

______________SD_START_BASIC_NAMESPACE_______________

ECSManager::ECSManager()
{
    SD_SINGLETON_DECLARATION_REGISTER;
}

ECSManager::~ECSManager()
{
}

EntityWeakReferenceObject ECSManager::CreateEntity(const ObjectName &i_object_name)
{
    EntityStrongReferenceObject entity = new Entity(i_object_name);
    m_entity_list.push_back(entity);
    return entity.DynamicCastToPtr<Entity>();
}

bool ECSManager::DeleteEntity(const EntityWeakReferenceObject &i_entity)
{
    //1. Find out entity.
    std::list<EntityStrongReferenceObject>::iterator entity_iter;
    for (entity_iter = m_entity_list.begin(); entity_iter != m_entity_list.end(); ) {
        if ((*entity_iter) == i_entity) {
            //2. Remove its weak reference at all groups.
            for (EntityGroupStrongReferenceObject &group : m_entity_group_list) {
                SD_SREF(group).RemoveEntity(i_entity);
            }
            //3. Remove it form entity list.
            entity_iter = m_entity_list.erase(entity_iter);
            return true;
        }
        else {
            entity_iter++;
        }
    }
    return false;
}

EntityGroupWeakReferenceObject ECSManager::AddEntityGroup(const ObjectName &i_group_name, const std::vector<std::type_index> &i_conditions)
{
    //1. Change entity groups is exist or not.
    for (EntityGroupStrongReferenceObject &entity_group : m_entity_group_list) {
        if (SD_SREF(entity_group).IsCorresponded(i_conditions) == true) {
            return entity_group;
        }
    }

    //2. Collect all entities belog this group.
    EntityGroupStrongReferenceObject group = new EntityGroup(i_group_name, i_conditions);
    m_entity_group_list.push_back(group);    
    for (EntityStrongReferenceObject &entity : m_entity_list) {
        SD_SREF(group).AddEntity(entity);
    }
    return group;
}

bool ECSManager::RemoveComponentFromEntity(const EntityWeakReferenceObject &i_entity, const std::type_index &i_type_index)
{
    bool result = false;
    if (i_entity.IsNull() == false) {
        //1. Unregister component at entity.
        ComponentBaseWeakReferenceObject rem_comp = SD_WREF(i_entity).UnregisterComponent(i_type_index);
        //2. Delete component at entity.
        result = DeleteComponent(i_type_index, rem_comp);
        //3. Update all entity groups for this entity.
        for (EntityGroupStrongReferenceObject &group : m_entity_group_list) {
            SD_SREF(group).RemoveEntity(i_entity);
        }
    }
    else {
        SDLOGE("Remove component with type(%s) at null entity.", i_type_index.name());
    }
    return false;
}

void ECSManager::LinkComponentAndEntity(const EntityWeakReferenceObject &i_entity, const ComponentBaseWeakReferenceObject &i_comp, const std::type_index &i_type)
{
    SD_WREF(i_entity).RegisterComponent(i_type, i_comp);
    SD_WREF(i_comp.DynamicCastTo<Component>()).SetEntity(i_entity);
}

bool ECSManager::DeleteComponent(const std::type_index &i_type_index, const ComponentBaseWeakReferenceObject &i_comp)
{
    std::map<std::type_index, ComponentPoolStrongReferenceObject>::iterator comp_pool_iter = m_comp_pools.find(i_type_index);
    if (comp_pool_iter != m_comp_pools.end()) {
        return SD_SREF((*comp_pool_iter).second).DeleteComponent(i_comp);
    }
    else {
        SDLOGW("Can't find component pool with [%s].", i_type_index.name());
        return false;
    }
}

bool ECSManager::UnregisterSystem(const std::type_index &i_type_index)
{
    std::map<std::type_index, SystemStrongReferenceObject>::iterator sys_iter = m_system_map.find(i_type_index);
    if (sys_iter != m_system_map.end()) {
        m_system_map.erase(sys_iter);
        return true;
    }
    else {
        SDLOGE("Unregister non-exist system.");
        return false;
    }
}

SystemWeakReferenceObject ECSManager::GetSystem(const std::type_index &i_type_index)
{
    return m_system_map[i_type_index];
}


void ECSManager::AddNewSystemIntoOrder(const std::type_index &i_type_index)
{
    std::list<std::type_index>::iterator cur_tid_iter;
    std::list<std::type_index>::iterator pre_tid_iter;

    for (cur_tid_iter = m_system_updating_orders.begin(); cur_tid_iter != m_system_updating_orders.end(); ++cur_tid_iter) {
        if ((*cur_tid_iter) == i_type_index) {
            return;
        }
    }

    cur_tid_iter = m_system_updating_orders.begin();
    pre_tid_iter = cur_tid_iter;

    if ((*cur_tid_iter) == typeid(GraphicsSystem)) {
        m_system_updating_orders.push_front(i_type_index);
    }
    else {
        for (; cur_tid_iter != m_system_updating_orders.end(); ++cur_tid_iter) {
            if ((*cur_tid_iter) == typeid(GraphicsSystem)) {
                m_system_updating_orders.insert(pre_tid_iter, i_type_index);
                break;
            }
            pre_tid_iter = cur_tid_iter;
        }
    }
}

void ECSManager::UpdateSystemOrders()
{
    m_system_updating_list.clear();
    for (std::type_index &tid : m_system_updating_orders) {
        std::map<std::type_index, SystemStrongReferenceObject>::iterator sys_iter = m_system_map.find(tid);
        if (sys_iter != m_system_map.end()) {
            m_system_updating_list.push_back((*sys_iter).second);
        }
    }
}

void ECSManager::OverrideSystemUpdatingOrder(const std::list<std::type_index> &i_orders)
{
    m_system_updating_orders = i_orders;
}

void ECSManager::NotifyEntityChanged(const EntityWeakReferenceObject &i_entity)
{
    for (EntityGroupStrongReferenceObject &entity_group : m_entity_group_list) {
        SD_SREF(entity_group).AddEntity(i_entity);
    }
}

void ECSManager::Initialize()
{
    m_system_updating_orders.push_back(typeid(GraphicsSystem));
}

void ECSManager::Update()
{
    for (SystemWeakReferenceObject &system : m_system_updating_list) {
        SD_WREF(system).Update();
    }
}

void ECSManager::Terminate()
{
    SDLOG("ECS Manager terminating start.");
    for (std::list<EntityGroupStrongReferenceObject>::iterator group_iter = m_entity_group_list.begin();
        group_iter != m_entity_group_list.end();) {
        group_iter = m_entity_group_list.erase(group_iter);
    }

    for (std::list<EntityStrongReferenceObject>::iterator entity_iter = m_entity_list.begin();
        entity_iter != m_entity_list.end();) {
        entity_iter = m_entity_list.erase(entity_iter);
    }
    //
    std::map<std::type_index, ComponentPoolStrongReferenceObject>::iterator comp_pool_iter;
    for (comp_pool_iter = m_comp_pools.begin();
        comp_pool_iter != m_comp_pools.end();) {
        comp_pool_iter = m_comp_pools.erase(comp_pool_iter);
    }

    std::map<std::type_index, SystemStrongReferenceObject>::iterator system_iter;
    for (system_iter = m_system_map.begin(); system_iter != m_system_map.end();) {
        SD_SREF((*system_iter).second).Destroy();
        system_iter = m_system_map.erase(system_iter);
    }
    SDLOG("ECS Manager terminating end.");
}

void ECSManager::Resize()
{
    for (SystemWeakReferenceObject &system : m_system_updating_list) {
        SD_WREF(system).Resize();
    }
}

_______________SD_END_BASIC_NAMESPACE________________