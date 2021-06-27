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

#include "LogManager.h"
#include "ECSManager.h"
#include "Component.h"
#include "Entity.h"

______________SD_START_BASIC_NAMESPACE_______________

Entity::Entity(const ObjectName &i_object_name)
: EventObject(i_object_name)
{
}

Entity::~Entity()
{
    std::map<std::type_index, ComponentBaseWeakReferenceObject>::iterator comp_iter;
    for (comp_iter = m_comps.begin(); comp_iter != m_comps.end();) {
        comp_iter = m_comps.erase(comp_iter);
    }
}

bool Entity::RegisterComponent(const std::type_index &i_type, const ComponentBaseWeakReferenceObject &i_comp)
{
    if (i_comp.IsNull() == false) {
        std::map<std::type_index, ComponentBaseWeakReferenceObject>::iterator comp_iter = m_comps.find(i_type);
        if (comp_iter == m_comps.end()) {
            m_comps[i_type] = i_comp;
            return true;
        }
        else {
            SDLOGE("Component(type is %s) is existed in entity[%s]!!!", i_type.name(), m_object_name.c_str());
            return false;
        }
    }
    else {
        SDLOGE("Add null component(type is %s) to entity[%s]!!!", i_type.name(), m_object_name.c_str());
        return false;
    }
}

ComponentBaseWeakReferenceObject Entity::UnregisterComponent(const std::type_index &i_target_type)
{
    ComponentBaseWeakReferenceObject del_comp;
    std::map<std::type_index, ComponentBaseWeakReferenceObject>::iterator comp_iter = m_comps.find(i_target_type);
    if (comp_iter != m_comps.end()) {
        del_comp = (*comp_iter).second;
        comp_iter = m_comps.erase(comp_iter);
    }
    else {
        SDLOGW("we can't find any component with type[%s]", i_target_type.name());
    }
    return del_comp;
}

ComponentBaseWeakReferenceObject Entity::GetComponent(const std::type_index &i_target_type) const
{
    std::map<std::type_index, ComponentBaseWeakReferenceObject>::const_iterator comp_iter = m_comps.find(i_target_type);
    if (comp_iter != m_comps.end()) {
        return (*comp_iter).second;
    }
    else {
        return ComponentBaseWeakReferenceObject();
    }
}

bool Entity::IsMatch(const std::vector<std::type_index> &i_condition) const
{
    std::map<std::type_index, ComponentBaseWeakReferenceObject>::const_iterator comp_iter;
    for (const std::type_index &type : i_condition) {
        comp_iter = m_comps.find(type);
        if (comp_iter != m_comps.end()) {
            return true;
        }
    }
    return false;
}

bool Entity::IsComponentExisted(const std::type_index &i_target_type) const
{
    return (m_comps.find(i_target_type) != m_comps.end());
}

std::string Entity::ToString() const
{
    std::string result;
    result += "Entity Name : " + m_object_name + "\n";
    for (std::map<std::type_index, ComponentBaseWeakReferenceObject>::const_iterator comp_iter = m_comps.begin();
        comp_iter != m_comps.end();
        ++comp_iter)
    {
        result += "\tComp : " + m_object_name + " with type : " + (*comp_iter).first.name() + "\n";
    }
    return result;
}

_______________SD_END_BASIC_NAMESPACE________________