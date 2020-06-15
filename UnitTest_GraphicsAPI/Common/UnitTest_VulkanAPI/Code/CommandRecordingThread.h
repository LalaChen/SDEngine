#pragma once


#include <queue>

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using SDE::Graphics::CommandPool;
using SDE::Graphics::CommandPoolStrongReferenceObject;
using SDE::Graphics::CommandPoolWeakReferenceObject;

using SDE::Graphics::CommandBuffer;
using SDE::Graphics::CommandBufferStrongReferenceObject;
using SDE::Graphics::CommandBufferWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CommandRecordingThread);

class CommandRecordingThread : public SDE::Basic::Object
{
public:
    explicit CommandRecordingThread(const std::string &i_name);
    virtual ~CommandRecordingThread();
public:
    void Initialize(uint32_t i_default_cbs = 1);
    void AddTask(const std::function<void(const CommandBufferWeakReferenceObject&)> &i_task);
    void Wait(std::list<CommandBufferWeakReferenceObject> &io_submitted_list);
protected:
    void Run();
protected:
    CommandPoolStrongReferenceObject m_cp_sref;
    std::list<CommandBufferWeakReferenceObject> m_cb_wrefs;
    std::list<CommandBufferWeakReferenceObject>::iterator m_current_cb_iter;
protected:
    std::thread m_thread;
    std::mutex m_mutex;
    std::queue<std::function<void(const CommandBufferWeakReferenceObject&)>> m_task_pool;
    std::condition_variable m_task_cv;
    bool m_stop;
};