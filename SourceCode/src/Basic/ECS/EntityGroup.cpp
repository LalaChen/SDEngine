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
#include "EntitiyGroup.h"

______________SD_START_BASIC_NAMESPACE_______________

EntityGroup::EntityGroup(const ObjectName &i_object_name, const std::vector<std::type_index> &i_conditions)
: EventObject(i_object_name)
, m_conditions(i_conditions)
{
    EventStrongReferenceObject changed_event = new Event("ChangedEvent");
    RegisterEvent(changed_event);
}

EntityGroup::~EntityGroup()
{
}

bool EntityGroup::AddEntity(const EntityWeakReferenceObject &i_entity_wref)
{
    if (i_entity_wref.IsNull() == false) {
        if (i_entity_wref.GetRef().IsMatch(m_conditions) == true) {
            if (std::find(m_entity_wrefs.begin(), m_entity_wrefs.end(), i_entity_wref) == m_entity_wrefs.end()) {
                m_entity_wrefs.push_back(i_entity_wref);
                NotifyEvent("ChangedEvent", EventArg());
                return true;
            }
            else {
                SDLOGD("Add E(%s) entity to G[%s] repeatly.", i_entity_wref.GetRef().GetObjectName().c_str(), m_object_name.c_str());
                return false;
            }
        }
        else {
            SDLOGD("Add E(%s) entity to G[%s] failure", i_entity_wref.GetRef().GetObjectName().c_str(), m_object_name.c_str());
            return false;
        }
    }
    else {
        SDLOGE("Add nullptr entity to group[%s]", m_object_name.c_str());
        return false;
    }
}

bool EntityGroup::RemoveEntity(const EntityWeakReferenceObject &i_entity_wref)
{
    std::list<EntityWeakReferenceObject>::iterator iter;
    for (iter = m_entity_wrefs.begin(); iter != m_entity_wrefs.end(); ) {
        if ((*iter) == i_entity_wref) {
            iter = m_entity_wrefs.erase(iter);
            return true;
        }
        else {
            iter++;
        }
    }
    return false;
}

bool EntityGroup::IsCorresponded(const std::vector<std::type_index> &i_conditions) const
{
    if (i_conditions.size() != m_conditions.size()) {
        return false;
    }
    else {
        for (uint32_t src_id = 0; src_id < i_conditions.size(); ++src_id) {
            bool is_type_exist = false;
            for (uint32_t id = 0; id < m_conditions.size(); ++id) {
                if (m_conditions[id] == i_conditions[src_id]) {
                    is_type_exist = true;
                    break;
                }
            }
            if (is_type_exist == false) {
                return false;
            }
        }
        return true;
    }
}

_______________SD_END_BASIC_NAMESPACE________________