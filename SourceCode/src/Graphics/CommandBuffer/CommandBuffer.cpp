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

#include "GraphicsManager.h"
#include "CommandPool.h"
#include "CommandBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

CommandBuffer::CommandBuffer(const ObjectName &i_object_name, const WeakReferenceObject<Object> &i_cmd_pool, const CommandBufferLevelEnum &i_level)
: Object(i_object_name)
, m_origin_pool(i_cmd_pool)
{
    m_identity.m_cmd_buffer_level = i_level;
    //Create command buffer by manager.
    GraphicsManager::GetRef().AllocateCommandBuffer(m_identity, m_origin_pool.DynamicCastTo<CommandPool>());
}

CommandBuffer::~CommandBuffer()
{
    GraphicsManager::GetRef().FreeCommandBuffer(m_identity, m_origin_pool.DynamicCastTo<CommandPool>());
}

void CommandBuffer::Begin(const CommandBufferInheritanceInfo &i_cmd_inheritance_info)
{
    GraphicsManager::GetRef().BeginCommandBuffer(m_identity, i_cmd_inheritance_info);
}

void CommandBuffer::End()
{
    GraphicsManager::GetRef().EndCommandBuffer(m_identity);
}

______________SD_END_GRAPHICS_NAMESPACE______________