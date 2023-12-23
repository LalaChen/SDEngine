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
#include "ImageLoader.h"
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
    if (m_tex_identity.m_is_swapchain == false) {
        GraphicsManager::GetRef().DeleteTextureImage(m_tex_identity, m_sampler_identity);
    }
}

//--- Texture Data Function
void Texture::InitializeFromImageResource(const FilePathString &i_filename, Size_ui32 i_mipmap_levels)
{
    if (ImageLoader::IsNull() == false) {
        BitmapStrongReferenceObject target = ImageLoader::GetRef().ReadBitmap(i_filename);
        if (target.IsNull() == false) {
            InitializeFromBitmap(target, i_mipmap_levels);
        }
        else {
            SDLOGW("We can't find texture(%s)", i_filename.c_str());
        }
    }
    else {
        SDLOGE("ImageLoader is nullptr.");
    }
}

void Texture::InitializeFromBitmap(const BitmapWeakReferenceObject &i_bitmap, Size_ui32 i_mipmap_level)
{
    if (i_bitmap.IsNull() == false) {
        if (m_tex_identity.m_handle == SD_NULL_HANDLE) {
            //1. collect necessary datas.
            Size_ui32 img_w = SD_WREF(i_bitmap).GetWidth();
            Size_ui32 img_h = SD_SREF(i_bitmap).GetHeight();
            Size_ui32 img_n_of_c = SD_SREF(i_bitmap).GetNumOfChannel();
            Size_ui32 img_buf_size = static_cast<Size_ui32>(SD_SREF(i_bitmap).GetBufferSize());
            BitmapPixelValueType bitmap_p_type = SD_SREF(i_bitmap).GetPixelValueType();
            const VoidPtr img_ptr = reinterpret_cast<const VoidPtr>(SD_SREF(i_bitmap).GetBitmap());

            //2. write down texture data.
            m_tex_identity.m_texture_type = TextureType_TEXTURE_2D;
            m_tex_identity.m_texture_view_type = TextureViewType_TEXTURE_2D;
            m_tex_identity.m_mipmap_levels = i_mipmap_level;
            m_tex_identity.m_image_size.m_width = img_w;
            m_tex_identity.m_image_size.m_height = img_h;
            m_tex_identity.m_image_size.m_length = 1;
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_SRC);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_DST);
            m_tex_identity.m_image_usages.push_back(ImageUsage_SAMPLED);
            m_tex_identity.m_image_usages.push_back(ImageUsage_STORAGE);
            if (img_n_of_c == 3 || img_n_of_c == 4) {
                m_tex_identity.m_aspect = ImageAspect_ASPECT_COLOR;
            }
            else {
                m_tex_identity.m_aspect = ImageAspect_ASPECT_DEPTH;
            }
            m_tex_identity.m_init_layout = ImageLayout_SHADER_READ_ONLY_OPTIMAL;

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

            //3. Create image.
            GraphicsManager::GetRef().CreateTextureImage(m_tex_identity, m_sampler_identity);

            //4. Refresh texture image.
            if (m_tex_identity.m_handle != SD_NULL_HANDLE) {
                GraphicsManager::GetRef().RefreshTextureImage(m_tex_identity, img_ptr, ImageOffset(), m_tex_identity.m_image_size, img_buf_size, ImageLayout_SHADER_READ_ONLY_OPTIMAL);
            }
            else {
                SDLOGW("This tex[%s] cannot create handle.", m_object_name.c_str());
            }
        }
        else {
            SDLOGW("This tex[%s] is initialized. Please reinitalize tex after releasing old data.", m_object_name.c_str());
        }
    }
    else {
        SDLOGW("Bitmap is null!!!");
    }
}

void Texture::Initialize2DColorOrDepthBuffer(Size_ui32 i_width, Size_ui32 i_height, TextureFormatEnum i_format, ImageLayoutEnum i_layout, Size_ui32 i_mipmap_levels)
{
    if (m_tex_identity.m_handle == SD_NULL_HANDLE) {
        m_tex_identity.m_texture_type = TextureType_TEXTURE_2D;
        m_tex_identity.m_texture_view_type = TextureViewType_TEXTURE_2D;
        m_tex_identity.m_mipmap_levels = i_mipmap_levels;
        m_tex_identity.m_image_size.m_width = i_width;
        m_tex_identity.m_image_size.m_height = i_height;
        m_tex_identity.m_image_size.m_length = 1;
        if (i_layout == ImageLayout_COLOR_ATTACHMENT_OPTIMAL) {
            if (GraphicsManager::GetRef().IsSupportedColorBufferFormat(i_format) == true) {
                m_tex_identity.m_texture_format = i_format;
            }
            else {
                SDLOGW("Depth TextureFormat(%d) isn't supported in this device. Use default.", i_format);
                m_tex_identity.m_texture_format = GraphicsManager::GetRef().GetDefaultColorBufferFormat();
            }
            m_tex_identity.m_init_layout = i_layout;
            m_tex_identity.m_aspect = ImageAspect_ASPECT_COLOR;
            m_tex_identity.m_image_usages.push_back(ImageUsage_COLOR_ATTACHMENT);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_SRC);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_DST);
            m_tex_identity.m_image_usages.push_back(ImageUsage_SAMPLED);
            GraphicsManager::GetRef().CreateTextureImage(m_tex_identity, m_sampler_identity);
        }
        else if (i_layout == ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            if (GraphicsManager::GetRef().IsSupportedDepthBufferFormat(i_format) == true) {
                m_tex_identity.m_texture_format = i_format;
            }
            else {
                SDLOGW("Depth TextureFormat(%d) isn't supported in this device. Use default.", i_format);
                m_tex_identity.m_texture_format = GraphicsManager::GetRef().GetDefaultDepthBufferFormat();
            }
            m_tex_identity.m_init_layout = i_layout;
            m_tex_identity.m_aspect = ImageAspect_ASPECT_DEPTH;
            m_tex_identity.m_image_usages.push_back(ImageUsage_DEPTH_ATTACHMENT);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_SRC);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_DST);
            m_tex_identity.m_image_usages.push_back(ImageUsage_SAMPLED);
            GraphicsManager::GetRef().CreateTextureImage(m_tex_identity, m_sampler_identity);
        }
        else {
            m_tex_identity = TextureIdentity();
            SDLOGW("We cannot initialize texture[%s] whose layout isn't color attachment or depth attachment.", m_object_name.c_str());
        }
    }
}

void Texture::InitializeVRColorOrDepthBuffer(Size_ui32 i_width, Size_ui32 i_height, TextureFormatEnum i_format, ImageLayoutEnum i_layout, Size_ui32 i_mipmap_levels)
{
    if (m_tex_identity.m_handle == SD_NULL_HANDLE) {
        m_tex_identity.m_texture_type = TextureType_TEXTURE_2D_ARRAY;
        m_tex_identity.m_texture_view_type = TextureViewType_TEXTURE_2D_ARRAY;
        m_tex_identity.m_mipmap_levels = i_mipmap_levels;
        m_tex_identity.m_image_size.m_width = i_width;
        m_tex_identity.m_image_size.m_height = i_height;
        m_tex_identity.m_image_size.m_length = 1;
        m_tex_identity.m_array_layers = 2;
        if (i_layout == ImageLayout_COLOR_ATTACHMENT_OPTIMAL) {
            if (GraphicsManager::GetRef().IsSupportedColorBufferFormat(i_format) == true) {
                m_tex_identity.m_texture_format = i_format;
            }
            else {
                SDLOGW("Depth TextureFormat(%d) isn't supported in this device. Use default.", i_format);
                m_tex_identity.m_texture_format = GraphicsManager::GetRef().GetDefaultColorBufferFormat();
            }
            m_tex_identity.m_init_layout = i_layout;
            m_tex_identity.m_aspect = ImageAspect_ASPECT_COLOR;
            m_tex_identity.m_image_usages.push_back(ImageUsage_COLOR_ATTACHMENT);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_SRC);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_DST);
            m_tex_identity.m_image_usages.push_back(ImageUsage_SAMPLED);
            GraphicsManager::GetRef().CreateTextureImage(m_tex_identity, m_sampler_identity);
        }
        else if (i_layout == ImageLayout_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            if (GraphicsManager::GetRef().IsSupportedDepthBufferFormat(i_format) == true) {
                m_tex_identity.m_texture_format = i_format;
            }
            else {
                SDLOGW("Depth TextureFormat(%d) isn't supported in this device. Use default.", i_format);
                m_tex_identity.m_texture_format = GraphicsManager::GetRef().GetDefaultDepthBufferFormat();
            }
            m_tex_identity.m_init_layout = i_layout;
            m_tex_identity.m_aspect = ImageAspect_ASPECT_DEPTH;
            m_tex_identity.m_image_usages.push_back(ImageUsage_DEPTH_ATTACHMENT);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_SRC);
            m_tex_identity.m_image_usages.push_back(ImageUsage_TRANSFER_DST);
            m_tex_identity.m_image_usages.push_back(ImageUsage_SAMPLED);
            GraphicsManager::GetRef().CreateTextureImage(m_tex_identity, m_sampler_identity);
        }
        else {
            m_tex_identity = TextureIdentity();
            SDLOGW("We cannot initialize texture[%s] whose layout isn't color attachment or depth attachment.", m_object_name.c_str());
        }
    }
}

void Texture::InitializeWithSwapchainImage(CompHandle i_sw_image, CompHandle i_sw_image_view, Size_ui32 i_width, Size_ui32 i_height, TextureTypeEnum i_type, TextureViewTypeEnum i_view_type, TextureFormatEnum i_format)
{
    if (m_tex_identity.m_handle == SD_NULL_HANDLE) {
        uint32_t layer_count = 1;
        if (i_type == TextureType_TEXTURE_2D_ARRAY) {
            layer_count = 2;
        }
        m_tex_identity.m_handle = i_sw_image;
        m_tex_identity.m_image_view = i_sw_image_view;
        m_tex_identity.m_texture_type = i_type;
        m_tex_identity.m_texture_view_type = i_view_type;
        m_tex_identity.m_texture_format = i_format;
        m_tex_identity.m_mipmap_levels = 1;
        m_tex_identity.m_image_size.m_width = i_width;
        m_tex_identity.m_image_size.m_height = i_height;
        m_tex_identity.m_image_size.m_length = 1;
        m_tex_identity.m_array_layers = layer_count;
        m_tex_identity.m_aspect = ImageAspect_ASPECT_COLOR;
        m_tex_identity.m_init_layout = ImageLayout_COLOR_ATTACHMENT_OPTIMAL;
        m_tex_identity.m_image_usages.push_back(ImageUsage_COLOR_ATTACHMENT);
        m_tex_identity.m_is_swapchain = true;
        GraphicsManager::GetRef().InitializeSwapchainTextureImage(m_tex_identity, m_sampler_identity);
    }
}

void Texture::SetTextureMemoryTilingMode(const ImageTilingEnum &i_tiling_mode)
{
    m_tex_identity.m_tiling = i_tiling_mode;
}

void Texture::SetTextureSampleCount(const SampleCountEnum &i_sample_count)
{
    m_tex_identity.m_sample_count = i_sample_count;
}

void Texture::SetSamplerFilterType(const SamplerFilterTypeEnum &i_mag_type, const SamplerFilterTypeEnum &i_min_type)
{
    m_sampler_identity.m_mag_filter_type = i_mag_type;
    m_sampler_identity.m_min_filter_type = i_min_type;
}

void Texture::SetSamplerMipmapMode(const SamplerMipmapModeEnum &i_mipmap_mode)
{
    m_sampler_identity.m_mipmap_mode = i_mipmap_mode;
}

void Texture::SetSamplerWrapMode(const SamplerWrapModeEnum &i_s_mode, const SamplerWrapModeEnum &i_t_mode, const SamplerWrapModeEnum &i_r_mode)
{
    m_sampler_identity.m_wrap_mode_s = i_s_mode;
    m_sampler_identity.m_wrap_mode_t = i_t_mode;
    m_sampler_identity.m_wrap_mode_r = i_r_mode;
}

void Texture::SetSamplerBorderColorType(const SamplerBorderColorTypeEnum &i_color_type)
{
    m_sampler_identity.m_sampler_b_color_type = i_color_type;
}

void Texture::SetCompareFunction(bool i_signal)
{
    m_sampler_identity.m_use_compare = i_signal;
}

void Texture::SetCompareOp(const CompareOperatorEnum &i_op)
{
    m_sampler_identity.m_compare_op = i_op;
}

void Texture::SetAnisotropy(bool i_signal)
{
    m_sampler_identity.m_use_anisotropy = i_signal;
}

void Texture::SetMaxAnisotropy(float i_max_anisotropy)
{
    m_sampler_identity.m_max_anisotropy = i_max_anisotropy;
}

______________SD_END_GRAPHICS_NAMESPACE______________