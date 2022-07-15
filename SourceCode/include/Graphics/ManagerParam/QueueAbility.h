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

/*! \file      QueueAbility.h
 *  \brief     Introduce of classes about QueueAbility.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/25
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

enum QueueAbilityEnum
{
    QueueAbility_GRAPHICS = 0,
    QueueAbility_COMPUTE,
    QueueAbility_TRANSFER,
    QueueAbility_SPARSE_BINDING,
    QueueAbility_PROTECTED,
    QueueAbility_MAX_DEFINE_VALUE
};

class QueueAbility
{
public:
    QueueAbility();
    ~QueueAbility();
public:
    void Enable(const std::vector<QueueAbilityEnum>& i_funcs);
public:
    uint32_t m_enable_masks[QueueAbility_MAX_DEFINE_VALUE];
};

inline QueueAbility::QueueAbility()
: m_enable_masks{ 0u }
{
}

inline QueueAbility::~QueueAbility()
{
}

inline void QueueAbility::Enable(const std::vector<QueueAbilityEnum>& i_funcs)
{
    for (const QueueAbilityEnum &func : i_funcs) {
        if (func != QueueAbility_MAX_DEFINE_VALUE) {
            m_enable_masks[func] = UINT32_MAX;
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________