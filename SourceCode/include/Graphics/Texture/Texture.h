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
#include "SDEngineCommonType.h"
#include "Bitmap.h"
#include "SamplerIdentity.h"
#include "TextureIdentity.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;
using SDE::Basic::FilePathString;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Texture);

/*! \class Texture
 *  In our graphic system, texture can be used for many case. For an example, 2D Texture is usually used to post on an object in game world. In another case,
 *  1D texture can be used to store a look-up table. Finally, 3D texture can be used to handle 3D medical image for volume rendering.
 *  We usually input diverse dimension datas into graphics card for using in shader program via texture. Therefore, the another important parameter group 
 *  is sampler. Sampler is used to tell graphics card how to get data. We need to set mag and min filter, wrap mode about texture coordinate,
 *  getting nearest texel or linear interpolated texels, ...etc.
 */
class SDENGINE_CLASS Texture : public Object
{
public:
    friend class GraphicsIdentityGetter;
public:
    /*! \fn explicit Texture(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this texture.
     *  \brief Default constructor about texture.
     */
    explicit Texture(const ObjectName &i_object_name);

    /*! \fn virtual ~Texture();
     *  \brief Destructor of texture.
     */
    virtual ~Texture();
public:
    /*! \fn void InitializeFromImageResource(const FilePathString &i_filename, Size_ui32 i_mipmap_levels = 1);
     *  \param [in] i_filename Filename of resource..
     *  \param [in] i_mipmap_levels Mipmap level.
     *  \brief This function is used to create texture that is used to input to shader. We will ask data from ImagerLoader.
     */
    void InitializeFromImageResource(const FilePathString &i_filename, Size_ui32 i_mipmap_levels = 1);
    
    /*! \fn void InitializeFromBitmap(const BitmapWeakReferenceObject &i_bitmap, Size_ui32 i_mipmap_levels = 1);
     *  \param [in] i_bitmap Target bitmap.
     *  \param [in] i_mipmap_levels Mipmap level.
     *  \brief This function is used to create texture that is used to input to shader.
     */
    void InitializeFromBitmap(const BitmapWeakReferenceObject &i_bitmap, Size_ui32 i_mipmap_levels = 1); //Input 2D texture.
    
    /*! \fn void Initialize2DColorOrDepthBuffer(Size_ui32 i_width, Size_ui32 i_height, TextureFormatEnum i_format, const ImageLayoutEnum &i_layout, Size_ui32 i_mipmap_levels = 1);
     *  \param [in] i_width Width of buffer.
     *  \param [in] i_height Height of buffer.
     *  \param [in] i_format Format about this buffer.
     *  \param [in] i_layout Set target for this buffer.
     *  \brief This function is used to create texture that is used to input to shader. If i_layout is color attachment,
     *         we will create color attachment and set its aspect as color. Else if i_layout is depth attachment, we will create
     *         depth attachment and set its aspect as depth. Please note that this function only can used for creating color 
     *         or depth buffer.
     */
    void Initialize2DColorOrDepthBuffer(Size_ui32 i_width, Size_ui32 i_height, TextureFormatEnum i_format, ImageLayoutEnum i_layout, Size_ui32 i_mipmap_levels = 1);

    /*! \fn void InitializeVRColorOrDepthBuffer(Size_ui32 i_width, Size_ui32 i_height, TextureFormatEnum i_format, const ImageLayoutEnum &i_layout, Size_ui32 i_mipmap_levels = 1);
     *  \param [in] i_width Width of buffer.
     *  \param [in] i_height Height of buffer.
     *  \param [in] i_format Format about this buffer.
     *  \param [in] i_layout Set target for this buffer.
     *  \brief This function is used to create texture that is used to input to shader. If i_layout is color attachment,
     *         we will create color attachment and set its aspect as color. Else if i_layout is depth attachment, we will create
     *         depth attachment and set its aspect as depth. Please note that this function only can used for creating color
     *         or depth buffer for VR.
     */
    void InitializeVRColorOrDepthBuffer(Size_ui32 i_width, Size_ui32 i_height, TextureFormatEnum i_format, ImageLayoutEnum i_layout, Size_ui32 i_mipmap_levels = 1);
public:
    /*! \fn void SetTextureMemoryTilingMode(const ImageTilingEnum &i_tiling_mode);
     *  \param [in] i_tiling_mode Target tiling mode.
     *  \brief This function is specify the way about creating memory this texture.
     *         If tiling mode is linear, the row length is equal to nofc * width and we can allocate host memory for creating it.
     *         Otherwise(mode is optimal), the row length isn't equal to nofc * width and we only can allocate device for creating it. 
     */
    void SetTextureMemoryTilingMode(const ImageTilingEnum &i_tiling_mode);

    void SetTextureSampleCount(const SampleCountEnum &i_sample_count);
public:
    void SetSamplerFilterType(const SamplerFilterTypeEnum &i_mag_type, const SamplerFilterTypeEnum &i_min_type);

    void SetSamplerMipmapMode(const SamplerMipmapModeEnum &i_mipmap_mode);

    void SetSamplerWrapMode(const SamplerWrapModeEnum &i_s_mode, const SamplerWrapModeEnum &i_t_mode, const SamplerWrapModeEnum &i_r_mode);

    void SetSamplerBorderColorType(const SamplerBorderColorTypeEnum &i_color_type);

    void SetCompareFunction(bool i_signal);

    void SetCompareOp(const CompareOperatorEnum &i_op);

    void SetAnisotropy(bool i_signal);

    void SetMaxAnisotropy(float i_max_anisotropy);

    bool IsInitialized() const;
public:

    TextureTypeEnum GetTextureType() const;

    TextureFormatEnum GetTextureFormat() const;

    Size_ui32 GetWidth() const;

    Size_ui32 GetHeight() const;
    
    Size_ui32 GetLength() const;
    
    ImageSize GetDataSize() const;

    Size_ui32 GetLayerCount() const;

    Size_ui32 GetMipMapLevels() const;
protected:
    TextureIdentity m_tex_identity;

    SamplerIdentity m_sampler_idnetity;
};

inline TextureTypeEnum Texture::GetTextureType() const
{
    return m_tex_identity.m_texture_type;
}

inline TextureFormatEnum Texture::GetTextureFormat() const
{
    return m_tex_identity.m_texture_format;
}

inline Size_ui32 Texture::GetWidth() const
{
    return m_tex_identity.m_image_size.m_width;
}

inline Size_ui32 Texture::GetHeight() const
{
    return m_tex_identity.m_image_size.m_height;
}

inline Size_ui32 Texture::GetLength() const
{
    return m_tex_identity.m_image_size.m_length;
}

inline ImageSize Texture::GetDataSize() const
{
    return m_tex_identity.m_image_size;
}

inline Size_ui32 Texture::GetLayerCount() const
{
    return m_tex_identity.m_array_layers;
}

inline Size_ui32 Texture::GetMipMapLevels() const
{
    return m_tex_identity.m_mipmap_levels;
}

inline bool Texture::IsInitialized() const
{
    return m_tex_identity.IsValid() && m_sampler_idnetity.IsValid();
}

______________SD_END_GRAPHICS_NAMESPACE______________