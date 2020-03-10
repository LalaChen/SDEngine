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
#include "EventObject.h"

______________SD_START_BASIC_NAMESPACE_______________

EventObject::EventObject(const ObjectName &i_object_name)
: Object(i_object_name)
{

}


EventObject::~EventObject()
{
}


bool EventObject::RegisterEvent(const EventStrongReferenceObject &i_src_event)
{
    EventContainer::iterator event_iter;

    if (i_src_event.IsNull() == true) {
        SDLOGE("EventReferenceObject is null.");
        return false;
    }
    
    for (event_iter = m_event_container.begin() ;
         event_iter != m_event_container.end() ; 
         event_iter++) {
        if( (*event_iter).GetRef().GetObjectName().compare(i_src_event.GetRef().GetObjectName()) == 0) {
            //this event is already register.
            SDLOGW("Event(%s) is exist.", (*event_iter).GetRef().GetObjectName().c_str());
            return false;
        }
    }

    m_event_container.push_back(i_src_event);
    return true;
}

bool EventObject::RegisterSlotFunctionIntoEvent(const std::string &i_event_name, const FunctionSlotBaseStrongReferenceObject &i_src_event_slot_func)
{
    EventContainer::iterator event_iter;
    
    for (event_iter = m_event_container.begin() ;
         event_iter != m_event_container.end() ;
         event_iter++) {
        //this event is already register.
        if( (*event_iter).GetRef().GetObjectName().compare(i_event_name) == 0) {
            return (*event_iter).GetRef().RegisterSlotFunction(i_src_event_slot_func);
        }
    }

    return false;
}

bool EventObject::UnregisterSlotFunctionFromEvent(const std::string& i_event_name, const FunctionSlotBaseStrongReferenceObject& i_src_event_slot_func)
{
    EventContainer::iterator event_iter;

    for (event_iter = m_event_container.begin();
        event_iter != m_event_container.end();
        event_iter++) {
        //this event is already register.
        if ((*event_iter).GetRef().GetObjectName().compare(i_event_name) == 0) {
            return (*event_iter).GetRef().UnregisterSlotFunction(i_src_event_slot_func);
        }
    }

    return false;
}

bool EventObject::NotifyEvent(const std::string &i_event_name,const EventArg &i_arg)
{
    EventContainer::iterator event_iter;
    
    for(event_iter = m_event_container.begin() ;
        event_iter != m_event_container.end() ; 
        event_iter++)
    {
        //this event is already register.
        if( (*event_iter).GetRef().GetObjectName().compare(i_event_name) == 0)
        {
            return (*event_iter).GetRef().NotifyEvent(i_arg);
        }
    }

    return false;
}

_______________SD_END_BASIC_NAMESPACE________________