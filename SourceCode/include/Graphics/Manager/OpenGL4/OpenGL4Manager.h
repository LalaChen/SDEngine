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

/*! \file      OpenGL4Manager.h
 *  \brief     Introduce of class OpenGL4Manager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */
#pragma once

#if defined(_WIN32) || defined(_WIN64)

#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class OpenGL4Manager
 *  In our system, OpenGL4Manager is a implementation for opengl4 graphics API.
 */
class SDENGINE_CLASS OpenGL4Manager : public GraphicsManager
{
public:
    /*! \fn OpenGL4Manager();
     *  \brief The constructor of OpenGL4Manager Class.
     */
    OpenGL4Manager();
    
    /*! \fn virtual ~OpenGL4Manager();
     *  \brief The destructor of OpenGL4Manager Class.
     */
    virtual ~OpenGL4Manager();
public:
    /*! \fn void InitializeGraphicsSystem() override;
     *  \param [in] i_arg Nothing. implement for following interface.
     *  \brief Initialize graphics API. (link dll, ...)
     */
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    
    /*! \fn void ReleaseGraphicsSystem() override;
     *  \brief Release graphics API.
     */
    void ReleaseGraphicsSystem() override;
public:
    /*! \fn void PrintSystemInformation() override;
     *  \brief Print system information.
     */
    void PrintSystemInformation() override;
public:
    void GetDesiredVulkanValidLayers(std::vector<const char*> &io_valid_layers) const override;
//----------- Descriptor Set Function (OpenGL doesn't have command buffer) ------------
public:
    void CreateDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity,  const std::vector<UniformVariableDescriptorWeakReferenceObject> &i_uvds) override;
    void DestroyDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity) override;
    void CreateDescriptorPool(DescriptorPoolIdentity &io_identity) override;
    void DestroyDescriptorPool(DescriptorPoolIdentity &io_identity) override;
    void AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool, const DescriptorSetLayoutWeakReferenceObject &i_layout) override;
    void WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uvs) override;
    void FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool) override;
public:
//----------- Command Buffer and Pool Function (OpenGL doesn't have command buffer) ------------
    void CreateCommandPool(CommandPoolIdentity &io_identity) override;
    void DestroyCommandPool(CommandPoolIdentity &io_identity) override;
    void AllocateCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool) override;
    void BeginCommandBuffer(const CommandBufferIdentity &i_identity, const CommandBufferInheritanceInfo &i_inheritance_info) override;
    void EndCommandBuffer(const CommandBufferIdentity &i_identity) override;
    void FreeCommandBuffer(CommandBufferIdentity &io_identity, const CommandPoolWeakReferenceObject &i_pool) override;
    void ExecuteCommandsToPrimaryCommandBuffer(const CommandBufferWeakReferenceObject &i_primary_cb, const std::list<CommandBufferWeakReferenceObject> &i_secondary_cbs) override;
public:
//----------- Vertex Buffer Function ------------
    void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size) override;
    void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteVertexBuffer(VertexBufferIdentity &io_identity) override;
    void MapVertexBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) override;
    void UnmapVertexBuffer(const VertexBufferIdentity &i_identity) override;
public:
//----------- Index Buffer Function ------------   
    void CreateIndexBuffer(IndexBufferIdentity &io_identity, Size_ui64 i_data_size) override;
    void RefreshStaticIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicIndexBuffer(const IndexBufferIdentity &i_identity, const void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteIndexBuffer(IndexBufferIdentity &io_identity) override;
    void MapIndexBuffer(const IndexBufferIdentity &i_identity, VoidPtr &io_buffer_handle)override;
    void UnmapIndexBuffer(const IndexBufferIdentity &i_identity) override;
public:
//----------- Uniform Buffer Interface Function ------------
    void CreateUniformBuffer(UniformBufferIdentity &io_identity) override;
    void MapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub, VoidPtr &io_buffer_handle) override;
    void UnmapUniformBuffer(const UniformBufferWeakReferenceObject &i_ub) override;
    void DeleteUnifromBuffer(UniformBufferIdentity &io_identity) override;
public:
    void CreateTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) override;
    void InitializeSwapchainTextureImage(TextureIdentity &io_tex_identity, SamplerIdentity &io_sampler_identity) override;
    void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size, const ImageLayoutEnum& i_dst_layout = ImageLayout_MAX_DEFINE_VALUE) override;
    void DeleteTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity) override;
public:
    void BindVertexBuffer(const VertexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, uint32_t i_binding_id, Size_ui64 i_offset) override;
    void BindIndexBuffer(const IndexBufferIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, Size_ui64 i_offset) override;
public:
    void CreateShaderModule(ShaderModuleIdentity &io_identity, const std::vector<UByte> &i_content) override;
    void DeleteShaderModule(ShaderModuleIdentity &io_identity) override;
public:
    void CreateGraphicsPipeline(GraphicsPipelineIdentity &io_identity, const ShaderModules &i_shaders, const RenderPassWeakReferenceObject &i_rp, const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_dsls) override;
    void BindGraphicsPipeline(const GraphicsPipelineIdentity &i_identity, const CommandBufferWeakReferenceObject &i_cb, const std::vector<DescriptorSetWeakReferenceObject> &i_dss) override;
    void DestroyGraphicsPipeline(GraphicsPipelineIdentity &io_identity) override;
public:
    void CreateRenderPass(RenderPassIdentity &io_identity) override;
    void BeginRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, const RenderPassWeakReferenceObject &i_rp, const ImageOffset &i_start_pos, const ImageSize &i_render_size) override;
    void GoToNextStepOfRenderPass(const CommandBufferWeakReferenceObject &i_cb, const FrameBufferWeakReferenceObject &i_fb, uint32_t i_sp_id) override;
    void EndRenderPass(const CommandBufferWeakReferenceObject &i_cb) override;
    void DestroyRenderPass(RenderPassIdentity &io_identity) override;
public:
    void CreateFrameBuffer(FrameBufferIdentity &io_identity, const RenderPassWeakReferenceObject &i_rp, const std::vector<TextureWeakReferenceObject> &i_bufs) override;
    void CreateFrameBufferGroup(FrameBufferGroupIdentity &io_identity) override;
    void DestroyFrameBufferGroup(FrameBufferGroupIdentity &io_identity) override;
    void DestroyFrameBuffer(FrameBufferIdentity &io_identity) override;
public:
    void SetViewport(const CommandBufferWeakReferenceObject &i_cb, const Viewport &i_vp) override;
    void SetViewports(const CommandBufferWeakReferenceObject &i_cb, const std::vector<Viewport> &i_vps) override;
    void SetScissor(const CommandBufferWeakReferenceObject &i_cb, const ScissorRegion &i_region) override;
    void SetScissors(const CommandBufferWeakReferenceObject &i_cb, const std::vector<ScissorRegion> &i_regions) override;
    void DrawByIndices(const CommandBufferWeakReferenceObject &i_cb, const IndexBufferWeakReferenceObject &i_ib, uint32_t i_first_id, int32_t i_offset, uint32_t i_first_ins_id, uint32_t i_ins_number) override;
public:
    void CreateQueue(GraphicsQueueIdentity &io_identity) override;
    void SubmitCommandBuffersToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsFenceWeakReferenceObject &i_fence, const std::vector<CommandBufferWeakReferenceObject> &i_cbs) override;
    void PresentToQueue(const GraphicsQueueIdentity &i_identity, const GraphicsSwapchainIdentity &i_sw_identity, uint32_t i_img_id, const std::vector<GraphicsSemaphoreWeakReferenceObject> &i_semas) override;
    void DestroyQueue(GraphicsQueueIdentity &io_identity) override;
public:
    void CreateFence(GraphicsFenceIdentity &io_identity) override;
    void ResetFence(const GraphicsFenceIdentity &i_identity) override;
    void DestroyFence(GraphicsFenceIdentity &io_identity) override;
public:
    void CreateSemaphoreObject(GraphicsSemaphoreIdentity &io_identity) override;
    void DestroySemaphoreObject(GraphicsSemaphoreIdentity &io_identity) override;
public:
    void CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) override;
    void DestroyGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity) override;
    void GetReadyTextureOfSwapchain(const GraphicsSwapchainIdentity &i_identity, const GraphicsSemaphoreWeakReferenceObject &i_acq_sema, uint32_t &io_idx) override;
    void RenderTextureToSwapchain(const GraphicsSwapchainIdentity &i_identity, uint32_t i_idx, const GraphicsQueueWeakReferenceObject &i_queue, const CommandBufferWeakReferenceObject &i_cmd_buffer, const GraphicsSemaphoreWeakReferenceObject &i_present_sema, const TextureWeakReferenceObject &i_texture, const ImageBlitParam &i_param) override;
public:
    void Resize(CompHandle i_new_surface, Size_ui32 i_w, Size_ui32 i_h) override;
public:
    Resolution GetScreenResolution() const override { return Resolution(); }
};

______________SD_END_GRAPHICS_NAMESPACE______________

#endif