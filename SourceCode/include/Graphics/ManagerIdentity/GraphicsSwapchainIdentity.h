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

/*! \file      GraphicsSwapchainIdentity.h
 *  \brief     Introduce of class about GraphicsSwapchainIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/24
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsElementIdentity.h"
#include "Resolution.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS GraphicsSwapchainIdentity : public GraphicsElementIdentity
{
public:
    /*! \fn GraphicsSwapchainIdentity();
     *  \brief constructor.
     */
    GraphicsSwapchainIdentity();
    
    /*! \fn GraphicsSwapchainIdentity();
     *  \brief destructor.
     */
    ~GraphicsSwapchainIdentity();
public:
    /*! \var CompHandle m_handle;
     *  \brief Swapchain handle.
     */
    CompHandle m_handle;
    
    /*! \var CompHandle m_phy_device;
     *  \brief phy devices handle. (No use in OpenGL)
     */
    CompHandle m_phy_device;

    /*! \var CompHandle m_device;
     *  \brief devices handle. (Original Context in OpenGL, VkDevice in Vulkan)
     */
    CompHandle m_device;

    /*! \var CompHandle m_surface;
     *  \brief m_surface handle. (No use in OpenGL)
     */
    CompHandle m_surface;

    /*! \var CompHandle m_screen_size;
     *  \brief screen size of swapchain.
     */
    Resolution m_screen_size;

    /*! \var std::vector<CompHandle> m_swapchain_images;
     *  \brief swapchain color buffer images.
     */
    std::vector<CompHandle> m_swapchain_images;
};

______________SD_END_GRAPHICS_NAMESPACE______________