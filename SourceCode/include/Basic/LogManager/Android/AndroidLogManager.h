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

/*! \file      AndroidLogManager.h
 *  \brief     Introduce of class AndroidLogManager
 *  \author    Kuan-Chih, Chen
 *  \date      2020/03/15
 *  \copyright MIT License.
 */
#pragma once

#if defined(__ANDROID__) 

#include "LogManager.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class AndroidLogManager
 *  \brief LogManager on Android OS.
 */
class SDENGINE_CLASS AndroidLogManager : public LogManager
{
public:
    /*! \fn explicit AndroidLogManager();
     *  \brief Constructor of AndroidLogManager.
     */
	explicit AndroidLogManager();

    /*! \fn virtual ~AndroidLogManager();
     *  \brief Destructor of AndroidLogManager.
     */
	virtual ~AndroidLogManager();
public:
    /*! \fn void Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...) override;
     *  \param [in] i_type type of log.
     *  \param [in] i_prefix prefix string.
     *  \param [in] i_log log string.
     *  \brief Compose log string to buffer.
     */
	void Log(LogType i_type, const std::string &i_prefix, const char *i_log , ...) override;

    /*! \fn void Log(LogType i_type, const std::string &i_prefix, const char *i_log, va_list i_args) override;
     *  \param [in] i_type type of log.
     *  \param [in] i_prefix prefix string.
     *  \param [in] i_log log string.
     *  \param [in] i_args all arguments in log.
     *  \brief Compose log string to buffer.
     */
	void Log(LogType i_type, const std::string &i_prefix, const char *i_log , va_list i_args) override;
protected:

    /*! \fn void LogToOutput(LogType i_type) override;
     *  \param [in] i_type type of log.
     *  \brief Flush this log to OutputDebugString.
     */
	void LogToOutput(LogType i_type) override;
};

_______________SD_END_BASIC_NAMESPACE________________

#endif
