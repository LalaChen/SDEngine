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

/*! \file      EventArg.h
 *  \brief     Introduce of class EventArg
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/07
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class EventArg
 *  \brief In our event system, we allow user delievrying parameters by inheriting Class EventArg.
 *  It's used for unifying param of interface for all slot. User can check the real arg type while
 *  slot triggered.
 */
class SDENGINE_CLASS EventArg
{
public:
    /*! \fn EventArg();
     *  \brief The constructor of EventArg Class.
     */
    EventArg()
    {
    }

    /*! \fn virtual ~EventArg();
     *  \brief The destructor of EventArg Class.
     */
    virtual ~EventArg()
    {
    }
};

_______________SD_END_BASIC_NAMESPACE________________