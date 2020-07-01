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

/*! \file      DescriptorSet.h
 *  \brief     Introduce of class about DescriptorSet.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/21
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsPipeline.h"
#include "UniformVariable.h"
#include "DescriptorSetIdentity.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(DescriptorSet);

class DescriptorSet : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit DescriptorSet(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of DescriptorSet
     */
    explicit DescriptorSet(const ObjectName &i_object_name);

    /*! \fn virtual ~DescriptorSet();
     *  \brief Destructor of DescriptorSet.
     */
    ~DescriptorSet();
public:
    void Initialize();
protected:
    /*! \var std::vector<std::vector<UniformVariableStrongReferenceObject>> m_sp_uniform_srefs;
     *  \brief UniformVariables of each subpass.
     */
    std::vector<std::vector<UniformVariableStrongReferenceObject>> m_sp_uniform_srefs;

    /*! \var DescriptorSetIdentity m_identity;
     *  \brief Identity of descriptor set.
     */
    DescriptorSetIdentity m_identity;
};

______________SD_END_GRAPHICS_NAMESPACE______________