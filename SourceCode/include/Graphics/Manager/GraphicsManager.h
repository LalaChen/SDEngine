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
#include "PeriodCounter.h"
#include "GraphicsPipeline.h"
#include "CommandBufferInheritanceInfo.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "RenderPass.h"
#include "ImageLoader.h"
#include "Resolution.h"
#include "EventArg.h"

using SDE::Basic::EventArg;
using SDE::Basic::PeriodCounter;

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
    virtual void CreateDescriptorPool(DescriptorPoolIdentity &io_identity) = 0;
    virtual void DestroyDescriptorPool(DescriptorPoolIdentity &io_identity) = 0;
    virtual void AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref, const GraphicsPipelineWeakReferenceObject &i_pipe_wref) = 0;
    virtual void WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uv_wrefs) = 0;
    virtual void BindDescriptorSet(const DescriptorSetIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref, const GraphicsPipelineWeakReferenceObject &i_pipe_wref) = 0;
    virtual void FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref) = 0;
public:
    virtual void CreateCommandPool(CommandPoolIdentity &io_identity) = 0;
    virtual void DestroyCommandPool(CommandPoolIdentity &io_identity) = 0;
    virtual void AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool_wref) = 0;
    virtual void BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info) = 0;
    virtual void EndCommandBuffer(const CommandBufferIdentity &i_identity) = 0;
    virtual void FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool_wref) = 0;
    virtual void SubmitCommandBuffersToQueue(const std::vector<CommandBufferWeakReferenceObject> &i_cb_wrefs) = 0;
    virtual void SubmitCommandBufferToQueue(const CommandBufferWeakReferenceObject &i_cb_wref) = 0;
    virtual void ExecuteCommandsToPrimaryCommandBuffer(const CommandBufferWeakReferenceObject &i_primary_cb_wref, const std::list<CommandBufferWeakReferenceObject> &i_secondary_cb_wrefs) = 0;
public:
//----------- Vertex Buffer Function ------------
    virtual void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size) = 0;
    virtual void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void DeleteVertexBuffer(VertexBufferIdentity &io_identity) = 0;
    virtual void MapVertexBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapVertexBuffer(const VertexBufferIdentity &i_identity) = 0;
public:
//----------- Index Buffer Interface Function ------------
    virtual void CreateIndexBuffer(IndexBufferIdentity &io_identity, Size_ui64 i_data_size) = 0;
    virtual void RefreshStaticIndexBuffer(const IndexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void RefreshDynamicIndexBuffer(const IndexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void DeleteIndexBuffer(IndexBufferIdentity &io_identity) = 0;
    virtual void MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapIndexBuffer(const IndexBufferIdentity &i_identity) = 0;
public:
//----------- Uniform Buffer Interface Function ------------
    virtual void CreateUniformBuffer(UniformBufferIdentity &io_identity) = 0;
    virtual void MapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub_wref, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub_wref) = 0;
    virtual void DeleteUnifromBuffer(UniformBufferIdentity &io_identity) = 0;
public:
    virtual void CreateTextureImage(TextureIdentity &io_tex_identity, SamplerIdentity &io_sampler_identity) = 0;
    virtual void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum &i_dst_layout = ImageLayout_MAX_DEFINE_VALUE) = 0;
    virtual void DeleteTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) = 0;
public:
    virtual void BindVertexBuffer(const VertexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_binding_id, Size_ui64 i_offset) = 0;
    virtual void BindIndexBuffer(const IndexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref, Size_ui64 i_offset) = 0;
public:
    virtual void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) = 0;
    virtual void DeleteShaderModule(ShaderModuleIdentity &io_identity) = 0;
public:
    virtual void CreateGraphicsPipeline(GraphicsPipelineIdentity &io_identity, const ShaderModules &i_shaders, const RenderPassWeakReferenceObject &i_rp_wref) = 0;
    virtual void BindGraphicsPipeline(const GraphicsPipelineIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb_wref) = 0;
    virtual void DestroyGraphicsPipeline(GraphicsPipelineIdentity &io_identity) = 0;
public:
    virtual void CreateRenderPass(RenderPassIdentity &io_identity) = 0;
    virtual void BeginRenderPass(const CommandBufferWeakReferenceObject &i_cb_wref, const FrameBufferWeakReferenceObject &i_fb_wref, const RenderPassWeakReferenceObject &i_rp_wref, const ImageOffset &i_start_pos, const ImageSize &i_render_size) = 0;
    virtual void GoToNextStepOfRenderPass(const CommandBufferWeakReferenceObject &i_cb_wref, const FrameBufferWeakReferenceObject &i_fb_wref, uint32_t i_sp_id) = 0;
    virtual void EndRenderPass(const CommandBufferWeakReferenceObject &i_cb_wref) = 0;
    virtual void DestroyRenderPass(RenderPassIdentity &io_identity) = 0;
public:
    virtual void CreateFrameBuffer(FrameBufferIdentity &io_identity, const RenderPassWeakReferenceObject &i_rp_wref, const std::vector<TextureWeakReferenceObject> &i_buf_wrefs) = 0;
    virtual void CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity)  = 0;
    virtual void DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity) = 0;
    virtual void DestroyFrameBuffer(FrameBufferIdentity &io_identity) = 0;
public:
    virtual void SetViewport(const CommandBufferWeakReferenceObject &i_cb_wref, const Viewport &i_vp) = 0;
    virtual void SetViewports(const CommandBufferWeakReferenceObject &i_cb_wref, const std::vector<Viewport> &i_vps) = 0;
    virtual void SetScissor(const CommandBufferWeakReferenceObject &i_cb_wref, const ScissorRegion &i_region) = 0;
    virtual void SetScissors(const CommandBufferWeakReferenceObject &i_cb_wref, const std::vector<ScissorRegion> &i_regions) = 0;
    virtual void DrawByIndices(const IndexBufferWeakReferenceObject &i_ib_wref, const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_first_id, int32_t i_offset, uint32_t i_first_ins_id, uint32_t i_ins_number) = 0;
public:
//------------- Resize Function -----------------
    virtual void Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h) = 0;
public:
//------------- Render Function -----------------
    void Render();
public:
    void InitializeDefaultPipelineInfos();
    void GetBasicVertexAttribInfos(std::vector<VertexAttribBindingDescription> &io_binds, std::vector<VertexAttribLocationDescription> &io_locations, uint32_t i_tex_dimension = 2) const;
//-------- Managing RenderPass Function ---------
    void InitializeDefaultRenderPasses();
    void RegisterRenderPass(const RenderPassStrongReferenceObject &i_rp_sref);
    void UnregisterRenderPass(const ObjectName &i_target_rp_name);
    void ReleaseRenderPasses();
    RenderPassWeakReferenceObject GetRenderPass(const ObjectName &i_target_rp_name) const;
public:
    TextureFormatEnum GetDefaultDepthBufferFormat() const;
    bool IsSupportedDepthBufferFormat(TextureFormatEnum i_fmt) const;
    TextureFormatEnum GetDefaultColorBufferFormat() const;
    bool IsSupportedColorBufferFormat(TextureFormatEnum i_fmt) const;
protected:
    const TextureIdentity& GetIdentity(const TextureWeakReferenceObject &i_tex_wref) const;
    const SamplerIdentity& GetIdentityFromTexture(const TextureWeakReferenceObject &i_tex_wref) const;
    const FrameBufferIdentity& GetIdentity(const FrameBufferWeakReferenceObject &i_fb_wref) const;
    const CommandBufferIdentity& GetIdentity(const CommandBufferWeakReferenceObject &i_cb_wref) const;
    const CommandPoolIdentity& GetIdentity(const CommandPoolWeakReferenceObject &i_cp_wref) const;
    const RenderPassIdentity& GetIdentity(const RenderPassWeakReferenceObject &i_rp_wref) const;
    const VertexBufferIdentity& GetIdentity(const VertexBufferWeakReferenceObject &i_vb_wref) const;
    const IndexBufferIdentity& GetIdentity(const IndexBufferWeakReferenceObject &i_ib_wref) const;
    const UniformBufferIdentity& GetIdentity(const UniformBufferWeakReferenceObject &i_ub_wref) const;
    const GraphicsPipelineIdentity& GetIdentity(const GraphicsPipelineWeakReferenceObject &i_pipe_wref) const;
    const DescriptorPoolIdentity& GetIdentity(const DescriptorPoolWeakReferenceObject &i_pool_wref) const;
    const DescriptorSetIdentity& GetIdentity(const DescriptorSetWeakReferenceObject &i_desc_wref) const;
protected:
//------------ Render Flow Function -------------
    virtual void RenderBegin() = 0;
    virtual void RenderToScreen() = 0;
    virtual void RenderEnd() = 0;
protected:
    SD_DECLARE_ATTRIBUTE_VAR_GET(Resolution, m_screen_size, ScreenResolution);
protected:
    std::list<RenderPassStrongReferenceObject> m_rp_list;
protected:
    std::vector<TextureFormatEnum> m_supported_depth_buffer_formats;
    std::vector<TextureFormatEnum> m_supported_color_buffer_formats;
protected:
    std::vector<VertexAttribBindingDescription> m_default_va_binding_descs;
    std::vector<VertexAttribLocationDescription> m_default_va_location_descs;
protected:
    PeriodCounter m_fps;
};

______________SD_END_GRAPHICS_NAMESPACE______________