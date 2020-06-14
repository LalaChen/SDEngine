#pragma once


#include <queue>

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CommandRecordingThread);

class CommandRecordingThread : public SDE::Basic::Object
{
public:
    explicit CommandRecordingThread(const std::string &i_name);
    virtual ~CommandRecordingThread();
public:
    void AddTask(const std::function<void()> &i_task);
    void Wait();
protected:
    void Run();
protected:
    std::thread m_thread;
    std::mutex m_mutex;
    std::queue<std::function<void()>> m_task_pool;
    std::condition_variable m_task_cv;
    bool m_stop;
};