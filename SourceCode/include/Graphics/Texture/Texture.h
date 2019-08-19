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
 *  \brief     Introduce of class Texture.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/08/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "Bitmap.h"
#include "SamplerIdentity.h"
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

/*! \class Texture
 *  In our graphic system, texture can be used for many case. For an example, 2D Texture is usually used to post on an object in game world. In another case,
 *  1D texture can be used to store a look-up table. Finallt, 3D texture can be used to handle 3D medical image for volume rendering.
 *  We usually input diverse dimension datas into graphics card for using in shader program via texture. Therefore, the another important parameter group 
 *  is sampler. Sampler is used to tell graphics card how to get data. We need to set mag and min filter, wrap mode about texture coordinate,
 *  getting nearest texel or linear interpolated texels, ...etc.
 */
class SDENGINE_API Texture : public Object
{
public:
    /*! \fn explicit Texture(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this texture.
     *  \param [in] i_register_mgr Flag for telling us is this texture registered to GraphicsManager or not.
     *  \brief Default constructor about texture.
     */
    explicit Texture(const ObjectName &i_object_name, bool i_register_mgr);

    /*! \fn virtual ~Texture();
     *  \brief Destructor of texture.
     */
    virtual ~Texture();
public:
    void InitializeDataFromBitmap(const BitmapWeakReferenceObject &i_bitmap_wref, Size_ui32 i_mipmap_levels = 1); //Input 2D texture.
public:
    void InitializeSampler();

    void SetTextureFilterType(const TextureFilterTypeEnum &i_mag_type, const TextureFilterTypeEnum &i_min_type);

    void SetTextureMipmapMode(const TextureMipmapModeEnum &i_mipmap_mode);

    void SetTextureWrapMode(const TextureWrapModeEnum &i_s_mode, const TextureWrapModeEnum &i_t_mode, const TextureWrapModeEnum &i_r_mode);
public:
    Size_ui32 GetTextureImageWidth() const;
    Size_ui32 GetTextureImageHeight() const;
    Size_ui32 GetTextureImageLength() const;
    ImageSize GetTextureImageSize() const;
protected:
    TextureIdentity m_tex_identity;

    SamplerIdentity m_sampler_idnetity;
};

inline Size_ui32 Texture::GetTextureImageWidth() const
{
    return m_tex_identity.m_image_size.m_width;
}

inline Size_ui32 Texture::GetTextureImageHeight() const
{
    return m_tex_identity.m_image_size.m_height;
}

inline Size_ui32 Texture::GetTextureImageLength() const
{
    return m_tex_identity.m_image_size.m_length;
}

inline ImageSize Texture::GetTextureImageSize() const
{
    return m_tex_identity.m_image_size;
}

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}