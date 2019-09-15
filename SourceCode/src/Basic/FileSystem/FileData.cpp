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

#include "FileData.h"

______________SD_START_BASIC_NAMESPACE_______________

//---------------- File Data -------------------
FileData::FileData()
{
}

FileData::FileData(const FileData &i_src)
: m_file_content(i_src.m_file_content)
{
}

FileData::~FileData()
{
}

void FileData::SetFileData(const std::vector<unsigned char> &i_src)
{
    m_file_content = i_src;
}

void FileData::SetFileData(const std::string &i_src)
{
    m_file_content.resize(i_src.size());
    for (int i = 0; i < i_src.size(); i++)
    {
        m_file_content[i] = static_cast<unsigned char>(i_src[i]);
    }
}

bool FileData::IsEmpty() const
{
    if (m_file_content.size() == 0) return true;
    else return false;
}

size_t FileData::GetSize() const
{
    return m_file_content.size();
}

const BytePtr FileData::GetDataCharAddr() const
{
    return (const BytePtr)(&m_file_content[0]);
}

const UBytePtr FileData::GetDataUCharAddr() const
{
    return (const UBytePtr)(&m_file_content[0]);
}

std::string FileData::GetDataString() const
{
    std::string content;
    for (int i = 0; i < m_file_content.size(); i++)
        content += m_file_content[i];
    content += '\0';
    return content;
}

std::wstring FileData::GetDataWString() const
{
    return StringToWString(GetDataString());
}

_______________SD_END_BASIC_NAMESPACE________________