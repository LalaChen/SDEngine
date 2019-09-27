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

/*! \file      BasicTextures.h
 *  \brief     Introduce of class BasicTextures.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/09/24
 *  \copyright MIT License.
 */

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "MaterialData.h"
#include "Texture.h"

using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS BasicTextures
{
public:
    /*! \fn BasicTextures();
     *  \brief Ctor BasicTextures.
     */
    BasicTextures();

    /*! \fn ~BasicTextures();
     *  \brief dtor BasicTextures.
     */
    ~BasicTextures();
public:
    /*! \fn void RegisterTexture(MaterialTextureTypeEnum i_tex_type, const TextureWeakReferenceObject &i_tex_sref);
     *  \param [in] i_tex_type Texture type.
     *  \param [in] i_tex_sref target reference.
     *  \brief register texture to this BasicTextures.
     */
    void RegisterTexture(MaterialTextureTypeEnum i_tex_type, TextureWeakReferenceObject &i_tex_sref);
protected:
    /*! \var TextureWeakReferenceObject m_tex_wrefs[MaterialTextureType_MAX_DEFINE_VALUE];
     *  \brief Weak references of texture datas. We should keep texture strong reference object at
     *         other objects(like Asset).
     */
    TextureWeakReferenceObject m_tex_wrefs[MaterialTextureType_MAX_DEFINE_VALUE];
};

______________SD_END_GRAPHICS_NAMESPACE______________