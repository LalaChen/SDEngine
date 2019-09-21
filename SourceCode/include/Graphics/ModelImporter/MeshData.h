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

/*! \file      MeshData.h
 *  \brief     Introduce of class MeshData.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/09/14
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "VertexBufferUsage.h"
#include "Transform.h"
#include "Color4f.h"
#include "Vec.h"

using SDE::Basic::ObjectName;
using SDE::Graphics::Color4f;
using SDE::Math::Transform;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class MeshData
 *  In our graphic system, MeshData is used to keep parsed data.
 */
class SDENGINE_CLASS MeshData
{
public:
    /*! \fn explicit MeshData();
     *  \brief Default constructor about MeshData.
     */
    explicit MeshData();

    /*! \fn ~MeshData();
     *  \brief MeshData destructor.
     */
    ~MeshData();
public:
    std::string ToString(uint32_t i_level = 0) const;
public:
    /*! \var std::vector<vec3> m_vertex_attribs[VertexBufferUsage_BUFFER_GROUP];
     *  \brief vertex attributes.
     */
    std::vector<vec3> m_vertex_attribs[VertexBufferUsage_BUFFER_GROUP];

    /*! \var std::vector<Color4f> m_vertex_colors;
     *  \brief vertex colors.
     */
    std::vector<Color4f> m_vertex_colors;

    /*! \var std::vector<uint32_t> m_face_indices;
     *  \brief vertex indices.
     */
    std::vector<uint32_t> m_face_indices;

    /*! \var ObjectName m_name;
     *  \brief mesh name.
     */
    ObjectName m_name;

    /*! \var int32_t m_material_ID;
     *  \brief Material ID. Default is -1.(No material)
     */
    int32_t m_material_ID;
};

______________SD_END_GRAPHICS_NAMESPACE______________