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

/*! \file      Timer.h
 *  \brief     Introduce of class Timer
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/04
 *  \copyright MIT License.
 */

#pragma once

#include <chrono>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class Timer
 *  \brief In our system, Timer is used to record some infos about time. We will record the program
 *  starting time while this program starting. For each frame, we will cache the previous time and
 *  get current time by GetCurrentTimeByOS. Please note that we need to implement function
 *  GetCurrentTimeByOS for following OS platform. And then we calculate delta time by the current
 *  and prevoius times.
 */
class SDENGINE_CLASS Timer
{
public:
    SD_SINGLETON_DECLARATION(Timer);
public:
    /*! \fn explicit Timer();
     *  \brief The constructor of Timer Class.
     */
    explicit Timer();

    /*! \fn virtual ~Timer();
     *  \brief The destructor of Timer Class.
     */
    virtual ~Timer();

public:
    /*! \fn void Start();
     *  \brief Start the timer. This function is used while application lanuching. We will get
     *         current time by OS and save it as program start time. And then we initialize previous
     *         and current time by start time.
     */
    void Start();

    /*! \fn void Update();
     *  \brief Update the timer. This function is used while application updating. We will record current
     *         time as previous time and then get current time by OS as currnet time.
     */
    void Update();

    /*! \fn void End();
     *  \brief End the timer. This function is used while application is destroyed. We will record current time
     *         as end time.
     */
    void End();
public: //getter function.
    /*! \var double GetProgramStartTime() const;
     *  \brief Program start time.
     */
    double GetProgramStartTime() const;

    /*! \var double GetProgramPreviousTime() const;
     *  \brief Program previous time.
     */
    double GetProgramPreviousTime() const;

    /*! \var double GetProgramCurrentTime() const;
     *  \brief Program current time.
     */
    double GetProgramCurrentTime() const;

    /*! \var double GetProgramEndTime() const;
     *  \brief Program end time.
     */
    double GetProgramEndTime() const;

protected:
    /*! \var double m_start_time;
     *  \brief Program start time.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
    
    /*! \var double m_previous_time;
     *  \brief Program previous time.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_previous_time;
    
    /*! \var double m_current_time;
     *  \brief Program current time.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_current_time;
    
    /*! \var double m_end_time;
     *  \brief Program end time.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_end_time;
    
    /*! \var double m_delta_time;
     *  \brief The delta time between two frames. Unit is seconds.[VarGet Attribute]
     */
    SD_DECLARE_ATTRIBUTE_VAR_GET(double, m_delta_time, ProgramDeltaTime);
};

inline double Timer::GetProgramStartTime() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_start_time.time_since_epoch()).count()) / 1000.0;
}

inline double Timer::GetProgramPreviousTime() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_previous_time.time_since_epoch()).count()) / 1000.0;
}

inline double Timer::GetProgramCurrentTime() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_current_time.time_since_epoch()).count()) / 1000.0;
}

inline double Timer::GetProgramEndTime() const
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_end_time.time_since_epoch()).count()) / 1000.0;
}

_______________SD_END_BASIC_NAMESPACE________________