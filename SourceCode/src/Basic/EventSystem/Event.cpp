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
#include "EventArg.h"
#include "Event.h"

______________SD_START_BASIC_NAMESPACE_______________

Event::Event(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

Event::~Event()
{
}

bool Event::RegisterSlotFunction(const FunctionSlotBaseStrongReferenceObject &i_function_slot_ref_obj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    bool is_slot_existed = false;
    SlotFunctionContainer::iterator slot_iter;
    //If this slot is null, we return false.
    if(i_function_slot_ref_obj.IsNull() == true)
    {
        SDLOGW("Register null reference.");
        return false;
    }
    else
    {
        //We check the i_function_slot_ref_obj is existed in slot container. If it's existed, we return true.
        //Otherwise, we add it to container and return true.
        for(slot_iter = m_function_slot_container.begin() ;
            slot_iter != m_function_slot_container.end()   ;
            slot_iter++)
        {
            if ((*slot_iter).IsNull() == false) {
                if ((*slot_iter).IsEqualTo(i_function_slot_ref_obj) == true)
                {
                    is_slot_existed = true;
                }
            }
        }

        if(is_slot_existed == false)
        {
            m_function_slot_container.push_back(i_function_slot_ref_obj);
            return true;
        }
        else
        {
            //warrning : slot is existed.
            SDLOGW("Slot %s is existed!!! Please check.", i_function_slot_ref_obj.GetRef().ToString().c_str());
            return true;
        }
    }
}

bool Event::UnregisterSlotFunction(const FunctionSlotBaseStrongReferenceObject &i_function_slot_ref_obj)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    bool is_slot_existed = false;
    SlotFunctionContainer::iterator slot_iterator;
    //If this slot is null, we return false.
    if (i_function_slot_ref_obj.IsNull() == true)
    {
        return false;
    }
    else
    {
        //We check the i_function_slot_ref_obj is existed in slot container. If it's existed, we erase it and return true.
        //Otherwise, we will return false.
        for (slot_iterator = m_function_slot_container.begin(); slot_iterator != m_function_slot_container.end();)
        {
            if ((*slot_iterator).IsEqualTo(i_function_slot_ref_obj) == true)
            {
                slot_iterator = m_function_slot_container.erase(slot_iterator);
                return true;
            }
            else
            {
                slot_iterator++;
            }
        }

        return false;

    }
}

bool Event::NotifyEvent(const EventArg &i_arg)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    SlotFunctionContainer::iterator slot_iter;
    bool notify_return = true;

    for(slot_iter = m_function_slot_container.begin() ; slot_iter != m_function_slot_container.end() ;)
    {
        notify_return = (*slot_iter).GetRef().NotifyFunction(i_arg);

        if (notify_return == false) slot_iter = m_function_slot_container.erase(slot_iter);
        else slot_iter++;
    }

    return notify_return;
}


_______________SD_END_BASIC_NAMESPACE________________