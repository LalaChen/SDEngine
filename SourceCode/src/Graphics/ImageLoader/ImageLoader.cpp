#include "ImageLoader.h"

using SDE::Basic::FileSystemManager;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//-------------------------- start of namespace Graphics -------------------------
namespace Graphics
{

SD_SINGLETON_DECLARATION_IMPL(ImageLoader);

ImageLoader::ImageLoader()
{
    SD_SINGLETON_DECLARATION_REGISTER
}

ImageLoader::~ImageLoader()
{

}

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}