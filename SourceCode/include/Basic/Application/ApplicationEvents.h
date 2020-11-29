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

/*! \file      ApplicationEvent.h
 *  \brief     Introduce of class ApplicationEvents
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/02
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "EventObject.h"

______________SD_START_BASIC_NAMESPACE_______________

enum AppEventEnum {
    AppEvent_INITIALIZED = 0,
    AppEvent_GRAPHICS_INITIALIZED,
    AppEvent_GRAPHICS_RELESAED,
    AppEvent_TERMINATED,
    AppEvent_SCREEN_RESIZED,
    AppEvent_NONE
};

class SDENGINE_CLASS AppEventArg : public EventArg
{
public:
    explicit AppEventArg(AppEventEnum i_event = AppEvent_NONE) : EventArg(), m_app_event(i_event) {}
    virtual ~AppEventArg() {}
public:
    AppEventEnum m_app_event;
};

_______________SD_END_BASIC_NAMESPACE________________
