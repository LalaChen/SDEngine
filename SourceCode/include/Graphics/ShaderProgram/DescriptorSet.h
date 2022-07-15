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

#include <map>

#include "Object.h"
#include "WeakReferenceObject.h"
#include "StrongReferenceObject.h"
#include "GraphicsPipeline.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSetIdentity.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;
using SDE::Basic::StrongReferenceObject;
using SDE::Basic::WeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(DescriptorSet);

class SDENGINE_CLASS DescriptorSet : public Object
{
public:
    friend class GraphicsIdentityGetter;
public:
    /*! \fn explicit DescriptorSet(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of DescriptorSet.
     */
    explicit DescriptorSet(const ObjectName &i_object_name);

    /*! \fn virtual ~DescriptorSet();
     *  \brief Destructor of DescriptorSet.
     */
    ~DescriptorSet();
public:
    /*! \fn void Initialize(const WeakReferenceObject<Object> &i_pool, const DescriptorSetLayoutWeakReferenceObject &i_layout);
     *  \param [in] i_pool Pool owner this descriptor set.
     *  \param [in] i_layout Layout of this descriptor set.
     *  \brief Destructor of DescriptorSet.
     */
    void Initialize(const WeakReferenceObject<Object> &i_pool, const DescriptorSetLayoutWeakReferenceObject &i_layout);

    /*! \fn void WriteDescriptor();
     *  \brief Bind uniform variables of DescriptorSet. Please call this after data setted into variables.
     */
    void WriteDescriptor();

    /*! \fn void GetAllocatedUniformVariables(std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uvs) const;
     *  \param [inout] io_uvs Container for storing link variables.
     *  \brief Get weak reference links.
     */
    void GetAllocatedUniformVariables(std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uvs) const;

    bool IsThisLayout(const DescriptorSetLayoutWeakReferenceObject &i_layout) const;
protected:
    /*! \var WeakReferenceObject<Object> m_pool;
     *  \brief pool weak reference of this descriptor set.
     */
    WeakReferenceObject<Object> m_pool;

    /*! \var std::vector<UniformVariableStrongReferenceObject> m_uvs;
     *  \brief uniform objects of this descriptor set.
     */
    std::vector<UniformVariableStrongReferenceObject> m_uvs;

    /*! \var DescriptorSetLayoutWeakReferenceObject m_layout;
     *  \brief Layout weak reference of this descriptor set.
     */
    DescriptorSetLayoutWeakReferenceObject m_layout;

    /*! \var DescriptorSetIdentity m_identity;
     *  \brief Identity of descriptor set.
     */
    DescriptorSetIdentity m_identity;
};

inline bool DescriptorSet::IsThisLayout(const DescriptorSetLayoutWeakReferenceObject &i_layout) const
{
    return (m_layout == i_layout);
}

______________SD_END_GRAPHICS_NAMESPACE______________