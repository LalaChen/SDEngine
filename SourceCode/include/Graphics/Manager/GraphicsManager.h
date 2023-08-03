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
#include "GraphicsConfig.h"
#include "PeriodCounter.h"
#include "GraphicsIdentityGetter.h"
#include "ImageBlitParam.h"
#include "ImageLoader.h"
#include "Resolution.h"
#include "EventArg.h"
#include "CommandBufferInheritanceInfo.h"

using SDE::Basic::EventArg;
using SDE::Basic::PeriodCounter;

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum GraphicsLibraryEnum {
    GraphicsLibrary_OpenGL4 = 0,
    GraphicsLibrary_Vulkan  = 1,
    GraphicsLibrary_MaxEnum,
    GraphicsLibrary_Unknown = GraphicsLibrary_MaxEnum
};

enum VertexLocationKindEnum {
    VertexLocationKind_GENERAL = 0,
    VertexLocationKind_LOOKUP_TABLE,
    VertexLocationKind_VOL_DATA,
    VertexLocationKind_GUI,
    VertexLocationKind_MAX_DEFINE_VALUE
};

static const std::string sRenderPass_Forward = "ForwardPass";
static const std::string sRenderPass_VRForward = "VRForwardPass";
static const std::string sRenderPass_GUI = "GUIPass";
static const std::string sRenderPass_Compositing = "CompositingPass";

static const std::string sUniformDescriptorSetLayout_Camera = "Camera";
static const std::string sUniformBuffer_Camera = "camera";
static const std::string sUniformDescriptorSetLayout_VRCamera = "VRCamera";
static const std::string sUniformBuffer_VRCamera = "vrcamera";
static const std::string sUniformDescriptorSetLayout_MeshRender = "MeshRender";
static const std::string sUniformBuffer_MeshRender_Geometry = "geometry";
static const std::string sUniformDescriptorSetLayout_Light = "Light";
static const std::string sUniformBuffer_Light = "light";
static const std::string sUniformImages_Light_ShadowMaps = "shadowMaps";
static const std::string sUniformDescriptorSetLayout_Material = "Material";
static const std::string sUniformBuffer_Material = "material";
static const std::string sUniformImages_Material_Textures = "textures";

static const std::string sUniformDescriptorSetLayout_Compositing = "Compositing";
static const std::string sUniformBuffer_Compositing_UVData = "uvData";
static const std::string sUniformImage_Compositing_Texture = "mainTexture";

static const std::string sUniformDescriptorSetLayout_GUI = "GUI";
static const std::string sUniformBuffer_GUI_Offset = "offsets";
static const std::string sUniformImages_GUI_Font = "font";

/*! \class GraphicsManager
 *  \brief In our system, GraphicsManager is a interface for all graphics API(opengl, gles, vulkan) and manage
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

    /*! \fn virtual void InitializeBasicResource();
     *  \brief Initialize common necessary components.
     */
    virtual void InitializeBasicResource();

    /*! \fn void ReleaseBasicResource();
     *  \brief Release common necessary components.
     */
    virtual void ReleaseBasicResource();
public:
    /*! \fn virtual void PrintSystemInformation() = 0;
     *  \brief Print system information.
     */
    virtual void PrintSystemInformation() = 0;
public:
    virtual void GetDesiredVulkanValidLayers(std::vector<const char*>& io_valid_layers) const = 0;
public:
    virtual void CreateDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity,  const std::vector<UniformVariableDescriptorWeakReferenceObject> &i_uvds) = 0;
    virtual void DestroyDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity) = 0;
    virtual void CreateDescriptorPool(DescriptorPoolIdentity &io_identity) = 0;
    virtual void DestroyDescriptorPool(DescriptorPoolIdentity &io_identity) = 0;
    virtual void AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool, const DescriptorSetLayoutWeakReferenceObject &i_layout) = 0;
    virtual void WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uvs) = 0;
    virtual void FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool) = 0;
public:
    virtual void CreateCommandPool(CommandPoolIdentity &io_identity) = 0;
    virtual void DestroyCommandPool(CommandPoolIdentity &io_identity) = 0;
    virtual void AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool) = 0;
    virtual void BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info) = 0;
    virtual void EndCommandBuffer(const CommandBufferIdentity &i_identity) = 0;
    virtual void FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool) = 0;
    virtual void ExecuteCommandsToPrimaryCommandBuffer(const CommandBufferWeakReferenceObject &i_primary_cb, const std::list<CommandBufferWeakReferenceObject> &i_secondary_cbs) = 0;
public:
//----------- Vertex Buffer Function ------------
    virtual void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size) = 0;
    virtual void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void DeleteVertexBuffer(VertexBufferIdentity &io_identity) = 0;
    virtual void MapVertexBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapVertexBuffer(const VertexBufferIdentity &i_identity) = 0;
public:
//----------- Index Buffer Interface Function ------------
    virtual void CreateIndexBuffer(IndexBufferIdentity &io_identity, Size_ui64 i_data_size) = 0;
    virtual void RefreshStaticIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void RefreshDynamicIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void DeleteIndexBuffer(IndexBufferIdentity &io_identity) = 0;
    virtual void MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapIndexBuffer(const IndexBufferIdentity &i_identity) = 0;
public:
//----------- Uniform Buffer Interface Function ------------
    virtual void CreateUniformBuffer(UniformBufferIdentity &io_identity) = 0;
    virtual void MapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub, VoidPtr &io_buffer_handle) = 0;
    virtual void UnmapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub) = 0;
    virtual void DeleteUnifromBuffer(UniformBufferIdentity &io_identity) = 0;
public:
    virtual void CreateTextureImage(TextureIdentity &io_tex_identity, SamplerIdentity &io_sampler_identity) = 0;
    virtual void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum &i_dst_layout = ImageLayout_MAX_DEFINE_VALUE) = 0;
    virtual void DeleteTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) = 0;
public:
    virtual void BindVertexBuffer(const VertexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, uint32_t i_binding_id, Size_ui64 i_offset) = 0;
    virtual void BindIndexBuffer(const IndexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset) = 0;
public:
    virtual void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) = 0;
    virtual void DeleteShaderModule(ShaderModuleIdentity &io_identity) = 0;
public:
    virtual void CreateGraphicsPipeline(GraphicsPipelineIdentity &io_identity, const ShaderModules &i_shaders, const RenderPassWeakReferenceObject &i_rp, const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_dsls) = 0;
    virtual void BindGraphicsPipeline(const GraphicsPipelineIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, const std::vector<DescriptorSetWeakReferenceObject> &i_dss) = 0;
    virtual void DestroyGraphicsPipeline(GraphicsPipelineIdentity &io_identity) = 0;
public:
    virtual void CreateRenderPass(RenderPassIdentity &io_identity) = 0;
    virtual void BeginRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, const RenderPassWeakReferenceObject &i_rp, const ImageOffset &i_start_pos, const ImageSize &i_render_size) = 0;
    virtual void GoToNextStepOfRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, uint32_t i_sp_id) = 0;
    virtual void EndRenderPass(const CommandBufferWeakReferenceObject &i_cb) = 0;
    virtual void DestroyRenderPass(RenderPassIdentity &io_identity) = 0;
public:
    virtual void CreateFrameBuffer(FrameBufferIdentity &io_identity, const RenderPassWeakReferenceObject &i_rp, const std::vector<TextureWeakReferenceObject> &i_bufs) = 0;
    virtual void CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity)  = 0;
    virtual void DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity) = 0;
    virtual void DestroyFrameBuffer(FrameBufferIdentity &io_identity) = 0;
public:
    virtual void SetViewport(const CommandBufferWeakReferenceObject &i_cb, const Viewport &i_vp) = 0;
    virtual void SetViewports(const CommandBufferWeakReferenceObject &i_cb, const std::vector<Viewport> &i_vps) = 0;
    virtual void SetScissor(const CommandBufferWeakReferenceObject &i_cb, const ScissorRegion &i_region) = 0;
    virtual void SetScissors(const CommandBufferWeakReferenceObject &i_cb, const std::vector<ScissorRegion> &i_regions) = 0;
    virtual void DrawByIndices(const CommandBufferWeakReferenceObject &i_cb, const IndexBufferWeakReferenceObject &i_ib, uint32_t i_first_id, int32_t i_offset, uint32_t i_first_ins_id, uint32_t i_ins_number) = 0;
public:
    virtual void CreateQueue(GraphicsQueueIdentity &io_identity) = 0;
    virtual void SubmitCommandBuffersToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsFenceWeakReferenceObject &i_fence, const std::vector<CommandBufferWeakReferenceObject> &i_cbs) = 0;
    virtual void PresentToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsSwapchainIdentity &i_sw_identity, uint32_t i_img_id, const std::vector<GraphicsSemaphoreWeakReferenceObject> &i_semas) = 0;
    virtual void DestroyQueue(GraphicsQueueIdentity &io_identity) = 0;
public:
    virtual void CreateFence(GraphicsFenceIdentity &io_identity) = 0;
    virtual void ResetFence(const GraphicsFenceIdentity &i_identity) = 0;
    virtual void DestroyFence(GraphicsFenceIdentity &io_identity) = 0;
public:
    virtual void CreateSemaphoreObject(GraphicsSemaphoreIdentity &io_identity) = 0;
    virtual void DestroySemaphoreObject(GraphicsSemaphoreIdentity &io_identity) = 0;
public:
    virtual void CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) = 0;
    virtual void DestroyGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) = 0;
    virtual void GetReadyTextureOfSwapchain(const GraphicsSwapchainIdentity &i_identity, const GraphicsSemaphoreWeakReferenceObject &i_acq_sema, uint32_t &io_idx) = 0;
    virtual void RenderTextureToSwapchain(const GraphicsSwapchainIdentity &i_identity, uint32_t i_idx, const GraphicsQueueWeakReferenceObject &i_queue, const CommandBufferWeakReferenceObject &i_cmd_buffer, const GraphicsSemaphoreWeakReferenceObject &i_present_sema, const TextureWeakReferenceObject &i_texture, const ImageBlitParam &i_param) = 0;
public:
//------------- Resize Function -----------------
    virtual void Resize(CompHandle i_new_surface, Size_ui32 i_w, Size_ui32 i_h) = 0;
public:
//------------- Render Function -----------------
    virtual void RenderTextureToScreen(const TextureWeakReferenceObject &i_tex) = 0;
public:
    virtual Resolution GetScreenResolution() const = 0;
public:
    void GetBasicVertexAttribInfos(
        std::vector<VertexAttribBindingDescription> &io_binds,
        std::vector<VertexAttribLocationDescription> &io_locations,
        VertexLocationKindEnum i_vl_kind = VertexLocationKind_GENERAL) const;
public:
    float GetFPS() const;

    GraphicsSwapchainWeakReferenceObject GetSwapchain() const;
public:
//-------- Managing RenderPass Function ---------
    void RegisterRenderPass(const RenderPassStrongReferenceObject &i_rp);
    void UnregisterRenderPass(const ObjectName &i_target_rp_name);
    void ReleaseRenderPasses();
    RenderPassWeakReferenceObject GetRenderPass(const ObjectName &i_target_rp_name) const;
public:
    TextureFormatEnum GetDefaultDepthBufferFormat() const;
    bool IsSupportedDepthBufferFormat(TextureFormatEnum i_fmt) const;
    TextureFormatEnum GetDefaultColorBufferFormat() const;
    bool IsSupportedColorBufferFormat(TextureFormatEnum i_fmt) const;
public:
    virtual DescriptorSetLayoutWeakReferenceObject GetBasicDescriptorSetLayout(const ObjectName &i_dsl_name) const;
    virtual UniformVariableDescriptorStrongReferenceObject GetDefaultMaterialUniformVariableDescriptor(const ObjectName &i_uvd_name) const;
    virtual ShaderProgramWeakReferenceObject GetShaderProgram(const ObjectName &i_sp_name) const;
    virtual void RegisterShaderProgram(const ShaderProgramStrongReferenceObject &i_sp);
    virtual void RegisterShaderProgram(const ObjectName &i_sp_name, const FilePathString &i_path);
public:
    void SubmitGraphicsCommands(const std::vector<CommandBufferWeakReferenceObject> &i_cbs);
protected:
    void InitializeDefaultPipelineInfos();
    void InitializeBasicDescriptorSetLayout();
    void InitializeBasicMaterialUniformDescriptors();
    void InitializeBasicShaderPrograms();
    void InitializeDefaultRenderPasses();
protected:
    GraphicsConfig m_graphics_config;
protected:
    std::vector<TextureFormatEnum> m_supported_color_buffer_formats;
    std::vector<TextureFormatEnum> m_supported_depth_buffer_formats;
protected:
    std::vector<VertexAttribBindingDescription> m_default_va_binding_descs;
    std::vector<VertexAttribLocationDescription> m_default_va_location_descs;
protected:
    std::map<ObjectName, RenderPassStrongReferenceObject> m_rp_map;
protected:
    /*! \var std::map<ObjectName, DescriptorSetLayoutStrongReferenceObject> m_basic_dsl_maps;
     *  \brief Map of DescriptorSetLeyouts for MeshRenderer(MVP Matrices), Light(Light Parameter) and Material(Textures).
     *         It record all descriptor layout belonging mesh.
     */
    std::map<ObjectName, DescriptorSetLayoutStrongReferenceObject> m_basic_dsl_maps;

    std::map<ObjectName, UniformVariableDescriptorStrongReferenceObject> m_material_basic_uvd_maps;

    std::map<ObjectName, ShaderProgramStrongReferenceObject> m_shader_program_maps;

    GraphicsIdentityGetterStrongReferenceObject m_graphics_identity_getter;

protected:
    GraphicsQueueStrongReferenceObject m_present_queue;

    GraphicsQueueStrongReferenceObject m_graphics_queue;

    GraphicsQueueStrongReferenceObject m_loading_queue;

    CommandPoolStrongReferenceObject m_loading_cmd_pool;

    CommandBufferWeakReferenceObject m_loading_cmd_buffer;

    GraphicsSwapchainStrongReferenceObject m_swapchain;

    std::mutex m_loading_buffer_lock;

protected:
    PeriodCounter m_fps_counter;

protected:
    float m_FPS;
};

inline float GraphicsManager::GetFPS() const
{
    return m_FPS;
}

inline GraphicsSwapchainWeakReferenceObject GraphicsManager::GetSwapchain() const
{
    return m_swapchain;
}

______________SD_END_GRAPHICS_NAMESPACE______________