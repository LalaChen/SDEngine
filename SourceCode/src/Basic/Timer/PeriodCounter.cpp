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

#include "LogManager.h"
#include "PeriodCounter.h"

______________SD_START_BASIC_NAMESPACE_______________

PeriodCounter::PeriodCounter(const std::string &i_name)
: m_name(i_name)
, m_period_ms(0.0)
, m_start(false)
, m_only_once(false)
{
}

PeriodCounter::~PeriodCounter()
{
}

void PeriodCounter::Start(double i_period_millsec, const std::function<void(uint64_t, double)> &i_cbk, bool i_only_once)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    m_cbk = i_cbk;
    m_period_ms = i_period_millsec;
    m_count = 0;
    m_only_once = i_only_once;
    m_last_reset_time = std::chrono::high_resolution_clock::now();
    
}

void PeriodCounter::AddCount(uint32_t i_count)
{
    m_mutex.lock();
    m_count++;
    std::chrono::time_point<std::chrono::high_resolution_clock> current = std::chrono::high_resolution_clock::now();
    double diff_ms = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(current - m_last_reset_time).count());
    if (diff_ms > m_period_ms) {
        m_cbk(m_count, diff_ms);
        m_last_reset_time = std::chrono::high_resolution_clock::now();
        m_count = 0;
    }
    m_mutex.unlock();

    if (m_only_once == true) {
        Stop();
    }
}

void PeriodCounter::Stop()
{
    std::lock_guard<std::mutex> lck(m_mutex);
    m_period_ms = 0.0;
    m_start = false;
    m_count = 0;
    m_cbk = nullptr;
}

_______________SD_END_BASIC_NAMESPACE________________