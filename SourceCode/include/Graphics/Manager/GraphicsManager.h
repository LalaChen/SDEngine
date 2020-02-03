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

/*! \file      GraphicsManager.h
 *  \brief     Introduce of class GraphicsManager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */

#pragma once

#include <list>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "ManagerParam.h"
#include "ManagerIdentity.h"
#include "RenderPass.h"
#include "ImageLoader.h"
#include "Resolution.h"
#include "EventArg.h"

using SDE::Basic::EventArg;

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum GraphicsLibraryEnum {
    GraphicsLibrary_OpenGL4 = 0,
    GraphicsLibrary_Vulkan  = 1,
    GraphicsLibrary_MaxEnum,
    GraphicsLibrary_Unknown = GraphicsLibrary_MaxEnum
};

/*! \class GraphicsManager
 *  \brief In our system, GraphicsManager is a interface for all graphics API(opengl, gles, vulkan) and manage \n
 *  graphics resource those need to be managed.
 */
class SDENGINE_CLASS GraphicsManager
{
public:
    SD_SINGLETON_DECLARATION(GraphicsManager);
public:
    /*! \fn GraphicsManager();
     *  \brief The constructor of GraphicsManager Class. 
     */
    GraphicsManager();
    
    /*! \fn virtual ~GraphicsManager();
     *  \brief The destructor of GraphicsManager Class.
     */
    virtual ~GraphicsManager();
public:
//------------ Initialize and Release -------------
    /*! \fn virtual void InitializeGraphicsSystem(const EventArg &i_arg) = 0;
     *  \param [in] i_arg argumnet for initializing manager. We should define necessary arguments class(derived EventArg) and input those arguments.
     *  \brief Initialize graphics system. (link dll, ...)
     */
    virtual void InitializeGraphicsSystem(const EventArg &i_arg) = 0;
    
    /*! \fn virtual void ReleaseGraphicsSystem() = 0;
     *  \brief Release graphics system.
     */
    virtual void ReleaseGraphicsSystem() = 0;

    /*! \fn virtual void Initialize();
     *  \brief Initialize common necessary components.
     */
    virtual void Initialize();

    /*! \fn void Release();
     *  \brief Release common necessary components.
     */
    virtual void Release();
public:
    /*! \fn virtual void PrintSystemInformation() = 0;
     *  \brief Print system information.
     */
    virtual void PrintSystemInformation() = 0;
public:
//----------- Vertex Buffer Function ------------
    virtual void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type) = 0;
    virtual void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void DeleteVertexBuffer(VertexBufferIdentity &io_identity) = 0;
    virtual void MapBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapBuffer(const VertexBufferIdentity &i_identity) = 0;
public:
    virtual void CreateTextureImage(TextureIdentity &io_tex_identity, SamplerIdentity &io_sampler_identity) = 0;
    virtual void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum &i_dst_layout = ImageLayout_MAX_DEFINE_VALUE) = 0;
    virtual void DeleteTextureImage(TextureIdentity &io_identity) = 0;
public:
    virtual void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) = 0;
    virtual void DeleteShaderModule(ShaderModuleIdentity &io_identity) = 0;
public:
    virtual void CreateRenderPass(RenderPassIdentity &io_identity) = 0;
    virtual void BeginRenderPass(const CompHandle i_cmd_buffer_handle, const FrameBufferIdentity &i_fb_identity, const ImageOffset &i_start_pos, const ImageSize &i_render_size, const std::vector<ClearValue> &i_clear_values) = 0;
    virtual void GoToNextStepOfRenderPass(const CompHandle i_cmd_buffer_handle, const FrameBufferGroupIdentity &i_target_fbg_identity) = 0;
    virtual void EndRenderPass(const CompHandle i_cmd_buffer_handle) = 0;
    virtual void DestroyRenderPass(RenderPassIdentity &io_identity) = 0;
public:
    virtual void CreateFrameBuffer(FrameBufferIdentity &io_identity, const std::vector<TextureWeakReferenceObject> &i_buf_wrefs) = 0;
    virtual void CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity)  = 0;
    virtual void DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity) = 0;
    virtual void DestroyFrameBuffer(FrameBufferIdentity &io_identity) = 0;
public:
//------------- Resize Function -----------------
    virtual void Resize(Size_ui32 i_w, Size_ui32 i_h) = 0;
public:
//------------- Render Function -----------------
    void Render();
public:
//-------- Managing RenderPass Function ---------
    void InitializeDefaultRenderPasses();
    void RegisterRenderPass(const RenderPassStrongReferenceObject &i_rp_sref);
    void UnregisterRenderPass(const ObjectName &i_target_rp_name);
    void ReleaseRenderPasses();
    RenderPassWeakReferenceObject GetRenderPass(const ObjectName &i_target_rp_name) const;
protected:
//------------ Render Flow Function -------------
    virtual void RenderBegin() = 0;
    virtual void RenderToScreen() = 0;
    virtual void RenderEnd() = 0;
protected:
    SD_DECLARE_ATTRIBUTE_VAR_GET(Resolution, m_screen_size, ScreenResolution);
protected:
    std::list<RenderPassStrongReferenceObject> m_rp_list;
};

______________SD_END_GRAPHICS_NAMESPACE______________