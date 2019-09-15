#include "ImageLoader.h"

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

______________SD_END_GRAPHICS_NAMESPACE______________