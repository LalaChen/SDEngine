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

/*! \file      WindowsTimer.h
 *  \brief     Introduce of class WindowsTimer
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/04
 *  \copyright MIT License.
 */

#pragma once

#include "Timer.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \class WindowsTimer \n
 *  Timer in Windows OS.
 *  \sa Timer
 */
class SDENGINE_CLASS WindowsTimer : public Timer
{
public:
    /*! \fn explicit WindowsTimer();
     *  \brief The constructor of WindowsTimer Class.
     */
    explicit WindowsTimer();

    /*! \fn virtual ~WindowsTimer();
     *  \brief The destructor of WindowsTimer Class.
     */
    virtual ~WindowsTimer();
public:
    /*! \fn void GetCurrentTimeByOS(double &io_time) override;
     *  \param [inout] io_time double time variable.(unit : second)
     *  \brief Get current time by QueryPerformanceFrequency and QueryPerformanceCounter functions.
     */
    void GetCurrentTimeByOS(double &io_time) override;
};

//--------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}