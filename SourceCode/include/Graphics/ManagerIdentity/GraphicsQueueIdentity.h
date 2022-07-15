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

/*! \file      GraphicsQueueIdentity.h
 *  \brief     Introduce of class about GraphicsQueueIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/24
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsElementIdentity.h"
#include "QueueAbility.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS GraphicsQueueIdentity : public GraphicsElementIdentity
{
public:
    /*! \fn GraphicsQueueIdentity();
     *  \brief constructor.
     */
    GraphicsQueueIdentity();

    /*! \fn GraphicsQueueIdentity();
     *  \brief destructor.
     */
    ~GraphicsQueueIdentity();
public:
    /*! \var CompHandle m_handle;
     *  \brief Queue handle.  (VkQueue for vulkan, EGLContext for OpenGL)
     */
    CompHandle m_handle;

    /*! \var CompHandle m_phy_device;
     *  \brief phy devices handle. (Vulkan Only)
     */
    CompHandle m_phy_device;

    /*! \var CompHandle m_device;
     *  \brief devices handle. (Original Context in OpenGL, VkDevice in Vulkan)
     */
    CompHandle m_device;

    /*! \var CompHandle m_ability;
     *  \brief ability for queue.
     */
    QueueAbility m_abilities;

    /*! \var uint32_t m_family_id;
     *  \brief Queue family id. (Vulkan Only)
     */
    uint32_t m_family_id;

    /*! \var uint32_t m_queue_id;
     *  \brief Queue id. (Vulkan Only)
     */
    uint32_t m_queue_id;
};

______________SD_END_GRAPHICS_NAMESPACE______________