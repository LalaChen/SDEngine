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

/*! \file      GraphicsPipelineParam.h
 *  \brief     Introduce of class GraphicsPipelineParam.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/27
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

#include "CompareOperator.h"
#include "Primitive.h"
#include "PolygonMode.h"
#include "FrontFaceMode.h"
#include "FaceCulling.h"
#include "SampleCount.h"
#include "StencilOperator.h"
#include "BlendFactor.h"
#include "BlendOperator.h"
#include "LogicOperator.h"
#include "DynamicState.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class Viewport
 *  \brief Viewport is used to tell gpu we want to draw scene at the region.
 */
class SDENGINE_CLASS Viewport
{
public:
    /*! \fn Viewport();
     *  \brief The constructor of Viewport Class.
     */
    Viewport();

    /*! \fn ~Viewport();
     *  \brief The destructor of Viewport Class.
     */
    ~Viewport();
public:
    /*! \var float m_x;
     *  \brief left x value in screen.
     */
    float m_x;

    /*! \var float m_y;
     *  \brief up y value in screen.
     */
    float m_y;

    /*! \var float m_width;
     *  \brief x-length about rendering region.
     */
    float m_width;

    /*! \var float m_height;
     *  \brief y-length about rendering region.
     */
    float m_height;

    /*! \var float m_min_depth;
     *  \brief min depth value about rendering region.
     */
    float m_min_depth;

    /*! \var float m_max_depth;
     *  \brief max depth value about rendering region.
     */
    float m_max_depth;
};

/*! \class ScissorRegion
 *  \brief ScissorRegion is used to tell GPU we want to use shader at this region.
 */
class SDENGINE_CLASS ScissorRegion
{
public:
    /*! \fn ScissorRegion();
     *  \brief The constructor of ScissorRegion Class.
     */
    ScissorRegion();

    /*! \fn ~ScissorRegion();
     *  \brief The destructor of ScissorRegion Class.
     */
    ~ScissorRegion();
public:
    /*! \var float m_x;
     *  \brief left x value in screen.
     */
    float m_x;

    /*! \var float m_y;
     *  \brief left y value in screen.
     */
    float m_y;

    /*! \var float m_width;
     *  \brief Width in screen.
     */
    float m_width;

    /*! \var float m_height;
     *  \brief Height in screen.
     */
    float m_height;
};


/*! \class PrimitiveInfo
 *  \brief PrimitiveInfo is used to tell GPU what primitive we want to draw with this shader.
 */
class SDENGINE_CLASS PrimitiveInfo
{
public:
    /*! \fn PrimitiveInfo();
     *  \brief The constructor of PrimitiveInfo Class.
     */
    PrimitiveInfo();

    /*! \fn ~PrimitiveInfo();
     *  \brief The destructor of PrimitiveInfo Class.
     */
    ~PrimitiveInfo();
public:
    /*! \var PrimitiveEnum m_primitive;
     *  \brief Record the primitive. Default is triangle.
     */
    PrimitiveEnum m_primitive;
};

/*! \class RasterizationInfo
 *  \brief RasterizationInfo is used to decide some setting about rasterization.
 */
class SDENGINE_CLASS RasterizationInfo
{
public:
    /*! \fn RasterizationInfo();
     *  \brief The constructor of RasterizationInfo Class.
     */
    RasterizationInfo();

    /*! \fn ~RasterizationInfo();
     *  \brief The destructor of RasterizationInfo Class.
     */
    ~RasterizationInfo();
public:
    /*! \var bool m_enable_discard;
     *  \brief Enable discard or not. Default is false.
     */
    bool m_enable_discard; //close rasterization or not.

    /*! \var FrontFaceModeEnum m_front_face;
     *  \brief Tell use what kind of face is front face. Default is counter clockwise.
     */
    FrontFaceModeEnum m_front_face;

    /*! \var FaceCullingEnum m_face_culling;
     *  \brief Tell use what kind of face we want to cull. Default is back face.
     */
    FaceCullingEnum m_face_culling;

    /*! \var bool m_enable_depth_bias;
     *  \brief Tell us we enable depth bias or not. Default is false.
     */
    bool m_enable_depth_bias;

    /*! \var float m_depth_bias_constant_factor;
     *  \brief Depth bias formula is D = S * b + C. C is constant factor.
     */
    float m_depth_bias_constant_factor;

    /*! \var float m_depth_bias_slope_factor;
     *  \brief Depth bias formula is D = S * b + C. S is slope factor.
     */
    float m_depth_bias_slope_factor;

    /*! \var float m_depth_bias_clamp;
     *  \brief Clamp value about bias.
     */
    float m_depth_bias_clamp;

    /*! \varfloat m_line_width;
     *  \brief Line width.
     */
    float m_line_width;
};

/*! \class SampleShadingInfo
 *  \brief SampleShadingInfo is used to decide some setting about sample shading.
 */
class SDENGINE_CLASS SampleShadingInfo
{
public:
    /*! \fn SampleShadingInfo();
     *  \brief The constructor of SampleShadingInfo Class.
     */
    SampleShadingInfo();

    /*! \fn ~SampleShadingInfo();
     *  \brief The destructor of SampleShadingInfo Class.
     */
    ~SampleShadingInfo();
public:
    /*! \var SampleCountEnum m_sample_count;
     *  \brief Tell use which sample count we want to use. Default is SampleCount_1.
     */
    SampleCountEnum m_sample_count;

    /*! \var bool m_sample_shading_enable;
     *  \brief Tell use did we want to enable sample shading.
     */
    bool m_sample_shading_enable;

    /*! \var float m_min_sample_shading;
     *  \brief Min sample value.
     */
    float m_min_sample_shading;

    /*! \var std::vector<uint32_t> m_sample_mask;
     *  \brief Sample mask.
     */
    std::vector<uint32_t> m_sample_mask;

    /*! \var bool m_alpha_to_coverage_enable;
     *  \brief Alpha to coverage .
     */
    bool m_alpha_to_coverage_enable;

    /*! \var bool m_alpha_to_one_enable;
     *  \brief Alpha to one.
     */
    bool m_alpha_to_one_enable;
};

/*! \class StencilOpInfo
 *  \brief Information to describe stencil operator in some condition.
 */
class SDENGINE_CLASS StencilOpInfo
{
public:
    /*! \fn StencilOpInfo();
     *  \brief The constructor of StencilOpInfo Class.
     */
    StencilOpInfo();

    /*! \fn ~StencilOpInfo();
     *  \brief The destructor of StencilOpInfo Class.
     */
    ~StencilOpInfo();
public:
    /*! \var StencilOperatorEnum m_fail_op;
     *  \brief Operator executed when stencil test fail.
     */
    StencilOperatorEnum m_fail_op;

    /*! \var StencilOperatorEnum m_pass_op;
     *  \brief Operator executed when stencil test pass.
     */
    StencilOperatorEnum m_pass_op;

    /*! \var StencilOperatorEnum m_pass_op;
     *  \brief Operator executed when depth test fail.
     */
    StencilOperatorEnum m_depth_fail_op;

    /*! \var StencilOperatorEnum m_pass_op;
     *  \brief Comparing operator for stencil test.
     */
    CompareOperatorEnum m_compare;

    /*! \var uint32_t m_compare_mask;
     *  \brief Tell gpu which bits are used in stencil test.
     */
    uint32_t m_compare_mask;

    /*! \var uint32_t m_write_mask;
     *  \brief Tell gpu which bits can be write in stencil test.
     */
    uint32_t m_write_mask;

    /*! \var uint32_t m_reference;
     *  \brief The integer used in integer stencil test.
     */
    uint32_t m_reference;
};

/*! \class DepthStencilInfo
 *  \brief Information to describe stencil and depth test.
 */
class SDENGINE_CLASS DepthStencilInfo
{
public:
    /*! \fn DepthStencilInfo();
     *  \brief The constructor of DepthStencilInfo Class.
     */
    DepthStencilInfo();

    /*! \fn ~DepthStencilInfo();
     *  \brief The destructor of DepthStencilInfo Class.
     */
    ~DepthStencilInfo();
public:
    /*! \var bool m_depth_test_enable;
     *  \brief Use depth test or not. Default false.
     */
    bool m_depth_test_enable;

    /*! \var bool m_depth_write_enable;
     *  \brief Write depth to buffer or not. Default true.
     */
    bool m_depth_write_enable;

    /*! \var CompareOperatorEnum m_comp_op;
     *  \brief Tell gpu how to pass depth test.
     */
    CompareOperatorEnum m_comp_op;

    /*! \var float m_min_depth_bounds;
     *  \brief Tell gpu minimum of depth value.
     */
    float m_min_depth_bounds;

    /*! \var float m_min_depth_bounds;
     *  \brief Tell gpu maximum of depth value.
     */
    float m_max_depth_bounds;

    /*! \var bool m_stencil_test_enable;
     *  \brief Use stencil test or not. Default false.
     */
    bool m_stencil_test_enable;

    /*! \var StencilOpInfo m_front;
     *  \brief Front face stencil operator.
     */
    StencilOpInfo m_front;

    /*! \var StencilOpInfo m_back;
     *  \brief Back face stencil operator.
     */
    StencilOpInfo m_back;
};

/*! \class ColorBlendAttachmentInfo
 *  \brief Information to describe blending operator for RGB and alpha.
 */
class SDENGINE_CLASS ColorBlendAttachmentInfo
{
public:
    /*! \fn ColorBlendAttachmentInfo();
     *  \brief The constructor of ColorBlendAttachmentInfo Class.
     */
    ColorBlendAttachmentInfo();

    /*! \fn ~ColorBlendAttachmentInfo();
     *  \brief The destructor of ColorBlendAttachmentInfo Class.
     */
    ~ColorBlendAttachmentInfo();
public:
    /*! \var bool m_color_mask[4];
     *  \brief Specify which channel we don't want to write.
     */
    bool m_color_mask[4]; //R,G,B,A

    /*! \var bool m_blend_enable;
     *  \brief Specify blending state we need to blend.
     */
    bool m_blend_enable;

    /*! \var BlendFactorEnum m_src_color_factor;
     *  \brief Specify blending state we need to blend.
     */
    BlendFactorEnum m_src_color_factor;
    BlendFactorEnum m_dst_color_factor;
    BlendOperatorEnum m_color_op;
    BlendFactorEnum m_src_alpha_factor;
    BlendFactorEnum m_dst_alpha_factor;
    BlendOperatorEnum m_alpha_op;
};

/*! \class ColorBlendState
 *  \brief Information to describe blending info for each output color attachment.
 */
class SDENGINE_CLASS ColorBlendState
{
public:
    ColorBlendState();
    ~ColorBlendState();
public:
    bool m_logic_op_enable;
    LogicOperatorEnum m_logic_op;
    std::vector<ColorBlendAttachmentInfo> m_blend_infos;
    float m_blend_constants[4];
};

class SDENGINE_CLASS GraphicsPipelineParam
{
public:
    GraphicsPipelineParam();
    ~GraphicsPipelineParam();
public:
    Viewport m_viewport;
    ScissorRegion m_scissor_region;
    PrimitiveInfo m_primitive_info;
    RasterizationInfo m_rasterization_info;
    DepthStencilInfo m_depth_stencil_info;
    std::vector<ColorBlendState> m_att_color_b_states;
    std::vector<DynamicStateEnum> m_dynamic_states;
};

______________SD_END_GRAPHICS_NAMESPACE______________