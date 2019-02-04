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

#include <locale>
#include <cstdlib>
#include <cstring>
#ifdef _WIN_PLATFORM_
#include <windows.h>
#endif

#include "SDEngineCommonFunction.h"

//---------------------------- start of namespace SDEngine ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

std::string StringFormat(const char *i_fmt_str, ...)
{
	int final_n, n = static_cast<int>(strlen(i_fmt_str) * 2); /* Reserve two times as much as the length of the fmt_str */
	std::vector<char> buffer;
	va_list ap;
	while (1) {
		buffer.resize(n); /* Wrap the plain char array into the unique_ptr */
		strcpy_s(&buffer[0], n, i_fmt_str);
		va_start(ap, i_fmt_str);
		final_n = vsnprintf(&buffer[0], n, i_fmt_str, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(&buffer[0]);
}

std::wstring WStringFormat(const char *i_fmt_str, ...)
{
	int final_n, n = static_cast<int>(strlen(i_fmt_str) * 2); /* Reserve two times as much as the length of the i_fmt_str */
	std::vector<char> buffer;
	va_list ap;
	while (1) {
		buffer.resize(n); /* Wrap the plain char array into the unique_ptr */
		strcpy_s(&buffer[0], n, i_fmt_str);
		va_start(ap, i_fmt_str);
		final_n = vsnprintf(&buffer[0], n, i_fmt_str, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return StringToWString(std::string(&buffer[0]));
}

void StringSplit(const std::string &i_s, char i_delim, std::vector<std::string> &io_elems)
{
	std::string token;
	int start_pt = 0,current_pt = 0;
	for (int pt = 0; pt < i_s.size();)
	{
		current_pt = pt;
		start_pt = current_pt;
		for (; current_pt < i_s.size(); current_pt++)
		{
			if (i_s[current_pt] == i_delim)
			{
				token = i_s.substr(start_pt, current_pt - start_pt);
				io_elems.push_back(token);
				current_pt++;
				break;
			}
			else if (i_s[current_pt] == '\r')
			{
				token = i_s.substr(start_pt, current_pt - start_pt);
				io_elems.push_back(token);
				if (current_pt < i_s.size())
				{
					current_pt++;
					if (i_s[current_pt] == '\n')
					{
						current_pt++;
					}
				}
				else
				{
					current_pt++;
				}
				break;
			}
			else if (i_s[current_pt] == '\n')
			{
				token = i_s.substr(start_pt, current_pt - start_pt);
				io_elems.push_back(token);
				current_pt++;
				break;
			}
			else if (current_pt + 1 >= i_s.size()) //last one
			{
				token = i_s.substr(start_pt, current_pt + 1 - start_pt);
				io_elems.push_back(token);
				current_pt++;
				break;
			}
		}
		pt = current_pt;
	}
}

std::vector<std::string> StringSplit(const std::string &i_s, char i_delim)
{
	std::vector<std::string> elems;
	StringSplit(i_s, i_delim, elems);
	return elems;
}

void SpliteTargetFileToPathAndName(const std::string &i_path, std::string &io_path, std::string &io_name)
{
	size_t found = i_path.find_last_of("/\\");
	io_path = i_path.substr(0, found);
	io_name = i_path.substr(found + 1);
}

std::wstring StringToWString(const std::string& i_str, CodePageID i_code_page_id)
{
	// Convert an ASCII string to a Unicode String
	std::wstring result;
	size_t result_size = i_str.length() + 1;
	result.resize(result_size);
	result[result_size] = L'\0';
#ifdef _WIN_PLATFORM_
	MultiByteToWideChar(i_code_page_id, 0, i_str.c_str(), -1, &result[0], static_cast<int>(result_size));
#else
#endif
	return result;
}

std::string WStringToString(const std::wstring& i_wstr, CodePageID i_code_page_id)
{
	// Convert a Unicode string to an ASCII string
	std::string result;
	size_t result_size = i_wstr.size() * 2 + 1;
	result.resize(result_size);
#ifdef _WIN_PLATFORM_
	int real_length = WideCharToMultiByte(i_code_page_id, 0, i_wstr.c_str(), -1, &result[0], static_cast<int>(result_size), NULL, NULL);
	if(real_length < 0) real_length = 0;
	else real_length = static_cast<int>(strlen(result.c_str()));
	result.resize(real_length);
#else
#endif
	return result;
}

std::string GetLocalTimeByFormat(time_t i_time, const std::string &i_format)
{
	if (i_time == -1) i_time = 0;
	char time_str[256];
	tm tf;
	localtime_s(&tf, &i_time);
	strftime(time_str, 256, i_format.c_str(), &tf);
	return std::string(time_str);
}

std::string GetLocalTimeYMDHMS(time_t i_time)
{
	if (i_time == -1) i_time = 0;
	char time_str[256];
	tm tf;
	localtime_s(&tf, &i_time);
	strftime(time_str, 256, "%Y-%m-%d %H:%M:%S", &tf);
	return std::string(time_str);
}

std::string GetLocalTimeYMDHMSForFile(time_t i_time)
{
	if (i_time == -1) i_time = 0;
	char time_str[256];
	tm tf;
	localtime_s(&tf, &i_time);
	strftime(time_str, 256, "%Y-%m-%d-%H-%M-%S", &tf);
	return std::string(time_str);
}

std::string GetCurrentLocalTimeByFormat(const std::string &i_format)
{
	time_t cur_time = time(0);
	return GetLocalTimeByFormat(cur_time, i_format);
}

std::string GetCurrentLocalTimeYMDHMS()
{
	time_t cur_time = time(0);
	return GetLocalTimeYMDHMS(cur_time);
}

std::string GetCurrentLocalTimeYMDHMSForFile()
{
	time_t cur_time = time(0);
	return GetLocalTimeYMDHMSForFile(cur_time);
}

time_t ConvertTMToTimeT(const tm &i_monent)
{
	tm t = i_monent;
	return mktime(&t);
}

std::string ReplaceIllegalCharOfFile(const std::string &i_str, const char i_replace_symbol)
{
	std::string result = i_str;
	for (int idx = 0; idx < result.size(); idx++)
	{
		if (result[idx] == '\\' || result[idx] == '/' || result[idx] == ':' || result[idx] == '!' || result[idx] == '*' || result[idx] == '?' || result[idx] == '<' || result[idx] == '>' || result[idx] == '|')
			result[idx] = i_replace_symbol;
	}
	return result;
}

}
//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDEngine ----------------------------