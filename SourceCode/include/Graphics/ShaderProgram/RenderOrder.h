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

/*! \file      RenderOrder.h
 *  \brief     Introduce of class about RenderOrder.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/07/30
 *  \copyright MIT License.
 */

#pragma once

#include <string>

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum RenderOrderEnum
{
    RenderOrder_OPAQUE = 0,
    RenderOrder_ALPHA_BLEND,
    RenderOrder_VOLUME_RENDERING,
    RenderOrder_MAX_DEFINE_VALUE
};

class SDENGINE_CLASS RenderOrder
{
public:
    static const std::string sOrderNames[RenderOrder_MAX_DEFINE_VALUE];
    static const uint32_t sOrderNumbers[RenderOrder_MAX_DEFINE_VALUE];
public:
    RenderOrder() :m_order(0) {}
    RenderOrder(const std::string &i_desc);
    ~RenderOrder() {}
public:
    uint32_t GetOrder() const;
    void SetOrder(RenderOrderEnum i_order_group, uint32_t i_offset);
protected:
    uint32_t m_order;
};

inline uint32_t RenderOrder::GetOrder() const
{
    return m_order;
}

______________SD_END_GRAPHICS_NAMESPACE______________