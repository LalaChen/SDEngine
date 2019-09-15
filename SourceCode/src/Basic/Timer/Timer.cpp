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

#include <ctime>

#include "LogManager.h"
#include "Timer.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(Timer);

Timer::Timer()
: m_start_time(0.0)
, m_previous_time(0.0)
, m_current_time(0.0)
, m_end_time(0.0)
, m_delta_time(0.0)
{
    SD_SINGLETON_DECLARATION_REGISTER;
}

Timer::~Timer()
{
}

void Timer::Start()
{
    GetCurrentTimeByOS(m_start_time);
    m_previous_time = m_start_time;
    m_current_time = m_start_time;
}

void Timer::Update()
{
    m_previous_time = m_current_time;
    GetCurrentTimeByOS(m_current_time);
    m_delta_time = m_current_time - m_previous_time;
}

void Timer::End()
{
    GetCurrentTimeByOS(m_end_time);
}

_______________SD_END_BASIC_NAMESPACE________________