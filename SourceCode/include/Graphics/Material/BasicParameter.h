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

/*! \file      BasicParameter.h
 *  \brief     Introduce of class BasicParameter.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/12
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "MaterialData.h"

#include "Matrix4X4f.h"
#include "Vector3f.h"

using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class BasicParameter
 *    In our system, we use class BasicParameter to keeping uniform variables.
 */
class SDENGINE_CLASS BasicParameter
{
public:
    /* \fn BasicParameter();
     * \brief Ctor of BasicParameter.
     */
    BasicParameter();

    /* \fn ~BasicParameter();
     * \brief Dtor of BasicParameter.
     */
    ~BasicParameter();
public:
    /* \var Matrix4X4f m_proj;
     * \brief Convert vertices in view space to projection space(-1,1,-1,1,-1,1). 
     */
    Matrix4X4f m_proj;

    /* \var Matrix4X4f m_view;
     * \brief Convert vertices in world space to view space.
     */
    Matrix4X4f m_view;

    /* \var Matrix4X4f m_worid;
     * \brief Convert vertices in object space to view space.
     */
    Matrix4X4f m_worid;

    /* \var Matrix4X4f m_worid;
     * \brief Convert normals in object space to world space.
     */
    Matrix4X4f m_normal;

    /* \var Vector3f m_view_eye;
     * \brief camera position in world space.
     */
    Vector3f m_view_eye;
};

______________SD_END_GRAPHICS_NAMESPACE______________
