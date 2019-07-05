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

/*! \file      LogManager.h
 *  \brief     Introduce of class LogManager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/02
 *  \copyright MIT License.
 */

#pragma once

#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#define SD_MAX_WORD_PER_LINE 65536

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \class LogManager
    Class LogManager is used to print log. It's singleton class.
*/
class SDENGINE_CLASS LogManager
{
public:
    /*! \enum LogManager::LogType
     *  \brief Represent the log type.
     */
    enum LogType
    {
        Normal   = 1, /*!< Show log with prefix Normal. */
        Warning  = 2, /*!< Show log with prefix Warning. */
        Error    = 3, /*!< Show log with prefix Error. */
        Internal = 4  /*!< Show log with prefix Internal. It only show when app build by debug configuration.*/
    };
public:
    SD_SINGLETON_DECLARATION(LogManager);
public:
    /*! \fn explicit LogManager();
     *	\brief Constructor of LogManager.
     */
    explicit LogManager();

    /*! \fn virtual ~LogManager();
     *	\brief Destructor of LogManager.
     */
    virtual ~LogManager();
public:
    /*! \fn virtual void Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...) = 0;
     *  \param [in] i_type type of log.
     *  \param [in] i_prefix prefix string.
     *  \param [in] i_log log string.
     *  \brief Compose log string to buffer.
     */
    virtual void Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...) = 0;

    /*! \fn virtual void Log(LogType i_type, const std::string &i_prefix, const char *i_log , va_list i_args) = 0;
     *  \param [in] i_type type of log.
     *  \param [in] i_prefix prefix string.
     *  \param [in] i_log log string.
     *  \param [in] i_args all arguments in log.
     *  \brief Compose log string to buffer.
     */
    virtual void Log(LogType i_type, const std::string &i_prefix, const char *i_log , va_list i_args) = 0;
protected:
    /*! \fn virtual void LogToOutput(LogType i_type) = 0;
     *  \param [in] i_type type of log.
     *  \brief Flush this log to output.
     */
    virtual void LogToOutput(LogType i_type) = 0;
protected:
    /*! \var char m_log_buffer[MAX_WORD_PER_LINE];
     *  \brief store log result.
     */
    char m_log_buffer[SD_MAX_WORD_PER_LINE];

    /*! \var std::string m_prefix;
     *  \brief store log prefix.
     */
    std::string m_prefix;
};

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}


//Log Define
#ifndef __func__
    #define __func__ __FUNCTION__
#endif

#define INFO_PREFIX std::string(__FILE__) + SDE::Basic::StringFormat("(%d):", __LINE__) + std::string(__func__) + std::string("() : ")

/*! \def SDLOGD(log,...)
 *  \brief Print debug log.
 */
#define SDLOGD(log,...) \
    if(SDE::Basic::LogManager::IsNull() == false) \
        SDE::Basic::LogManager::GetRef().Log(SDE::Basic::LogManager::Internal, INFO_PREFIX, log, ##__VA_ARGS__);

/*! \def SDLOGE(log,...)
 *  \brief Print error log.
 */
#define SDLOGE(log,...) \
    if(SDE::Basic::LogManager::IsNull() == false) \
        SDE::Basic::LogManager::GetRef().Log(SDE::Basic::LogManager::Error, INFO_PREFIX , log, ##__VA_ARGS__);

/*! \def SDLOGW(log,...)
 *  \brief Print warning log.
 */
#define SDLOGW(log,...) \
    if(SDE::Basic::LogManager::IsNull() == false) \
        SDE::Basic::LogManager::GetRef().Log(SDE::Basic::LogManager::Warning, INFO_PREFIX , log, ##__VA_ARGS__); 

/*! \def SDLOG(log,...)
 *  \brief Print log.
 */
#define SDLOG(log,...) \
    if(SDE::Basic::LogManager::IsNull() == false) \
        SDE::Basic::LogManager::GetRef().Log(SDE::Basic::LogManager::LogType::Normal, INFO_PREFIX, log,##__VA_ARGS__); 