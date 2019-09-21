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

/*! \file      Bitmap.h
 *  \brief     The introduction of Bitmap.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/20
 *  \copyright MIT License.
 */
#include <sstream>
#include <cstring>

#include "SDEngineCommonFunction.h"
#include "LogManager.h"
#include "Bitmap.h"

using SDE::Basic::LogManager;
using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

std::string BitmapConfig::BitmapConfigNames[BitmapConfig_MAX_DEFINE_VALUE] = {
    "Android_GrayLevel",
    "Android_RGB",
    "Android_RGBA",
    "DevIL",
    "Android_A8",
    "Android_RGBA_4444",
    "Android_RGB_565",
    "Android_RGBA_F16",
    "Android_HARDWARE"
};

Bitmap::Bitmap(const ObjectName &i_object_name)
: Object(i_object_name)
, m_bitmap(nullptr)
, m_width(0)
, m_height(0)
, m_num_of_c(0)
, m_v_type(BitmapPixelDataType_MAX_DEFINE_VALUE)
{
}

Bitmap::Bitmap(const ObjectName &i_object_name, const BitmapConfig &i_conf, Size_ui32 i_width, Size_ui32 i_height, const ImageBufferAddr i_data, ImageBufferSize i_size, int i_n_of_c, const BitmapPixelValueType &i_v_type)
: Object(i_object_name)
, m_bitmap(nullptr)
, m_width(0)
, m_height(0)
, m_num_of_c(0)
, m_v_type(BitmapPixelDataType_MAX_DEFINE_VALUE)
{
	SetBitmap(i_conf, i_width, i_height, i_data, i_size, i_n_of_c, i_v_type);
}

Bitmap::~Bitmap()
{
	ClearBitmap();
}

//--- Protected
void Bitmap::DecideNumberOfChannel(int i_n_of_c)
{
	if (m_config == BitmapConfig_GRAYLEVEL) {
		m_num_of_c = 1;
		//v_type setted on outside.
	}
	else if (m_config == BitmapConfig_RGBA_F16) {
		m_num_of_c = 4;
	}
	else if (m_config == BitmapConfig_RGBA_4444) {
		m_num_of_c = 1;
		m_v_type = BitmapPixelDataType_UNSIGNED_SHORT_4_4_4_4;
	}
	else if (m_config == BitmapConfig_RGB_565) {
		m_num_of_c = 1;
		m_v_type = BitmapPixelDataType_UNSIGNED_SHORT_5_6_5;
	}
	else if (m_config == BitmapConfig_RGBA_8888) {
		m_num_of_c = 4;
		m_v_type = BitmapPixelDataType_UNSIGNED_BYTE;
	}
	else if (m_config == BitmapConfig_RGB_888) {
		m_num_of_c = 3;
		m_v_type = BitmapPixelDataType_UNSIGNED_BYTE;
	}
	else if (m_config == BitmapConfig_ALPHA_8) {
		m_num_of_c = 1;
		m_v_type = BitmapPixelDataType_UNSIGNED_BYTE;
	}
	else if (m_config == BitmapConfig_DevIL || m_config == BitmapConfig_HARDWARE) {
		m_num_of_c   = i_n_of_c;
		//v_type setted on outside.
	}
}

void Bitmap::ClearBitmap()
{
	m_config      = BitmapConfig_UNKNOWN;
	m_buffer_size = 0;
	m_width       = 0;
	m_height      = 0;
	m_num_of_c    = 0;
	if (m_bitmap != nullptr) {
		delete[] m_bitmap;
		m_bitmap = nullptr;
	}
}
//--- Public
std::string Bitmap::ToString() const
{
    std::stringstream ss;
    ss << StringFormat("Bitmap[%s]:W(%u)H(%u)Bsize(%u)C(%u)", m_object_name.c_str(),
        m_width, m_height, m_buffer_size, m_num_of_c);
    return ss.str();
}

bool Bitmap::SetBitmap(const BitmapConfig &i_conf, Size_ui32 i_width, Size_ui32 i_height, const ImageBufferAddr i_data, ImageBufferSize i_size, int i_n_of_c, const BitmapPixelValueType &i_v_type)
{
	if (i_conf != BitmapConfig_UNKNOWN && i_data != nullptr) {
		ClearBitmap();
		//
		m_v_type      = i_v_type;
		m_config      = i_conf;
		m_width       = i_width;
		m_height      = i_height;
		m_buffer_size = i_size;
		DecideNumberOfChannel(i_n_of_c);
		m_bitmap = new unsigned char[m_buffer_size];
		std::memcpy(m_bitmap, i_data, m_buffer_size);
		return true;
	}
	else {
		SDLOGE("Unsupport bitmap type : %d", m_config.m_enum);
		return false;
	}
}

______________SD_END_GRAPHICS_NAMESPACE______________