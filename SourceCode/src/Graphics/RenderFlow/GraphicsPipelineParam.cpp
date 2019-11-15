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

#include "GraphicsPipelineParam.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________
//--- Viewport
Viewport::Viewport()
: m_x(0.0f)
, m_y(0.0f)
, m_width(0.0f)
, m_height(0.0f)
, m_min_depth(0.0f)
, m_max_depth(1.0f)
{
}

Viewport::~Viewport()
{
}

//--- ScissorRegion
ScissorRegion::ScissorRegion()
: m_x(0.0f)
, m_y(0.0f)
, m_width(0.0f)
, m_height(0.0f)
{
}

ScissorRegion::~ScissorRegion()
{
}

//--- PrimitiveInfo
PrimitiveInfo::PrimitiveInfo()
: m_primitive(Primitive_TRIANGLE)
{
}

PrimitiveInfo::~PrimitiveInfo()
{
}

//--- RasterizationInfo
RasterizationInfo::RasterizationInfo()
: m_enable_discard(false)
, m_front_face(FrontFaceMode_COUNTER_CLOCKWISE)
, m_face_culling(FaceCulling_BACK_FACE)
, m_enable_depth_bias(false)
, m_depth_bias_constant_factor(0.0f)
, m_depth_bias_slope_factor(1.0f)
, m_depth_bias_clamp(1.0f)
, m_line_width(1.0f)
{
}

RasterizationInfo::~RasterizationInfo()
{
}

//--- SampleShadingInfo
SampleShadingInfo::SampleShadingInfo()
: m_sample_count(SampleCount_1)
, m_sample_shading_enable(false)
, m_min_sample_shading(0.0f)
, m_alpha_to_coverage_enable(false)
, m_alpha_to_one_enable(false)
{
}

SampleShadingInfo::~SampleShadingInfo()
{
}

//--- StencilOpInfo
StencilOpInfo::StencilOpInfo()
: m_fail_op(StencilOperator_KEEP)
, m_pass_op(StencilOperator_KEEP)
, m_depth_fail_op(StencilOperator_KEEP)
, m_compare(CompareOperator_ALWAYS)
, m_compare_mask(0u)
, m_write_mask(0u)
, m_reference(0u)
{
}

StencilOpInfo::~StencilOpInfo()
{
}

//--- DepthStencilInfo
DepthStencilInfo::DepthStencilInfo()
: m_depth_test_enable(false)
, m_depth_write_enable(true)
, m_comp_op(CompareOperator_LESS_OR_EQUAL)
, m_min_depth_bounds(-1.0f)
, m_max_depth_bounds(1.0f)
, m_stencil_test_enable(false)
{
}

DepthStencilInfo::~DepthStencilInfo()
{
}

//--- ColorBlendAttachmentInfo
ColorBlendAttachmentInfo::ColorBlendAttachmentInfo()
: m_color_mask{true, true, true, true}
, m_blend_enable(false)
, m_src_color_factor(BlendFactor_SRC_ALPHA)
, m_dst_color_factor(BlendFactor_ONE_MINUS_SRC_ALPHA)
, m_color_op(BlendOperator_ADD)
, m_src_alpha_factor(BlendFactor_SRC_ALPHA)
, m_dst_alpha_factor(BlendFactor_ONE_MINUS_SRC_ALPHA)
, m_alpha_op(BlendOperator_ADD)
{
}

ColorBlendAttachmentInfo::~ColorBlendAttachmentInfo()
{
}

//--- ColorBlendState
ColorBlendState::ColorBlendState()
: m_logic_op_enable(false)
, m_logic_op(LogicOperator_CLEAR)
, m_blend_constants{0.0f, 0.0f, 0.0f, 0.0f}
{
}

ColorBlendState::~ColorBlendState()
{
}

//--- GraphicsPipelineParam
GraphicsPipelineParam::GraphicsPipelineParam()
{
}

GraphicsPipelineParam::~GraphicsPipelineParam()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________