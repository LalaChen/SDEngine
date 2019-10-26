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

/*! \file      Shader.h
 *  \brief     Introduce of class ShaderModule.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/24
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "ShaderModuleIdentity.h"
#include "Object.h"

using SDE::Basic::UByte;
using SDE::Basic::ObjectName;
using SDE::Basic::Object;
using SDE::Basic::FilePathString;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(ShaderModule);

/*! \class ShaderModule
 *  Class ShaderModule is used to keep shader handle(such like shader
 *  id in Opengl or handle of shader module in Vulkan). We will combine
 *  some different kind shades to a graphics pipeline(gl program).
 */
class SDENGINE_CLASS ShaderModule : public Object
{
public:
    /*! \fn explicit ShaderModule(const ObjectName i_shader_name);
     *  \brief The constructor of ShaderModule Class.
     */
    explicit ShaderModule(const ObjectName i_shader_name);

    /*! \fn ~ShaderModule();
     *  \brief The destructor of ShaderModule Class.
     */
    ~ShaderModule();
public:
    /*! \fn bool LoadBinaryShader(ShaderKindEnum i_kind, const FilePathString &i_binary_fp, const char *i_entry_name);
     *  \param [in] i_kind The kind about this shader.
     *  \param [in] i_binary_fp The file path of target binary shader about this shader.
     *  \param [in] i_entry_name Name of entry function for this shader.
     *  \brief Load binary file of this shader.
     *  \retval true Load successfully.
     *  \retval false Load failure.
     */
    bool LoadBinaryShader(ShaderKindEnum i_kind, const FilePathString &i_binary_fp, const char *i_entry_name);

    /*! \fn int32_t LoadBinaryShader(ShaderKindEnum i_kind, const  &i_binary_fp, const char *i_entry_name);
     *  \param [in] i_kind The kind about this shader.
     *  \param [in] i_content The file path of target binary shader about this shader.
     *  \param [in] i_entry_name Name of entry function for this shader.
     *  \brief Load binary content of this shader. We return value as follow :
     *  \retval 1 Successful.
     *  \retval -1 Invalid binary data.
     *  \retval -2 Empty content.
     */
    int32_t LoadBinaryShader(ShaderKindEnum i_kind, const std::vector<UByte> &i_content, const char *i_entry_name);
protected:
    /*! \var ShaderModuleIdentity m_identity;
     *  \brief Idenity of shader module.
     */
    ShaderModuleIdentity m_identity;
};

______________SD_END_GRAPHICS_NAMESPACE______________