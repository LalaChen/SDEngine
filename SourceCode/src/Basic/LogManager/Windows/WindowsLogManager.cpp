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

//------- OS Platform Only -------
#if defined(_WIN32) || defined(_WIN64) 

#define OUTPUT_TO_FILE

#include "WindowsLogManager.h"

#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <mutex>

______________SD_START_BASIC_NAMESPACE_______________

std::mutex sMutex;
std::ofstream sLogFile;

WindowsLogManager::WindowsLogManager()
{
#if defined(OUTPUT_TO_FILE)
    sLogFile.open("log.txt");
#endif
}

WindowsLogManager::~WindowsLogManager()
{
#if defined(OUTPUT_TO_FILE)
    sLogFile.close();
#endif
}

void WindowsLogManager::Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...)
{
    std::lock_guard<std::mutex> lck(sMutex);
    m_prefix = i_prefix;
    va_list args;
    va_start(args, i_log);
    vsnprintf(m_log_buffer, SD_MAX_WORD_PER_LINE - 3 , i_log, args);
    va_end(args);
    LogToOutput(i_type);
}

void WindowsLogManager::Log(LogType i_type, const std::string &i_prefix, const char *i_log, va_list i_args)
{
    m_prefix = i_prefix;
    vsnprintf(m_log_buffer, SD_MAX_WORD_PER_LINE - 3 , i_log, i_args);
    LogToOutput(i_type);
}

#if !defined(OUTPUT_TO_FILE)
void WindowsLogManager::LogToOutput(LogType i_type)
{
    if (i_type == LogType::Normal) {
        OutputDebugString(SD_ADT_OS_STRCSTR(std::string("[Normal] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
    }
    else if (i_type == LogType::Warning) {
        OutputDebugString(SD_ADT_OS_STRCSTR(std::string("[Warning] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
    }
    else if (i_type == LogType::Internal) {
#if defined(_DEBUG)
        OutputDebugString(SD_ADT_OS_STRCSTR(std::string("[Internal] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
#endif
    }
    else {
        OutputDebugString(SD_ADT_OS_STRCSTR(std::string("[Error] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
    }
}
#else
void WindowsLogManager::LogToOutput(LogType i_type)
{
    if (i_type == LogType::Normal) {
        sLogFile << std::string("[Normal] ") + m_prefix + std::string(m_log_buffer) + std::string("\n");
    }
    else if (i_type == LogType::Warning) {
        sLogFile << std::string("[Warning] ") + m_prefix + std::string(m_log_buffer) + std::string("\n");
    }
    else if (i_type == LogType::Internal) {
#if defined(_DEBUG)
        sLogFile << std::string("[Internal] ") + m_prefix + std::string(m_log_buffer) + std::string("\n");
#endif
    }
    else {
        sLogFile << std::string("[Error] ") + m_prefix + std::string(m_log_buffer) + std::string("\n");
    }
}
#endif

_______________SD_END_BASIC_NAMESPACE________________

#endif