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

/*! \file      ModelData.h
 *  \brief     Introduce of class ModelData.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/09/15
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "Bitmap.h"
#include "Matrix4X4f.h"
#include "MaterialData.h"
#include "MeshData.h"

using SDE::Math::Matrix4X4f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

typedef std::map<FilePathString, BitmapStrongReferenceObject> TextureMap;

class SDENGINE_CLASS NodeData
{
public:
    NodeData() {}
    ~NodeData() {}
public:
    /*! \var std::vector<NodeData> m_child_nodes;
     *  \brief a vector record all child of this node.
     */
    std::vector<NodeData> m_child_nodes;

    /*! \var Matrix4X4f m_local_trans;
     *  \brief local transformation.
     */
    Matrix4X4f m_local_trans;
    
    /*! \var Matrix4X4f m_world_trans;
     *  \brief world transformation.
     */
    Matrix4X4f m_world_trans;

    /*! \var  std::vector<uint32_t> m_mesh_idices;
     *  \brief a vector record all meshes of this node.
     */
    std::vector<uint32_t> m_mesh_idices;

    /*! \var ObjectName m_name;
     *  \brief Name of this node.
     */
    ObjectName m_name;
};

class SDENGINE_CLASS ModelData
{
public:
    explicit ModelData();
    ~ModelData();
public:
    std::string ToString() const;
public:
    std::vector<MeshData> m_mesh_datas;
    std::vector<MaterialData> m_materials;
    TextureMap m_textures;
    NodeData m_root_node;
    ObjectName m_model_name;
};

______________SD_END_GRAPHICS_NAMESPACE______________