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

/*! \file      MaterialUniforms.h
 *  \brief     Introduce of class MaterialUniforms.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/12
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "MaterialData.h"

#include "Vector3f.h"
#include "Color4f.h"

using SDE::Math::Vector3f;
using SDE::Graphics::Color4f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class MaterialUniforms
 *  \brief In our system, MaterialUniforms is used to record parameters of object
 *         about phong shading.
 */
class SDENGINE_CLASS MaterialUniforms
{
public:
    /* \fn MaterialUniforms();
     * \brief Ctor of MaterialUniforms.
     */
    MaterialUniforms();

    /* \fn ~MaterialParameter();
     * \brief Dtor of MaterialUniforms.
     */
    ~MaterialUniforms();
public:
    /* \var Color4f m_ambient;
     * \brief ambient color of object.
     */
    Color4f m_ambient;

    /* \var Color4f m_diffuse;
     * \brief diffuse color of object.
     */
    Color4f m_diffuse;

    /* \var Color4f m_specular;
     * \brief specular color of object.
     */
    Color4f m_specular;

    /* \var Color4f m_emission;
     * \brief emission color of object.
     */
    Color4f m_emission;

    /* \var float m_shininess;
     * \brief shineness of object.
     */
    float m_shininess;
};

______________SD_END_GRAPHICS_NAMESPACE______________