#include "AssimpModelLoader.h"

//--------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//------------------------- start of namespace Graphics ------------------------
namespace Graphics
{

SD_SINGLETON_DECLARATION_IMPL(AssimpModelLoader);

AssimpModelLoader::AssimpModelLoader()
{
    SD_SINGLETON_DECLARATION_REGISTER;
}

AssimpModelLoader::~AssimpModelLoader()
{
}

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}