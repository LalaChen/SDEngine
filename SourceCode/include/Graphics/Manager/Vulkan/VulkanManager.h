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

/*! \file      VulkanManager.h
 *  \brief     Introduce of class VulkanManager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/09
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsManager.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class VulkanManager
 *  In our system, VulkanManager is a implementation for opengl4 graphics API.
 */
class SDENGINE_CLASS VulkanManager : public GraphicsManager
{
public:
	typedef void* SDVKHandle;
	typedef int32_t SDVKValue;
protected:
	static std::vector<const char*> ValidLayers;
	static std::vector<const char*> NecessaryExtensions;
public:
	static const std::vector<const char*>& GetValidLayers();
public:
	/*! \fn VulkanManager();
	 *  \brief The constructor of VulkanManager Class.
	 */
	VulkanManager();

	/*! \fn virtual ~VulkanManager();
	 *  \brief The destructor of VulkanManager Class.
	 */
	virtual ~VulkanManager();
public:
	/*! \fn void InitializeGraphicsSystem(const EventArg &i_arg) override;
	 *  \param [in] i_arg Vulkan creating arguments.
	 *  \brief Initialize graphics API. (link dll, ...)
	 */
	void InitializeGraphicsSystem(const EventArg &i_arg) override;

	/*! \fn void ReleaseGraphicsSystem() override;
	 *  \brief Release graphics API.
	 */
	void ReleaseGraphicsSystem() override;
protected:
	void InitializeDebugMessage();
	void InitializePhysicalDevice();
	void InitializeLogicDevice();
	void InitializeSwapChain();
	void InitializeImageViews();
protected:
	SDVKHandle m_VK_instance; //VkInstance
	SDVKHandle m_VK_surface; //VkSurfaceKHR
	SDVKHandle m_VK_debug_report_cbk; //VkDebugReportCallbackEXT
	SDVKHandle m_VK_physical_device; //VkPhysicalDevice
	SDVKHandle m_VK_logic_device; //VkDevice
	SDVKHandle m_VK_graphics_queue;//VkQueue
	SDVKHandle m_VK_present_queue;//VkQueue
	SDVKHandle m_VK_swap_chain;//VkSwapchainKHR
	std::vector<SDVKHandle> m_VK_sc_images; //VkImage
	std::vector<SDVKHandle> m_VK_sc_image_views; //VkImageView
protected:
	SDVKValue m_scimg_format;
	Size_ui m_viewport_width;
	Size_ui m_viewport_height;
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}