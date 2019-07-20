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

/*! \file      Bitmap.h
 *  \brief     Introduce of class Bitmap.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/20
 *  \copyright MIT License.
 */

#pragma once

#include <string>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ManagerParam.h"
#include "Object.h"

using SDE::Basic::UByte;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;

//----------------------------- start of namespace SDE --------------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

enum BitmapConfigEnum
{
//-------- Common ----------
    BitmapConfig_GRAYLEVEL = 0,
    BitmapConfig_RGB_888 = 1,
    BitmapConfig_RGBA_8888 = 2,
//---------- IL ------------
    BitmapConfig_DevIL = 3,
//----- Android Bitmap -----
	BitmapConfig_ALPHA_8 = 4,
	BitmapConfig_RGBA_4444 = 5,
	BitmapConfig_RGB_565 = 6,
	BitmapConfig_RGBA_F16 = 7,
	BitmapConfig_HARDWARE = 8,
	BitmapConfig_UNKNOWN
};

class SDENGINE_CLASS BitmapConfig
{
public:
	BitmapConfig() : m_enum(BitmapConfig_UNKNOWN){}
	BitmapConfig(const BitmapConfigEnum i_b) : m_enum(i_b){}
	BitmapConfig(const BitmapConfig &i_b) : m_enum(i_b.m_enum){}
	~BitmapConfig(){}
public:
	BitmapConfig &operator=(const BitmapConfig &i_b){ m_enum = i_b.m_enum; return *this; }
	BitmapConfig &operator=(const BitmapConfigEnum &i_b){ m_enum = i_b; return *this; }
	bool operator==(const BitmapConfig &i_b) const{ return (m_enum == i_b.m_enum);}
	bool operator==(const BitmapConfigEnum &i_b) const{ return (m_enum == i_b); }
	bool operator!=(const BitmapConfig &i_b) const{ return (m_enum != i_b.m_enum); }
	bool operator!=(const BitmapConfigEnum &i_b) const{ return (m_enum != i_b); }
public:
	BitmapConfigEnum m_enum;
};

class SDENGINE_CLASS PixelValueType
{
public:
	PixelValueType() : m_enum(PixelDataType_MAX_DEFINE_VALUE){}
	PixelValueType(const PixelDataTypeEnum i_b) : m_enum(i_b){}
	PixelValueType(const PixelValueType &i_b) : m_enum(i_b.m_enum){}
	~PixelValueType(){}
public:
	PixelValueType &operator=(const PixelValueType &i_b){ m_enum = i_b.m_enum; return *this; }
	PixelValueType &operator=(const PixelDataTypeEnum &i_b){ m_enum = i_b; return *this; }
	bool operator==(const PixelValueType &i_b) const{ return (m_enum == i_b.m_enum); }
	bool operator==(const PixelDataTypeEnum &i_b) const{ return (m_enum == i_b); }
	bool operator!=(const PixelValueType &i_b) const{ return (m_enum != i_b.m_enum); }
	bool operator!=(const PixelDataTypeEnum &i_b) const{ return (m_enum != i_b); }
public:
	PixelDataTypeEnum m_enum;
};

/*! \class VertexBuffer
 *  In our graphic system, VertexBuffer objects are used to keep vertex datas.
 */
SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Bitmap);

class SDENGINE_API Bitmap : public Object
{
public:
	Bitmap(const std::string &i_object_name);

	Bitmap(const std::string    &i_object_name, 
		   const BitmapConfig   &i_conf,
		   Size_ui32             i_width,
		   Size_ui32             i_height,
		   const ImageBufferAddr i_data,
		   ImageBufferSize       i_size,
		   int                   i_n_of_c = -1, 
		   const PixelValueType &i_v_type = PixelDataType_MAX_DEFINE_VALUE);
	
	virtual ~Bitmap();
public:
	bool SetBitmap(const BitmapConfig    &i_conf,
                   Size_ui32              i_width, 
                   Size_ui32              i_height,
                   const ImageBufferAddr  i_data,
                   ImageBufferSize        i_size,
                   int                    i_n_of_c = -1,
                   const PixelValueType  &i_v_type = PixelDataType_MAX_DEFINE_VALUE);
	const ImageBufferAddr GetBitmap() const;
	bool  InNull() const;
protected:
	void DecideNumberOfChannel(int i_n_of_c);
	void ClearBitmap();
protected:
	SD_DECLARE_ATTRIBUTE_VAR_GET(BitmapConfig, m_config , BitmapConfig);
	SD_DECLARE_ATTRIBUTE_VAR_GET(PixelValueType, m_v_type , PixelValueType);
	SD_DECLARE_ATTRIBUTE_VAR_GET(Size_ui32, m_width, Width);
	SD_DECLARE_ATTRIBUTE_VAR_GET(Size_ui32, m_height, Height);
	SD_DECLARE_ATTRIBUTE_VAR_GET(uint32_t, m_num_of_c, NumOfChannel);
	SD_DECLARE_ATTRIBUTE_VAR_GET(ImageBufferSize, m_buffer_size, BufferSize);
protected:
	ImageBufferAddr m_bitmap;
};

inline bool Bitmap::InNull() const{ return (m_bitmap == nullptr); }
inline const ImageBufferAddr Bitmap::GetBitmap() const{ return m_bitmap; }

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}