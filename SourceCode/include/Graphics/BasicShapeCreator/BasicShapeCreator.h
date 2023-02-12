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

/*! \file      BasicShapeCreator.h
 *  \brief     Introduce of class BasicShapeCreator.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/04/11
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "Area.h"
#include "Vector3f.h"
#include "Mesh.h"
#include "ModelData.h"

using SDE::Math::Vector3f;
using SDE::Graphics::Mesh;

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS BasicShapeCreator
{
public:
    static vec3 CalculateTangetVector(const vec3 &i_v1, const vec3 &i_v2, const vec2 &i_st1, const vec2 &i_st2);
public:
    SD_SINGLETON_DECLARATION(BasicShapeCreator);
public:
    BasicShapeCreator();
    ~BasicShapeCreator();
public:
    MeshStrongReferenceObject CreateCube(
        const Vector3f &i_center = Vector3f::Zero,
        const Vector3f &i_scale = Vector3f(1.0f, 1.0f, 1.0f),
        const Vector3f& i_tex_scale = Vector3f(1.0f, 1.0f, 1.0f));

    MeshStrongReferenceObject CreatePlane(
        const Vector3f &i_center = Vector3f::Zero, 
        const Vector3f &i_u = Vector3f::PositiveX,
        const Vector3f &i_v = Vector3f::PositiveY,
        float i_width = 1.0f, float i_length = 1.0f,
        float i_tessellated_u_unit_length = 1.0f,
        float i_tessellated_v_unit_length = 1.0f,
        float i_texture_wrap_x = 1.0f,
        float i_texture_wrap_y = 1.0f);

    MeshStrongReferenceObject CreateAxis(float i_width = 0.2f, float i_length = 2.0f);

    MeshStrongReferenceObject CreateWorldGUI(float i_width = 0.5f, float i_height = 0.5f);

    MeshStrongReferenceObject CreateWorldGUIViaDepthArea2D(const DepthArea2D &i_area);

    MeshStrongReferenceObject CreateCone(
        const Vector3f &i_center = Vector3f::Zero,
        const Vector3f &i_u = Vector3f::PositiveX,
        const Vector3f &i_v = Vector3f::PositiveY,
        const Vector3f &i_w = Vector3f::PositiveZ,
        float i_height = 1.0f, float i_angle = 15.0f, uint32_t i_slices = 15);

    MeshStrongReferenceObject CreateCircle(
        const Vector3f &i_u = Vector3f::PositiveX,
        const Vector3f &i_v = Vector3f::PositiveY,
        float i_radius = 1.0f,
        uint32_t i_slices = 15);

};

______________SD_END_GRAPHICS_NAMESPACE______________