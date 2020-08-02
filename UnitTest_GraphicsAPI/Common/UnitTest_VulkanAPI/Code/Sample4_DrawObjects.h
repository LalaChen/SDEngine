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
    void InitalizeMaterial(const ShaderProgramWeakReferenceObject &i_sp_wref, const SampleCameraData &i_camera, const LightData &i_light, const TextureWeakReferenceObject &i_tex_wref);
    void UpdateMaterial(const SampleCameraData &i_camera, const LightData &i_light);
    void Draw(const RenderPassWeakReferenceObject &i_rp_wref, const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_sp_id);
public:
    MeshWeakReferenceObject m_mesh;
public:
    TextureWeakReferenceObject m_texture;
    Transform m_trans;
    MaterialStrongReferenceObject m_mat_sref;
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
protected:
    void RecordCommandBuffer();
private:
    void CreateShaderProgram();
    void CreateTexture();
    void CreateCamera();
    void CreateLight();
    void CreateObjects();
    void CreatePipeline();
    void UpdateCamera();
protected:
    SampleCameraData m_camera;
    LightData m_light;
    RenderPassStrongReferenceObject m_forward_rp_sref;
    TextureStrongReferenceObject m_tex_sref;
    ShaderProgramStrongReferenceObject m_phong_shader_sref;
    MeshStrongReferenceObject m_cube_sref;
    MeshStrongReferenceObject m_floor_sref;
protected:
#if !defined(SINGLE_FLOW)
#if defined(RECORD_POOL_V2)
    std::vector<SecondaryCommandPoolThreadStrongReferenceObject> m_rec_threads;
#else
    std::vector<CommandRecordingThreadStrongReferenceObject> m_rec_threads;
#endif
#endif
    CommandBufferWeakReferenceObject m_main_cb_wref;
    CommandPoolStrongReferenceObject m_cp_sref;
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