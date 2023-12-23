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

/*! \file      ImageLoader.h
 *  \brief     The introduction of ImageLoader.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/20
 *  \copyright MIT License.
 */
#define STB_IMAGE_IMPLEMENTATION

#include "ImageLoader.h"

#include <stb/stb_image.h>

#include "LogManager.h"
#include "FileResourceRequester.h"


using SDE::Basic::FileData;
using SDE::Basic::LogManager;
using SDE::Basic::FileResourceRequester;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_SINGLETON_DECLARATION_IMPL(ImageLoader);

ImageLoader::ImageLoader()
{
    SD_SINGLETON_DECLARATION_REGISTER
}

ImageLoader::~ImageLoader()
{

}

void ImageLoader::Initialize()
{
}

BitmapStrongReferenceObject ImageLoader::ReadBitmap(const FilePathString &i_fp) const
{
    FileData file_data;
    FileResourceRequester::GetRef().AskFile(i_fp, file_data);
    return ReadBitmap(i_fp, file_data.GetDataUCharAddr(), file_data.GetSize());
}

BitmapStrongReferenceObject ImageLoader::ReadBitmap(const ObjectName &i_name, const UBytePtr i_file_buffer, size_t i_file_size) const
{
    int width, height, n_of_c;
    stbi_uc *image_ptr = stbi_load_from_memory(i_file_buffer, static_cast<int>(i_file_size), &width, &height, &n_of_c, 4);
    size_t image_size = static_cast<uint32_t>(width) * static_cast<uint32_t>(height) * 4;
    BitmapStrongReferenceObject bitmap = new Bitmap(i_name, BitmapConfig_STB, width, height, image_ptr, image_size, 4, BitmapPixelDataType_UNSIGNED_BYTE);
    stbi_image_free(image_ptr);
    return bitmap;
}

______________SD_END_GRAPHICS_NAMESPACE______________