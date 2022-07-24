/* ==============  SD Engine License ==============
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

/*! \file      SDEngineTemplates.h
 *  \brief     SDEngineTemplates collects a lot of useful template class.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/08
 *  \copyright MIT License.
 */

#pragma once

#include <typeindex>
#include <cstdarg>  // For va_start, etc.
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <thread>
#include <functional>
#include <condition_variable>

#include "SDEngineCommonType.h"
#include "SDEngineMacro.h"
#include "Object.h"

______________SD_START_BASIC_NAMESPACE_______________

template <class CallbackType>
class Dispatcher : public Object
{
public:
    explicit Dispatcher(const ObjectName &i_object_name);
    virtual ~Dispatcher();
public:
    void AddTask(const CallbackType &i_func);
    void Run();
protected:
    /*! \fn std::thread m_thread;
     *  \brief Thread for launching recording task.
     */
    std::thread m_thread;

    /*! \fn std::mutex m_mutex;
     *  \brief Mutex for blocking critical section.
     */
    std::mutex m_mutex;

    /*! \fn std::queue<std::function<void()>> m_task_pool;
     *  \brief pool for queueing tasks.
     */
    std::queue< CallbackType > m_task_pool;

    /*! \fn std::condition_variable m_task_cv;
     *  \brief signal to notify working.
     */
    std::condition_variable m_task_cv;

    /*! \fn bool m_stop;
     *  \brief signal to notify thread closing.
     */
    bool m_stop;
};

template <class CallbackType>
inline Dispatcher<CallbackType>::Dispatcher(const ObjectName &i_object_name)
: Object(i_object_name)
, m_stop(false)
{
}

template <class CallbackType>
inline Dispatcher<CallbackType>::~Dispatcher()
{
    m_mutex.lock();
    m_stop = true;
    m_task_cv.notify_one();
    m_mutex.unlock();

    if (m_thread.joinable() == true) {
        m_thread.join();
    }
}

template <class CallbackType>
inline void Dispatcher<CallbackType>::AddTask(const CallbackType& i_func)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (m_stop == false) {
        m_task_pool.push(i_func);
        m_task_cv.notify_one();
    }
}

template <class CallbackType>
inline void Dispatcher<CallbackType>::Run()
{
    if (m_thread.joinable() == true) {
        m_thread.detach();
    }

    m_thread = std::thread(
        [this]() {
            while (m_stop == false) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lck(m_mutex); //Delivery to task cv for ensuring ppol size and stop.
                    m_task_cv.wait(lck, [this]() {return m_task_pool.empty() == false || m_stop == true; });
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
    );
}

_______________SD_END_BASIC_NAMESPACE________________