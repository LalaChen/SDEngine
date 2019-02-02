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


#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "WindowsLogManager.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

WindowsLogManager::WindowsLogManager()
{
}

WindowsLogManager::~WindowsLogManager()
{
}

void WindowsLogManager::Log(LogType i_type, const std::string &i_prefix, const char *i_log, ...)
{
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

void WindowsLogManager::LogToOutput(LogType i_type)
{
	if(i_type == LogType::Normal)
	{
		OutputDebugString(LC_ADT_OSLOGSTRCSTR(std::string("[Normal] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
	}
	else if(i_type == LogType::Warning)
	{
		OutputDebugString(LC_ADT_OSLOGSTRCSTR(std::string("[Warning] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
	}
	else if (i_type == LogType::Internal)
	{
#ifdef _DEBUG
		OutputDebugString(LC_ADT_OSLOGSTRCSTR(std::string("[Internal] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
#endif
	}
	else
	{
		OutputDebugString(LC_ADT_OSLOGSTRCSTR(std::string("[Error] ") + m_prefix + std::string(m_log_buffer) + std::string("\n")));
	}
}

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}