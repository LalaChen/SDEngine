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

/*! \file      QueryFlags.h
 *  \brief     Introduce of classes about Query Flags.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/05/02
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum QueryControlFlagEnum
{
    QueryControlFlag_PRECISE = 0,
    QueryControlFlag_MAX_DEFINE_VALUE
};

class QueryControlFlagMask
{
public:
    QueryControlFlagMask();
    ~QueryControlFlagMask();
public:
    void Enable(const std::vector<QueryControlFlagEnum> &i_funcs);
public:
    uint32_t m_enable_masks[QueryControlFlag_MAX_DEFINE_VALUE];
};

inline QueryControlFlagMask::QueryControlFlagMask()
: m_enable_masks{0u}
{
}

inline QueryControlFlagMask::~QueryControlFlagMask()
{
}

inline void QueryControlFlagMask::Enable(const std::vector<QueryControlFlagEnum> &i_funcs)
{
    for (const QueryControlFlagEnum &func : i_funcs) {
        if (func != QueryControlFlag_MAX_DEFINE_VALUE) {
            m_enable_masks[func] = UINT32_MAX;
        }
    }
}

enum QueryPipelineStatisticFlagEnum
{
    QueryPipelineStatisticFlag_INPUT_ASSEMBLY_VERTICES = 0, /*!< Assemble vertices.*/
    QueryPipelineStatisticFlag_INPUT_ASSEMBLY_PRIMITIVES, /*!< Assemble primitives.*/
    QueryPipelineStatisticFlag_VERTEX_SHADER_INVOCATIONS,
    QueryPipelineStatisticFlag_GEOMETRY_SHADER_INVOCATIONS,
    QueryPipelineStatisticFlag_GEOMETRY_SHADER_PRIMITIVES,
    QueryPipelineStatisticFlag_CLIPPING_INVOCATIONS,
    QueryPipelineStatisticFlag_CLIPPING_PRIMITIVES,
    QueryPipelineStatisticFlag_FRAGMENT_SHADER_INVOCATIONS,
    QueryPipelineStatisticFlag_TESSELLATION_CONTROL_SHADER_PATCHES,
    QueryPipelineStatisticFlag_TESSELLATION_EVALUATION_SHADER_INVOCATIONS,
    QueryPipelineStatisticFlag_COMPUTE_SHADER_INVOCATIONS,
    QueryPipelineStatisticFlag_MAX_DEFINE_VALUE
};

class QueryPipelineStatisticFlagMask
{
public:
    QueryPipelineStatisticFlagMask();
    ~QueryPipelineStatisticFlagMask();
public:
    void Enable(const std::vector<QueryPipelineStatisticFlagEnum> &i_funcs);
public:
    uint32_t m_enable_masks[QueryPipelineStatisticFlag_MAX_DEFINE_VALUE];
};

inline QueryPipelineStatisticFlagMask::QueryPipelineStatisticFlagMask()
: m_enable_masks{0u}
{
}

inline QueryPipelineStatisticFlagMask::~QueryPipelineStatisticFlagMask()
{
}

inline void QueryPipelineStatisticFlagMask::Enable(const std::vector<QueryPipelineStatisticFlagEnum> &i_funcs)
{
    for (const QueryPipelineStatisticFlagEnum &func : i_funcs) {
        if (func != QueryPipelineStatisticFlag_MAX_DEFINE_VALUE) {
            m_enable_masks[func] = UINT32_MAX;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________