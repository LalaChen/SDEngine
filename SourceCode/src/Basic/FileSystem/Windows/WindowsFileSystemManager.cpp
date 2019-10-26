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
#include <windows.h>
#include <direct.h>
#include <Shlwapi.h>

#include "LogManager.h"
#include "WindowsFileSystemManager.h"

______________SD_START_BASIC_NAMESPACE_______________

WindowsFileSystemManager::WindowsFileSystemManager()
: FileSystemManager()
{
}

WindowsFileSystemManager::~WindowsFileSystemManager()
{
}

void WindowsFileSystemManager::Initialize()
{
    char current_dir_path[260] = {0};
    //1. get current work dir.
    _getcwd(current_dir_path,260);
    //2. set current dir as work dir.
    m_work_dir_path = AdjustFilePath(current_dir_path);
}

bool WindowsFileSystemManager::OpenFile(const FilePathString &i_location, FileData &io_file)
{
    std::ifstream  dest_file;
    FilePathString file_path;
    std::vector<UByte> file_buffer;
    std::streamoff file_length = 0;

    //check absolute path.
    if (IsAbsolutePath(i_location) == true) {

        dest_file.open(StringToWString(i_location, SD_CODE_PAGE), std::ios_base::binary);

        if (dest_file.is_open() == true) {
            //-- calculate file size
            dest_file.seekg(0,std::ios_base::end);
            file_length = dest_file.tellg();
            dest_file.seekg(0,std::ios_base::beg);
            //-- create buffer
            file_buffer.resize(file_length);
            //-- load file
            if (file_length > 0) dest_file.read((char*)(&file_buffer[0]),file_length);
            //--
            io_file.SetFileData(file_buffer);
            dest_file.close();
            
            return true;
        }
    }
    else {
        for (FilePathString &path : m_relative_paths) {
            file_path = m_work_dir_path + path + i_location;
            dest_file.open(StringToWString(file_path, SD_CODE_PAGE), std::ios_base::binary);
            if (dest_file.is_open() == true) {
                //-- calculate file size
                dest_file.seekg(0,std::ios_base::end);
                file_length = dest_file.tellg();
                dest_file.seekg(0,std::ios_base::beg);
                //-- create buffer
                file_buffer.resize(file_length);
                //-- load file
                dest_file.read((char*)(&file_buffer[0]),file_length);
                //--
                io_file.SetFileData(file_buffer);
                dest_file.close();
            
                return true;
            }
        }
    }
    return false;
}

void WindowsFileSystemManager::SaveFile(const FilePathString &i_location, const FileData &i_file)
{
    std::ofstream file(StringToWString(i_location, SD_CODE_PAGE), std::ios_base::binary);
    if (file.is_open() == true) {
        file.write(i_file.GetDataCharAddr(), i_file.GetSize());
    }
    file.close();
}

FilePathString WindowsFileSystemManager::ReturnFilePossibleLocation(const FilePathString &i_fn)
{
    std::ifstream result;
    FilePathString result_file_path = "";
    FilePathString file_path;
    for (FilePathString &path : m_relative_paths) {
        file_path = m_work_dir_path + path + i_fn;
        result.open(file_path.c_str());
        if (result.is_open() == true) {
            result_file_path = file_path;
            result.close();
            break;
        }
    }

    return result_file_path;
}

FilePathString WindowsFileSystemManager::GetDirOfThisFile(const FilePathString &i_location)
{
    size_t last_slash = i_location.find_last_of("\\");
    if (last_slash != FilePathString::npos) {
        return i_location.substr(0, last_slash);
    }
    last_slash = i_location.find_last_of("/");
    if (last_slash != FilePathString::npos) {
        return i_location.substr(0, last_slash);
    }
    return FilePathString();
}

FilePathString WindowsFileSystemManager::GetFirstFileNameInTargetDir(const FilePathString &i_dir, const FilePathString &i_ext_name, bool i_is_recursive)
{
    WIN32_FIND_DATA find_data;
    HANDLE          find_handle;
    FilePathString  filename, dirname;
    FilePathString  search_dir_name;
    FilePathString  total_path;
    if (i_dir[i_dir.size() - 1] != '\\') {
        search_dir_name = i_dir + "\\*";
    }
    else {
        search_dir_name = i_dir + "*";
    }

    find_handle = FindFirstFile(SD_ADT_OS_STRCSTR(search_dir_name), &find_data);

    if (find_handle != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {//File
                filename = SD_CVT_OS_CHARS_TOSTR(find_data.cFileName);
                //ext file checking.
                if (filename.find(i_ext_name) != filename.npos)
                {
                    filename = i_dir + filename;
                    break;
                }
            }
            else {//Dir
                dirname = SD_CVT_OS_CHARS_TOSTR(find_data.cFileName);
                if (dirname.compare(".") != 0 && dirname.compare("..") != 0 && i_is_recursive == true) {
                    dirname = i_dir + "\\" + dirname + "\\";
                    filename = GetFirstFileNameInTargetDir(dirname, i_ext_name,  i_is_recursive);
                }
            }
        } while (FindNextFile(find_handle, &find_data));
    }

    FindClose(find_handle);

    return filename;
}

bool WindowsFileSystemManager::IsExistedFileOrPath(const FilePathString &i_location)
{
    if (IsAbsolutePath(i_location) == true) {
        return static_cast<bool>(PathFileExists(SD_ADT_OS_STRCSTR(i_location)));
    }
    else {
        for (FilePathString &path : m_relative_paths) {
            FilePathString file_path;
            file_path = m_work_dir_path + path + i_location;
            bool exist = static_cast<bool>(PathFileExists(SD_ADT_OS_STRCSTR(i_location)));
            if (exist == true) return true;
        }
        return false;
    }
}

bool WindowsFileSystemManager::IsExistedFile(const FilePathString &i_location)
{
    std::ifstream dest_file;
    dest_file.open(StringToWString(i_location, SD_CODE_PAGE), std::ios_base::binary);
    if (dest_file.is_open() == false) {
        dest_file.close();
        return false;
    }
    else {
        dest_file.close();
        return true;
    }
}

int WindowsFileSystemManager::CreateDir(const FilePathString &i_location)
{
    CreateDirectory(SD_ADT_OS_STRCSTR(i_location), NULL);
    return GetLastError();
}

int WindowsFileSystemManager::DeleteDir(const FilePathString &i_location)
{
    RemoveDirectory(SD_ADT_OS_STRCSTR(i_location));
    return GetLastError();
}

int WindowsFileSystemManager::RenameFile(const FilePathString &i_src_fn, const FilePathString &i_new_fn)
{
#ifdef _UNICODE
    return _wrename(SD_ADT_OS_STRCSTR(i_src_fn), SD_ADT_OS_STRCSTR(i_new_fn));
#elif UNICODE
    return _wrename(SD_ADT_OS_STRCSTR(i_src_fn), SD_ADT_OS_STRCSTR(i_new_fn));
#else
    return rename(i_src_fn.c_str(), i_new_fn.c_str());
#endif
}

int WindowsFileSystemManager::CopyFileTo(const FilePathString &i_src_location, const FilePathString &i_dst_location)
{
    //If file exist, we don't copy.
    if (IsExistedFileOrPath(i_dst_location) == false) {
        FilePathString temp_name = i_dst_location + ".temp";
        DeleteFile(SD_ADT_OS_STRCSTR(temp_name));
        CopyFile(SD_ADT_OS_STRCSTR(i_src_location), SD_ADT_OS_STRCSTR(temp_name), true);
        RenameFile(temp_name, i_dst_location);
    }
    return GetLastError();
}

int WindowsFileSystemManager::DeleteTargetFile(const FilePathString &i_location)
{
    DeleteFile(SD_ADT_OS_STRCSTR(i_location));
    return GetLastError();
}

int WindowsFileSystemManager::MoveFileTo(const FilePathString &i_from, const FilePathString &i_to)
{
    MoveFile(SD_ADT_OS_STRCSTR(i_from), SD_ADT_OS_STRCSTR(i_to));
    return GetLastError();
}

void WindowsFileSystemManager::VisitDir(const FilePathString &i_dir, const EntryCallback &i_file_callback, const EntryCallback &i_dir_callback, bool i_is_recursive, int i_level)
{
    WIN32_FIND_DATA find_data;
    HANDLE find_handle;
    FilePathString filename, dirname;
    FilePathString search_dir_name;
    FilePathString total_path;
    FilePathString this_dir = i_dir;
    
    for (int c_id = static_cast<int>(this_dir.size() - 1); c_id >= 0; c_id--) {
        if (this_dir[c_id] == '\\' || this_dir[c_id] == '/')
            this_dir.erase(c_id, 1);
        else break;
    }

    search_dir_name = this_dir + "\\*";

    find_handle = FindFirstFile(SD_ADT_OS_STRCSTR(search_dir_name), &find_data);

    if (find_handle != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {//File
                filename = SD_CVT_OS_CHARS_TOSTR(find_data.cFileName);
                if (i_file_callback != nullptr)
                    i_file_callback(this_dir + "\\", filename, i_level);
            }
            else {//Dir
                dirname = SD_CVT_OS_CHARS_TOSTR(find_data.cFileName);
                if (dirname.compare(".") != 0 && dirname.compare("..") != 0 && i_is_recursive == true) {
                    dirname = this_dir + "\\" + dirname + "\\";
                    VisitDir(dirname, i_file_callback, i_dir_callback, i_is_recursive, (i_level+1));

                    if (i_dir_callback != nullptr)
                        i_dir_callback(dirname, FilePathString(""), i_level);
                }
            }
        } while (FindNextFile(find_handle, &find_data));
    }

    FindClose(find_handle);
}

void WindowsFileSystemManager::DeleteAllFilesInDir(const FilePathString &i_dir, bool i_delete_target_dir, bool i_delete_sub)
{
    EntryCallback file_callback = [this, &i_dir](const FilePathString &i_path, const FilePathString &i_name, int i_level)
    {
        FilePathString full_path = AdjustFilePath(i_path) + i_name;
        DeleteTargetFile(full_path);
    };
    
    EntryCallback dir_callback = [this, &i_dir](const FilePathString &i_path, const FilePathString &i_name, int i_level)
    {
        if (i_path.compare(i_dir) != 0) DeleteDir(i_path);
    };

    VisitDir(i_dir, file_callback, dir_callback, i_delete_sub);

    if (i_delete_target_dir == true) {
        DeleteDir(i_dir);
    }
}

FilePathString WindowsFileSystemManager::GetExeFilePath()
{
    SDStrChar buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    FilePathString::size_type pos = SD_CVT_OS_CHARS_TOSTR(buffer).find_last_of("\\/");
    return SD_CVT_OS_CHARS_TOSTR(buffer).substr(0, pos);
}

_______________SD_END_BASIC_NAMESPACE________________