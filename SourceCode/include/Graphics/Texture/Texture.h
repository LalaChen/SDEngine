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

/*! \file      Texture.h
 *  \brief     Introduce of class Texturee.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/08/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "Bitmap.h"
#include "TextureIdentity.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics -------------------------
namespace Graphics
{

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Texture);

class SDENGINE_API Texture : public Object
{
public:
    explicit Texture(const ObjectName &i_object_name);
    virtual ~Texture();
public:
    void InitializeDataFromBitmap(const BitmapWeakReferenceObject &i_bitmap_wref); //Input 2D texture.
public:
    void SetTextureFilterType(const TextureFilterTypeEnum &i_mag_type, const TextureFilterTypeEnum &i_min_type);

    void SetTextureWrapMode(const TextureWrapModeEnum &i_s_mode, const TextureWrapModeEnum &i_t_mode, const TextureWrapModeEnum &i_r_mode);
protected:
    TextureIdentity m_identity;
};

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}