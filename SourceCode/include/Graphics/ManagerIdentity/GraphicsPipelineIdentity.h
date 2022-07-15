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

/*! \file      GraphicsPipelineIdentity.h
 *  \brief     Introduce of classes about handle of shader program.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/16
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "GraphicsElementIdentity.h"

#include "GraphicsPipelineParam.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class GraphicsPipelineIdentity
 *  \brief Class GraphicsPipelineIdentity is used to record some necessary information
           for pipeline.
 */
class SDENGINE_CLASS GraphicsPipelineIdentity : public GraphicsElementIdentity
{
public:
   /*! \fn explicit GraphicsPipelineIdentity();
    *  \brief The constructor of GraphicsPipelineIdentity Class.
    */
    GraphicsPipelineIdentity();

    /*! \fn ~GraphicsPipelineIdentity();
     *  \brief The destructor of GraphicsPipelineIdentity Class.
     */
    ~GraphicsPipelineIdentity();
public:
    /*! \var CompHandle m_handle;
     *  \brief The pipeline handle. It is valid while the value is not equal 0.
     */
    CompHandle m_handle;

    /* \var CompHandle m_device;
     * \brief Device this handle belong.
     */
    CompHandle m_device;

    /*! \var CompHandle m_pipeline_layout;
     *  \brief The pipeline layout handle. It is valid while the value is not equal 0.(Opengl doesn't need.)
     */
    CompHandle m_pipeline_layout;

    /*! \var uint32_t m_subpass_id;
     *  \brief Claim this pipeline is used at which sub pass of this render pass.
     */
    uint32_t m_subpass_id;

    /*! \var GraphicsPipelineParam m_params;
     *  \brief Problem about GraphicsPipeline.
     */
    GraphicsPipelineParam m_params;
};

______________SD_END_GRAPHICS_NAMESPACE______________