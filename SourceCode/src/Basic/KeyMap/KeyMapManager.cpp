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

#include "KeyEventArg.h"
#include "KeyMapManager.h"

______________SD_START_BASIC_NAMESPACE_______________

KeyMapManager::KeyMapManager(const ObjectName &i_object_name)
: EventObject(i_object_name)
, m_key_maps{false}
{
    EventStrongReferenceObject key_event = new Event("KeyEvent");
    m_key_event_wref = key_event.StaticCastToWeakPtr<Event>();
    RegisterEvent(key_event);
}

KeyMapManager::~KeyMapManager()
{
}

void KeyMapManager::SetKeyboardStatus(int32_t i_key_id, bool i_is_pressed)
{
    if (i_key_id >= 0 && i_key_id < 256) {
        KeyEventArg event_arg;
        event_arg.m_key_id = i_key_id;
        m_key_maps[i_key_id] = i_is_pressed;

        if (i_is_pressed == true) {
            event_arg.m_key_state = 1;
        }
        else {
            event_arg.m_key_state = 0;
        }

        m_key_event_wref.GetRef().NotifyEvent(event_arg);
    }
}



_______________SD_END_BASIC_NAMESPACE________________