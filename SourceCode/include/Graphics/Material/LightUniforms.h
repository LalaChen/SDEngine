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

/*! \file      LightParameter.h
 *  \brief     Introduce of class LightParameter.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/13
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "Vector3f.h"
#include "Color4f.h"

using SDE::Math::Vector3f;
using SDE::Graphics::Color4f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class LightUniforms
 *  \brief In our system, LightUniforms is used to record parameters of light
 *         about phong shading.
 */
class SDENGINE_CLASS LightUniforms
{
public:
    /* \fn LightParameter();
     * \brief Ctor of LightUniforms.
     */
    LightUniforms();

    /* \fn ~LightUniforms();
     * \brief Dtor of LightUniforms.
     */
    ~LightUniforms();
public:
    /* \var Color4f m_ambient;
     * \brief Ambient color of this light. Default is C(1.0, 1.0, 1.0, 1.0).
     */
    Color4f m_ambient;

    /* \var Color4f m_diffuse;
     * \brief Diffuse color of this light. Default is C(1.0, 1.0, 1.0, 1.0).
     */
    Color4f m_diffuse;

    /* \var Color4f m_specular;
     * \brief Specular color of this light. Default is C(1.0, 1.0, 1.0, 1.0).
     */
    Color4f m_specular;

    /* \var  Vector3f m_position;
     * \brief Position of this light. It's used when this light is point or spot light.
     */
    Vector3f m_position;

    /* \var  Vector3f m_direction;
     * \brief Direction of this light. It's used when this light is directional or spot light.
     */
    Vector3f m_direction;

    /* \var float m_spot_exp;
     * \brief Spot attenuating exponent of this spot light. It's use for calculate attenuation at edge of lighten.
     */
    float m_spot_exp;

    /* \var float m_spot_cos_cutoff;
     * \brief Cosine of half angle about this spot light.
     */
    float m_spot_cos_cutoff;

    /* \var float m_constant_attenuation;
     * \brief Constant attenuation. Attenuating function is 1.0 / (a + b * dis + c * dis * dis).
     *        Constant attenuation is coef a in attenuating function.
     */
    float m_constant_attenuation;

    /* \var float m_linear_attenuation;
     * \brief Linear attenuation. Attenuating function is 1.0 / (a + b * dis + c * dis * dis).
     *        Linear attenuation is coef b in attenuating function.
     */
    float m_linear_attenuation;

    /* \var float m_quadratic_attenuation;
     * \brief Quadratic attenuation. Attenuating function is 1.0 / (a + b * dis + c * dis * dis).
     *        Quadratic attenuation is coef c in attenuating function.
     */
    float m_quadratic_attenuation;

    /* \var int32_t m_kind;
     * \brief Light kind. 0 is directional light. 1 is spot light. 2 is point light.
     */
    int32_t m_kind;
};

______________SD_END_GRAPHICS_NAMESPACE______________