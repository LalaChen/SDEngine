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

#include "Component.h"
#include "LogManager.h"
#include "ECSManager.h"

using SDE::Basic::ECSManager;

SD_SINGLETON_DECLARATION_IMPL(ECSManager);

______________SD_START_BASIC_NAMESPACE_______________

ECSManager::ECSManager()
{
    SD_SINGLETON_DECLARATION_REGISTER;
}

ECSManager::~ECSManager()
{
    for (std::list<EntityGroupStrongReferenceObject>::iterator group_iter = m_entity_group_srefs.begin();
        group_iter != m_entity_group_srefs.end();) {
        group_iter = m_entity_group_srefs.erase(group_iter);
    }

    for (std::list<EntityStrongReferenceObject>::iterator entity_iter = m_entity_srefs.begin();
        entity_iter != m_entity_srefs.end();) {
        entity_iter = m_entity_srefs.erase(entity_iter);
    }
    //
    std::map<std::type_index, ComponentPoolStrongReferenceObject>::iterator comp_pool_iter;
    for (comp_pool_iter = m_comp_pool_srefs.begin();
         comp_pool_iter != m_comp_pool_srefs.end();) {
        comp_pool_iter = m_comp_pool_srefs.erase(comp_pool_iter);
    }
}

EntityWeakReferenceObject ECSManager::CreateEntity(const ObjectName &i_object_name)
{
    EntityStrongReferenceObject entity_sref = new Entity(i_object_name);
    m_entity_srefs.push_back(entity_sref);
    return entity_sref.DynamicCastToPtr<Entity>();
}

bool ECSManager::DeleteEntity(const EntityWeakReferenceObject &i_entity_wref)
{
    //1. Find out entity.
    std::list<EntityStrongReferenceObject>::iterator entity_iter;
    for (entity_iter = m_entity_srefs.begin(); entity_iter != m_entity_srefs.end(); ) {
        if ((*entity_iter) == i_entity_wref) {
            //2. Remove its weak reference at all groups.
            for (EntityGroupStrongReferenceObject &group_sref : m_entity_group_srefs) {
                group_sref.GetRef().RemoveEntity(i_entity_wref);
            }
            //3. Remove it form entity list.
            entity_iter = m_entity_srefs.erase(entity_iter);
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
    //1. Create entity groups.
    EntityGroupStrongReferenceObject group_sref = new EntityGroup(i_group_name, i_conditions);
    m_entity_group_srefs.push_back(group_sref);
    //2. Collect all entities belog this group.
    for (EntityStrongReferenceObject &entity_sref : m_entity_srefs) {
        group_sref.GetRef().AddEntity(entity_sref);
    }
    return group_sref;
}

bool ECSManager::RemoveComponentFromEntity(const EntityWeakReferenceObject &i_entity_wref, const std::type_index &i_type_index)
{
    bool result = false;
    if (i_entity_wref.IsNull() == false) {
        //1. Unregister component at entity.
        ComponentBaseWeakReferenceObject rem_comp_wref = i_entity_wref.GetRef().UnregisterComponent(i_type_index);
        //2. Delete component at entity.
        result = DeleteComponent(i_type_index, rem_comp_wref);
        //3. To do : Update all entity groups for this entity.
    }
    else {
        SDLOGE("Remove component with type(%s) at null entity.", i_type_index.name());
    }
    return false;
}

void ECSManager::LinkComponentAndEntity(const EntityWeakReferenceObject &i_entity_wref, const ComponentBaseWeakReferenceObject &i_comp_wref, const std::type_index &i_type)
{
    i_entity_wref.GetRef().RegisterComponent(i_type, i_comp_wref);
    i_comp_wref.DynamicCastTo<Component>().GetRef().SetEntity(i_entity_wref);
}

bool ECSManager::DeleteComponent(const std::type_index &i_type_index, const ComponentBaseWeakReferenceObject &i_comp_wref)
{
    std::map<std::type_index, ComponentPoolStrongReferenceObject>::iterator comp_pool_iter = m_comp_pool_srefs.find(i_type_index);
    if (comp_pool_iter != m_comp_pool_srefs.end()) {
        return (*comp_pool_iter).second.GetRef().DeleteComponent(i_comp_wref);
    }
    else {
        SDLOGW("Can't find component pool with [%s].", i_type_index.name());
        return false;
    }
}

bool ECSManager::UnregisterSystem(const std::type_index &i_type_index)
{
    std::map<std::type_index, SystemStrongReferenceObject>::iterator sys_iter = m_system_srefs.find(i_type_index);
    if (sys_iter != m_system_srefs.end()) {
        m_system_srefs.erase(sys_iter);
        return true;
    }
    else {
        SDLOGE("Unregister non-exist system.");
        return false;
    }
}

void ECSManager::NotifyEntityChanged(const EntityWeakReferenceObject &i_entity_wref)
{
    for (EntityGroupStrongReferenceObject &eg_sref : m_entity_group_srefs) {
        eg_sref.GetRef().AddEntity(i_entity_wref);
    }
}

void ECSManager::Initialize()
{
}

void ECSManager::Update()
{
    std::list<SystemStrongReferenceObject>::iterator sys_iter;
    for (std::map<std::type_index, SystemStrongReferenceObject>::iterator sys_iter = m_system_srefs.begin();
        sys_iter != m_system_srefs.end();
        ++sys_iter) {
        (*sys_iter).second.GetRef().Update();
    }
}

void ECSManager::Terminate()
{
}

_______________SD_END_BASIC_NAMESPACE________________