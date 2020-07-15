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

/*! \file      Material.h
 *  \brief     Introduce of class Material.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/21
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "DescriptorPool.h"
#include "UniformImages.h"
#include "UniformBuffer.h"
#include "ShaderProgram.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Material);

/*! \class Material
 *  \brief In our graphic system, Material is used to be the bridge between data and pipeline.
 *         We keep data in material and used it when we update data into graphics card.
 */
class SDENGINE_CLASS Material : public Object
{
public:
    friend class GraphicsManager;
public:
    explicit Material(const ObjectName &i_object_name);
    virtual ~Material();
public:
    void Initialize(const ShaderProgramWeakReferenceObject &i_sp_wref);
protected:
    std::map<ObjectName, UniformVariableStrongReferenceObject> m_uv_srefs;
    ShaderProgramWeakReferenceObject m_sp_wref;
    DescriptorPoolStrongReferenceObject m_dsp_sref;
};

______________SD_END_GRAPHICS_NAMESPACE______________