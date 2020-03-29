#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>
#include "ImageLoader.h"

using SDE::Basic::FileData;
using SDE::Basic::FileSystemManager;

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

BitmapStrongReferenceObject ImageLoader::ReadBitmap(const FilePathString& i_fp) const
{
    FileData file_data;
    FileSystemManager::GetRef().OpenFile(i_fp, file_data);
    return ReadBitmap(i_fp, file_data.GetDataUCharAddr(), file_data.GetSize());
}

BitmapStrongReferenceObject ImageLoader::ReadBitmap(const ObjectName& i_name, const UBytePtr i_file_buffer, size_t i_file_size) const
{
    int width, height, n_of_c;
    stbi_uc *image_ptr = stbi_load_from_memory(i_file_buffer, static_cast<int>(i_file_size), &width, &height, &n_of_c, 4);
    size_t image_size = static_cast<uint32_t>(width) * static_cast<uint32_t>(height) * 4;
    BitmapStrongReferenceObject bitmap = new Bitmap(i_name, BitmapConfig_STB, width, height, image_ptr, image_size, 4, BitmapPixelDataType_UNSIGNED_BYTE);
    stbi_image_free(image_ptr);
    return bitmap;
}

______________SD_END_GRAPHICS_NAMESPACE______________