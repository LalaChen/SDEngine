#pragma once

#include <queue>
#include <thread>

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using SDE::Basic::Object;
using SDE::Basic::StrongReferenceObject;
using SDE::Basic::WeakReferenceObject;

using SDE::Graphics::CommandPool;
using SDE::Graphics::CommandPoolStrongReferenceObject;
using SDE::Graphics::CommandPoolWeakReferenceObject;

using SDE::Graphics::CommandBuffer;
using SDE::Graphics::CommandBufferStrongReferenceObject;
using SDE::Graphics::CommandBufferWeakReferenceObject;

using SDE::Graphics::CommandBufferInheritanceInfo;

using SDE::Basic::ObjectName;

typedef std::function<void(const CommandBufferWeakReferenceObject&)> RecordingTaskV2;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CommandRecordingThreadV2);

class CommandRecordingThreadV2 : public Object
{
public:
    CommandRecordingThreadV2(const ObjectName &i_object_name);
    ~CommandRecordingThreadV2();
public:
    void Initialize();
    void AddTask(const std::function<void(const CommandBufferWeakReferenceObject&)> &i_task);
    void StartRecording(const CommandBufferInheritanceInfo &i_info);
    void WaitAndStopRecording(std::list<CommandBufferWeakReferenceObject> &io_submitted_list);
protected:
    void Record();
protected:
    CommandPoolStrongReferenceObject m_cp_sref;
    CommandBufferWeakReferenceObject m_cb_wref;
protected:
    std::thread m_thread;
    std::mutex m_mutex;
    std::queue<RecordingTaskV2> m_task_pool;
    std::condition_variable m_task_cv;
    bool m_stop;
    bool m_recording;
};