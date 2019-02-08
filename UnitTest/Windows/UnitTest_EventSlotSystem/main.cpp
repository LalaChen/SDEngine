#include "SDEngine.h"

using namespace SDE;
using namespace SDE::Basic;

//------------------------- Log ----------------------

class UnitTestLogManager : public WindowsLogManager
{
public:
	void LogToOutput(LogType i_type) override;
};

//construct for log.
void UnitTestLogManager::LogToOutput(LogType i_type)
{
	if (i_type == LogType::Normal)
	{
		std::cout << ((std::string("[Normal] ") + std::string(m_log_buffer) + std::string("\n")));
	}
	else if (i_type == LogType::Warning)
	{
		std::cout << ((std::string("[Warning] ") + std::string(m_log_buffer) + std::string("\n")));
	}
	else if (i_type == LogType::Internal)
	{
#ifndef _DEBUG
		//std::cout << "[Internal] " << m_log_buffer << std::endl;
#else
		std::cout << ((std::string("[Internal] ") + std::string(m_log_buffer) + std::string("\n")));
#endif
	}
	else
	{
		std::cout << ((std::string("[Error] ") + std::string(m_log_buffer) + std::string("\n")));
	}
}

//-------------------- Class Event -------------------
///----- Sender Event Arg
class Event1Arg : public EventArg
{
public:
	Event1Arg(){}
	~Event1Arg(){}
public:
	std::string m_e1_arg1;
	int         m_el_arg2 = 50;
};

class Event2Arg : public EventArg
{
public:
	Event2Arg(){}
	~Event2Arg(){}
public:
	std::string m_e2_arg1;
	float       m_e2_arg2 = 56.78f;
	int         m_e2_arg3 = 1987;
};

///----- Sender Object
class SenderEventObject : public EventObject
{
public:
	explicit SenderEventObject(const std::string &i_object_name)
		: EventObject(i_object_name)
	{
		EventStrongReferenceObject event_1 = new Event("Event1");
		EventStrongReferenceObject event_2 = new Event("Event2");
		RegisterEvent(event_1);
		RegisterEvent(event_2);
	}

	virtual ~SenderEventObject()
	{
		SDLOG("Destroy SenderEventObject %s !!!", m_object_name.c_str());
	}

	void NotifyAllEvent()
	{
		Event1Arg e1;
		Event2Arg e2;
		e1.m_e1_arg1 = m_object_name;
		e2.m_e2_arg1 = m_object_name;
		NotifyEvent("Event1", e1);
		NotifyEvent("Event2", e2);
	}
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(SenderEventObject);

//-----
class ReceiverClass : public Object
{
public:
	explicit ReceiverClass(const std::string &i_object_name)
	: Object(i_object_name)
	{
	}
	virtual ~ReceiverClass()
	{
		SDLOG("Destroy ReceiverClass %s !!!", m_object_name.c_str());
	}
public:
	bool OnEvent1Notified(const EventArg &i_arg)
	{ 
		if (typeid(i_arg).hash_code() == typeid(Event1Arg).hash_code())
		{
			const Event1Arg &arg = dynamic_cast<const Event1Arg&>(i_arg);
			SDLOG("Valid Successful !!! Receive event 1 from obj %s.", arg.m_e1_arg1.c_str());
			return true;
		}
		else
		{
			SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
			return false;
		}
	}

	bool OnEvent2Notified(const EventArg &i_arg)
	{
		if (typeid(i_arg).hash_code() == typeid(Event2Arg).hash_code())
		{
			const Event2Arg &arg = dynamic_cast<const Event2Arg&>(i_arg);
			SDLOG("Valid Successful !!! Receive event 2 from obj %s.", arg.m_e2_arg1.c_str());
			return true;
		}
		else
		{
			SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
			return false;
		}
	}
public:
	void InitializeEventSlot(const SenderEventObjectStrongReferenceObject &i_src)
	{
		if (i_src.IsNull() == false)
		{
			StrongReferenceObject<ReceiverClass> this_object = GetThisSharedPtrByType<ReceiverClass>();
			SDLOG("ReceiverClass use_count = %d.", this_object.UseCount());
			FunctionSlotBaseStrongReferenceObject event1_slot = 
				new MemberFunctionSlot<ReceiverClass>("ReceiverClass::OnEvent1Notified", this_object, &ReceiverClass::OnEvent1Notified);
			i_src.GetRef().RegisterSlotFunctionIntoEvent("Event1", event1_slot);
			FunctionSlotBaseStrongReferenceObject event2_slot =
				new MemberFunctionSlot<ReceiverClass>("ReceiverClass::OnEvent2Notified", this_object, &ReceiverClass::OnEvent2Notified);
			i_src.GetRef().RegisterSlotFunctionIntoEvent("Event2", event2_slot);
		}
	}
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(ReceiverClass);

//------------------------- main ----------------------
int main(int argc, char **argv)
{
	new UnitTestLogManager();

	SenderEventObjectStrongReferenceObject sender_obj1 = new SenderEventObject("Sender Object 1");
	ReceiverClassStrongReferenceObject receiver_obj1 = new ReceiverClass("Receive Object 1");
	receiver_obj1.GetRef().InitializeEventSlot(sender_obj1);

	SenderEventObjectStrongReferenceObject sender_obj2   = new SenderEventObject("Sender Object 2");
	ReceiverClassStrongReferenceObject receiver_obj2 = new ReceiverClass("Receive Object 2");
	receiver_obj2.GetRef().InitializeEventSlot(sender_obj2);
	
	sender_obj1.GetRef().NotifyAllEvent();
	sender_obj2.GetRef().NotifyAllEvent();
	
	UnitTestLogManager::Destroy();
}