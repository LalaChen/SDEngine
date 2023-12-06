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
#include "GraphicsManager.h"

#include "WorldGUISystem.h"
#include "GraphicsSystem.h"
#include "ECSManager.h"
#include "LogManager.h"
#include "BasicShapeCreator.h"
#include "AssimpModelLoader.h"

using SDE::Basic::ECSManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_SINGLETON_DECLARATION_IMPL(GraphicsManager);

GraphicsManager::GraphicsManager()
: m_fps_counter("FPS")
, m_graphics_identity_getter(new GraphicsIdentityGetter("GraphicsIdentityGetter"))
{
    // Register instance.
    SD_SINGLETON_DECLARATION_REGISTER;
    // Load Graphics Config.
    m_graphics_config.LoadFromFile("Common/Configs/EngineConfig.json");
    // Assimp Model Loader.
    new AssimpModelLoader();
    // Basic Shader Creator
    new BasicShapeCreator();
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::InitializeBasicResource()
{
    std::function<void(uint64_t i_count, double i_period_ms)> fps_cbk = 
        std::function<void(uint64_t, double)>(
            [this](uint64_t i_count, double i_period_ms) {
                m_FPS = (static_cast<double>(i_count) / i_period_ms) * 1000.0;
                //SDLOGD("FPS : %lf.", m_FPS);
            }
        );

    SDLOG("Initialize Basic Resource Start!!!");

    ECSManager::GetRef().RegisterSystem<WorldGUISystem>("WorldGUISystem");
    ECSManager::GetRef().RegisterSystem<GraphicsSystem>("GraphicsSystem");

    InitializeBasicDescriptorSetLayout();
    InitializeBasicMaterialUniformDescriptors();
    InitializeDefaultRenderPasses();
    InitializeDefaultPipelineInfos();
    InitializeBasicShaderPrograms();

    m_fps_counter.Start(1000.0, fps_cbk, false);
    SDLOG("Initialize Basic Resource End!!!");
}

void GraphicsManager::ReleaseBasicResource()
{
    SDLOG("Release Basic Resource.");
    for (std::map<ObjectName, ShaderProgramStrongReferenceObject>::iterator sp_iter = m_shader_program_maps.begin();
        sp_iter != m_shader_program_maps.end();) {
        sp_iter = m_shader_program_maps.erase(sp_iter);
    }

    for (std::map<ObjectName, DescriptorSetLayoutStrongReferenceObject>::iterator ds_iter = m_basic_dsl_maps.begin();
        ds_iter != m_basic_dsl_maps.end();) {
        ds_iter = m_basic_dsl_maps.erase(ds_iter);
    }

    for (std::map<ObjectName, UniformVariableDescriptorStrongReferenceObject>::iterator mvud_iter = m_material_basic_uvd_maps.begin();
        mvud_iter != m_material_basic_uvd_maps.end();) {
        mvud_iter = m_material_basic_uvd_maps.erase(mvud_iter);
    }

    for (std::list<GraphicsLayerStrongReferenceObject>::iterator iter = m_layers.begin();
        iter != m_layers.end(); ) {
        iter = m_layers.erase(iter);
    }

    ReleaseRenderPasses();
    m_fps_counter.Stop();
}

TextureFormatEnum GraphicsManager::GetDefaultDepthBufferFormat() const
{
    if (m_supported_depth_buffer_formats.size() > 0) {
        return m_supported_depth_buffer_formats[0];
    }
    else {
        SDLOGE("We don't have default supported depth format.");
        return TextureFormat_MAX_DEFINE_VALUE;
    }
}

bool GraphicsManager::IsSupportedDepthBufferFormat(TextureFormatEnum i_fmt) const
{
    for (const TextureFormatEnum &fmt : m_supported_depth_buffer_formats) {
        if (fmt == i_fmt) {
            return true;
        }
    }

    return false;
}

TextureFormatEnum GraphicsManager::GetDefaultColorBufferFormat() const
{
    if (m_supported_color_buffer_formats.size() > 0) {
        return m_supported_color_buffer_formats[0];
    }
    else {
        SDLOGE("We don't have default supported depth format.");
        return TextureFormat_MAX_DEFINE_VALUE;
    }
}

bool GraphicsManager::IsSupportedColorBufferFormat(TextureFormatEnum i_fmt) const
{
    for (const TextureFormatEnum &fmt : m_supported_color_buffer_formats) {
        if (fmt == i_fmt) {
            return true;
        }
    }
    return false;
}

GraphicsLayerWeakReferenceObject GraphicsManager::RegisterLayer(
    const ObjectName &i_name,
    const TextureWeakReferenceObject &i_color_buffer,
    const TextureWeakReferenceObject &i_depth_buffer,
    uint32_t i_layer_id)
{
    GraphicsLayerStrongReferenceObject result = new GraphicsLayer(i_name);
    SD_SREF(result).RegisterTexture(i_color_buffer, i_depth_buffer, i_layer_id);
    SD_SREF(result).Initialize();
    m_layers.push_back(result);
    m_layers.sort([](const GraphicsLayerStrongReferenceObject &a, const GraphicsLayerStrongReferenceObject &b) -> bool {
        return (SD_SREF(a).GetLayerOrder() < SD_SREF(b).GetLayerOrder());
        });
    return result;
}

void GraphicsManager::UnregisterLayer(
    const GraphicsLayerWeakReferenceObject &i_layer)
{
    GraphicsLayerStrongReferenceObject layer_sref = i_layer.DynamicCastToSharedPtr<GraphicsLayer>();
    for (std::list<GraphicsLayerStrongReferenceObject>::iterator iter = m_layers.begin();
        iter != m_layers.end(); ) {
        if ((*iter).IsEqualTo(layer_sref) == true) {
            iter = m_layers.erase(iter);
            return;
        }
        else {
            iter++;
        }
    }
}

void GraphicsManager::SubmitGraphicsCommands(const std::vector<CommandBufferWeakReferenceObject> &i_cbs)
{
    SD_SREF(m_graphics_queue).SubmitCommandBuffers(i_cbs);
}

void GraphicsManager::RenderTextureToScreen(const TextureWeakReferenceObject &i_tex)
{
    if (m_swapchain.IsNull() == false) {
        if (i_tex.IsNull() == true) {
            return;
        }

        if (SD_WREF(i_tex).IsInitialized() == false) {
            return;
        }

        SD_SREF(m_swapchain).RenderTextureToSwapchain(i_tex);

        m_fps_counter.AddCount();
    }
}

void GraphicsManager::RenderLayersToSwapchain()
{
    if (m_swapchain.IsNull() == false) {
        SD_SREF(m_swapchain).RenderLayersToSwapchain(m_layers);
        m_fps_counter.AddCount();
    }
}

void GraphicsManager::PresentSwapchain()
{
    if (m_swapchain.IsNull() == false) {
        SD_SREF(m_swapchain).Present();
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________