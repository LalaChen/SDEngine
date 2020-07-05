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

/*! \file      UniformBinding.h
 *  \brief     Introduce of class UniformBinding.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/04/18
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "ShaderKind.h"
#include "UniformBindingType.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class UniformBinding
 *  \brief In our graphic system, UniformBinding is used to describing binding location and
 *         binding variable type.
 */
class SDENGINE_CLASS UniformBinding
{
public:
    /*! \fn UniformBinding();
     *  \brief Constructor of UniformBinding.
     */
    UniformBinding();

    /*! \fn ~UniformBinding();
     *  \brief Destructor of UniformBinding.
     */
    ~UniformBinding();
public:
    /*! \var uint32_t m_binding_id;
     *  \brief Binding location of this uniform binding.
     */
    uint32_t m_binding_id;

    /*! \var UniformBindingTypeEnum m_binding_type;
     *  \brief Binding type of this uniform binding.
     */
    UniformBindingTypeEnum m_binding_type;

    /*! \var uint32_t m_element_number;
     *  \brief Element number of this uniform binding.
     */
    uint32_t m_element_number;

    /*! \var std::vector<ShaderStageEnum> m_target_stages;
     *  \brief Stages can access this uniform binding.
     */
    std::vector<ShaderStageEnum> m_target_stages;
};

______________SD_END_GRAPHICS_NAMESPACE______________
