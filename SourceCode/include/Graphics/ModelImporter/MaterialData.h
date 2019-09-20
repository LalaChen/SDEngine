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

/*! \file      MaterialData.h
 *  \brief     Introduce of class MaterialData.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/09/15
 *  \copyright MIT License.
 */

#pragma once

#include <map>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "FileData.h"
#include "Color4f.h"

using SDE::Basic::ObjectName;
using SDE::Basic::FilePathString;

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum MaterialTextureTypeEnum
{
    MaterialTextureType_Diffuse = 0,
    MaterialTextureType_Specular,
    MaterialTextureType_Ambient,
    MaterialTextureType_Emissive,
    MaterialTextureType_Height,
    MaterialTextureType_Normals,
    MaterialTextureType_Shiness,
    MaterialTextureType_Opacity,
    MaterialTextureType_Displacement,
    MaterialTextureType_LightMap,
    MaterialTextureType_Reflection,
    MaterialTextureType_MAX_DEFINE_VALUE
};

typedef std::map<MaterialTextureTypeEnum, FilePathString> TextureMap;

class MaterialData
{
public:
    static const uint32_t AI_TEXTURE_ID[MaterialTextureType_MAX_DEFINE_VALUE];
public:
    explicit MaterialData();
    ~MaterialData();
public:
    FilePathString m_textures_fns[MaterialTextureType_MAX_DEFINE_VALUE];
    TextureMap m_texture_map;
    Color4f m_ambient;
    Color4f m_diffuse;
    Color4f m_specular;
    Color4f m_emission;
    float m_shineness; //shineness_strength
    ObjectName m_material_name;
};

______________SD_END_GRAPHICS_NAMESPACE______________