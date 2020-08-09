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

/*! \file      UniformVariable.h
 *  \brief     Introduce of class about UniformVariable.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/21
 *  \copyright MIT License.
 */

#pragma once

#include <list>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "UniformBindingType.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(UniformVariable);

class SDENGINE_CLASS UniformVariable : public Object
{
public:
    /*! \fn explicit UniformVariable(const ObjectName &i_object_name, uint32_t i_binding_id);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of UniformVariable.
     */
    explicit UniformVariable(const ObjectName &i_object_name, uint32_t i_binding_id);

    /*! \fn virtual ~UniformVariable();
     *  \brief Destructor of UniformVariable.
     */
    virtual ~UniformVariable();

public:
    virtual UniformBindingTypeEnum GetType() const = 0;

    virtual void Update() = 0;
public:
    uint32_t GetBindingID() const;

protected:
    uint32_t m_binding_id;
};

inline uint32_t UniformVariable::GetBindingID() const
{
    return m_binding_id;
}

______________SD_END_GRAPHICS_NAMESPACE______________