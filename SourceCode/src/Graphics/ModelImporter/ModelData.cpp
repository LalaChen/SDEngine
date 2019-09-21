#include <sstream>

#include "SDEngine.h"
#include "ModelData.h"

using SDE::Basic::StringFormat;

_____________SD_START_GRAPHICS_NAMESPACE_____________

ModelData::ModelData()
{
}

ModelData::~ModelData()
{
}

std::string ModelData::ToString() const
{
    std::stringstream ss;
    ss << StringFormat("\nModel : %s\n", m_model_name.c_str());
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
    
    return ss.str();
}

______________SD_END_GRAPHICS_NAMESPACE______________