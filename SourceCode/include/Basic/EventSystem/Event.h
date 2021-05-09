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

/*! \file      Event.h
 *  \brief     Introduce of class Event.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/07
 *  \copyright MIT License.
 */

#pragma once

#include <mutex>
#include <functional>
#include <list>

#include "FunctionSlot.h"
#include "EventArg.h"
#include "Object.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class Event
 *  \brief In our event system, we allow user register and use events in the component inheritance EventObject.
 *  User can NOTIFY event where he wanted. Via notifying event, some slot functions registered into event
 *  will triggered and then received some parameters we want to utility.
 *  
 *  \sa More introduction about event and slot system at \ref EventSlotSystem
 */
class SDENGINE_CLASS Event : public Object
{
public:
    /*! \var typedef std::list< FunctionSlotBaseReferenceObject > SlotFunctionContainer
     *  \brief SlotFunctionContainer is the type for stored all slot reference object. We type-define it for using conveniently.
     */
    typedef std::list< FunctionSlotBaseStrongReferenceObject > SlotFunctionContainer;
public: 
    /*! \fn Event(const std::string &i_object_name)
     *  \param [in] i_object_name name of this event.
     *  \brief The constructor of Event Class.
     */
    explicit Event(const ObjectName &i_object_name);
    
    /*! \fn ~Event()
     *  \brief The destructor of Event Class.
     */
    virtual ~Event();
public:
    /*! \fn bool RegisterSlotFunction(const FunctionSlotBaseReferenceObject &i_function_slot)
     *  \param [in] i_function_slot The slot we want to register into this event.
     *  \brief Adding slot into this event. If we register successful, we will return true.
     *         Otherwise, we will return false for slot existed.
     */
    bool RegisterSlotFunction(const FunctionSlotBaseStrongReferenceObject &i_function_slot);
    
    /*! \fn bool UnregisterSlotFunction(const FunctionSlotBaseReferenceObject &i_function_slot)
     *  \param [in] i_function_slot The slot we want to register into this event.
     *  \brief Deleting slot in this event. If we delete successful, we will return true.
     *         Otherwise, we will return false that slot doesn't exist.
     */
    bool UnregisterSlotFunction(const FunctionSlotBaseStrongReferenceObject &i_function_slot);
    
    /*! \fn bool NotifyEvent(const EventArg& i_arg)
     *  \param [in] i_arg The event arg we want to delivery to slots registered into this event.
     *  \brief Delivery params to all slots and remove slots returning false.
     */
    bool NotifyEvent(const EventArg &i_arg);
private:
    /*! \var SlotFunctionContainer m_function_slot_container;
     *  \brief container of function slot.
     */
    SlotFunctionContainer m_function_slot_container;
    
    /*! \var std::mutex m_mutex;
     *  \brief lock for avoiding threading problem.
     */
    std::mutex m_mutex;
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Event);

_______________SD_END_BASIC_NAMESPACE________________