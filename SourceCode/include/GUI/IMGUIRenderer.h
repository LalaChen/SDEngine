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

/*! \file      IMGUIRenderer.h
 *  \brief     The class IMGUIRenderer is used to prepare GUI command buffer for rendering GUI.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/01
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "ShaderProgram.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DynamicVertexBuffer.h"
#include "DynamicIndexBuffer.h"
#include "Texture.h"
#include "RenderFlow.h"
#include "UniformBuffer.h"
#include "CommandBuffer.h"

#include "IMGUIBatch.h"

using SDE::Graphics::DynamicVertexBuffer;
using SDE::Graphics::DynamicVertexBufferStrongReferenceObject;
using SDE::Graphics::DynamicVertexBufferWeakReferenceObject;

using SDE::Graphics::DynamicIndexBuffer;
using SDE::Graphics::DynamicIndexBufferStrongReferenceObject;
using SDE::Graphics::DynamicIndexBufferWeakReferenceObject;

using SDE::Graphics::Texture;
using SDE::Graphics::TextureStrongReferenceObject;
using SDE::Graphics::TextureWeakReferenceObject;

using SDE::Graphics::RenderFlow;
using SDE::Graphics::RenderFlowStrongReferenceObject;
using SDE::Graphics::RenderFlowWeakReferenceObject;

using SDE::Graphics::CommandBuffer;
using SDE::Graphics::CommandBufferStrongReferenceObject;
using SDE::Graphics::CommandBufferWeakReferenceObject;

using SDE::Graphics::ShaderProgram;
using SDE::Graphics::ShaderProgramStrongReferenceObject;
using SDE::Graphics::ShaderProgramWeakReferenceObject;

using SDE::Graphics::DescriptorPool;
using SDE::Graphics::DescriptorPoolStrongReferenceObject;
using SDE::Graphics::DescriptorPoolWeakReferenceObject;

using SDE::Graphics::DescriptorSet;
using SDE::Graphics::DescriptorSetStrongReferenceObject;
using SDE::Graphics::DescriptorSetWeakReferenceObject;

using SDE::Graphics::UniformBuffer;
using SDE::Graphics::UniformBufferStrongReferenceObject;
using SDE::Graphics::UniformBufferWeakReferenceObject;

using SDE::Graphics::UniformImages;
using SDE::Graphics::UniformImagesStrongReferenceObject;
using SDE::Graphics::UniformImagesWeakReferenceObject;

________________SD_START_GUI_NAMESPACE_______________

class SDENGINE_CLASS IMGUIRenderer
{
public:
    SD_SINGLETON_DECLARATION(IMGUIRenderer);
public:
    IMGUIRenderer();
    ~IMGUIRenderer();
public:
    void InitializeGraphicsSystem();
    void ReleaseGraphicsSystem();
public:
    void RecordGUICommands(
        const RenderFlowWeakReferenceObject &i_flow,
        const CommandBufferWeakReferenceObject &i_cmd_buffer,
        const IMGUIBatchWeakReferenceObject &i_gui_batch);
protected:
    bool PrepareVerticesData(
        DynamicVertexBufferWeakReferenceObject &io_vertices,
        DynamicVertexBufferWeakReferenceObject &io_texcoords,
        DynamicVertexBufferWeakReferenceObject &io_colors,
        DynamicIndexBufferWeakReferenceObject &io_indices);
protected:
    TextureStrongReferenceObject m_font_atlas;
protected:
    ShaderProgramWeakReferenceObject m_GUI_shader;
    DescriptorPoolStrongReferenceObject m_dp;
    DescriptorSetWeakReferenceObject m_ds;
    UniformBufferWeakReferenceObject m_gui_ub;
    UniformImagesWeakReferenceObject m_atlas_ui;
};

_________________SD_END_GUI_NAMESPACE________________
