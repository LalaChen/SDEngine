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

#include "LogManager.h"
#include "AssimpModelLoader.h"
#include "GraphicsManager.h"

using SDE::Graphics::AssimpModelLoader;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_SINGLETON_DECLARATION_IMPL(GraphicsManager);

GraphicsManager::GraphicsManager()
{
    // Register instance.
    SD_SINGLETON_DECLARATION_REGISTER;
    // Assimp Model Loader.
    new AssimpModelLoader();
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Initialize()
{
    SDLOG("Initialize.");
    InitializeDefaultRenderPasses();
}

void GraphicsManager::Release()
{
    SDLOG("Release.");
    ReleaseRenderPasses();
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
}

______________SD_END_GRAPHICS_NAMESPACE______________