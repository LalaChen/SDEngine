#pragma once

//#define RECORD_EVERY_FRAME
#define RECORD_POOL_V2
//#define TIME_MEASURE
//#define SINGLE_FLOW

#include "CommandRecordingThread.h"
#include "Sample.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::Math;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(SampleDrawObjects);

class LightData
{
public:
    LightData();
    ~LightData();
public:
    LightParameter m_light_data;
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
    void InitializeCommonUniformSet(const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_common_dsl_wrefs);
    void UpdateCommonUniformSet(const SampleCameraData &i_camera, const LightData &i_light);
    void Draw(const RenderPassWeakReferenceObject &i_rp_wref, const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_sp_id);
public:
    UniformBufferWeakReferenceObject m_basic_wrefs;
    UniformBufferWeakReferenceObject m_light_wrefs;
    DescriptorPoolStrongReferenceObject m_common_pool_sref;
    std::vector<DescriptorSetWeakReferenceObject> m_common_set_wrefs;
public:
    MeshWeakReferenceObject m_mesh;
    MaterialWeakReferenceObject m_shared_mat_wref;
    MaterialStrongReferenceObject m_mat_sref;
public:
    Transform m_trans;
};

class SampleDrawObjects : public Sample
{
public:
    explicit SampleDrawObjects(const ObjectName &i_sample_name);
    virtual ~SampleDrawObjects();
protected:
    void CreateCommandBufferAndPool() override;
    void CreateRenderPass() override;
    void CreateFramebuffer() override;
public:
    void Initialize() override;
    void Render() override;
    void Destroy() override;
    void Resize(Size_ui32 i_width, Size_ui32 i_height) override;
    TextureWeakReferenceObject GetColorBuffer() const override;
protected:
    void RecordCommandBuffer();
private:
    void CreateCommonUniformVariablesAndLayouts();
    void CreateGeneralUniformVariablesAndLayouts();
    void CreateShaderProgram();
    void CreateTexture();
    void CreateCamera();
    void InitializeCameraPosition();
    void CreateLight();
    void CreateSharedMaterial();
    void CreateObjects();
    void UpdateCamera();
protected:
    std::vector<DescriptorSetLayoutStrongReferenceObject> m_common_dsl_srefs;
protected:
    std::vector<DescriptorSetLayoutStrongReferenceObject> m_general_dsl_srefs;
protected:
    SampleCameraData m_camera;
    LightData m_light;
    RenderPassStrongReferenceObject m_forward_rp_sref;
    TextureStrongReferenceObject m_tex_sref;
    ShaderProgramStrongReferenceObject m_phong_shader_sref;
    ShaderProgramStrongReferenceObject m_axes_shader_sref;
    MaterialStrongReferenceObject m_shared_material_sref;
    MaterialStrongReferenceObject m_axes_shared_material_sref;
    MeshStrongReferenceObject m_cube_sref;
    MeshStrongReferenceObject m_floor_sref;
    MeshStrongReferenceObject m_axes_sref;
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

inline TextureWeakReferenceObject SampleDrawObjects::GetColorBuffer() const
{
    return m_camera.m_color_buffer;
}