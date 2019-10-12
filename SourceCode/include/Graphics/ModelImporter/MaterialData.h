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

/*! \enum MaterialTextureTypeEnum
 *  \brief Enumerate material texture type.
 */
enum MaterialTextureTypeEnum
{
    MaterialTextureType_Diffuse = 0, /*!< Main texture of this model. */
    MaterialTextureType_Specular, /*!< Specular texture of this model. */
    MaterialTextureType_Ambient, /*!< Ambient texture of this model. */
    MaterialTextureType_Emissive, /*!< Emissive texture of this model. */
    MaterialTextureType_Height, /*!< Height map of this model. */
    MaterialTextureType_Normals, /*!< Normal map of this model. */
    MaterialTextureType_Shiness, /*!< Shiness map of this model. */
    MaterialTextureType_Opacity, /*!< Opacity texture of this model. */
    MaterialTextureType_Displacement, /*!< Displacement texture of this model. */
    MaterialTextureType_LightMap, /*!< Light map of this model. */
    MaterialTextureType_Reflection, /*!< Reflection map of this model. */
    MaterialTextureType_MAX_DEFINE_VALUE
};

/*! \class MaterialData
 *  \brief Material data is used to keep material parameters of this model.
 */
class SDENGINE_CLASS MaterialData
{
public:
    /*! \var static const uint32_t AI_TEXTURE_ID[MaterialTextureType_MAX_DEFINE_VALUE];
     *  \brief List all assimp texture enumerate.
     */
    static const uint32_t AI_TEXTURE_ID[MaterialTextureType_MAX_DEFINE_VALUE];
public:
    /*! \fn MaterialData();
     *  \brief Ctor about MaterialData.
     */
    MaterialData();

    /*! \fn ~MaterialData();
     *  \brief Dtor about MaterialData.
     */
    ~MaterialData();
public:

    /*! \fn std::string ToString(uint32_t i_level = 0) const;
     *  \param [in] i_level Use to add prefix before show this data.
     *  \brief Return string to describe this data.
     */
    std::string ToString(uint32_t i_level = 0) const;
public:
    /* \var FilePathString m_textures_fns[MaterialTextureType_MAX_DEFINE_VALUE];
     * \brief Variable for keep related filename of each basic texture.
     *        Please note that all texture file should be located in folder with its model file.
     */
    FilePathString m_textures_fns[MaterialTextureType_MAX_DEFINE_VALUE];

    /* \var Color4f m_ambient;
     * \brief Ambient color of this material.
     */
    Color4f m_ambient;

    /* \var Color4f m_diffuse;
     * \brief Diffuse color of this material.
     */
    Color4f m_diffuse;

    /* \var Color4f m_specular;
     * \brief Specular color of this material.
     */
    Color4f m_specular;

    /* \var Color4f m_emission;
     * \brief Emission color of this material.
     */
    Color4f m_emission;

    /* \var float m_shininess;
     * \brief shininess of this material.
     */
    float m_shininess; //shineness_strength

    /* \var ObjectName m_name;
     * \brief name of this material.
     */
    ObjectName m_name;
};

______________SD_END_GRAPHICS_NAMESPACE______________