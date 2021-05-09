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

/*! \file      Component.h
 *  \brief     Introduce of class Component
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/02
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "ComponentBase.h"
#include "Entity.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Component);

class SDENGINE_CLASS Component : public ComponentBase
{
public:
    friend class Entity;
    friend class ECSManager;
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(Component, Component);
public:
    explicit Component(const ObjectName &i_object_name);
    virtual ~Component();
protected:
    void SetEntity(const EntityWeakReferenceObject &i_entity);
    EntityWeakReferenceObject GetEntity() const;
protected:
    EntityWeakReferenceObject m_entity;
};

_______________SD_END_BASIC_NAMESPACE________________