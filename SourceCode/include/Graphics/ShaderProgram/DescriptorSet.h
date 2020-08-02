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

#include "Object.h"
#include "WeakReferenceObject.h"
#include "StrongReferenceObject.h"
#include "GraphicsPipeline.h"
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
    void Initialize(
        const WeakReferenceObject<Object> &i_pool_wref,
        const GraphicsPipelineWeakReferenceObject &i_pipe_wref);

    void RegisterUniformVariable(const UniformVariableWeakReferenceObject &i_uv_wref, uint32_t i_binding_id);

    void WriteDescriptor();

    void Bind(const CommandBufferWeakReferenceObject &i_cb_wref) const;
protected:
    WeakReferenceObject<Object> m_pool_wref;

    std::vector<UniformVariableWeakReferenceObject> m_uv_wrefs;

    GraphicsPipelineWeakReferenceObject m_target_pipe_wref;

    /*! \var DescriptorSetIdentity m_identity;
     *  \brief Identity of descriptor set.
     */
    DescriptorSetIdentity m_identity;
};

______________SD_END_GRAPHICS_NAMESPACE______________