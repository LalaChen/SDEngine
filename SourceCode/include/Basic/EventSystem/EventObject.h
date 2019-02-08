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

/*! \file      EventObject.h
 *  \brief     Introduce of class EventObject.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/08
 *  \copyright FreeBSD Public License.
 */

#pragma once

#include "Event.h"
#include "Object.h"

 //---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \class EventObject
    In our event system, EventObject is used to keep Events. The EventObject offer a EventContainer for \n
	storing events. Developer can design events for purpose and register the ones into EventContainer. \n
	And then we notify the event while we thought suitablely. Finally we register the corresponding callback \n
	functions to those event. When the target event be notified, those callbacks will be executed.\n
	Note : this is simular with slot-signal design in QT. \n\n
	
	\sa More introduction about event and slot system at \ref EventSlotSystem
*/

class SDENGINE_CLASS EventObject : public Object
{
public:
	/*! \var typedef std::list<EventStrongReferenceObject> EventContainer
		\brief EventContainer is the type for stored all event reference object. \n
		       We type-define it for using conveniently. \n
	*/
	typedef std::vector< EventStrongReferenceObject > EventContainer;
public:
	/*! \fn explicit EventObject(const ObjectName &i_object_name);
	    \param [in] i_object_name name of this object.
		\brief The constructor of EventObject Class.
	*/
	explicit EventObject(const ObjectName &i_object_name);

	/*! \fn virtual ~EventObject();
	    \brief The destructor of EventObject Class.
	*/
	virtual ~EventObject();
public:
	/*! \fn bool RegisterEvent(const EventStrongReferenceObject &i_src_event);
	    \param [in] i_src_event The event we want to add into.
		\brief Adding target event into this event object. \n
		       It means the target event will be notified at somewhere of this object. \n
			   If we register successful, we will return true. \n
			   Otherwise, we will return false for event existed.
	*/
	bool RegisterEvent(const EventStrongReferenceObject &i_src_event);
	
	/*! \fn bool RegisterSlotFunctionIntoEvent(const std::string &i_event_name, const FunctionSlotBaseStrongReferenceObject &i_src_event_slot_func);
	    \param [in] i_event_name The event we want to add slot into.
		       [in] i_src_event_slot_func The target slot we want to trigger while the event notified.
		\brief Adding target slot into target event of this event object.
		       If we register successful, we will return true. \n
			   Otherwise, we will return false for slot existed.
	*/
	bool RegisterSlotFunctionIntoEvent(const std::string &i_event_name, const FunctionSlotBaseStrongReferenceObject &i_src_event_slot_func);
	
	/*! \fn bool NotifyEvent(const std::string &i_event_name,const EventArg &i_arg);
	    \param [in] i_event_name The event we want to notified.
		       [in] i_arg The parameters we want to deliver to all slots in the target event.
		\brief To notify the event named by the i_event_name. \n
		       We need to notified event at suitable place in our custom component. \n
			   If we notify successfully, we will return true. \n
			   Otherwise, we will return false for that event isn't existed.
	*/
	bool NotifyEvent(const std::string &i_event_name,const EventArg &i_arg);
protected:
	/*! \var EventContainer m_event_container;
	    \brief List for storing all event reference.
	*/
	EventContainer m_event_container;
};

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}