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

#include <IL/il.h>
#include <IL/ilu.h>

#include "LogManager.h"
#include "FileSystemManager.h"
#include "WindowsImageLoader.h"

using SDE::Basic::FileSystemManager;
using SDE::Basic::LogManager;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics -------------------------
namespace Graphics
{
	
WindowsImageLoader::WindowsImageLoader()
{
}

WindowsImageLoader::~WindowsImageLoader()
{
}

void WindowsImageLoader::Initialize()
{
	ilInit();
	iluInit();
}

BitmapStrongReferenceObject WindowsImageLoader::LoadBitmap(const FilePathString &i_fp) const
{
    BitmapStrongReferenceObject result;

	FilePathString fullpath = FileSystemManager::GetRef().ReturnFilePossibleLocation(i_fp);

	if (fullpath.empty() == true) {
		SDLOGE("file [%s] doesn't exist.", fullpath.c_str());
	}
	else {
		ILboolean success;
		ILuint  src_image = 0;
		Size_ui32 width, height;
		size_t image_size;
		ILubyte *image_data_ptr = nullptr;
		BitmapPixelValueType img_pv_type;
		int temp_n_of_c;
		int pixel_type;
		//1. load source image
		//--- load source
		src_image = ilGenImage();
		ilBindImage(src_image);
		std::wstring fullpathw = SD_ADT_OS_STRCSTR(fullpath);
		success = ilLoadImage(fullpathw.c_str());
		if (success == IL_TRUE) {
			width = static_cast<size_t>(ilGetInteger(IL_IMAGE_WIDTH));
			height = static_cast<size_t>(ilGetInteger(IL_IMAGE_HEIGHT));
			temp_n_of_c = ilGetInteger(IL_IMAGE_CHANNELS);
			pixel_type = ilGetInteger(IL_IMAGE_TYPE);

			//--- pixel format.
			if (pixel_type == IL_UNSIGNED_BYTE) {
				img_pv_type = BitmapPixelDataType_UNSIGNED_BYTE;
			}
			else if (pixel_type == IL_BYTE) {
				img_pv_type = BitmapPixelDataType_BYTE;
			}
			else if (pixel_type == IL_UNSIGNED_SHORT) {
				img_pv_type = BitmapPixelDataType_UNSIGNED_SHORT;
			}
			else if (pixel_type == IL_SHORT) {
				img_pv_type = BitmapPixelDataType_SHORT;
			}
			else if (pixel_type == IL_UNSIGNED_INT) {
				img_pv_type = BitmapPixelDataType_UNSIGNED_INT;
			}
			else if (pixel_type == IL_INT) {
				img_pv_type = BitmapPixelDataType_INT;
			}
			else if (pixel_type == IL_FLOAT) {
				img_pv_type = BitmapPixelDataType_FLOAT;
			}
			else if (pixel_type == IL_DOUBLE) {
                SDLOGW("Image [%s] exist non-supported double pixel type!!!", fullpath.c_str());
			}
			else {
                SDLOGW("Image [%s] exists illegal pixel type!!! (pixel_type = %d)", fullpath.c_str(), pixel_type);
			}

			if (img_pv_type != BitmapPixelDataType_MAX_DEFINE_VALUE) {
				//--- format
				if (temp_n_of_c == 3) {//RGB
					success = ilConvertImage(IL_RGB, pixel_type);
					if (success == IL_TRUE) {
						image_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
						image_data_ptr = ilGetData();
					}
					else {
                        SDLOGW("Image [%s] can't convert to RGB!!!", fullpath.c_str());
						result = false;
					}
				}
				else if (temp_n_of_c == 4) {//RGBA
					success = ilConvertImage(IL_RGBA, pixel_type);
					if (success == IL_TRUE) {
						image_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
						image_data_ptr = ilGetData();
					}
					else {
                        SDLOGW("Image [%s] can't convert to GRBA!!!", fullpath.c_str());
					}
				}
				else if (temp_n_of_c == 1) { //Gray
					success = ilConvertImage(IL_LUMINANCE, pixel_type);
					if (success == IL_TRUE) {
						image_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
						image_data_ptr = ilGetData();
					}
					else {
                        SDLOGW("Image [%s] can't convert to gray!!!", fullpath.c_str());
						result = false;
					}
				}

				result = new Bitmap(i_fp, BitmapConfig_DevIL,
					width, height, image_data_ptr, image_size, temp_n_of_c,
					img_pv_type);
			}
			else {
                SDLOGW("Image invalid format in file [%s] !!!", fullpath.c_str());
			}
		}
		else {
            SDLOGW("Image [%s] can't be loaded!!!", fullpath.c_str());
		}

		ilDeleteImage(src_image);
	}

	return result;
}

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}