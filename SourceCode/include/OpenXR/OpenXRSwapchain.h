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


#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>

#include "SDEngineMacro.h"
#include "Texture.h"
#include "Resolution.h"
#include "GraphicsSwapchain.h"

using SDE::Graphics::Resolution;

using SDE::Graphics::GraphicsSwapchain;
using SDE::Graphics::GraphicsSwapchainWeakReferenceObject;
using SDE::Graphics::GraphicsSwapchainStrongReferenceObject;

using SDE::Graphics::Texture;
using SDE::Graphics::TextureWeakReferenceObject;
using SDE::Graphics::TextureStrongReferenceObject;

using SDE::Graphics::GraphicsQueue;
using SDE::Graphics::GraphicsQueueWeakReferenceObject;
using SDE::Graphics::GraphicsQueueStrongReferenceObject;


______________SD_START_OPENXR_NAMESPACE______________

class SDENGINE_CLASS OpenXRSwapchain : public GraphicsSwapchain
{
public:
	explicit OpenXRSwapchain(const ObjectName &i_name, const GraphicsQueueWeakReferenceObject &i_queue, const Resolution &i_eye_buffer_size);
	virtual ~OpenXRSwapchain();
public:
	void Initialize() override;
	void RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex) override;
protected:
	XrSession m_session;
	XrSwapchain m_swapchain;
	std::vector<XrSwapchainImageBaseHeader*> m_images;
};

_______________SD_END_OPENXR_NAMESPACE_______________