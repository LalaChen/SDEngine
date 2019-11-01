#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

#include "Sample.h"

using SDE::Graphics::ModelData;

using SDE::Graphics::Texture;
using SDE::Graphics::TextureStrongReferenceObject;
using SDE::Graphics::TextureWeakReferenceObject;

using SDE::Graphics::Mesh;
using SDE::Graphics::MeshStrongReferenceObject;
using SDE::Graphics::MeshWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Sample2_DrawScene);

class Sample2_DrawScene : public Sample
{
public:
    typedef std::map<FilePathString, TextureStrongReferenceObject> TextureResourceMap;
public:
    explicit Sample2_DrawScene(VulkanAPITestManager *i_mgr);
    virtual ~Sample2_DrawScene();
public:
    void Initialize() override;
    void Render() override;
    void Destroy() override;
public:
    VkImage GetColorBuffer() override { return VK_NULL_HANDLE; }
protected:
    void CreateRenderPassAndFramebuffer() override;
    void CreateCommandBufferAndPool() override;
private:
    void CreateModel();
    void ImportAssimpModel(ModelData &io_model);
    void CreateRenderFlow();
protected:
    //render pass.
    VkRenderPass m_VK_render_pass;
protected:
    //for camera
    VkImage m_VK_color_buffer;
    VkDeviceMemory m_VK_color_buffer_memory;
    VkImageView m_VK_color_buffer_image_view;
    VkImage m_VK_depth_buffer;
    VkDeviceMemory m_VK_depth_buffer_memory;
    VkImageView m_VK_depth_buffer_image_view;
    VkFramebuffer m_VK_frame_buffer;
protected:
    //prepare for work thread or camera.
    VkCommandPool m_VK_cmd_pool;
    VkCommandBuffer m_VK_cmd_buffer;
protected:
    std::vector<MeshStrongReferenceObject> m_meshes;
    TextureResourceMap m_textures;
};