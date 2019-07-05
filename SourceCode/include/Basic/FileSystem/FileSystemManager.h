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

/*! \file      FileSystemManager.h
 *  \brief     The introduction of FileSystemManager.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/04
 *  \copyright MIT License.
 */

#pragma once

#include <functional>
#include <fstream>
#include <vector>
#include <string>

#include "FileData.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \class FileSystemManager
 *  In our system, FileSystemManager offer a series interface for file operating. We will \n
 *  implement the real logic according OS.
 */
class SDENGINE_CLASS FileSystemManager
{
public:
    /*! \typedef FilePathString RelativePath
     *  \brief type of relative path.
     */
    typedef FilePathString RelativePath;

    /*! \typedef std::vector<RelativePath> RelativePaths
     *  \brief type of relative path container.
     */
    typedef std::vector<RelativePath> RelativePaths;

    /*! \typedef typedef std::function<void(const FilePathString &i_fp, const FilePathString &i_fn, int i_level) > EntryCallback;
     *  \brief type of visit callback. EntryCallback is means we want to do somthings when we visit file entry(directory or file).
     */
    typedef std::function<void(const FilePathString &i_fp, const FilePathString &i_fn, int i_level) > EntryCallback;

public:
    SD_SINGLETON_DECLARATION(FileSystemManager);
public:
    /*! \fn FileSystemManager();
     *  \brief Constructor of FileSystemManager.
     */
    explicit FileSystemManager();

    /*! \fn virtual ~FileSystemManager();
     *  \brief Destructor of FileSystemManager.
     */
    virtual ~FileSystemManager();
public:
    /*! \fn virtual void Initialize() = 0;
     *  \brief Initialize manager. Implement according OS.
     */
    virtual void Initialize() = 0;

    /*! \fn virtual bool OpenFile(const FilePathString &i_location,  FileData &io_file) = 0;
     *  \param [in] i_location file location.
     *  \param [inout] io_file file data for keeping content of opened file.
     *  \brief Load file in i_location and keep it at io_file. Implement according OS.
     */
    virtual bool OpenFile(const FilePathString &i_location, FileData &io_file) = 0;

    /*! \fn virtual void SaveFile(const FilePathString &i_location, const FileData &i_file) = 0;
     *  \param [in] i_location file location.
     *  \param [in] i_file file content.
     *  \brief Save file at i_location. Implement according OS.
     */
    virtual void SaveFile(const FilePathString &i_location, const FileData &i_file) = 0;

    /*! \fn virtual FilePathString ReturnFilePossibleLocation(const FilePathString &i_fn) = 0;
     *  \param [in] i_fn file name.
     *  \brief Find out the relative loaction that the i_fn is in. Implement according OS.
     */
    virtual FilePathString ReturnFilePossibleLocation(const FilePathString &i_fn) = 0;

    /*! \fn virtual FilePathString GetDirOfThisFile(const FilePathString &i_location) = 0;
     *  \param [in] i_location file location.
     *  \brief Return the directory of the file location. Implement according OS.
     */
    virtual FilePathString GetDirOfThisFile(const FilePathString &i_location) = 0;

    /*! \fn virtual FilePathString GetFirstFileNameInTargetDir(const FilePathString &i_start_dir, const FilePathString &i_ext_name, bool i_is_recursive = false) = 0;
     *  \param [in] i_dir target dir.
     *  \param [in] i_ext_name ext filename.
     *  \param [in] i_is_recursive find first file in sub directory if we can't file any this in target directory.
     *  \brief Return the first file with extension name in i_dir. Implement according OS.
     */
    virtual FilePathString GetFirstFileNameInTargetDir(const FilePathString &i_dir, const FilePathString &i_ext_name, bool i_is_recursive = false) = 0;
    
    /*! \fn virtual bool IsExistedFileOrPath(const FilePathString &i_location) = 0;
     *  \param [in] i_location target location.
     *  \brief Return the i_location is existed entry or not. Implement according OS.
     */
    virtual bool IsExistedFileOrPath(const FilePathString &i_location) = 0;

    /*! \fn virtual bool IsExistedFile(const FilePathString &i_location) = 0;
     *  \param [in] i_location target location.
     *  \brief Return the i_location is exist file or not. Implement according OS.
     */
    virtual bool IsExistedFile(const FilePathString &i_location) = 0;

    /*! \fn virtual int CreateDir(const FilePathString &i_location) = 0;
     *  \param [in] i_location target location.
     *  \brief Create directory at i_location and then return error code. Implement according OS.
     */
    virtual int CreateDir(const FilePathString &i_location) = 0;

    /*! \fn virtual int DeleteDir(const FilePathString &i_location) = 0;
     *  \param [in] i_location target location.
     *  \brief Delete directory at i_location and then return error code. Implement according OS.
     */
    virtual int DeleteDir(const FilePathString &i_location) = 0;

    /*! \fn virtual int RenameFile(const FilePathString &i_src_fn, const FilePathString &i_new_fn) = 0;
     *  \param [in] i_src_fn source filename.
     *  \param [in] i_new_fn new filename.
     *  \brief rename file. Implement according OS.
     */
    virtual int RenameFile(const FilePathString &i_src_fn, const FilePathString &i_new_fn) = 0;

    /*! \fn virtual int CopyFileTo(const FilePathString &i_src_location, const FilePathString &i_dst_location) = 0;
     *  \param [in] i_src_location source location.
     *  \param [in] i_dst_location destinated location.
     *  \brief copy file from i_src_location to i_dst_location and then return error. Implement according OS.
     */
    virtual int CopyFileTo(const FilePathString &i_src_location, const FilePathString &i_dst_location) = 0;

    /*! \fn virtual int DeleteTargetFile(const FilePathString &i_location) = 0;
     *  \param [in] i_location target location.
     *  \brief delete file at i_location and then return error. Implement according OS.
     */
    virtual int DeleteTargetFile(const FilePathString &i_location) = 0;

    /*! \fn virtual int MoveFileTo(const FilePathString &i_from, const FilePathString &i_to) = 0;
     *  \param [in] i_from source location.
     *  \param [in] i_to destinated location.
     *  \brief move file from i_from to i_to. Implement according OS.
     */
    virtual int MoveFileTo(const FilePathString &i_from, const FilePathString &i_to) = 0;

    /*! \fn virtual void VisitDir(const FilePathString &i_dir, const EntryCallback &i_file_callback = nullptr, const EntryCallback &i_dir_callback = nullptr, bool i_is_recursive = false, int i_level = 0) = 0;
     *  \param [in] i_dir target directory.
     *  \param [in] i_file_callback the callback called if current entry is file.
     *  \param [in] i_dir_callback the callback called if current entry is directory.
     *  \param [in] i_is_recursive does need to visit child directory.
     *  \param [in] i_level the number represent how many level current directory under the target dir.
     *  \brief Visit target directory.
     */
    virtual void VisitDir(const FilePathString &i_dir, const EntryCallback &i_file_callback = nullptr, const EntryCallback &i_dir_callback = nullptr, bool i_is_recursive = false, int i_level = 0) = 0;
    
    /*! \fn virtual void DeleteAllFilesInDir(const FilePathString &i_dir, bool i_delete_target_dir = false, bool i_delete_sub = false) = 0;
     * \param [in] i_dir target directory.
     * \param [in] i_del_target_dir flag for deleting target dir or not.
     * \param [in] i_delete_sub delete sub dir or not.
     * \brief delete target directory.
     */
    virtual void DeleteAllFilesInDir(const FilePathString &i_dir, bool i_del_target_dir = false, bool i_delete_sub = false) = 0;

    /*! \fn virtual FilePathString GetExeFilePath() = 0;
     *  \brief return the location of this executable file.
     */
    virtual FilePathString GetExeFilePath() = 0;
public:
    /*! \fn bool IsAbsolutePath(const FilePathString &i_location);
     *  \param [in] i_location location.
     *  \brief Return ture if the i_location is absolute path.
     */
    bool IsAbsolutePath(const FilePathString &i_location);
public:
    /*! \fn void AddRelativePath(const FilePathString &i_path);
     *  \param [in] i_path target directory.
     *  \brief Add relative directory.
     */
    void AddRelativePath(const FilePathString &i_path);

    /*! \fn bool IsExistedRelativePath(const FilePathString &i_path) const;
     *  \param [in] i_path target directory.
     *  \brief Return that the i_path is one of relative paths or not.
     */
    bool IsExistedRelativePath(const FilePathString &i_path) const;
protected:
    /*! \fn RelativePath AdjustFilePath(const FilePathString &i_path) const;
     *  \param [in] i_path target path.
     *  \brief replace anti-slash to slash.
     */
    FilePathString AdjustFilePath(const FilePathString &i_path) const;
protected:

    /*! \var FilePathString m_work_dir_path;
     *  \brief work directory. [CRefGet Attribute]
     */
    SD_DECLARE_ATTRIBUTE_CONSTREF_GET(FilePathString, m_work_dir_path, WorkDir);

    /*! \var RelativePaths m_relative_paths;
     *  \brief container storing all relative paths. [CRefGet Attribute]
     */
    SD_DECLARE_ATTRIBUTE_CONSTREF_GET(RelativePaths, m_relative_paths, RelativePaths);
};

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}