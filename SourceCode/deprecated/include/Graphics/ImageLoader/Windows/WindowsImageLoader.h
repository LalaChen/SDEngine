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

/*! \file      WindowsImageLoader.h
 *  \brief     Introduce of class WindowsImageLoader.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/20
 *  \copyright MIT License.
 */

 //------- OS Platform Only -------
#if defined(_WIN32) || defined(_WIN64) 

#pragma once

#include "ImageLoader.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class WindowsImageLoader
 *  \brief In our system, WindowsImageLoader is used at windows OS and based on DevIL. 
 */
class SDENGINE_CLASS WindowsImageLoader : public ImageLoader
{
public:
    /*! \fn explicit WindowsImageLoader();
     *  \brief Constructor of WindowsImageLoader.
     */
	explicit WindowsImageLoader();

    /*! \fn explicit ~WindowsImageLoader();
     *  \brief Destructor of WindowsImageLoader.
     */
	virtual ~WindowsImageLoader();
public:
    /*! \fn virtual void Initialize() = 0;
     *  \brief Initialize image loader.
     */
    void Initialize() override;
public:
    /*! \fn virtual BitmapStrongReferenceObject LoadBitmap(const FilePathString &i_fp) const = 0;
     *  \param [in] i_fp File path of image data.
     *  \brief Load data. Please note that we will convert RGB to RGBA automatically.
     */
    BitmapStrongReferenceObject LoadBitmap(const FilePathString &i_fp) const override;
};

______________SD_END_GRAPHICS_NAMESPACE______________

#endif