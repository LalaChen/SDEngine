/* ==============  SD Engine License ==============
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

/*! \file      SDEngineCommonFunction.h
 *  \brief     SDEngineCommonFunction collects a lot of useful functions about time or string composition.
 *             Those function will be used widely in SDEngine and others side project made by me.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/01
 *  \copyright MIT License.
 */

#pragma once

#include <cstdarg>  // For va_start, etc.
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>

#include "SDEngineCommonType.h"
#include "SDEngineMacro.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \fn SDENGINE_API std::string StringFormat(const char *i_fmt_str, ...);
    \param [in] i_fmt_str; The format string.
    \brief Function StringFormat is used to composed string by some variables and return the result. It's just like sprintf. 
	       The usage : std::string str = string_format("Number%d".int_var);
*/
SDENGINE_API std::string StringFormat(const char *i_fmt_str, ...);

/*! \fn SDENGINE_API std::wstring WStringFormat(const char *i_fmt_str, ...);
	\param [in] i_fmt_str The format string.
    \brief Function WStringFormat is used to composed string by some variables. It's just like sprintf and then output a wide char string.
           The usage : std::wstring str = wstring_format("Number%d".int_var);
*/
SDENGINE_API std::wstring WStringFormat(const char *i_fmt_str, ...);

/*! \fn SDENGINE_API void StringSplit(const std::string &i_s, char i_delim, std::vector<std::string> &io_elems)
    \brief Function StringSplit is used to split string to many toke by symbol i_delim.
	       Please notice that \\r\\n or \\n will be seen as i_delim.
           The usage : StringSplit(\"a,b,c\",\',\',tokens);
    \param [in] i_s The source string.
	\param [in] _delim The delimiter.
	\param [inout] io_elems The tokens storage.
*/
SDENGINE_API void StringSplit(const std::string &i_s, char i_delim, std::vector<std::string> &io_elems);

/*! \fn SDENGINE_API std::vector<std::string> StringSplit(const std::string &i_s, char i_delim)
    \brief Function StringSplit is used to split string to many toke by symbol i_delim and return token storages.
	       Please notice that \\r\\n or \\n will be seen as i_delim.
           The usage : tokens = StringSplit(\"a,b,c\",\',\');
    \param [in] i_s The source string.
	\param [in] i_delim The delimiter.
*/
SDENGINE_API std::vector<std::string> StringSplit(const std::string &i_s, char i_delim);

/*! \fn SDENGINE_API void SpliteTargetFileToPathAndName(const std::string& i_path, std::string &io_path, std::string &io_name)
    \brief Function SpliteTargetFileToPathAndName is used to separate file name and path by file last of '\\' or '/'.
    \param [in] i_path The resource file path.
	\param [inout] io_path The ref to save path result.
	\param [inout] io_name The ref to save name result.
*/
SDENGINE_API void SpliteTargetFileToPathAndName(const std::string &i_path, std::string &io_path, std::string &io_name);

/*! \fn SDENGINE_API std::wstring StringToWString(const std::string& i_str, CodePageID i_code_page_id = 0)
    \brief Function string_to_wstring is used to convert std::string to std::wstring with codepage.
	       CodePage 0 is represented system default.
    \param [in] i_str The target string
	\param [in] i_code_page_id Target CodePage(default 0 (system default)).
*/
SDENGINE_API std::wstring StringToWString(const std::string &i_str, CodePageID i_code_page_id = 0);

/*! \fn SDENGINE_API std::string WStringToString(const std::wstring& i_wstr, CodePageID i_code_page_id = 0)
    \brief Function wstring_to_string is used to convert std::wstring to std::string with codepage.
	       CodePage 0 is represented system default.
    \param [in] i_wstr The target string
	\param [in] i_code_page_id Target CodePage(default 0 (system default)).
*/
SDENGINE_API std::string WStringToString(const std::wstring &i_wstr, CodePageID i_code_page_id = 0);

/*! \fn SDENGINE_API std::string GetLocalTimeByFormat(time_t i_time, const std::string &i_format)
    \brief Convert time to string by specific format.(use strftime)
	       Usage :GetLocalTimeByFormat(time(0), \"%Y-%m-%d %H:%M:%S\");
    \param [in] i_time a time long integer time_t
	\param [in] i_format format
*/
SDENGINE_API std::string GetLocalTimeByFormat(time_t i_time, const std::string &i_format);

/*! \fn SDENGINE_API std::string GetLocalTimeYMDHMS(time_t i_time)
    \brief Convert time to string by YYYY/MM/DD HH:MM:SS
	       Usage : GetLocalTimeYMDHMS(time(0));
    \param i_time    a time long integer time_t
*/
SDENGINE_API std::string GetLocalTimeYMDHMS(time_t i_time);

/*! \fn SDENGINE_API std::string GetLocalTimeYMDHMSForFile(time_t i_time)
    \brief Convert time to string by YYYY-MM-DD-HH-MM-SS
	       Usage : GetLocalTimeYMDHMSForFile(time(0));
    \param i_time a time long integer time_t
*/
SDENGINE_API std::string GetLocalTimeYMDHMSForFile(time_t i_time);

/*! \fn SDENGINE_API std::string GetCurrentLocalTimeByFormat(const std::string &i_format)
    \brief Convert current time to string by specific format.(use strftime)
	       Usage : GetLocalTimeYMDHMSForFile(time(0));
    \param i_format format
*/
SDENGINE_API std::string GetCurrentLocalTimeByFormat(const std::string &i_format);

/*! \fn SDENGINE_API std::string GetCurrentLocalTimeYMDHMS()
\brief Convert current time to string by YYYY/MM/DD HH:MM:SS
*/
SDENGINE_API std::string GetCurrentLocalTimeYMDHMS();

/*! \fn SDENGINE_API std::string GetCurrentLocalTimeYMDHMSForFile()
    \brief Convert current time to string by YYYY-MM-DD-HH-MM-SS
*/
SDENGINE_API std::string GetCurrentLocalTimeYMDHMSForFile();

/*! \fn SDENGINE_API time_t ConvertTMToTimeT(const tm &i_monent)
    \brief Convert struct tm to time_t.
	\param i_monent is tm struct.
*/
SDENGINE_API time_t ConvertTMToTimeT(const tm &i_monent);

/*! \fn SDENGINE_API std::string ReplaceIllegalCharOfFile(const std::string &i_str, const char i_replace_symbol = ' ')
    \brief replace all illegal character in file path i_str.
	       For rule of function name, illegal chars are \\ / : ! * ? < > |
	\param i_str is file path.
	\param i_replace_symbol is the char be used to replace illegal char.
*/
SDENGINE_API std::string ReplaceIllegalCharOfFile(const std::string &i_str, const char i_replace_symbol = ' ');

}
//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------