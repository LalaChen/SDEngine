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

/*! \file      AssimpModelLoader.h
 *  \brief     Introduce of class AssimpModelLoader.
 *  \author    Kuan-Chih, Chen
 *  \date      2018/09/14
 *  \copyright MIT Public License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ModelData.h"
#include "FileData.h"
#include "Matrix4X4f.h"

using SDE::Basic::FilePathString;
using SDE::Basic::FileData;
using SDE::Math::Matrix4X4f;

namespace Assimp {
    struct aiScene;
    struct aiNode;
}

class AssimpLogWarpper;

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class AssimpModelLoader
 *  \brief In our engine, class AssimpModelLoader is used to load model file and convert the file to
 *  class ModelData. Class ModelData is used to keeping parsed data. For all vertex attributes,
 *  we store those data to vector<vec3>. For each texture, we will keep it own bitmap.
 */
class SDENGINE_CLASS AssimpModelLoader
{
public:
    SD_SINGLETON_DECLARATION(AssimpModelLoader);
public:
    /*! \fn explicit AssimpModelLoader();
     *  \brief Default constructor about AssimpModelLoader.
     */
    AssimpModelLoader();

    /*! \fn ~AssimpModelLoader();
     *  \brief Destructor about AssimpModelLoader.
     */
    ~AssimpModelLoader();
public:
    /*! \fn bool ImportModel(const FilePathString &i_model_fn, ModelData &io_model);
     *  \param [in] i_model_fn Model filepath.
     *  \param [inout] io_model The parsed data.
     *  \brief This function is used to import model file to our model. Please note
     *         that we should put all texture files with model file together.
     */
    bool ImportModel(const FilePathString &i_model_fn, ModelData &io_model);
protected:
    void ParseMaterialsAndTheirTextures(const struct aiScene *i_scene, const FilePathString &i_model_dir, ModelData &io_model);
    void ParseMeshes(const struct aiScene *i_scene, ModelData &io_model);
    void ParseNodes(const struct aiScene *i_scene, const Matrix4X4f &i_p_trans, struct aiNode *i_node, NodeData &io_node);
protected:
    AssimpLogWarpper *m_logger;
};

______________SD_END_GRAPHICS_NAMESPACE______________