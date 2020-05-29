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

/*! \file      PeriodCounter.h
 *  \brief     Introduce of class PeriodCounter
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/27
 *  \copyright MIT License.
 */


#pragma once

#include <chrono>
#include <functional>
#include <mutex>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

______________SD_START_BASIC_NAMESPACE_______________

class SDENGINE_CLASS PeriodCounter
{
public:
    PeriodCounter(const std::string &i_name);
    ~PeriodCounter();
public:
    void Start(double i_period_millsec, const std::function<void(uint64_t, double)> &i_cbk, bool i_only_once);
    void AddCount(uint32_t i_count = 1); //will lock mutex, so don't use another function at cbk.
    void Stop();
protected:
    std::string m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_reset_time;
    uint64_t m_count;
    double m_period_ms;
    bool m_start;
    bool m_only_once;
    std::function<void(uint64_t, double)> m_cbk;
    std::mutex m_mutex;
};

_______________SD_END_BASIC_NAMESPACE________________