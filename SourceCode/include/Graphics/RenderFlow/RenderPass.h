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

/*! \file      RenderPass.h
 *  \brief     Introduce of class about render pass.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/16
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "AttachmentDescription.h"
#include "AttachmentReference.h"
#include "ImageViewIdentity.h"
#include "RenderPassIdentity.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(RenderPass);

/*! \class RenderPass
 *  \brief In our graphic system, class RenderPass is used to describe rendering flow.
 *         There are one or many subpasses in one render pass. And then we will specify
 *         The relation between subpasses.
 */
class SDENGINE_CLASS RenderPass : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit RenderPass(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of RenderPass
     */
    explicit RenderPass(const ObjectName &i_object_name);

    /*! \fn virtual ~RenderPass();
     *  \brief Destructor of RenderPass.
     */
    virtual ~RenderPass();
public:
    /*! \fn void AddRenderPassDescription(const std::vector<AttachmentDescription> &i_att_descs, const std::vector<SubpassDescription> &i_sp_descs, const std::vector<SubpassDependency> &i_sp_deps);
     *  \param [in] i_att_descs Descriptions about attachments.
     *  \param [in] i_sp_descs Descriptions about subpasses.
     *  \param [in] i_sp_deps Descriptions about dependencies.
     *  \brief Add descriptions of render pass.
     */
    void AddRenderPassDescription(const std::vector<AttachmentDescription> &i_att_descs, const std::vector<SubpassDescription> &i_sp_descs, const std::vector<SubpassDependency> &i_sp_deps);
public:
    /*! \fn void Initialize();
     *  \brief initialize.
     */
    void Initialize();
public:
    /*! \fn const CompHandle GetHandle() const;
     *  \brief return render pass handle for create frame buffer.
     */
    const CompHandle GetHandle() const;

    /*! \fn std::vector<TextureFormatEnum> CreateImageViewFormats();
     *  \brief return ImageView format corresponding this render pass. We only assign format to description.
     */
    std::vector<TextureFormatEnum> CreateImageViewFormats() const;

    /*! \fn const std::vector<SubpassDescription>& GetSubpassDescriptions() const;
     *  \brief return subpass description.
     */
    const std::vector<SubpassDescription>& GetSubpassDescriptions() const;

    /*! \fn uint32_t GetSubpassAmount() const;
     *  \brief return amount subpass.
     */
    uint32_t GetSubpassAmount() const;

    /*! \fn const std::vector<AttachmentDescription>& GetAttachmentDescriptions() const;
     *  \brief return attachment description.
     */
    const std::vector<AttachmentDescription>& GetAttachmentDescriptions() const;

protected:
    /*! \var RenderPassIdentity m_identity;
     *  \brief Identity about render pass.
     */
    RenderPassIdentity m_identity;
};

inline const CompHandle RenderPass::GetHandle() const
{
    return m_identity.m_handle;
}

inline const std::vector<SubpassDescription>& RenderPass::GetSubpassDescriptions() const
{
    return m_identity.m_subpasses_descs;
}

inline uint32_t RenderPass::GetSubpassAmount() const
{
    return static_cast<uint32_t>(m_identity.m_subpasses_descs.size());
}

inline const std::vector<AttachmentDescription>& RenderPass::GetAttachmentDescriptions() const
{
    return m_identity.m_attachment_descs;
}
______________SD_END_GRAPHICS_NAMESPACE______________