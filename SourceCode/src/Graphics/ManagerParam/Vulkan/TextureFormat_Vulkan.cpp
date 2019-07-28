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

#include <vulkan/vulkan.h>

#include "TextureFormat_Vulkan.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

VkFormat TextureFormat_Vulkan::TextureFormatTypes[TextureFormat_MAX_DEFINE_VALUE] =
{
    //Byte
    //--- R
    VK_FORMAT_R8_UNORM, /*!read value to [0,1]. data is byte. channel number is 1.*/
    VK_FORMAT_R8_SNORM, /*!read value to [-1,1]. data is byte. channel number is 1.*/
    VK_FORMAT_R8_UINT, /*!read value to [0,255]. data is byte. channel number is 1.*/
    VK_FORMAT_R8_SINT, /*!read value to [-128,127]. data is byte. channel number is 1.*/
    //--- RG
    VK_FORMAT_R8G8_UNORM, /*!read value to [0,1]. data is byte. channel number is 2.*/
    VK_FORMAT_R8G8_SNORM, /*!read value to [-1,1]. data is byte. channel number is 2.*/
    VK_FORMAT_R8G8_UINT, /*!read value to [0,255]. data is byte. channel number is 2.*/
    VK_FORMAT_R8G8_SINT, /*!read value to [-128,127]. data is byte. channel number is 2.*/
    //--- RGB
    VK_FORMAT_R8G8B8_UNORM, /*!read value to [0,1]. data is byte. channel number is 3.*/
    VK_FORMAT_R8G8B8_SNORM, /*!read value to [-1,1]. data is byte. channel number is 3.*/
    VK_FORMAT_R8G8B8_UINT, /*!read value to [0,255]. data is byte. channel number is 3.*/
    VK_FORMAT_R8G8B8_SINT, /*!read value to [-128,127]. data is byte. channel number is 3.*/
    //--- RGBA
    VK_FORMAT_R8G8B8A8_UNORM, /*!read value to [0,1]. data is byte. channel number is 4.*/
    VK_FORMAT_R8G8B8A8_SNORM, /*!read value to [-1,1]. data is byte. channel number is 4.*/
    VK_FORMAT_R8G8B8A8_UINT, /*!read value to [0,255]. data is byte. channel number is 4.*/
    VK_FORMAT_R8G8B8A8_SINT, /*!read value to [-128,127]. data is byte. channel number is 4.*/
    //Short
    //--- R
    VK_FORMAT_R16_UNORM, /*!read value to [0,1]. data is short. channel number is 1.*/
    VK_FORMAT_R16_SNORM, /*!read value to [-1,1]. data is short. channel number is 1.*/
    VK_FORMAT_R16_UINT, /*!read value to [0,65535]. data is short. channel number is 1.*/
    VK_FORMAT_R16_SINT, /*!read value to [-32768,32767]. data is short. channel number is 1.*/
    //--- RG
    VK_FORMAT_R16G16_UNORM, /*!read value to [0,1]. data is short. channel number is 2.*/
    VK_FORMAT_R16G16_SNORM, /*!read value to [-1,1]. data is short. channel number is 2.*/
    VK_FORMAT_R16G16_UINT, /*!read value to [0,65535]. data is short. channel number is 2.*/
    VK_FORMAT_R16G16_SINT, /*!read value to [-32768,32767]. data is short. channel number is 2.*/
    //--- RGB
    VK_FORMAT_R16G16B16_UNORM, /*!read value to [0,1]. data is short. channel number is 3.*/
    VK_FORMAT_R16G16B16_SNORM, /*!read value to [-1,1]. data is short. channel number is 3.*/
    VK_FORMAT_R16G16B16_UINT, /*!read value to [0,65535]. data is short. channel number is 3.*/
    VK_FORMAT_R16G16B16_SINT, /*!read value to [-32768,32767]. data is short. channel number is 3.*/
    //--- RGBA
    VK_FORMAT_R16G16B16A16_UNORM, /*!read value to [0,1]. data is short. channel number is 4.*/
    VK_FORMAT_R16G16B16A16_SNORM, /*!read value to [-1,1]. data is short. channel number is 4.*/
    VK_FORMAT_R16G16B16A16_UINT, /*!read value to [0,65535]. data is short. channel number is 4.*/
    VK_FORMAT_R16G16B16A16_SINT, /*!read value to [-32768,32767]. data is short. channel number is 4.*/
    //Int
    //--- R
    VK_FORMAT_R32_UINT, /*!read value to [0,65535]. data is 4-byte. channel number is 1.*/
    VK_FORMAT_R32_SINT, /*!read value to [-32768,32767]. data is 4-byte. channel number is 1.*/
    //--- RG
    VK_FORMAT_R32G32_UINT, /*!read value to [0,MAX_UINT]. data is 4-byte. channel number is 2.*/
    VK_FORMAT_R32G32_SINT, /*!read value to [-32768,32767]. data is 4-byte. channel number is 2.*/
    //--- RGB
    VK_FORMAT_R32G32B32_UINT, /*!read value to [0,MAX_UINT]. data is 4-byte. channel number is 3.*/
    VK_FORMAT_R32G32B32_SINT, /*!read value to [MIN_INT,MAX_INT]. data is 4-byte. channel number is 3.*/
    //--- RGBA
    VK_FORMAT_R32G32B32A32_UINT, /*!read value to [0,MAX_UINT]. data is 4-byte. channel number is 4.*/
    VK_FORMAT_R32G32B32A32_SINT, /*!read value to [MIN_INT,MAX_INT]. data is 4-byte. channel number is 4.*/
    //Float
    //--- R
    VK_FORMAT_R32_SFLOAT, /*!read value by float. channel number is 1.*/
    //--- RG
    VK_FORMAT_R32G32_SFLOAT, /*!read value by float. channel number is 2.*/
    //--- RGB
    VK_FORMAT_R32G32B32_SFLOAT, /*!read value by float. channel number is 3.*/
    //--- RGBA
    VK_FORMAT_R32G32B32A32_SFLOAT, /*!read value by float. channel number is 4.*/
    //Special
    //--- DEPTH and STENCIL
    VK_FORMAT_D32_SFLOAT, /*! depth texture.*/
    VK_FORMAT_D24_UNORM_S8_UINT, /*! depth and stencil texture.*/
    VK_FORMAT_D32_SFLOAT_S8_UINT, /*! depth and stencil texture.*/
    //--- other
    VK_FORMAT_R4G4B4A4_UNORM_PACK16,
    VK_FORMAT_B5G6R5_UNORM_PACK16
};

std::string TextureFormat_Vulkan::TextureFormatNames[TextureFormat_MAX_DEFINE_VALUE] = 
{
    "VK_FORMAT_R8_UNORM",
    "VK_FORMAT_R8_SNORM",
    "VK_FORMAT_R8_UINT",
    "VK_FORMAT_R8_SINT",
    "VK_FORMAT_R8G8_UNORM",
    "VK_FORMAT_R8G8_SNORM",
    "VK_FORMAT_R8G8_UINT",
    "VK_FORMAT_R8G8_SINT",
    "VK_FORMAT_R8G8B8_UNORM",
    "VK_FORMAT_R8G8B8_SNORM",
    "VK_FORMAT_R8G8B8_UINT",
    "VK_FORMAT_R8G8B8_SINT",
    "VK_FORMAT_R8G8B8A8_UNORM",
    "VK_FORMAT_R8G8B8A8_SNORM",
    "VK_FORMAT_R8G8B8A8_UINT",
    "VK_FORMAT_R8G8B8A8_SINT",
    "VK_FORMAT_R16_UNORM",
    "VK_FORMAT_R16_SNORM",
    "VK_FORMAT_R16_UINT",
    "VK_FORMAT_R16_SINT",
    "VK_FORMAT_R16G16_UNORM",
    "VK_FORMAT_R16G16_SNORM",
    "VK_FORMAT_R16G16_UINT",
    "VK_FORMAT_R16G16_SINT",
    "VK_FORMAT_R16G16B16_UNORM",
    "VK_FORMAT_R16G16B16_SNORM",
    "VK_FORMAT_R16G16B16_UINT",
    "VK_FORMAT_R16G16B16_SINT",
    "VK_FORMAT_R16G16B16A16_UNORM",
    "VK_FORMAT_R16G16B16A16_SNORM",
    "VK_FORMAT_R16G16B16A16_UINT",
    "VK_FORMAT_R16G16B16A16_SINT",
    "VK_FORMAT_R32_UINT",
    "VK_FORMAT_R32_SINT",
    "VK_FORMAT_R32G32_UINT",
    "VK_FORMAT_R32G32_SINT",
    "VK_FORMAT_R32G32B32_UINT",
    "VK_FORMAT_R32G32B32_SINT",
    "VK_FORMAT_R32G32B32A32_UINT",
    "VK_FORMAT_R32G32B32A32_SINT",
    "VK_FORMAT_R32_SFLOAT",
    "VK_FORMAT_R32G32_SFLOAT",
    "VK_FORMAT_R32G32B32_SFLOAT",
    "VK_FORMAT_R32G32B32A32_SFLOAT",
    "VK_FORMAT_D32_SFLOAT",
    "VK_FORMAT_D24_UNORM_S8_UINT",
    "VK_FORMAT_D32_SFLOAT_S8_UINT",
    "VK_FORMAT_R4G4B4A4_UNORM_PACK16",
    "VK_FORMAT_B5G6R5_UNORM_PACK16"
};

std::string TextureFormat_Vulkan::DummyName = "Error Texture Format";

VkFormat TextureFormat_Vulkan::Convert(const TextureFormatEnum &i_src)
{
    if (i_src != TextureFormat_MAX_DEFINE_VALUE) {
        return TextureFormatTypes[static_cast<uint32_t>(i_src)];
    }
    else {
        return VK_FORMAT_UNDEFINED;
    }
}

const char* TextureFormat_Vulkan::GetTextureFormatName(const TextureFormatEnum &i_src)
{
    if (i_src != TextureFormat_MAX_DEFINE_VALUE) {
        return TextureFormatNames[static_cast<uint32_t>(i_src)].c_str();
    }
    else {
        return DummyName.c_str();
    }
}

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}