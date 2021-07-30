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

#include "LogManager.h"
#include "RenderOrder.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

const std::string RenderOrder::sOrderNames[RenderOrder_MAX_DEFINE_VALUE] = {
    "Opaque",
    "AlphaBlend",
    "VolumeRendering"
 };

const uint32_t RenderOrder::sOrderNumbers[RenderOrder_MAX_DEFINE_VALUE] = {
    0,
    1000,
    2000
 };

RenderOrder::RenderOrder(const std::string &i_desc) {
    for (uint32_t count = 0; count < RenderOrder_MAX_DEFINE_VALUE; ++count) {
        if (sOrderNames[count].compare(i_desc) == 0) {
            m_order = sOrderNumbers[count];
            break;
            
        }
    }
}

void RenderOrder::SetOrder(RenderOrderEnum i_order_group, uint32_t i_offset)
{
    if (i_order_group != RenderOrder_MAX_DEFINE_VALUE) {
        m_order = sOrderNumbers[i_order_group] + i_offset;
    }
    else {
        SDLOGE("Error order.");
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________