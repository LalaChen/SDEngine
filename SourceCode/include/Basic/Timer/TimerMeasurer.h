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

/*! \file      TimerMeasurer.h
 *  \brief     Introduce of class TimerMeasurer
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/27
 *  \copyright MIT License.
 */

#pragma once

#include <chrono>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

______________SD_START_BASIC_NAMESPACE_______________

class SDENGINE_CLASS TimerMeasurer
{
public:
    explicit TimerMeasurer(const std::string &i_name);
    virtual ~TimerMeasurer();
public:
    void Start();
    void Record();
    void Stop();
    std::string ToString() const;
protected:
    std::string m_name;
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> m_time_pts;
    bool m_start;
};

_______________SD_END_BASIC_NAMESPACE________________