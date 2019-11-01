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

/*! \file      WindowsLogManager.h
 *  \brief     Introduce of class WindowsLogManager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/02
 *  \copyright MIT License.
 */

#pragma once

#include "LogManager.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class WindowsLogManager
 *  \brief LogManager on Windows OS.
 */
class SDENGINE_CLASS WindowsLogManager : public LogManager
{
public:
    /*! \fn explicit WindowsLogManager();
     *  \brief Constructor of WindowsLogManager.
     */
    explicit WindowsLogManager();

    /*! \fn virtual ~WindowsLogManager();
     *  \brief Destructor of WindowsLogManager.
     */
    virtual ~WindowsLogManager();
public:
    /*! \fn void Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...) override;
     *  \param [in] i_type type of log.
     *  \param [in] i_prefix prefix string.
     *  \param [in] i_log log string.
     *  \brief Compose log string to buffer.
     */
    void Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...) override;

    /*! \fn void Log(LogType i_type, const std::string &i_prefix, const char *i_log, va_list i_args) override;
     *  \param [in] i_type type of log.
     *  \param [in] i_prefix prefix string.
     *  \param [in] i_log log string.
     *  \param [in] i_args all arguments in log.
     *  \brief Compose log string to buffer.
     */
    void Log(LogType i_type, const std::string &i_prefix, const char *i_log, va_list i_args) override;
protected:

    /*! \fn void LogToOutput(LogType i_type) override;
     *  \param [in] i_type type of log.
     *  \brief Flush this log to OutputDebugString.
     */
    void LogToOutput(LogType i_type) override;
};

_______________SD_END_BASIC_NAMESPACE________________