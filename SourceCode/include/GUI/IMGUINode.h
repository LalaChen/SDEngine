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

/*! \file      GUINode.h
 *  \brief     The class GUINode is the interface of all GUI component.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/01
 *  \copyright MIT License.
 */

#pragma once

#include <ImGui/imgui.h>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "EventObject.h"
#include "Vec.h"

using SDE::Basic::ObjectName;

using SDE::Basic::WeakReferenceObject;
using SDE::Basic::StrongReferenceObject;

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectWeakReferenceObject;
using SDE::Basic::EventObjectStrongReferenceObject;

using SDE::Graphics::vec2;
using SDE::Graphics::vec3;

________________SD_START_GUI_NAMESPACE_______________

class SDENGINE_CLASS IMGUIRect
{
public:
    IMGUIRect()
    : m_pos { 0, 0 }
    , m_size { 0, 0 }
    , m_tex_start { -1.0f, -1.0f }
    , m_tex_end{ -1.0f, -1.0f }
    {
    }

    ~IMGUIRect()
    {
    }

public:
    ImVec2 m_pos;
    ImVec2 m_size;
    ImVec2 m_tex_start;
    ImVec2 m_tex_end;
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(IMGUINode);

class SDENGINE_CLASS IMGUINode : public EventObject
{
public:
    explicit IMGUINode(const ObjectName &i_node_name);
    virtual ~IMGUINode();
public:
    void SetUIVertices(const IMGUIRect &i_rect);
public:
    virtual void RecordCommand() = 0;
    virtual void Append(const IMGUINodeStrongReferenceObject &i_child) = 0;
public:
    template<typename T> WeakReferenceObject<T> GetGUINode(const ObjectName &i_name);
protected:
    IMGUIRect m_region;
protected:
    std::list<IMGUINodeStrongReferenceObject> m_children;
};

template<typename T>
WeakReferenceObject<T> IMGUINode::GetGUINode(const ObjectName &i_name)
{
    if (m_object_name.compare(i_name) == 0) {
        return GetThisWeakPtrByType<T>();
    }
    else {
        WeakReferenceObject<T> target;
        for (IMGUINodeStrongReferenceObject &node : m_children) {
            target = SD_SREF(node).GetGUINode<T>(i_name);
            if (target.IsNull() == false) {
                return target;
            }
        }
        return target;
    }
}

_________________SD_END_GUI_NAMESPACE________________