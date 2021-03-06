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

/*! \file      ShaderModuleIdentity.h
 *  \brief     Introduce of class ShaderModuleIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/26
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ShaderKind.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class ShaderModuleIdentity
 *  \brief Class ShaderModuleIdentity is used to record some necessary information for shader.
 */
class SDENGINE_CLASS ShaderModuleIdentity
{
public:
    /*! \fn explicit ShaderModuleIdentity();
     *  \brief The constructor of ShaderModuleIdentity Class.
     */
    ShaderModuleIdentity();

    /*! \fn ~ShaderModuleIdentity();
     *  \brief The destructor of ShaderModuleIdentity Class.
     */
    ~ShaderModuleIdentity();
public:
    /*! \var CompHandle m_handle;
     *  \brief The shader handle. It is valid while the value is not equal 0.
     */
    CompHandle m_handle;

    /*! \var ShaderKindEnum m_shader_kind;
     *  \brief kind of shader.
     */
    ShaderKindEnum m_shader_kind;

    /*! \var std::string m_entry_name;
     *  \brief The name of entry function of this shader.
     */
    std::string m_entry_name;
};

______________SD_END_GRAPHICS_NAMESPACE______________