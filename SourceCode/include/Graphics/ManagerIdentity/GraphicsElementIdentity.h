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

/*! \file      GraphicsElementIdentity.h
 *  \brief     Introduce of class about GraphicsElementIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/11
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class GraphicsEleIdentity
 *  Keep all graphics handle or ID in this struct.
 */
class SDENGINE_CLASS GraphicsElementIdentity
{
public:
    GraphicsElementIdentity()
    : m_valid(false)
    {
    }
    
    ~GraphicsElementIdentity()
    {
    }
public:
    void SetValid()
    {
        m_valid = true;
    }

    void SetInvalid()
    {
        m_valid = false;
    }
    
    bool IsValid() const
    {
        return m_valid;
    }
public:
    /*! \var bool m_valid;
     *  \brief Is valid handle or not.
     */
    bool m_valid;
};

______________SD_END_GRAPHICS_NAMESPACE______________