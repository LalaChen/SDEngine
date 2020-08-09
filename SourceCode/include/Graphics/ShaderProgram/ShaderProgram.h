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

/*! \file      ShaderProgram.h
 *  \brief     Introduce of class about ShaderProgram.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/27
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "DescriptorPool.h"
#include "UniformImages.h"
#include "UniformBuffer.h"
#include "UniformImagesDescriptor.h"
#include "UniformBufferDescriptor.h"
#include "DescriptorSetLayout.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS RenderSubPassPipelineInfo
{
public:
    RenderSubPassPipelineInfo() : m_pipe_id(-1) {}
    ~RenderSubPassPipelineInfo() {}
public:
    int32_t m_pipe_id;
    std::vector<DescriptorSetLayoutWeakReferenceObject> m_dsl_wrefs; //order is set number.
};

class SDENGINE_CLASS RenderPassInfo
{
public:
    RenderPassInfo() {}
    ~RenderPassInfo() {}
public:
    RenderPassWeakReferenceObject m_rp_wref;
    std::vector<RenderSubPassPipelineInfo> m_sp_pipe_infos;
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(ShaderProgram);

class SDENGINE_CLASS ShaderProgram : public Object
{
public:
    typedef std::vector<RenderPassInfo> RenderPassInfos;
    typedef std::vector<DescriptorSetLayoutStrongReferenceObject> DescriptorSetLayouts;
    typedef std::vector<DescriptorSetLayoutWeakReferenceObject> CommonDescriptorSetLayouts;
    typedef std::vector<GraphicsPipelineStrongReferenceObject> GraphicsPipelines;
public:
    explicit ShaderProgram(const ObjectName &i_name);
    virtual ~ShaderProgram();
public:
    //Input prepared data.
    void RegisterShaderProgramStructure(
        const RenderPassInfos &i_rp_infos,
        const GraphicsPipelines &i_gp_srefs,
        const CommonDescriptorSetLayouts &i_common_dsl_wrefs,
        const DescriptorSetLayouts &i_dsl_srefs);
public:
    void GetDescriptorCount(uint32_t i_d_counts[UniformBindingType_MAX_DEFINE_VALUE]) const;
    uint32_t GetPipelineAmount() const;
public: //Material Use.
    void AllocateEssentialObjects(
        std::map<ObjectName, UniformVariableWeakReferenceObject> &io_uv_wrefs,
        std::vector<DescriptorSetWeakReferenceObject> &io_desc_set_wrefs,
        DescriptorPoolWeakReferenceObject &io_pool_wref);
public:
    void UseProgramWithTargetDescriptorSet(
        const CommandBufferWeakReferenceObject &i_cb_wref,
        const RenderPassWeakReferenceObject &i_rp_wref,
        uint32_t i_sp_idx,
        const std::vector<DescriptorSetWeakReferenceObject> &i_desc_set_wrefs);

protected:
    CommonDescriptorSetLayouts m_common_dsl_wrefs;
    DescriptorSetLayouts m_dsl_srefs;
    GraphicsPipelines m_pipe_srefs;
    RenderPassInfos m_rp_infos;
    uint32_t m_descriptor_counts[UniformBindingType_MAX_DEFINE_VALUE]; //only calculate non-common number.
    bool m_registered;
};

inline uint32_t ShaderProgram::GetPipelineAmount() const
{
    return static_cast<uint32_t>(m_pipe_srefs.size());
}

______________SD_END_GRAPHICS_NAMESPACE______________