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

#include "FileSystemManager.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_SINGLETON_DECLARATION_IMPL(FileSystemManager);

//------------ File System Manager  ------------
FileSystemManager::FileSystemManager()
{
    SD_SINGLETON_DECLARATION_REGISTER
    //Add default folder.
    AddRelativePath("");
    AddRelativePath("Resources\\");
}

FileSystemManager::~FileSystemManager()
{
}

void FileSystemManager::AddRelativePath(const FilePathString &i_path)
{
    FilePathString adj_path = AdjustFilePath(i_path);
    if (IsExistedRelativePath(adj_path) == false) {
        m_relative_paths.push_back(adj_path);
    }
}

bool FileSystemManager::IsExistedRelativePath(const FilePathString &i_path) const
{
    for (FilePathString path : m_relative_paths) {
        if (path.compare(i_path) == 0) {
            return true;
        }
    }

    return false;
}

//------ Private work function
FilePathString FileSystemManager::AdjustFilePath(const FilePathString &i_path) const
{
    FilePathString result = i_path;

    if (result.empty() == false) {
        while (result[0] == '\\' || result[0] == '/') {
            result = result.substr(1,result.size() - 1);
        }
        
        if (result[result.size() - 1] != '\\' && result[result.size() - 1] != '/') {
            result += '\\';
        }
    }
    return result;

}

_______________SD_END_BASIC_NAMESPACE________________