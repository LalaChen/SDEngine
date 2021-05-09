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
#include "LogManager.h"
#include "CommandBuffer.h"
#include "CommandPool.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

CommandPool::CommandPool(const ObjectName &i_object_name)
: CommandPoolBase(i_object_name)
{
}

CommandPool::~CommandPool()
{
}

void CommandPool::Initialize()
{
    GraphicsManager::GetRef().CreateCommandPool(m_identity);
}

void CommandPool::Clear()
{
    for (std::list<CommandBufferStrongReferenceObject>::iterator iter = m_cmd_bufs.begin(); iter != m_cmd_bufs.end(); ) {
        iter = m_cmd_bufs.erase(iter);
    }
}

CommandBufferWeakReferenceObject CommandPool::AllocateCommandBuffer(const ObjectName &i_buffer_name, const CommandBufferLevelEnum &i_level)
{
    CommandBufferStrongReferenceObject alloc_buf_sref = new CommandBuffer(StringFormat("%s_%s", m_object_name.c_str(), i_buffer_name.c_str()), GetThisSharedPtrByType<CommandPoolBase>(), i_level);
    m_cmd_bufs.push_back(alloc_buf_sref);
    return alloc_buf_sref;
}

void CommandPool::RecycleCommandBuffer(const CommandBufferWeakReferenceObject &i_src_wref)
{
    for (std::list<CommandBufferStrongReferenceObject>::iterator iter = m_cmd_bufs.begin(); iter != m_cmd_bufs.end(); ) {
        if ((*iter) == i_src_wref) {
            iter = m_cmd_bufs.erase(iter);
            return;
        }
        else {
            iter++;
        }
    }
    SDLOGW("Command Buffer [%s] isn't in Pool[%s].", SD_WREF(i_src_wref).GetObjectName().c_str(), m_object_name.c_str());
}

______________SD_END_GRAPHICS_NAMESPACE______________