#include "TimerMeasurer.h"
#include "CommandRecordingThread.h"


using SDE::Basic::StringFormat;
using SDE::Graphics::CommandBufferLevel_SECONDARY;

CommandRecordingThread::CommandRecordingThread(const std::string &i_name)
: Object(i_name)
, m_stop(false)
{
}

CommandRecordingThread::~CommandRecordingThread()
{
    SDLOG("Recording thread(%s) dtor start !!!", m_object_name.c_str());
    
    m_mutex.lock();
    m_stop = true;
    m_task_cv.notify_one();
    m_mutex.unlock();

    if (m_thread.joinable() == true) {
        m_thread.join();
    }

    m_cp_sref.GetRef().Clear();

    SDLOG("Recording thread(%s) dtor end !!!", m_object_name.c_str());
}

void CommandRecordingThread::Initialize(uint32_t i_default_cbs)
{
    m_cp_sref = new CommandPool(StringFormat("%s_cp", m_object_name.c_str()));
    m_cp_sref.GetRef().Initialize();
    for (uint32_t cbID = 0; cbID < i_default_cbs; ++cbID) {
        m_cb_wrefs.push_back(m_cp_sref.GetRef().AllocateCommandBuffer(StringFormat("cb_%d", cbID), CommandBufferLevel_SECONDARY));
    }
    m_current_cb_iter = m_cb_wrefs.begin();

    m_thread = std::thread(std::bind(&CommandRecordingThread::Run, this));
}

void CommandRecordingThread::Run()
{
    while (m_stop == false) {
        //Pop task and wait if we don't have any task.
        std::function<void(const CommandBufferWeakReferenceObject&)> task;
        {
            std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
            m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == false) || m_stop == true;});
            if (m_stop == true) {
                break;
            }
            task = m_task_pool.front();
        }

        if (m_current_cb_iter == m_cb_wrefs.end()) {
            size_t cb_size = m_cb_wrefs.size();
            m_cb_wrefs.push_back(m_cp_sref.GetRef().AllocateCommandBuffer(StringFormat("cb_%d", cb_size), CommandBufferLevel_SECONDARY));
            m_current_cb_iter = m_cb_wrefs.end();
            m_current_cb_iter--;
        }

        task((*m_current_cb_iter));

        m_current_cb_iter++;

        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_task_pool.pop();
            m_task_cv.notify_one(); //for notify thread call wait for checking task all finished yet.
        }
    }
}

void CommandRecordingThread::AddTask(const std::function<void(const CommandBufferWeakReferenceObject&)> &i_task)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_stop == false) {
        m_task_pool.push(i_task);
        m_task_cv.notify_one();
    }
}

void CommandRecordingThread::Wait(std::list<CommandBufferWeakReferenceObject> &io_submitted_list)
{
    std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
    m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == true); });
    for (std::list<CommandBufferWeakReferenceObject>::iterator cb_iter = m_cb_wrefs.begin(); cb_iter != m_current_cb_iter; ++cb_iter) {
        io_submitted_list.push_back((*cb_iter));
    }
    m_current_cb_iter = m_cb_wrefs.begin();
}
