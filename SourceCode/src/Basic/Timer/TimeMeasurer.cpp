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
#include "TimerMeasurer.h"

______________SD_START_BASIC_NAMESPACE_______________

TimerMeasurer::TimerMeasurer(const std::string &i_name)
: m_name(i_name)
, m_start(false)
{
}

TimerMeasurer::~TimerMeasurer()
{
}

void TimerMeasurer::Start()
{
    if (m_start == false) {
        m_time_pts.clear();
        std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
        m_time_pts.push_back(tp);
        m_start = true;
    }
    else {
        SDLOGW("TM[%s] is started.", m_name.c_str());
    }
}

void TimerMeasurer::Record()
{
    if (m_start == true) {
        std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
        m_time_pts.push_back(tp);
    }
    else {
        SDLOGW("TM[%s] isn't started.", m_name.c_str());
    }
}

void TimerMeasurer::Stop()
{
    if (m_start == true) {
        std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
        m_time_pts.push_back(tp);
    }
    else {
        SDLOGW("TM[%s] isn't started.", m_name.c_str());
    }
}

std::string TimerMeasurer::ToString() const
{
    std::string result;
    if (m_time_pts.size() >= 2) {
        double total_cost = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_time_pts[m_time_pts.size() - 1] - m_time_pts[0]).count()) / 1000.0;
        result = StringFormat("TM[%s] : Total time = %lf\n", m_name.c_str(), total_cost);
        for (uint32_t tpID = 1; tpID < m_time_pts.size(); ++tpID) {
            double total_cost = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_time_pts[tpID] - m_time_pts[tpID - 1]).count()) / 1000.0;
            result += StringFormat("\tTM[%s][%d~%d] : %lf secs\n", m_name.c_str(), tpID, tpID - 1, total_cost);
        }
    }
    else {
        result = StringFormat("TM[%s] : Tps is small than 2. No information.", m_name.c_str());
    }
    return result;
}

_______________SD_END_BASIC_NAMESPACE________________