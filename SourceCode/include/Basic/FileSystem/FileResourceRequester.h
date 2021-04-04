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

/*! \file      FileResourceRequester.h
 *  \brief     The introduction of FileResourceRequester.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/03/31
 *  \copyright MIT License.
 */

#pragma once

#include <functional>
#include <fstream>
#include <vector>
#include <string>

#include "FileData.h"

______________SD_START_BASIC_NAMESPACE_______________

enum FilePathTypeEnum
{
    FilePathType_LOCAL_FILE = 0,
    FilePathType_ASSET,
    FilePathType_MAX_DEFINE_VALUE
};

class SDENGINE_CLASS FileResourceRequester
{
public:
    static const std::string sFilePathTypeStrings[FilePathType_MAX_DEFINE_VALUE];
public:
    SD_SINGLETON_DECLARATION(FileResourceRequester);
public:
    /*! \fn explicit FileResourceRequester();
     *  \brief Constructor of FileResourceIntermedinary.
     */
    explicit FileResourceRequester();

    /*! \fn virtual ~FileResourceIntermedinary();
     *  \brief Destructor of FileResourceIntermedinary.
     */
    virtual ~FileResourceRequester();
public:
    /*! \fn virtual bool OpenFile(const FilePathString &i_location,  FileData &io_file) = 0;
     *  \param [in] i_location file location.
     *  \param [inout] io_file file data for keeping content of opened file.
     *  \brief Ask file from FileSystemManager and AssetResourceManager with path i_location
     *         and then put loading result at io_file.
     */
    bool AskFile(const FilePathString &i_location, FileData &io_file);

    FilePathString CatenateFilePath(const FilePathString& i_location, const FilePathString& i_target_dir);
protected:
    FilePathTypeEnum ConvertFilePath(const FilePathString &i_location, FilePathString &io_final_path);
};

_______________SD_END_BASIC_NAMESPACE________________