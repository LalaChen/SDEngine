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

#include "SDEngineCommonFunction.h"
#include "LogManager.h"
#include "SecondaryCommandPoolThread.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SecondaryCommandPoolThread::SecondaryCommandPoolThread(const ObjectName &i_object_name)
: Object(i_object_name)
, m_stop(false)
, m_recording(false)
{
}

SecondaryCommandPoolThread::~SecondaryCommandPoolThread()
{
    SDLOG("SecondaryCommandPoolThread(%s) dtor start !!!", m_object_name.c_str());

    m_mutex.lock();
    m_stop = true;
    m_task_cv.notify_one();
    m_mutex.unlock();

    if (m_thread.joinable() == true) {
        m_thread.join();
    }

    m_cp_sref.GetRef().Clear();

    SDLOG("SecondaryCommandPoolThread(%s) dtor end !!!", m_object_name.c_str());
}

void SecondaryCommandPoolThread::Initialize()
{
    m_cp_sref = new CommandPool(StringFormat("%s_cp", m_object_name.c_str()));
    m_cp_sref.GetRef().Initialize();
    m_cb_wref = m_cp_sref.GetRef().AllocateCommandBuffer(StringFormat("secondary_cb"), CommandBufferLevel_SECONDARY);
    m_thread = std::thread(std::bind(&SecondaryCommandPoolThread::Record, this));
}

void SecondaryCommandPoolThread::AddTask(const CommandFunction &i_task)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_stop == false) {
        m_task_pool.push(i_task);
        m_task_cv.notify_one();
    }
}

void SecondaryCommandPoolThread::StartRecording(const CommandBufferInheritanceInfo &i_info, const Viewport &i_vp, const ScissorRegion &i_sr)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    m_cb_wref.GetRef().Begin(i_info);
    m_recording = true;
    m_viewport = i_vp;
    m_scissor_region = i_sr;
    m_task_cv.notify_one();
}

void SecondaryCommandPoolThread::Record()
{
    while (m_stop == false) {
        //Pop task and wait if we don't have any task.
        std::function<void(const CommandBufferWeakReferenceObject&)> task;
        {
            std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
            m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == false && m_recording == true) || m_stop == true; });
            if (m_stop == true) {
                break;
            }
            task = m_task_pool.front();
        }

        task(m_cb_wref);

        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_task_pool.pop();
            m_task_cv.notify_one(); //for notify thread call wait for checking task all finished yet.
        }
    }
}

void SecondaryCommandPoolThread::WaitAndStopRecording(std::list<CommandBufferWeakReferenceObject> &io_submitted_sc_list)
{
    std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
    m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == true); });

    m_cb_wref.GetRef().End();
    m_recording = false;
    io_submitted_sc_list.push_back(m_cb_wref);
}


______________SD_END_GRAPHICS_NAMESPACE______________