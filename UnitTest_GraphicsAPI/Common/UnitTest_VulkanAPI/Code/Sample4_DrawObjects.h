#pragma once

#define SINGLE_FLOW

#include "Sample.h"

using SDE::Graphics::Texture;
using SDE::Graphics::TextureStrongReferenceObject;
using SDE::Graphics::TextureWeakReferenceObject;

using SDE::Graphics::Mesh;
using SDE::Graphics::MeshStrongReferenceObject;
using SDE::Graphics::MeshWeakReferenceObject;

using SDE::Graphics::RenderPass;
using SDE::Graphics::RenderPassStrongReferenceObject;
using SDE::Graphics::RenderPassWeakReferenceObject;

using SDE::Graphics::RenderFlow;
using SDE::Graphics::RenderFlowStrongReferenceObject;
using SDE::Graphics::RenderFlowWeakReferenceObject;

using SDE::Graphics::GraphicsPipeline;
using SDE::Graphics::GraphicsPipelineStrongReferenceObject;
using SDE::Graphics::GraphicsPipelineWeakReferenceObject;

using SDE::Graphics::CommandBuffer;
using SDE::Graphics::CommandBufferStrongReferenceObject;
using SDE::Graphics::CommandBufferWeakReferenceObject;

using SDE::Graphics::CommandPool;
using SDE::Graphics::CommandPoolStrongReferenceObject;
using SDE::Graphics::CommandPoolWeakReferenceObject;

using SDE::Math::Transform;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample4_DrawObjects);

class ObjectMaterialData
{
public:
    ObjectMaterialData();
    ~ObjectMaterialData();
public:
    void Initialize(VulkanAPITestManager *i_mgr, const GraphicsPipelineWeakReferenceObject &i_pipeline_wref, const TextureWeakReferenceObject &i_main_tex_wref);
    void Release(VulkanAPITestManager *i_mgr);
public:
    GraphicsPipelineWeakReferenceObject m_pipeline_wref;
    TextureWeakReferenceObject m_main_tex_wref;
    VkDescriptorPool m_desc_pool;
    VkDescriptorSet m_desc_set;
    VkBuffer m_basic_ub; //Common
    VkDeviceMemory m_basic_mem;
    VkBuffer m_light_ub;
    VkDeviceMemory m_light_mem;
};

class LightData
{
public:
    LightData();
    ~LightData();
public:
    LightUniformBuffer m_light_data;
    Transform m_trans;
};

class SampleCameraData
{
public:
    SampleCameraData();
    ~SampleCameraData();
public:
    RenderFlowStrongReferenceObject m_forward_rf;
public:
    TextureStrongReferenceObject m_color_buffer;
    TextureStrongReferenceObject m_depth_buffer;
public:
    Transform m_trans;
    Matrix4X4f m_proj_mat;
};

class ObjectData
{
public:
    ObjectData();
    ~ObjectData();
public:
    void UpdateMaterial(VulkanAPITestManager* i_mgr, const SampleCameraData &i_camera, const LightData &i_light);
    void Draw(VulkanAPITestManager* i_mgr, const CommandBufferWeakReferenceObject &i_cb_wref);
public:
    MeshStrongReferenceObject m_mesh;
public:
    TextureWeakReferenceObject m_texture;
    Transform m_trans;
public:
    ObjectMaterialData m_material;
};


class Sample4_DrawObjects : public Sample
{
public:
    explicit Sample4_DrawObjects(VulkanAPITestManager *i_mgr);
    virtual ~Sample4_DrawObjects();
protected:
    void CreateRenderPassAndFramebuffer() override;
    void CreateCommandBufferAndPool() override;
public:
    void Initialize() override;
    void Render() override;
    void Destroy() override;
    void Resize(Size_ui32 i_width, Size_ui32 i_height) override;
    VkImage GetColorBuffer() override;
    VkImageView GetColorBufferImageView() override;
private:
    void CreateTexture();
    void CreateCamera();
    void CreateLight();
    void CreateObjects();
    void CreatePipeline();
protected:
    SampleCameraData m_camera;
    LightData m_light;
    RenderPassStrongReferenceObject m_forward_rp_sref;
    TextureStrongReferenceObject m_tex_sref;
    GraphicsPipelineStrongReferenceObject m_pipeline_sref;
protected:
    std::vector<CommandBufferWeakReferenceObject> m_cmd_buf_wrefs;
#if defined(SINGLE_FLOW)
    CommandPoolStrongReferenceObject m_cmd_pool_sref;
#else
#endif
protected:
    std::list<ObjectData> m_cube_objects;
    float m_cube_side_length;
    uint32_t m_cube_row;
    uint32_t m_cube_col;
    uint32_t m_cube_depth;
};

inline VkImage Sample4_DrawObjects::GetColorBuffer()
{
    if (m_camera.m_color_buffer.IsNull() == false) {
        return reinterpret_cast<VkImage>(m_camera.m_color_buffer.GetRef().GetHandle());
    }
    else {
        return VK_NULL_HANDLE;
    }
}

inline VkImageView Sample4_DrawObjects::GetColorBufferImageView()
{
    if (m_camera.m_color_buffer.IsNull() == false) {
        return reinterpret_cast<VkImageView>(m_camera.m_color_buffer.GetRef().GetViewHandle());
    }
    else {
        return VK_NULL_HANDLE;
    }
}