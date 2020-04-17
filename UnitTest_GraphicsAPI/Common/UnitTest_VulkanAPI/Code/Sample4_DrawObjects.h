#pragma once

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

using SDE::Math::Transform;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample1_DrawObjects);

class ObjectData
{
public:
    ObjectData();
    ~ObjectData();
public:
    MeshStrongReferenceObject m_mesh;
    TextureWeakReferenceObject m_texture;
public:
    Transform m_trans;
};

class SampleCameraData
{
public:
    SampleCameraData();
    ~SampleCameraData();
public:
    //for camera
    TextureStrongReferenceObject m_color_buffer;
    TextureStrongReferenceObject m_depth_buffer;
    RenderFlowStrongReferenceObject m_forward_rf;
public:
    Transform m_trans;
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
    void CreateObjects();
    void CreateCamera();
protected:
    SampleCameraData m_camera;
    RenderPassStrongReferenceObject m_forward_rp_sref;
    TextureStrongReferenceObject m_tex_sref;
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