#include "CommandRecordingThread.h"

CommandRecordingThread::CommandRecordingThread(const std::string &i_name)
: Object(i_name)
, m_stop(false)
{
    m_thread = std::thread(std::bind(&CommandRecordingThread::Run, this));
}

CommandRecordingThread::~CommandRecordingThread()
{
    SDLOG("Recording thread(%s) dtor start !!!", m_object_name.c_str());
    Wait();
    
    m_mutex.lock();
    m_stop = true;
    m_task_cv.notify_one();
    m_mutex.unlock();

    if (m_thread.joinable() == true) {
        m_thread.join();
    }
    SDLOG("Recording thread(%s) dtor end !!!", m_object_name.c_str());
}

void CommandRecordingThread::Run()
{
    while (m_stop == false) {
        //Pop task and wait if we don't have any task.
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
            m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == false) || m_stop == true; });
            if (m_stop == true) {
                break;
            }
            task = m_task_pool.front();
        }

        task();

        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_task_pool.pop();
            m_task_cv.notify_one(); //for notify thread call wait for checking task all finished yet.
        }
    }
}

void CommandRecordingThread::AddTask(const std::function<void()> &i_task)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_stop == false) {
        m_task_pool.push(i_task);
        m_task_cv.notify_one();
    }
}

void CommandRecordingThread::Wait()
{
    std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
    m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == true); });
}
