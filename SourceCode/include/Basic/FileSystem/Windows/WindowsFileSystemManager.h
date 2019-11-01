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

/*! \file      WindowsFileSystemManager.h
 *  \brief     The introduction of FileSystemManager.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/04
 *  \copyright MIT License.
 */

#pragma once

#include "FileSystemManager.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class WindowsFileSystemManager
 *  \brief FileSystemManager in Windows OS.
 */
class SDENGINE_CLASS WindowsFileSystemManager : public FileSystemManager
{
public:
    /*! \fn WindowsFileSystemManager();
     *  \brief Constructor of WindowsFileSystemManager.
     */
    WindowsFileSystemManager();

    /*! \fn virtual ~WindowsFileSystemManager();
     *  \brief Destructor of WindowsFileSystemManager.
     */
    virtual ~WindowsFileSystemManager();
public:
    /*! \fn void Initialize() override;
     *  \brief Initialize manager. Get current work directory.
     */
    void Initialize() override;
    
    /*! \fn bool OpenFile(const FilePathString &i_location, FileData &io_file) override;
     *  \param [in] i_location file location.
     *  \param [inout] io_file file data for keeping content of opened file.
     *  \brief Load file in i_location and keep it at io_file. If the i_location is absolute path,
     *         we will open directly. Otherwise, we will try to visit all relative dir to open file.
     */
    bool OpenFile(const FilePathString &i_location, FileData &io_file) override;
    
    /*! \fn void SaveFile(const FilePathString &i_location, const FileData &i_file) override;
     *  \param [in] i_location file location.
     *  \param [in] i_file file content.
     *  \brief Save file at i_location.
     */
    void SaveFile(const FilePathString &i_location, const FileData &i_file) override;
    
    /*! \fn FilePathString ReturnFilePossibleLocation(const FilePathString &i_fn) override;
     *  \param [in] i_fn file name.
     *  \brief Find out the relative loaction that the i_fn is in. Return empty string \n
     *         if the i_fn is not in any relative directory.
     */
    FilePathString ReturnFilePossibleLocation(const FilePathString &i_fn) override;
    
    /*! \fn FilePathString GetDirOfThisFile(const FilePathString &i_location) override;
     *  \param [in] i_location file location.
     *  \brief Return the directory of the file location.
     */
    FilePathString GetDirOfThisFile(const FilePathString &i_location) override;
    
    /*! \fn FilePathString GetFirstFileNameInTargetDir(const FilePathString &i_dir, const FilePathString &i_ext_name, bool i_is_recursive = false) override;
     *  \param [in] i_dir target dir.
     *  \param [in] i_ext_name ext filename.
     *  \param [in] i_is_recursive find first file in sub directory if we can't file any this in target directory.
     *  \brief Return the first file with extension name in i_dir.
     */
    FilePathString GetFirstFileNameInTargetDir(const FilePathString &i_dir, const FilePathString &i_ext_name, bool i_is_recursive = false) override;
    
    /*! \fn bool IsExistedFileOrPath(const FilePathString &i_location) override;
     *  \param [in] i_location target location.
     *  \brief Return the i_location is existed entry or not.
     */
    bool IsExistedFileOrPath(const FilePathString &i_location) override;
    
    /*! \fn bool IsExistedFile(const FilePathString &i_location) override;
     *  \param [in] i_location target location.
     *  \brief Return the i_location is exist file or not.
     */
    bool IsExistedFile(const FilePathString &i_location) override;
    
    /*! \fn int CreateDir(const FilePathString &i_location) override;
     *  \param [in] i_location target location.
     *  \brief Create directory at i_location and then return error code.
     */
    int CreateDir(const FilePathString &i_location) override;
    
    /*! \fn int DeleteDir(const FilePathString &i_dirpath) override;
     *  \param [in] i_location target location.
     *  \brief Delete directory at i_location and then return error code.
     */
    int DeleteDir(const FilePathString &i_location) override;
    
    /*! \fn int RenameFile(const FilePathString &i_src_fn, const FilePathString &i_new_fn) override;
     *  \param [in] i_src_fn source filename.
     *  \param [in] i_new_fn new filename.
     *  \brief Rename file.
     */
    int RenameFile(const FilePathString &i_src_fn, const FilePathString &i_new_fn) override;
    
    /*! \fn int CopyFileTo(const FilePathString &i_src_location, const FilePathString &i_dst_location) override;
     *  \param [in] i_src_location source location.
     *  \param [in] i_dst_location destinated location.
     *  \brief copy file from i_src_location to i_dst_location and then return error.
     */
    int CopyFileTo(const FilePathString &i_src_location, const FilePathString &i_dst_location) override;
    
    /*! \fn int DeleteTargetFile(const FilePathString &i_location) override;
     *  \param [in] i_location target location.
     *  \brief delete file at i_location and then return error. Implement according OS.
     */
    int DeleteTargetFile(const FilePathString &i_location) override;
    
    /*! \fn int MoveFileTo(const FilePathString &i_from, const FilePathString &i_to) override;
     *  \param [in] i_from source location.
     *  \param [in] i_to destinated location.
     *  \brief move file from i_from to i_to. Implement according OS.
     */
    int MoveFileTo(const FilePathString &i_from, const FilePathString &i_to) override;

    /*! \fn void VisitDir(const FilePathString &i_dir, const EntryCallback &i_file_callback = nullptr, const EntryCallback &i_dir_callback = nullptr, bool i_is_recursive = false, int i_level = 0) override;
     *  \param [in] i_dir target directory.
     *  \param [in] i_file_callback the callback called if current entry is file.
     *  \param [in] i_dir_callback the callback called if current entry is directory.
     *  \param [in] i_is_recursive does need to visit child directory.
     *  \param [in] i_level the number represent how many level current directory under the target dir.
     *  \brief Visit target directory.
     */
    void VisitDir(const FilePathString &i_dir, const EntryCallback &i_file_callback = nullptr, const EntryCallback &i_dir_callback = nullptr, bool i_is_recursive = false, int i_level = 0) override;
    
    /*! \fn void DeleteAllFilesInDir(const FilePathString &i_dir, bool i_del_target_dir = false, bool i_delete_sub = false) override;
     *  \param [in] i_dir target directory.
     *  \param [in] i_del_target_dir flag for deleting target dir or not.
     *  \param [in] i_delete_sub delete sub dir or not.
     *  \brief delete target directory.
     */
    void DeleteAllFilesInDir(const FilePathString &i_dir, bool i_del_target_dir = false, bool i_delete_sub = false) override;
    
    /*! \fn FilePathString GetExeFilePath() override;
     *  \brief return the location of this executable file.
     */
    FilePathString GetExeFilePath() override;
};

_______________SD_END_BASIC_NAMESPACE________________