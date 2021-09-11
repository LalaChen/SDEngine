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

#include "WorldGUISystem.h"
#include "GraphicsSystem.h"
#include "ECSManager.h"
#include "LogManager.h"
#include "BasicShapeCreator.h"
#include "AssimpModelLoader.h"
#include "GraphicsManager.h"

using SDE::Basic::ECSManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_SINGLETON_DECLARATION_IMPL(GraphicsManager);

GraphicsManager::GraphicsManager()
: m_fps_counter("FPS")
{
    // Register instance.
    SD_SINGLETON_DECLARATION_REGISTER;
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
    SDLOG("Release.");
    ReleaseRenderPasses();
    m_fps_counter.Stop();
}

void GraphicsManager::Render()
{
    //1. Execute some operations for each graphics API before rendering.
    RenderBegin();
    //2. Render scene by each camera.

    //3. Execute some operations for each graphics API when render to screen.
    RenderToScreen();
    //4. Execute some operations for each graphics API after rendering.
    RenderEnd();

    //
    m_fps_counter.AddCount();
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

______________SD_END_GRAPHICS_NAMESPACE______________