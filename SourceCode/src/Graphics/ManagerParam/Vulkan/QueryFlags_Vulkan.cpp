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

#define ENABLE_CTRL_FUNC_AND(func, mask) \
    (mask.m_enable_masks[SD_ENUM_TO_UINT(func)] & QueryControlFlags[SD_ENUM_TO_UINT(func)])

#define ENABLE_STATIC_FUNC_AND(func, mask) \
    (mask.m_enable_masks[SD_ENUM_TO_UINT(func)] & QueryPipelineStatisticFlags[SD_ENUM_TO_UINT(func)])

#include "QueryFlags_Vulkan.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VkQueryControlFlagBits QueryControlFlag_Vulkan::QueryControlFlags[QueryControlFlag_MAX_DEFINE_VALUE] = {
    VK_QUERY_CONTROL_PRECISE_BIT
};

VkQueryControlFlags QueryControlFlag_Vulkan::Convert(const QueryControlFlagMask &i_flag_mask)
{
    VkQueryControlFlags result =
        ENABLE_CTRL_FUNC_AND(QueryControlFlag_PRECISE, i_flag_mask);
    return result;
}

VkQueryPipelineStatisticFlagBits QueryPipelineStatisticFlag_Vulkan::QueryPipelineStatisticFlags[QueryPipelineStatisticFlag_MAX_DEFINE_VALUE] =
{
    VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT,
    VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT,
    VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT,
    VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT,
    VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT,
    VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT,
    VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT,
    VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT,
    VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT,
    VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT,
    VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT
};

VkQueryPipelineStatisticFlags QueryPipelineStatisticFlag_Vulkan::Convert(const QueryPipelineStatisticFlagMask &i_flag_mask)
{
    VkQueryPipelineStatisticFlags result = 
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_INPUT_ASSEMBLY_VERTICES, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_INPUT_ASSEMBLY_PRIMITIVES, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_VERTEX_SHADER_INVOCATIONS, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_GEOMETRY_SHADER_INVOCATIONS, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_GEOMETRY_SHADER_PRIMITIVES, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_CLIPPING_INVOCATIONS, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_CLIPPING_PRIMITIVES, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_FRAGMENT_SHADER_INVOCATIONS, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_TESSELLATION_CONTROL_SHADER_PATCHES, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_TESSELLATION_EVALUATION_SHADER_INVOCATIONS, i_flag_mask) |
        ENABLE_STATIC_FUNC_AND(QueryPipelineStatisticFlag_COMPUTE_SHADER_INVOCATIONS, i_flag_mask);
    
    return result;
}

______________SD_END_GRAPHICS_NAMESPACE______________