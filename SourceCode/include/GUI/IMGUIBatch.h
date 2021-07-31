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

/*! \file      IMGUIBatch.h
 *  \brief     The class IMGUIBatch is the root of GUI.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/01
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "DynamicVertexBuffer.h"
#include "DynamicIndexBuffer.h"
#include "Mesh.h"
#include "Vec.h"
#include "Color4f.h"

#include "IMGUIWindow.h"
#include "Application.h"

#include "EventObject.h"

using SDE::Basic::TouchButtonStateEnum;
using SDE::Basic::TouchButton;
using SDE::Basic::Application;

using SDE::Basic::ObjectName;

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectWeakReferenceObject;
using SDE::Basic::EventObjectStrongReferenceObject;

using SDE::Graphics::vec2;
using SDE::Graphics::Color4B;
using SDE::Graphics::Color4f;

using SDE::Graphics::Mesh;
using SDE::Graphics::MeshWeakReferenceObject;
using SDE::Graphics::MeshStrongReferenceObject;

using SDE::Graphics::VertexBufferUsageEnum;

using SDE::Graphics::VertexBuffer;
using SDE::Graphics::VertexBufferWeakReferenceObject;
using SDE::Graphics::VertexBufferStrongReferenceObject;

using SDE::Graphics::IndexBuffer;
using SDE::Graphics::IndexBufferWeakReferenceObject;
using SDE::Graphics::IndexBufferStrongReferenceObject;


using SDE::Graphics::DynamicVertexBuffer;
using SDE::Graphics::DynamicVertexBufferWeakReferenceObject;
using SDE::Graphics::DynamicVertexBufferStrongReferenceObject;

using SDE::Graphics::DynamicIndexBuffer;
using SDE::Graphics::DynamicIndexBufferWeakReferenceObject;
using SDE::Graphics::DynamicIndexBufferStrongReferenceObject;

________________SD_START_GUI_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(IMGUIBatch);

typedef std::function<bool(const IMGUIBatchWeakReferenceObject&)> IMGUIBatchLoadingCallback;

class SDENGINE_CLASS IMGUIBatch : public EventObject
{
public:
    explicit IMGUIBatch(const ObjectName &i_name, const ImVec2 &i_buffer_size);
    virtual ~IMGUIBatch();
public:
    bool LoadGUI(const IMGUIBatchLoadingCallback &i_load_func);
    void AddWindow(const IMGUIWindowStrongReferenceObject &i_window);
    void Initialize();
public:
    void GetBuffers(
        DynamicVertexBufferWeakReferenceObject &io_GUI_vertices,
        DynamicVertexBufferWeakReferenceObject &io_GUI_texcoords,
        DynamicVertexBufferWeakReferenceObject &io_GUI_colors,
        DynamicIndexBufferWeakReferenceObject &io_GUI_indices);

    ImVec2 GetBufferSize() const;

    template<typename T> WeakReferenceObject<T> GetGUINode(const ObjectName &i_name);
public:
    void RecordBatchCommand(const TouchButton &i_touch);
protected:
    std::list<IMGUIWindowStrongReferenceObject> m_windows;
    ImVec2 m_buffer_size;
protected:
    MeshStrongReferenceObject m_GUI_meshes;
    DynamicVertexBufferWeakReferenceObject m_GUI_vertices;
    DynamicVertexBufferWeakReferenceObject m_GUI_texcoords;
    DynamicVertexBufferWeakReferenceObject m_GUI_colors;
    DynamicIndexBufferWeakReferenceObject m_GUI_indices;
};

inline ImVec2 IMGUIBatch::GetBufferSize() const
{
    return m_buffer_size;
}

template<typename T> WeakReferenceObject<T> IMGUIBatch::GetGUINode(const ObjectName &i_name)
{
    WeakReferenceObject<T> target;
    for (IMGUIWindowStrongReferenceObject &window : m_windows) {
        target = SD_WREF(window).GetGUINode<T>(i_name);
        if (target.IsNull() == false) {
            return target;
        }
    }
    return target;
}


_________________SD_END_GUI_NAMESPACE________________