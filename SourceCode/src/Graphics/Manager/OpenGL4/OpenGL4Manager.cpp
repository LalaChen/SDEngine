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

#if defined(_WIN32) || defined(_WIN64)

#include <GL/glew.h>

#include "LogManager.h"
#include "OpenGL4Manager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

OpenGL4Manager::OpenGL4Manager()
: GraphicsManager()
{
    SDLOG("New OpenGL4Manager object.");
}

OpenGL4Manager::~OpenGL4Manager()
{
    SDLOG("Delete OpenGL4Manager object.");
}

void OpenGL4Manager::InitializeGraphicsSystem(const EventArg &i_arg)
{
    SDLOG("Initialize OpenGL4Manager.");
    glewInit();
}

void OpenGL4Manager::ReleaseGraphicsSystem()
{
    SDLOG("Release OpenGL4Manager.");
}

void OpenGL4Manager::Resize(Size_ui32 i_w, Size_ui32 i_h)
{
    glViewport(0, 0, i_w, i_h);
    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

//----------------------- Render Flow -----------------------
void OpenGL4Manager::RenderBegin()
{
}

void OpenGL4Manager::RenderToScreen()
{
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void OpenGL4Manager::RenderEnd() 
{
}

______________SD_END_GRAPHICS_NAMESPACE______________

#endif