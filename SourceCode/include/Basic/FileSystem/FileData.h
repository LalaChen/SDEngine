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

/*! \file      FileData.h
 *  \brief     The introduction of FileData.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class FileData
    For save the content of file.
*/
class SDENGINE_CLASS FileData
{
public:
    /*! \fn FileData();
        \brief Constructor of FileData.
    */
    FileData();

    /*! \fn FileData(const FileData &i_src);
        \brief Copy constructor of FileData.
    */
    FileData(const FileData &i_src);

    /*! \fn ~FileData();
        \brief Destructor of FileData.
    */
    ~FileData();
public:
    /*! \fn void SetFileData(const std::vector<UByte> &i_src);
        \param [in] i_src file content.
        \brief Set file content to this data.
    */
    void SetFileData(const std::vector<UByte> &i_src);

    /*! \fn void SetFileData(const std::string &i_src);
        \param [in] i_src file content.
        \brief Set string content to this data.
    */
    void SetFileData(const std::string &i_src);

    /*! \fn bool IsEmpty() const;
        \brief Set string content to this data.
    */
    bool IsEmpty() const;

    /*! \fn size_t GetSize() const;
        \brief return size of container.
    */
    size_t GetSize() const;

    /*! \fn const char* GetDataCharAddr() const;
        \brief return address with byte ptr of first element of container.
    */
    const BytePtr GetDataCharAddr() const;

    /*! \fn const UBytePtr GetDataUCharAddr() const;
        \brief return address with ubyte ptr of first element of container.
    */
    const UBytePtr GetDataUCharAddr() const;

    /*! \fn std::string GetDataString() const;
        \brief return content with string.
    */
    std::string GetDataString() const;

    /*! \fn std::string GetDataString() const;
        \brief return content with wstring.
    */
    std::wstring GetDataWString() const;
public:
    /*! \var std::vector<UByte> m_file_content;
        \brief file content.
    */
    std::vector<UByte> m_file_content;
};

_______________SD_END_BASIC_NAMESPACE________________