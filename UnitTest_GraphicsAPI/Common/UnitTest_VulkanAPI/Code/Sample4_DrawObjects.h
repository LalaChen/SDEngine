#pragma once

//#define RECORD_EVERY_FRAME
#define RECORD_POOL_V2
//#define TIME_MEASURE
//#define SINGLE_FLOW

#include "CommandRecordingThreadV2.h"
#include "CommandRecordingThread.h"
#include "Sample.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::Math;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample4_DrawObjects);

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
    void InitializeCommonUniformSet(const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_common_dsls);
    void UpdateCommonUniformSet(const SampleCameraData &i_camera, const LightData &i_light);
    void Draw(const RenderPassWeakReferenceObject &i_rp, const CommandBufferWeakReferenceObject &i_cb, uint32_t i_sp_id);
public:
    UniformBufferWeakReferenceObject m_basics;
    UniformBufferWeakReferenceObject m_lights;
    DescriptorPoolStrongReferenceObject m_common_pool;
    std::vector<DescriptorSetWeakReferenceObject> m_common_sets;
public:
    MeshWeakReferenceObject m_mesh;
    MaterialWeakReferenceObject m_shared_mat;
    MaterialStrongReferenceObject m_mat;
public:
    Transform m_trans;
};

class Sample4_DrawObjects : public Sample
{
public:
    explicit Sample4_DrawObjects(VulkanAPITestManager *i_mgr);
    virtual ~Sample4_DrawObjects();
protected:
    void CreateCommandBufferAndPool() override;
    void CreateRenderPass() override;
    void CreateFramebuffer() override;
public:
    void Initialize() override;
    void Render() override;
    void Destroy() override;
    void Resize(Size_ui32 i_width, Size_ui32 i_height) override;
    VkImage GetColorBuffer() override;
    VkImageView GetColorBufferImageView() override;
protected:
    void RecordCommandBuffer();
private:
    void CreateCommonUniformVariablesAndLayouts();
    void CreateGeneralUniformVariablesAndLayouts();
    void CreateShaderProgram();
    void CreateTexture();
    void CreateCamera();
    void CreateLight();
    void CreateSharedMaterial();
    void CreateObjects();
    void UpdateCamera();
protected:
    std::vector<DescriptorSetLayoutStrongReferenceObject> m_common_dsls;
protected:
    std::vector<DescriptorSetLayoutStrongReferenceObject> m_general_dsls;
protected:
    SampleCameraData m_camera;
    LightData m_light;
    RenderPassStrongReferenceObject m_forward_rp;
    TextureStrongReferenceObject m_tex;
    ShaderProgramStrongReferenceObject m_phong_shader;
    ShaderProgramStrongReferenceObject m_axes_shader;
    MaterialStrongReferenceObject m_shared_material;
    MaterialStrongReferenceObject m_axes_shared_material;
    MeshStrongReferenceObject m_cube;
    MeshStrongReferenceObject m_floor;
    MeshStrongReferenceObject m_axes;
protected:
#if !defined(SINGLE_FLOW)
#if defined(RECORD_POOL_V2)
    std::vector<SecondaryCommandPoolThreadStrongReferenceObject> m_rec_threads;
#else
    std::vector<CommandRecordingThreadStrongReferenceObject> m_rec_threads;
#endif
#endif
    CommandBufferWeakReferenceObject m_main_cb;
    CommandPoolStrongReferenceObject m_cp;
protected:
    std::list<ObjectData> m_scene_objects;
    float m_cube_interval;
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