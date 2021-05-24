#include "TimerMeasurer.h"
#include "CommandRecordingThreadV2.h"


using SDE::Basic::StringFormat;
using SDE::Graphics::CommandBufferLevel_SECONDARY;

CommandRecordingThreadV2::CommandRecordingThreadV2(const ObjectName &i_name)
: Object(i_name)
, m_stop(false)
, m_recording(false)
{
}

CommandRecordingThreadV2::~CommandRecordingThreadV2()
{
    SDLOG("Recording thread(%s) dtor start !!!", m_object_name.c_str());

    m_mutex.lock();
    m_stop = true;
    m_task_cv.notify_one();
    m_mutex.unlock();

    if (m_thread.joinable() == true) {
        m_thread.join();
    }

    m_cp.GetRef().Clear();

    SDLOG("Recording thread(%s) dtor end !!!", m_object_name.c_str());
}

void CommandRecordingThreadV2::Initialize()
{
    m_cp = new CommandPool(StringFormat("%s_cp", m_object_name.c_str()));
    m_cp.GetRef().Initialize();
    m_cb = m_cp.GetRef().AllocateCommandBuffer(StringFormat("secondary_cb"), CommandBufferLevel_SECONDARY);
    m_thread = std::thread(std::bind(&CommandRecordingThreadV2::Record, this));
}

void CommandRecordingThreadV2::Record()
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

        task(m_cb);

        {
            std::lock_guard<std::mutex> lck(m_mutex);
            m_task_pool.pop();
            m_task_cv.notify_one(); //for notify thread call wait for checking task all finished yet.
        }
    }
}

void CommandRecordingThreadV2::AddTask(const std::function<void(const CommandBufferWeakReferenceObject&)>& i_task)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_stop == false) {
        m_task_pool.push(i_task);
        m_task_cv.notify_one();
    }
}

void CommandRecordingThreadV2::StartRecording(const CommandBufferInheritanceInfo &i_info)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    m_cb.GetRef().Begin(i_info);
    m_recording = true;
    m_task_cv.notify_one();
}

void CommandRecordingThreadV2::WaitAndStopRecording(std::list<CommandBufferWeakReferenceObject> &io_submitted_list)
{
    std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
    m_task_cv.wait(lck, [this]() {return (m_task_pool.empty() == true); });
    m_cb.GetRef().End();
    m_recording = false;
    io_submitted_list.push_back(m_cb);
}
