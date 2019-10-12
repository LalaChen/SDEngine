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

#include <sstream>

#include "SDEngine.h"
#include "ModelData.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________
//-------------------------- NodeData -----------------------------
std::string NodeData::ToString(uint32_t i_level) const
{
    std::stringstream ss;
    std::string tab;
    for (uint32_t i = 0; i < i_level; ++i) {
        tab += "---";
    }
    ss << tab << StringFormat("Node[%s]\n", m_name.c_str());
    ss << m_local_trans.ToFormatString("Ml", tab);
    ss << m_world_trans.ToFormatString("Mw", tab);
    if (m_mesh_idices.size() > 0) {
        ss << tab << "OwnerIdices[";
        for (uint32_t i = 0; i < m_mesh_idices.size(); ++i) {
            ss << m_mesh_idices[i];
            if (i != m_mesh_idices.size() - 1) {
                ss << ",";
            }
            else {
                ss << "]\n";
            }
        }
    }
 
    for (uint32_t cID = 0; cID < m_child_nodes.size(); ++cID) {
        ss << m_child_nodes[cID].ToString(i_level + 1);
    }
    return ss.str();
}

//------------------------- ModelData -----------------------------
ModelData::ModelData()
{
}

ModelData::~ModelData()
{
}

std::string ModelData::ToString() const
{
    std::stringstream ss;
    ss << StringFormat("\nModel : %s\n", m_name.c_str());
    ss << "Materials : \n";
    for (uint32_t i = 0; i < m_materials.size(); ++i) {
        ss << m_materials[i].ToString(1);
    }
    ss << "Mesh : \n";
    for (uint32_t i = 0; i < m_mesh_datas.size(); ++i) {
        ss << m_mesh_datas[i].ToString(1);
    }
    ss << "TextureMap : \n";
    TextureMap::const_iterator t_iter;
    for (t_iter = m_textures.begin(); t_iter != m_textures.end(); t_iter++) {
        ss << StringFormat("\tF[%s]:%s\n",
            (*t_iter).first.c_str(), 
            (*t_iter).second.GetRef().ToString().c_str());
    }

    ss << "NodeData : \n";
    ss << m_root_node.ToString(0);
    
    return ss.str();
}

______________SD_END_GRAPHICS_NAMESPACE______________