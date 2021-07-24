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

/*! \file      SecondaryCommandPoolThread.h
 *  \brief     Introduce of class SecondaryCommandPoolThread.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/20
 *  \copyright MIT License.
 */

#pragma once

#include <thread>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "Object.h"
#include "GraphicsPipelineParam.h"
#include "CommandPool.h"
#include "CommandBuffer.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(SecondaryCommandPoolThread);

/*! \class SecondaryCommandPoolThread
 *  \brief In Vulkan graphics system, SecondaryCommandPoolThread is used to record command to SECONDARY command buffer at
 *         another thread(multiple thread rendering) in one frame. We should start recording at frame begin. And then we
 *         need to push tasks into this pool thread. After all task pushed, we should wait all of task finishing and bind 
 *         the command buffer to main command buffer.
 */
class SDENGINE_CLASS SecondaryCommandPoolThread : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \var typedef std::function<void(const CommandBufferWeakReferenceObject&)> CommandFunction;
     *  \brief CommandFunction is the type for representing command task function callback. We type-define it for using 
     *         conveniently.
     */
    typedef std::function<void(const CommandBufferWeakReferenceObject&)> CommandFunction;
public:
    /*! \fn explicit SecondaryCommandPoolThread(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of SecondaryCommandPoolThread
     */
    explicit SecondaryCommandPoolThread(const ObjectName &i_object_name);

    /*! \fn virtual ~SecondaryCommandPoolThread();
     *  \brief Destructor of SecondaryCommandPoolThread.
     */
    virtual ~SecondaryCommandPoolThread();
public:
    /*! \fn void Initialize();
     *  \brief Initialize command pool thread and its secondary command buffer for vulkan.
     */
    void Initialize();

    /*! \fn void AddTask(const CommandFunction &i_task);
     *  \param [in] i_task target record task.
     *  \brief Add task to this queue.
     */
    void AddTask(const CommandFunction &i_task);

    /*! \fn void Reset();
     *  \brief Reset pool at beginning.
     */
    void Reset();

    /*! \fn void StartRecording(const CommandBufferInheritanceInfo &i_info, const Viewport &i_vp, const ScissorRegion &i_sr);
     *  \param [in] i_info Notify target renderpass, framebuffer and other infos for this secondary buffer.
     *  \param [in] i_vp Viewport data.
     *  \param [in] i_sr ScissorRegion data.
     *  \brief Start accepting task and recording it.
     */
    void StartRecording(const CommandBufferInheritanceInfo &i_info, const Viewport &i_vp, const ScissorRegion &i_sr);

    /*! \fn void WaitAndStopRecording(std::list<CommandBufferWeakReferenceObject> &io_submitted_sc_list);
     *  \param [in] io_submitted_list Submit secondary command buffer to submitted_sc_list;
     *  \brief Initialize command pool thread and its secondary command buffer for vulkan.
     */
    void WaitAndStopRecording(std::list<CommandBufferWeakReferenceObject> &io_submitted_sc_list);
protected:
    /*! \fn void Record();
     *  \brief Recording command function.
     */
    void Record();

    void BeginCommandBuffer();

    void EndCommandBuffer(std::list<CommandBufferWeakReferenceObject>& io_submitted_sc_list);
protected:
    /*! \fn CommandPoolStrongReferenceObject m_cp;
     *  \brief Pool for manager secondary buffer for this poot thread.
     */
    CommandPoolStrongReferenceObject m_cp;

    /*! \fn CommandBufferData m_cbs;
     *  \brief command buffers for manager secondary buffer for this poot thread.
     */
    std::vector<CommandBufferWeakReferenceObject> m_cbs;

    uint32_t m_current_used_cb;
protected:
    /*! \fn std::thread m_thread;
     *  \brief Thread for launching recording task.
     */
    std::thread m_thread;

    /*! \fn std::mutex m_mutex;
     *  \brief Mutex for blocking critical section.
     */
    std::mutex m_mutex;

    /*! \fn std::queue<CommandFunction> m_task_pool;
     *  \brief pool for queueing tasks.
     */
    std::queue<CommandFunction> m_task_pool;

    /*! \fn std::condition_variable m_task_cv;
     *  \brief signal to notify working.
     */
    std::condition_variable m_task_cv;

    /*! \fn bool m_stop;
     *  \brief signal to notify thread closing.
     */
    bool m_stop;

    /*! \fn bool m_recording;
     *  \brief signal to notify starting recording for THIS FRAME.
     */
    bool m_recording;

    /*! \fn Viewport m_viewport;
     *  \brief Viewport information about this time recording.
     */
    Viewport m_viewport;

    /*! \fn ScissorRegion m_scissor_region;
     *  \brief ScissorRegion information about this time recording.
     */
    ScissorRegion m_scissor_region;
};

______________SD_END_GRAPHICS_NAMESPACE______________