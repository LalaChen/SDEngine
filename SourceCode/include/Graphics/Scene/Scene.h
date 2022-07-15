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

/*! \file      SceneManager.h
 *  \brief     Introduce of class about Scene.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/06/20
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "Entity.h"
#include "Object.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;
using SDE::Basic::FilePathString;
using SDE::Basic::Entity;
using SDE::Basic::EntityStrongReferenceObject;
using SDE::Basic::EntityWeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Scene);

class SDENGINE_CLASS Scene : public Object
{
public:
    explicit Scene(const ObjectName &i_object_name, const FilePathString &i_fp);
    virtual ~Scene();
public:
    virtual bool Load();
    virtual bool Unload();
public:
    EntityWeakReferenceObject GetRoot() const;
protected:
    std::list<EntityWeakReferenceObject> m_entities;
    FilePathString m_scene_fp;
    bool m_is_loading;
};

______________SD_END_GRAPHICS_NAMESPACE______________