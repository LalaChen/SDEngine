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

#include "OpenXRSwapchain.h"

#include "TextureFormat_Vulkan.h"
#include "LogManager.h"
#include "OpenXRAPIManager.h"
#include "OpenXRVulkanManager.h"

using namespace SDE::Graphics;

______________SD_START_OPENXR_NAMESPACE______________

OpenXRSwapchain::OpenXRSwapchain(const ObjectName &i_object_name, const GraphicsQueueWeakReferenceObject &i_queue, const Resolution &i_eye_buffer_size)
: GraphicsSwapchain(i_object_name, i_queue)
{
	m_identity.m_screen_size = i_eye_buffer_size;
}

OpenXRSwapchain::~OpenXRSwapchain()
{
	OpenXRAPIManager::GetRef().DestroySwapchain(m_swapchain);
}

void OpenXRSwapchain::Initialize()
{
	GraphicsManager::GetRef().CreateGraphicsSwapchain(m_identity);
	//Create OpenXRSwapchain
	XrSwapchainCreateInfo c_info = {};
	c_info.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
	c_info.arraySize = 2;
	c_info.format = TextureFormat_Vulkan::Convert(GraphicsManager::GetRef().GetDefaultColorBufferFormat());
	c_info.width = m_identity.m_screen_size.GetWidth();
	c_info.height = m_identity.m_screen_size.GetHeight();
	c_info.mipCount = 1;
	c_info.faceCount = 1;
	c_info.sampleCount = VK_SAMPLE_COUNT_1_BIT;
	c_info.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
	
	XrResult result = OpenXRAPIManager::GetRef().CreateSwapchain(c_info, m_swapchain);
	if (result != XR_SUCCESS) {
		throw std::exception("Create XrSwapchain failure");
	}
	else {
		XrResult result = OpenXRAPIManager::GetRef().EnumerateSwapchainImages(
			m_swapchain, m_images);
		if (result != XR_SUCCESS) {
			throw std::exception("Enuermate XrSwapchain Image failure");
		}
		//
		m_identity.m_swapchain_images.resize(m_images.size());
		for (uint32_t id = 0; id < m_images.size(); ++id) {
			if (m_images[id] != nullptr) {
				m_identity.m_swapchain_images[id] = reinterpret_cast<CompHandle>(
					((*m_images[id]).type == XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR) ?
					reinterpret_cast<XrSwapchainImageVulkanKHR*>(m_images[id])->image : SD_NULL_HANDLE);
			}
			else {
				SDLOGE("Get XrSwapchain Image[%d] nullptr !!!", id);
				throw std::exception("Get XrSwapchain Image failure");
			}
		}
		//
		m_pool = new CommandPool("GraphicsSwapchainCmdPool");
		SD_SREF(m_pool).Initialize();
		m_cmd_buffer = SD_SREF(m_pool).AllocateCommandBuffer("GraphicsSwapchainCmdBuffer");
		//
		m_acq_sema = new GraphicsSemaphore("AcquireSema", m_identity.m_device);
		SD_SREF(m_acq_sema).Initialize();
		m_present_sema = new GraphicsSemaphore("PresentSema", m_identity.m_device);
		SD_SREF(m_present_sema).Initialize();
	}
	m_identity.m_handle = reinterpret_cast<CompHandle>(m_swapchain);
}

void OpenXRSwapchain::RenderTextureToSwapchain(const TextureWeakReferenceObject &i_tex)
{
	//1. acquire image.
	XrResult result;
	uint32_t img_idx = UINT32_MAX;
	result = OpenXRAPIManager::GetRef().GetReadyTextureOfSwapchain(m_swapchain, img_idx);
	if (result != XR_SUCCESS) {
		throw std::exception("Get Image idx failure");
	}

	//2. Blit image to openxr swapchain.
	GraphicsManager::GetRef().RenderTextureToSwapchain(
		m_identity, img_idx,
		m_queue, m_cmd_buffer, m_present_sema, i_tex);

	//3. release image.
	result = OpenXRAPIManager::GetRef().ReleaseSwapchainImage(m_swapchain);
	if (result != XR_SUCCESS) {
		throw std::exception("Release Image idx failure");
	}
}

_______________SD_END_OPENXR_NAMESPACE_______________