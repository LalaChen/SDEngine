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

#include "LogManager.h"
#include "GraphicsManager.h"
#include "Texture.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

//--- Member Function.
Texture::Texture(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

Texture::~Texture()
{
    GraphicsManager::GetRef().DeleteTextureImage(m_tex_identity);
}

//--- Texture Data Function
void Texture::InitializeFromBitmap(const BitmapWeakReferenceObject &i_bitmap_wref, Size_ui32 i_mipmap_level)
{
    if (i_bitmap_wref.IsNull() == false) {
        if (m_tex_identity.m_image_handle == SD_NULL_HANDLE) {
            //1. collect necessary datas.
            Size_ui32 img_w = i_bitmap_wref.GetRef().GetWidth();
            Size_ui32 img_h = i_bitmap_wref.GetRef().GetHeight();
            Size_ui32 img_n_of_c = i_bitmap_wref.GetRef().GetNumOfChannel();
            Size_ui32 img_buf_size = static_cast<Size_ui32>(i_bitmap_wref.GetRef().GetBufferSize());
            BitmapPixelValueType bitmap_p_type = i_bitmap_wref.GetRef().GetPixelValueType();
            const VoidPtr img_ptr = reinterpret_cast<const VoidPtr>(i_bitmap_wref.GetRef().GetBitmap());
            //2. write down texture data.
            m_tex_identity.m_texture_type = TextureType_TEXTURE_2D;
            m_tex_identity.m_mipmap_levels = i_mipmap_level;
            m_tex_identity.m_image_size.m_width = img_w;
            m_tex_identity.m_image_size.m_height = img_h;
            m_tex_identity.m_image_size.m_length = 1;

            if (img_n_of_c == 1) {
                if (bitmap_p_type.m_enum == BitmapPixelDataType_UNSIGNED_BYTE) {
                    m_tex_identity.m_texture_format = TextureFormat_R8_UNORM;
                }
                else if (bitmap_p_type.m_enum == BitmapPixelDataType_BYTE) {
                    m_tex_identity.m_texture_format = TextureFormat_R8_SNORM;
                }
                else {
                    SDLOGE("We can't find suitable image format. nOfc(%d) pType(%d)", img_n_of_c, bitmap_p_type.m_enum);
                    return;
                }
            }
            else if (img_n_of_c == 2) {
                if (bitmap_p_type.m_enum == BitmapPixelDataType_UNSIGNED_BYTE) {
                    m_tex_identity.m_texture_format = TextureFormat_R8G8_UNORM;
                }
                else if (bitmap_p_type.m_enum == BitmapPixelDataType_BYTE) {
                    m_tex_identity.m_texture_format = TextureFormat_R8G8_SNORM;
                }
                else {
                    SDLOGE("We can't find suitable image format. nOfc(%d) pType(%d)", img_n_of_c, bitmap_p_type.m_enum);
                    return;
                }
            }
            else if (img_n_of_c == 4) {
                if (bitmap_p_type.m_enum == BitmapPixelDataType_UNSIGNED_BYTE) {
                    m_tex_identity.m_texture_format = TextureFormat_R8G8B8A8_UNORM;
                }
                else if (bitmap_p_type.m_enum == BitmapPixelDataType_BYTE) {
                    m_tex_identity.m_texture_format = TextureFormat_R8G8B8A8_SNORM;
                }
                else {
                    SDLOGE("We can't find suitable image format. nOfc(%d) pType(%d)", img_n_of_c, bitmap_p_type.m_enum);
                    return;
                }
            }
            else {
                SDLOGE("We can't find suitable image format. nOfc(%d)", img_n_of_c);
                return;
            }

            GraphicsManager::GetRef().CreateTextureImage(m_tex_identity, m_sampler_idnetity, img_ptr, img_buf_size);
        }
        else {
            SDLOGW("This tex[%s] is initialized. Please reinitalize tex after releasing old data.", m_object_name.c_str());
        }
    }
    else {
        SDLOGW("Bitmap is null!!!");
    }
}

void Texture::SetSamplerFilterType(const SamplerFilterTypeEnum &i_mag_type, const SamplerFilterTypeEnum &i_min_type)
{
    m_sampler_idnetity.m_mag_filter_type = i_mag_type;
    m_sampler_idnetity.m_min_filter_type = i_min_type;
}

void Texture::SetSamplerMipmapMode(const SamplerMipmapModeEnum &i_mipmap_mode)
{
    m_sampler_idnetity.m_mipmap_mode = i_mipmap_mode;
}

void Texture::SetSamplerWrapMode(const SamplerWrapModeEnum &i_s_mode, const SamplerWrapModeEnum &i_t_mode, const SamplerWrapModeEnum &i_r_mode)
{
    m_sampler_idnetity.m_wrap_mode_s = i_s_mode;
    m_sampler_idnetity.m_wrap_mode_t = i_t_mode;
    m_sampler_idnetity.m_wrap_mode_r = i_r_mode;
}

void Texture::SetSamplerBorderColorType(const SamplerBorderColorTypeEnum &i_color_type)
{
    m_sampler_idnetity.m_sampler_b_color_type = i_color_type;
}

void Texture::SetCompareFunction(bool i_signal)
{
    m_sampler_idnetity.m_use_compare = i_signal;
}

void Texture::SetCompareOp(const CompareOperatorEnum &i_op)
{
    m_sampler_idnetity.m_compare_op = i_op;
}

void Texture::SetAnisotropy(bool i_signal)
{
    m_sampler_idnetity.m_use_anisotropy = i_signal;
}

void Texture::SetMaxAnisotropy(float i_max_anisotropy)
{
    m_sampler_idnetity.m_max_anisotropy = i_max_anisotropy;
}

______________SD_END_GRAPHICS_NAMESPACE______________