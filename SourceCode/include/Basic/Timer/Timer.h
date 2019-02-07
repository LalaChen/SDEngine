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
	\brief     Introduce of class Timer
	\author    Kuan-Chih, Chen
	\date      2019/02/04
	\copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \class Timer \n
	In our system, Timer is used to record some infos about time. We will record the program\n
	starting time while this program starting. For each frame, we will cache the previous time and \n
	get current time by GetCurrentTimeByOS. Please note that we need to implement function \n
	GetCurrentTimeByOS for following OS platform. And then we calculate delta time by the current \n
	and prevoius times.
*/
class SDENGINE_CLASS Timer
{
public:
	SINGLETON_DECLARATION(Timer);
public:
	/*! \fn explicit Timer();
		\brief The constructor of Timer Class.
	*/
	explicit Timer();

	/*! \fn virtual ~Timer();
		\brief The destructor of Timer Class.
	*/
	virtual ~Timer();
public:

	/*! \fn void Start();
		\brief Start the timer. This function is used while application lanuching. We will get \n
		       current time by OS and save it as program start time. And then we initialize previous \n
			   and current time by start time.
	*/
	void Start();

	/*! \fn void Update();
		\brief Update the timer. This function is used while application updating. We will record current \n
		       time as previous time and then get current time by OS as currnet time.
	*/
	void Update();

	/*! \fn void End();
		\brief End the timer. This function is used while application is destroyed. We will record current time \n
		       as end time.
	*/
	void End();
public:
	/*! \fn virtual void GetCurrentTimeByOS(double &io_time) = 0;
	    \param [inout] io_time double time variable.(unit : second)
		\brief Get current time by OS function.
	*/
	virtual void GetCurrentTimeByOS(double &io_time) = 0;
protected:
	/*! \var double m_start_time;
		\brief Program start time. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(double, m_start_time, ProgramStartTime);

	/*! \var double m_previous_time;
		\brief Program previous time. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(double, m_previous_time, ProgramPreviousTime);

	/*! \var double m_current_time;
		\brief Program current time. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(double, m_current_time, ProgramCurrentTime);

	/*! \var double m_end_time;
		\brief Program end time. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(double, m_end_time, EndProgramTime);

	/*! \var double m_delta_time;
		\brief The delta time between two frames. [VarGet Attribute]
	*/
	DECLARE_ATTRIBUTE_VAR_GET(double, m_delta_time, ProgramDeltaTime);
};

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}