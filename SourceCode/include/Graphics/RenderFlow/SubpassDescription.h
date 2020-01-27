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

/*! \file      SubpassDescription.h
 *  \brief     Introduce of class SubpassDescription.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/27
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "PipelineBindPoint.h"
#include "AttachmentReference.h"
#include "Object.h"

using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class SubpassDescription
 *  \brief In our system, class SubpassDescription is used to specify attachments in this step.
 */
class SDENGINE_CLASS SubpassDescription
{
public:
    /*! \fn SubpassDescription();
     *  \brief Constructor of SubpassDescription.
     */
    SubpassDescription();

    /*! \fn ~SubpassDescription();
     *  \brief Destructor of Subpass.
     */
    ~SubpassDescription();
public:
    /*! \var ObjectName m_name;
     *  \brief name of this subpass.
     */
    ObjectName m_name;

    /*! \var PipelineBindPoint m_bind_point;
     *  \brief Specify this subpass is in which kind of pipeline.
     */
    PipelineBindPointEnum m_bind_point;

    /*! \var std::vector<AttachmentReference> m_color_attachment_refs;
     *  \brief Color attachment references of renderpass that are
     *         used in this subpass.
     */
    std::vector<AttachmentReference> m_color_attachment_refs;

    /*! \var std::vector<AttachmentReference> m_input_attachment_refs;
     *  \brief Input attachment references of renderpass that are
     *         used in this subpass.
     */
    std::vector<AttachmentReference> m_input_attachment_refs;

    /*! \var std::vector<AttachmentReference> m_res_attachment_refs;
     *  \brief Resolve attachment in descriptions of renderpass that are
     *         used in this subpass.
     */
    std::vector<AttachmentReference> m_res_attachment_refs;

    /*! \var std::vector<AttachmentReference> m_pre_attachment_ref_ids;
     *  \brief Indices about preserve attachment in descriptions of renderpass that are
     *         used in this subpass.
     */
    std::vector<uint32_t> m_pre_attachment_ref_ids;

    /*! \var AttachmentReference m_depth_attachment_ref;
     *  \brief Index about depth attachment in descriptions of renderpass that is
     *         used in this subpass.
     */
    AttachmentReference m_depth_attachment_ref;
};

______________SD_END_GRAPHICS_NAMESPACE______________