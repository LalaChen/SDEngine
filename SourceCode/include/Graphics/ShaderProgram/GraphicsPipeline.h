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

/*! \file      GraphicsPipeline.h
 *  \brief     Introduce of classes about shader program.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/16
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderPass.h"
#include "ShaderModule.h"
#include "CommandBuffer.h"
#include "DescriptorSetLayout.h"
#include "DescriptorSet.h"
#include "GraphicsPipelineParam.h"
#include "GraphicsPipelineIdentity.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS ShaderModules
{
public:
    ShaderModuleStrongReferenceObject m_shaders[ShaderKind_GRAPHICS_SHADER_NUMBER];
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GraphicsPipeline);

/*! \class GraphicsPipeline
 *  \brief In our graphic system, GraphicsPipeline is used to tell gpu how to draw a primitive to a fragment
 *         in target attachment(s).
 */
class SDENGINE_CLASS GraphicsPipeline : public Object
{
public:
    friend class GraphicsManager;
public:
    /*! \fn explicit GraphicsPipeline(const ObjectName &i_object_name);
     *  \param [in] i_object_name Name of this object.
     *  \brief Constructor of GraphicsPipeline
     */
    explicit GraphicsPipeline(const ObjectName &i_object_name);

    /*! \fn virtual ~GraphicsPipeline();
     *  \brief Destructor of GraphicsPipeline.
     */
    virtual ~GraphicsPipeline();
public:
    /*! \fn void Initialize(const ShaderModules &i_shaders);
     *  \param [in] i_shaders ShaderModules of this pipeline.
     *  \brief Initialize this pipeline.
     */
    void Initialize(const ShaderModules &i_shaders);
public:
    /*! \fn void SetGraphicsPipelineParams(const GraphicsPipelineParam &i_params, const RenderPassWeakReferenceObject &i_rp_wref, uint32_t i_sp_id);
     *  \param [in] i_params Description about this pipeline.
     *  \param [in] i_rp_wref Weak reference of render pass which is the reference of this pipeline.
     *  \param [in] i_dsl_wrefs Weak reference of Descriptors Set Layout for recording uniform variables using in this pipeline.
     *  \param [in] i_sp_id Target step that we can use this shader.
     *  \brief Set parameters for creating graphics pipeline and allocate uniform variable descriptor 
     *         weak references for let use registing.
     */
    void SetGraphicsPipelineParams(
        const GraphicsPipelineParam &i_params,
        const RenderPassWeakReferenceObject &i_rp_wref,
        const std::vector<DescriptorSetLayoutWeakReferenceObject> &i_dsl_wrefs,
        uint32_t i_sp_id);

public:
    /*! \fn const CompHandle GetHandle() const;
     *  \brief return handle of pipeline.
     */
    const CompHandle GetHandle() const;

    /*! \fn const CompHandle GetPipelineLayoutHandle() const;
     *  \brief return handle of pipeline layout of this pipeline.
     */
    const CompHandle GetPipelineLayoutHandle() const;

    /*! \fn const GraphicsPipelineParam& GetPipelineParams() const;
     *  \brief return pipeline parameters.
     */
    const GraphicsPipelineParam& GetPipelineParams() const;

    /*! \fn uint32_t GetSubpassID() const;
     *  \brief return subpass id that we use this pipeline.
     */
    uint32_t GetSubpassID() const;
public:
    /*! \fn void UseAndBindDescriptorSets(const CommandBufferWeakReferenceObject &i_cb_wref, const std::vector<DescriptorSetWeakReferenceObject> &i_ds_wrefs) const;
     *  \param [in] i_cb_wref Target command buffer.
     *  \brief return pipeline parameters.
     */
    void UseAndBindDescriptorSets(const CommandBufferWeakReferenceObject &i_cb_wref, const std::vector<DescriptorSetWeakReferenceObject> &i_ds_wrefs) const;
protected:
    /*! \var GraphicsPipelineIdentity m_identity;
     *  \brief Record basic identity
     */
    GraphicsPipelineIdentity m_identity;
    
    /*! \var RenderPassWeakReferenceObject m_target_rp_wref;
     *  \brief The render pass we want to use this in pipeline.
     */
    RenderPassWeakReferenceObject m_target_rp_wref;

    /*! \var std::vector<DescriptorSetLayoutWeakReferenceObject> m_dsl_wrefs;
     *  \brief The descriptor set weak references. The real dsl are kept at ShaderProgram and other object.
     *         The order in array is the order of set.
     */
    std::vector<DescriptorSetLayoutWeakReferenceObject> m_dsl_wrefs;

    /*! \var bool m_initialized;
     *  \brief Return this pipeline is initialized.
     */
    bool m_initialized;
};

inline const CompHandle GraphicsPipeline::GetHandle() const
{
    return m_identity.m_handle;
}

inline const CompHandle GraphicsPipeline::GetPipelineLayoutHandle() const
{
    return m_identity.m_pipeline_layout_handle;
}

inline const GraphicsPipelineParam& GraphicsPipeline::GetPipelineParams() const
{
    return m_identity.m_params;
}

inline uint32_t GraphicsPipeline::GetSubpassID() const
{
    return m_identity.m_subpass_id;
}

______________SD_END_GRAPHICS_NAMESPACE______________