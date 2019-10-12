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

/*! \class NodeData
 *    In our system, we use class NodeData to store node hierarchy about this model.
 */
class SDENGINE_CLASS NodeData
{
public:
    /*! \fn NodeData();
     *  \brief Ctor about NodeData.
     */
    NodeData() {}

    /*! \fn ~NodeData();
     *  \brief Dtor about NodeData.
     */
    ~NodeData() {}
public:
    /*! \fn std::string ToString(uint32_t i_level = 0) const;
     *  \param [in] i_level Use to add prefix before show this data.
     *  \brief Return string to describe this data.
     */
    std::string ToString(uint32_t i_level) const;
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

/*! \class ModelData
 *    In our system, we use class ModelData to store parsed data about model.
 * Please note that we usually use it when we need to convert 3D model to 
 * graphics data. So we shouldn't release them after converting.
 */
class SDENGINE_CLASS ModelData
{
public:
    /*! \fn ModelData();
     *  \brief Ctor about ModelData.
     */
    ModelData();

    /*! \fn ~ModelData();
     *  \brief Dtor about ModelData.
     */
    ~ModelData();
public:
    /*! \fn std::string ToString(uint32_t i_level = 0) const;
     *  \param [in] i_level Use to add prefix before show this data.
     *  \brief Return string to describe this data.
     */
    std::string ToString() const;
public:
    /*! \var std::vector<MeshData> m_mesh_datas;
     *  \brief m_mesh_datas is used to keep all meshes in this model.
     */
    std::vector<MeshData> m_mesh_datas;
    /*! \var std::vector<MaterialData> m_materials;
     *  \brief m_materials is used to keep all materials in this model.
     */
    std::vector<MaterialData> m_materials;

    /*! \var std::vector<MaterialData> m_materials;
     *  \brief m_textures is used to keep all texture bitmaps in this model.
     */
    TextureMap m_textures;

    /*! \var NodeData m_root_node;
     *  \brief m_root_node is used to keep all nodes in this model.
     */
    NodeData m_root_node;

    /*! \var ObjectName m_name;
     *  \brief name of this model.
     */
    ObjectName m_name;
};

______________SD_END_GRAPHICS_NAMESPACE______________