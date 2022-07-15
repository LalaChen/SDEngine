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

/*! \file      RenderPassIdentity.h
 *  \brief     Introduce of class RenderPassIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/26
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsElementIdentity.h"
#include "MultiviewInfo.h"
#include "AttachmentDescription.h"
#include "SubpassDescription.h"
#include "SubpassDependency.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class RenderPassIdentity
 *  \brief In our system, class RenderPassIdentity is used to record information
 *         about render pass.
 */
class SDENGINE_CLASS RenderPassIdentity : public GraphicsElementIdentity
{
public:
    /*! \fn explicit RenderPassIdentity();
     *  \brief Constructor of RenderPassIdentity
     */
    RenderPassIdentity();

    /*! \fn ~RenderPassIdentity();
     *  \brief Destructor of RenderPassIdentity
     */
    ~RenderPassIdentity();
public:
    /*! \var CompHandle m_handle;
     *  \brief handle about render pass.
     */
    CompHandle m_handle;

    /* \var CompHandle m_device;
     * \brief Device this handle belong.
     */
    CompHandle m_device;

    /*! \var std::vector<AttachmentDescription> m_attachment_descs;
     *  \brief Description about each attachment.
     */
    std::vector<AttachmentDescription> m_attachment_descs;

    /*! \var std::vector<SubpassDescription> m_subpasses_descs;
     *  \brief Subpass information.
     */
    std::vector<SubpassDescription> m_subpasses_descs;

    /*! \var std::vector<SubpassDependency> m_sp_dependencies;
     *  \brief describing relation between two subpasses.
     */
    std::vector<SubpassDependency> m_sp_dependencies;

    /*! \var MultiviewInfo m_multiview_info;
     *  \brief describing multiview information.
     */
    MultiviewInfo m_multiview_info;
};

______________SD_END_GRAPHICS_NAMESPACE______________