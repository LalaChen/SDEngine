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

using SDE::Graphics::RenderPass;
using SDE::Graphics::RenderPassStrongReferenceObject;
using SDE::Graphics::RenderPassWeakReferenceObject;

using SDE::Graphics::RenderFlow;
using SDE::Graphics::RenderFlowStrongReferenceObject;
using SDE::Graphics::RenderFlowWeakReferenceObject;

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
    void Resize(Size_ui32 i_width, Size_ui32 i_height) override;
    void Destroy() override;
public:
    VkImage GetColorBuffer() override { return VK_NULL_HANDLE; }
    VkImageView GetColorBufferImageView() override { return VK_NULL_HANDLE; }
protected:
    void CreateCommandBufferAndPool() override;
    void CreateRenderPass() override;
    void CreateFramebuffer() override;
private:
    void CreateModel();
    void ImportAssimpModel(ModelData &io_model);
    void CreateShaderProgram();
protected:
    TextureStrongReferenceObject m_color_buffer;
    TextureStrongReferenceObject m_depth_buffer;
    RenderFlowStrongReferenceObject m_render_flow;
protected:
    //prepare for work thread or camera.
    VkCommandPool m_VK_cmd_pool;
    VkCommandBuffer m_VK_cmd_buffer;
protected:
    std::vector<MeshStrongReferenceObject> m_meshes;
    TextureResourceMap m_textures;
};