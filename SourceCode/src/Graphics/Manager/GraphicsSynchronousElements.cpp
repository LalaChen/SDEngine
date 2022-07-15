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

#include "GraphicsSynchronousElements.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsFence::GraphicsFence(const ObjectName &i_name, CompHandle i_device)
: Object(i_name)
{
	m_identity.m_device = i_device;
}

GraphicsFence::~GraphicsFence()
{
	GraphicsManager::GetRef().DestroyFence(m_identity);
}

void GraphicsFence::Initialize()
{
	GraphicsManager::GetRef().CreateFence(m_identity);
}

void GraphicsFence::Reset()
{
	GraphicsManager::GetRef().ResetFence(m_identity);
}

GraphicsSemaphore::GraphicsSemaphore(const ObjectName &i_name, CompHandle i_device)
: Object(i_name)
{
	m_identity.m_device = i_device;
}

GraphicsSemaphore::~GraphicsSemaphore()
{
	GraphicsManager::GetRef().DestroySemaphoreObject(m_identity);
}

void GraphicsSemaphore::Initialize()
{
	GraphicsManager::GetRef().CreateSemaphoreObject(m_identity);
}

______________SD_END_GRAPHICS_NAMESPACE______________