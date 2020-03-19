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

/*! \file      AssetResourceManger.h
 *  \brief     The introduction of AssetResourceManger.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/03/17
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "FileData.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class AssetResourceManager
 *  \brief In our system, AssetResourceManager is used to manage compressed resource in zip file or
 *         asset in android os. 
 */
class SDENGINE_CLASS AssetResourceManager
{
public:
    SD_SINGLETON_DECLARATION(AssetResourceManager);
public:
    /*! \fn explicit AssetResourceManager();
     *  \brief The constructor of AssetResourceManager Class.
     */
    explicit AssetResourceManager();

    /*! \fn virtual ~AssetResourceManager();
     *  \brief The destructor of AssetResourceManager Class.
     */
    virtual ~AssetResourceManager();
public:
    virtual void Initialize() = 0;
    virtual bool LoadAsset(const FilePathString &i_asset_fn, FileData &io_file) = 0;
};

_______________SD_END_BASIC_NAMESPACE________________