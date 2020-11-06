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

/*! \file      EntityGroup.h
 *  \brief     Introduce of class EntityGroup
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/04
 *  \copyright MIT License.
 */

#pragma once

#include <map>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "Entity.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(EntityGroup);

class SDENGINE_CLASS EntityGroup : public EventObject
{
    friend class ECSManager;
public:
    explicit EntityGroup(const ObjectName &i_object_name, const std::vector<std::type_index> &i_conditions);
    virtual ~EntityGroup();
public:
    const std::list<EntityWeakReferenceObject>& GetEntities() const;
    bool AddEntity(const EntityWeakReferenceObject &i_entity_wref);
protected:
    bool RemoveEntity(const EntityWeakReferenceObject &i_entity_wref);
    bool IsCorresponded(const std::vector<std::type_index> &i_conditions) const;
protected:
    std::list<EntityWeakReferenceObject> m_entity_wrefs;
    std::vector<std::type_index> m_conditions;
};

inline const std::list<EntityWeakReferenceObject>& EntityGroup::GetEntities() const
{
    return m_entity_wrefs;
}

_______________SD_END_BASIC_NAMESPACE________________
