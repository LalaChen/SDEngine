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

/*! \file      CommandBuffer.h
 *  \brief     Introduce of class CommandBuffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/02/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "CommandBufferIdentity.h"
#include "CommandBufferInheritanceInfo.h"
#include "FrameBuffer.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

using SDE::Basic::WeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CommandBuffer);

class SDENGINE_CLASS CommandBuffer : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit CommandBuffer(const ObjectName &i_object_name, WeakReferenceObject<Object> &i_cmd_pool, const CommandPoolWeakReferenceObject &i_cmd_pool);
     *  \param [in] i_object_name Name of this object.
     *  \param [in] i_cmd_pool Pool that create this buffer.
     *  \param [in] i_level Level of this command buffer.
     *  \brief Constructor of CommandBuffer
     */
    explicit CommandBuffer(const ObjectName &i_object_name, const WeakReferenceObject<Object> &i_cmd_pool, const CommandBufferLevelEnum &i_level);

    /*! \fn virtual ~CommandBuffer();
     *  \brief Destructor of CommandBuffer.
     */
    virtual ~CommandBuffer();
public:
    
    void Begin(const CommandBufferInheritanceInfo &i_cmd_inheritance_info = CommandBufferInheritanceInfo());
    
    void End();
public:
    const CompHandle GetHandle() const;
protected:

    CommandBufferIdentity m_identity;

    WeakReferenceObject<Object> m_origin_pool;
};

inline const CompHandle CommandBuffer::GetHandle() const
{
    return m_identity.m_handle;
}

______________SD_END_GRAPHICS_NAMESPACE______________