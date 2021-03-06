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

 /*! \file      ImageLoader.h
  *  \brief     The introduction of ImageLoader.
  *  \author    Kuan-Chih, Chen
  *  \date      2019/07/20
  *  \copyright MIT License.
  */

#pragma once

#include "Bitmap.h"

using SDE::Basic::UBytePtr;
using SDE::Basic::FilePathString;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class ImageLoader
 *  \brief In our system, ImageLoader offer a series interface for file operating. We will \n
 *  implement the real logic according OS.
 */
class SDENGINE_CLASS ImageLoader
{
public:
    /*! \fn explicit ImageLoader();
     *  \brief Constructor of ImageLoader.
     */
	explicit ImageLoader();

    /*! \fn explicit ~ImageLoader();
     *  \brief Destructor of ImageLoader.
     */
	virtual ~ImageLoader();
public:
    SD_SINGLETON_DECLARATION(ImageLoader);
public:
    /*! \fn virtual void Initialize();
     *  \brief Initialize image loader.
     */
    virtual void Initialize();
public:
    /*! \fn virtual BitmapStrongReferenceObject ReadBitmap(const FilePathString &i_fp) const = 0;
     *  \param [in] i_fp File path of image data.
     *  \brief Load data.
     */
	virtual BitmapStrongReferenceObject ReadBitmap(const FilePathString &i_fp) const;

    /*! \fn virtual BitmapStrongReferenceObject ReadBitmap(const FilePathString &i_fp) const = 0;
     *  \param [in] i_fp File path of image data.
     *  \brief Load data.
     */
    virtual BitmapStrongReferenceObject ReadBitmap(const ObjectName &i_name, const UBytePtr i_file_buffer, size_t i_file_size) const;
};

______________SD_END_GRAPHICS_NAMESPACE______________