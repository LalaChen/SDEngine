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

/*! \file      WorldGUIComponent.h
 *  \brief     The class WorldGUIComponent is used to put GUI in world.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/03
 *  \copyright MIT License.
 */

#pragma once

#include "IMGUIBatch.h"
#include "IMGUIRenderer.h"
#include "MeshRenderComponent.h"

using SDE::Basic::Component;
using SDE::Basic::ComponentStrongReferenceObject;
using SDE::Basic::ComponentWeakReferenceObject;

using SDE::GUI::IMGUIBatch;
using SDE::GUI::IMGUIBatchStrongReferenceObject;
using SDE::GUI::IMGUIBatchWeakReferenceObject;
using SDE::GUI::IMGUIBatchLoadingCallback;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(WorldGUIComponent);

class SDENGINE_CLASS WorldGUIComponent : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(WorldGUIComponent, WorldGUIComponent);
public:
    explicit WorldGUIComponent(const ObjectName &i_object_name, uint32_t i_width = 200, uint32_t i_height = 200, float i_world_w = 0.5f, float i_world_h = 0.5f);
    virtual ~WorldGUIComponent();
public:
    void SetBufferSize(uint32_t i_width, uint32_t i_height);
    void SetWorldSize(float i_world_w, float i_world_h);
    CommandBufferWeakReferenceObject GetCommandBuffer() const;
    bool LoadGUI(const IMGUIBatchLoadingCallback &i_load_func);
public:
    template<typename T> WeakReferenceObject<T> GetGUINode(const ObjectName &i_name);
public:
    void Initialize() override;
public:
    void Update();
protected:
    void InitializeWorkspace();
    void ClearWorkspace();
protected:
    RenderFlowStrongReferenceObject m_GUI_render_flow;
    TextureStrongReferenceObject m_GUI_color_buffer;
    TextureStrongReferenceObject m_GUI_depth_buffer;
    CommandPoolStrongReferenceObject m_GUI_cp;
    CommandBufferWeakReferenceObject m_GUI_cb;
protected:
    IMGUIBatchStrongReferenceObject m_batch;
    MeshRenderComponentWeakReferenceObject m_GUI_mesh_render;
    MaterialStrongReferenceObject m_GUI_material;
    MeshStrongReferenceObject m_GUI_mesh;
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
    uint32_t m_buffer_size[2];
    float m_world_size[2];
    bool m_initialized;
};

inline CommandBufferWeakReferenceObject WorldGUIComponent::GetCommandBuffer() const
{
    return m_GUI_cb;
}

template<typename T> inline WeakReferenceObject<T> WorldGUIComponent::GetGUINode(const ObjectName &i_name)
{
    return SD_SREF(m_batch).GetGUINode<T>(i_name);
}


______________SD_END_GRAPHICS_NAMESPACE______________