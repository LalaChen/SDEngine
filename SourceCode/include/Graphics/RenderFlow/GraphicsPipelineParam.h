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
    /*! \fn float m_x;
     *  \brief left x value in screen.
     */
    float m_x;

    /*! \fn float m_y;
     *  \brief up y value in screen.
     */
    float m_y;

    /*! \fn float m_width;
     *  \brief x-length about rendering region.
     */
    float m_width;

    /*! \fn float m_height;
     *  \brief y-length about rendering region.
     */
    float m_height;

    /*! \fn float m_min_depth;
     *  \brief min depth value about rendering region.
     */
    float m_min_depth;

    /*! \fn float m_max_depth;
     *  \brief max depth value about rendering region.
     */
    float m_max_depth;
};

class SDENGINE_CLASS ScissorRegion
{
public:
    ScissorRegion();
    ~ScissorRegion();
public:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
};

class SDENGINE_CLASS PrimitiveInfo
{
public:
    PrimitiveInfo();
    ~PrimitiveInfo();
public:
    PrimitiveEnum m_primitive;
};

class SDENGINE_CLASS RasterizationInfo
{
public:
    RasterizationInfo();
    ~RasterizationInfo();
public:
    bool m_enable_discard; //close rasterization or not.
    FrontFaceModeEnum m_front_face;
    FaceCullingEnum m_face_culling;
    bool m_enable_depth_bias;
    float m_depth_bias_constant_factor;
    float m_depth_bias_slope_factor;
    float m_depth_bias_clamp;
    float m_line_width;
};

class SDENGINE_CLASS SampleShadingInfo
{
public:
    SampleShadingInfo();
    ~SampleShadingInfo();
public:
    SampleCountEnum m_sample_count;
    bool m_sample_shading_enable;
    float m_min_sample_shading;
    std::vector<uint32_t> m_sample_mask;
    bool m_alpha_to_coverage_enable;
    bool m_alpha_to_one_enable;
};

class SDENGINE_CLASS StencilOpInfo
{
public:
    StencilOpInfo();
    ~StencilOpInfo();
public:

};

class SDENGINE_CLASS DepthStencilInfo
{
public:
    DepthStencilInfo();
    ~DepthStencilInfo();
public:
    bool m_depth_test_enable;
    bool m_depth_write_enable;
    CompareOperatorEnum m_comp_op;
    float m_min_depth_bounds;
    float m_max_depth_bounds;
    bool m_stencil_test_enable;
    StencilOpInfo m_front;
    StencilOpInfo m_back;
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
};

______________SD_END_GRAPHICS_NAMESPACE______________