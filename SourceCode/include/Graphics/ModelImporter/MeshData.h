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
#include "Vec.h"

using SDE::Basic::ObjectName;
using SDE::Math::Transform;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class ModelMeshData
 *  In our graphic system, ModelMeshData is used to keep parsed data.
 */
class SDENGINE_CLASS ModelMeshData
{
public:
    /*! \fn explicit ModelMeshData(const ObjectName &i_mesh_name);
     *  \param [in] i_mesh_name Name of this ModelMeshData.
     *  \brief Default constructor about ModelMeshData.
     */
    ModelMeshData(const ObjectName &i_mesh_name);

    /*! \fn ~ModelMeshData();
     *  \brief ModelMeshData destructor.
     */
    ~ModelMeshData();
public:
    /*! \var std::vector<vec3> m_vertex_attribs[VertexBufferUsage_BUFFER_GROUP];
     *  \brief vertex attributes.
     */
    std::vector<vec3> m_vertex_attribs[VertexBufferUsage_BUFFER_GROUP];

    /*! \var Transform m_local_trans;
     *  \brief local transformation.
     */
    Transform m_local_trans;
    
    /*! \var Transform m_world_trans;
     *  \brief local transformation.
     */
    Transform m_world_trans;

    /*! \var ObjectName m_mesh_name;
     *  \brief mesh name.
     */
    ObjectName m_mesh_name;

    /*! \var int32_t m_material_id;
     *  \brief Material ID. Default is -1.(No material)
     */
    int32_t m_material_id;
};

______________SD_END_GRAPHICS_NAMESPACE______________