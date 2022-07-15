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

/*! \file      CommandPool.h
 *  \brief     Introduce of class CommandPool.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/02/04
 *  \copyright MIT License.
 */

#pragma once

#include <list>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "CommandPoolIdentity.h"
#include "CommandBuffer.h"
#include "CommandPoolBase.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CommandPool);

/*! \class CommandPool
 *  \brief In Vulkan graphics system, CommandPool is used to allocate command buffer and manage the one's handle.
 */
class SDENGINE_CLASS CommandPool : public CommandPoolBase
{
public:
    friend class GraphicsIdentityGetter;
public:
    /*! \fn explicit CommandPool(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of CommandPool
     */
    explicit CommandPool(const ObjectName &i_object_name);

    /*! \fn virtual ~CommandPool();
     *  \brief Destructor of CommandPool.
     */
    virtual ~CommandPool();
public:
    /*! \fn void Initialize() override;
     *  \brief Initialize command pool for vulkan.
     */
    void Initialize() override;

    /*! \fn void Clear() override;
     *  \brief Free allocated command buffers.
     */
    void Clear() override;

public:
    /*! \fn CommandBufferWeakReferenceObject  AllocateCommandBuffer(const ObjectName &i_buffer_name = "CommandBuffer", const CommandBufferLevelEnum &i_level = CommandBufferLevel_PRIMARY);
     *  \brief Allocate command buffer.
     */
    CommandBufferWeakReferenceObject AllocateCommandBuffer(const ObjectName &i_buffer_name = "CommandBuffer", const CommandBufferLevelEnum &i_level = CommandBufferLevel_PRIMARY);

    /*! \fn void RecycleCommandBuffer(const CommandBufferWeakReferenceObject &i_src);
     *  \param [in] i_src Target command buffer.
     *  \brief Free allocated command buffer.
     */
    void RecycleCommandBuffer(const CommandBufferWeakReferenceObject &i_src);
protected:
    /*! \var CommandPoolIdentity m_identity;
     *  \brief Identity of command pool.
     */
    CommandPoolIdentity m_identity;

    /*! \fn var std::list<CommandBufferStrongReferenceObject> m_cmd_bufs;
     *  \brief Allocated command buffers.
     */
    std::list<CommandBufferStrongReferenceObject> m_cmd_bufs;
};

______________SD_END_GRAPHICS_NAMESPACE______________