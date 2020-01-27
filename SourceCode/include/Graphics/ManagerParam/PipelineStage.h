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

/*! \file      PipelineStage.h
 *  \brief     Introduce of enum PipelineStageEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/01/26
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum PipelineStageEnum
{
    PipelineStage_TOP_OF_PIPE = 0,
    PipelineStage_DRAW_INDIRECT,
    PipelineStage_VERTEX_INPUT,
    PipelineStage_VERTEX_SHADER,
    PipelineStage_TESSELLATION_CONTROL_SHADER,
    PipelineStage_TESSELLATION_EVALUATION_SHADER,
    PipelineStage_GEOMETRY_SHADER,
    PipelineStage_FRAGMENT_SHADER,
    PipelineStage_EARLY_FRAGMENT_TESTS,
    PipelineStage_LATE_FRAGMENT_TESTS,
    PipelineStage_COLOR_ATTACHMENT_OUTPUT,
    PipelineStage_COMPUTE_SHADER,
    PipelineStage_TRANSFER,
    PipelineStage_BOTTOM_OF_PIPE,
    PipelineStage_HOST,
    PipelineStage_ALL_GRAPHICS,
    PipelineStage_ALL_COMMANDS,
    PipelineStage_TRANSFORM_FEEDBACK,
    PipelineStage_CONDITIONAL_RENDERING_BIT,
    PipelineStage_MAX_DEFINE_VALUE
};

______________SD_END_GRAPHICS_NAMESPACE______________