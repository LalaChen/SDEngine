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

/*! \file      VulkanSwapchain.h
 *  \brief     Introduce of class VulkanSwapchain
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/11
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"

#include "Object.h"
#include "VulkanWrapper.h"
#include "VulkanConfig.h"
#include "Resolution.h"
#include "GraphicsIdentityGetter.h"
#include "CommandPool.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(VulkanSwapchain);

class SDENGINE_CLASS VulkanSwapchain : public Object
{
public:
	VulkanSwapchain(const ObjectName &i_name, const GraphicsIdentityGetterWeakReferenceObject &i_id_getter);
	~VulkanSwapchain();
public:
	void Initialize(
		const VulkanConfig &i_config,
		VkPhysicalDevice i_phy_device,
		VkDevice i_device,
		VkSurfaceKHR i_surface,
		uint32_t i_present_q_fam_id,
		VkQueue i_present_queue);

	void RenderTexture2DToScreen(const TextureIdentity &i_tex_identity);
public:
	Resolution GetResolution() const;
protected:
	void InitializeVKSwapchain(
		const VulkanConfig &i_config,
		VkPhysicalDevice i_phy_device,
		VkDevice i_device,
		VkSurfaceKHR i_surface,
		uint32_t i_present_q_fam_id,
		VkQueue i_present_queue);
protected:
	Resolution m_screen_size;
protected:
	GraphicsIdentityGetterWeakReferenceObject m_id_getter;
protected:
	CommandPoolStrongReferenceObject m_pool;
	CommandBufferWeakReferenceObject m_cmd_buffer;
protected:
	uint32_t m_max_img_acq_time;
	uint32_t m_max_fence_wait_time;
protected:
	VkDevice m_device;
	VkQueue m_queue;
protected:
	VkSurfaceFormatKHR m_final_sur_format;
protected:
	VkSemaphore m_present_sema;
	VkSemaphore m_wait_img_sema;
protected:
	VkSwapchainKHR m_swapchain;
	VkPresentModeKHR m_final_p_mode;
	std::vector<VkImage> m_swapchain_images;
};

______________SD_END_GRAPHICS_NAMESPACE______________