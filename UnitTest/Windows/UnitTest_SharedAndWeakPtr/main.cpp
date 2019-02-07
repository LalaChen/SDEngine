#include <exception>
#include <memory>
#include <iostream>

#include "SDSmartObjectMacro.h"
#include "SDEngine.h"

using namespace SDE;
using namespace SDE::Basic;

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

class BaseClass : SDE::Basic::Object
{
public:
	explicit BaseClass(const std::string &i_object_name)
	: Object(i_object_name)
	{
		m_class_name = "TheOldestAncestor";
	}

	virtual ~BaseClass() {
		std::cout << "Obj "<< m_object_name << " died at TheOldestAncestor's Dtor!!!" << std::endl;
	}

	std::string ToString() const override{
		return SDE::Basic::StringFormat("Obj:%s(%s)[adr:%x]", m_object_name.c_str(), m_class_name.c_str(), this);
	}
protected:;
	DECLARE_ATTRIBUTE_CONSTREF_GET(std::string, m_class_name, ClassName);
};


class L1Class : public BaseClass
{
public:
	explicit L1Class(const std::string &i_object_name)
	: BaseClass(i_object_name)
	{
		m_class_name = "L1Class";
	}
	virtual ~L1Class(void)
	{
		std::cout << "L1ClassDtor->";
	}
protected:
	
};

class L1_2Class : public BaseClass
{
public:
	explicit L1_2Class(const std::string &i_object_name)
	: BaseClass(i_object_name)
	{
		m_class_name = "L1_2Class";
	}
	virtual ~L1_2Class()
	{
		std::cout << "L1_2ClassDtor->";
	}
};

//------------- L2 Class
class L2Class : public L1Class
{
public:
	explicit L2Class(const std::string &i_object_name)
	: L1Class(i_object_name)
	{
		m_class_name = "L2Class";
	}
	virtual ~L2Class()
	{
		std::cout << "L2ClassDtor->";
	}
};

//------------- L2 Class
class L2_2Class : public L1Class
{
public:
	explicit L2_2Class(const std::string &i_object_name)
	: L1Class(i_object_name)
	{
		m_class_name = "L2_2Class";
	}
	virtual ~L2_2Class()
	{
		std::cout <<"L2_2ClassDtor->";
	}
};

//------------- L3_1Class
class L3_1Class : public L2Class
{
public:
	explicit L3_1Class(const std::string &i_object_name)
		: L2Class(i_object_name)
	{
		m_class_name = "L3_1Class";
	}
	virtual ~L3_1Class()
	{
		std::cout << m_class_name << "Dtor->";
	}
};

//------------- L3 Class
class L3_2Class : public L2Class
{
public:
	explicit L3_2Class(const std::string &i_object_name)
		: L2Class(i_object_name)
	{
		m_class_name = "L3_2Class";
	}
	virtual ~L3_2Class()
	{
		std::cout << m_class_name << "Dtor->";
	}
};


int main(int argc, char **argv)
{
	//Based -> L1   -> L2   -> L3_1
	//                      -> L3_2
	//              -> L2_2
	//      -> L1_2
	std::shared_ptr<L1Class>   L1(new L1Class("L1"));
	std::shared_ptr<L1_2Class> L1_2(new L1_2Class("L1_2"));
	std::shared_ptr<L2Class>   L2(new L2Class("L2"));
	std::shared_ptr<L2_2Class> L2_2(new L2_2Class("L2_2"));
	std::shared_ptr<L3_1Class> L3_1(new L3_1Class("L3_1"));
	std::shared_ptr<L3_2Class> L3_2(new L3_2Class("L3_2"));

	new UnitTestLogManager();

	SDLOG("****************************** shared_ptr and weak_ptr testing *********************************");
	//CopyCtor L2 to L1 (O)
	{
		SDLOG("***** CopyCtor L2 to shared_ptr<L1Class> new_L1, L1 should keep L2 (O) ");
		std::shared_ptr<L1Class> new_L1(L2);
		if (new_L1.get() != nullptr)
		{
			SDLOG("Valid Successful !!! new L1 should be assigned by L2. L2 info : %s use_count = %d", new_L1.get()->ToString().c_str(), new_L1.use_count());
		}
		else
		{
			SDLOGE("Valid Failure !!! new L1 is nullptr");
		}
		SDLOG("");
	}

	//Assign L2 to L1 (O)
	{
		std::shared_ptr<L1Class> new_L1;

		SDLOG("***** Assign L2 to shared_ptr<L1Class> new_L1, L1 should keep L2 (O) ");
		new_L1 = L2;
		if (new_L1.get() != nullptr)
		{
			SDLOG("Valid Successful !!! new L1 should be assigned by L2. L2 info : %s use_count = %d", new_L1.get()->ToString().c_str(), new_L1.use_count());
		}
		else
		{
			SDLOGE("Valid Failure !!! new L1 is nullptr");
		}
		SDLOG("");
	}

	//Assign L1 to L2 (X)
	{
		SDLOG("***** Assign L1 to a std::shared_ptr<L2Class> new_L2 (X) ");
		//std::shared_ptr<L2Class> new_L2(L1); compile failure.
		std::shared_ptr<L2Class> new_L2;
		new_L2 = std::dynamic_pointer_cast<L2Class>(L1);
		if (new_L2.get() == nullptr)
		{
			SDLOG("Valid Successful!! new_L2 can't get L1 object dy std::dynamic_pointer_cast<L2Class>() !!! L1 info : %s use_count = %d", L1.get()->ToString().c_str(), L1.use_count());
		}
		else
		{
			SDLOGE("Valid failure!!! new_L2 shouldn't get L1 ");
		}
		SDLOG("");
	}

	//Assign L2_2 in L1 to new_L2
	{
		SDLOG("***** Assign L2_2 in L1 to new_L2 (X) ");
		//std::shared_ptr<L2Class> new_L2(L1); compile failure.
		std::shared_ptr<L1Class> L2_2_in_L1(L2_2);
		std::shared_ptr<L2Class> new_L2 = std::dynamic_pointer_cast<L2Class>(L2_2_in_L1);
		if (new_L2.get() == nullptr)
		{
			SDLOG("Valid Successful!! new_L2 can't get L2_2 in L1 object dy std::dynamic_pointer_cast<L2Class>() !!! L1 info : %s use_count = %d", L2_2.get()->ToString().c_str(), L2_2.use_count());
		}
		else
		{
			SDLOGE("Valid failure!!! new_L2 shouldn't get L1 ");
		}
		SDLOG("");
	}

	SDLOG("****************************** weak_ptr testing *********************************");
	//Assign weak_ptr 
	{
		SDLOG("weak_ptr test ");
		std::weak_ptr<L1Class> weak_L1(L1);
		SDLOG("***** weak_ptr<L1_Class> L1 info : %s use_count = %d", weak_L1.lock().get()->ToString().c_str(), weak_L1.use_count());
		SDLOG("");
	}

	{
		//std::weak_ptr<L1Class> weak_L1(new L1Class("GG")); (X)
		SDLOG("");
		std::weak_ptr<L1Class> weak_L1(L2);
		std::weak_ptr<L2Class> weak_L2 = std::dynamic_pointer_cast<L2Class>(weak_L1.lock());
	}

	SDLOG("****************************** StrongReferenceObject Test ******************************");
	//StrongReferenceTemplateTest
	{
		SDLOG("***** Series assigning test");
		StrongReferenceObject<L1Class> L1_ref(L1);
		SDLOG("* StrongReferenceObject L1_ref info : %s use_count = %d(Should be 2)", L1_ref.GetRef().ToString().c_str(), L1_ref.UseCount());

		StrongReferenceObject<BaseClass> base_ref(L1_ref);
		SDLOG("* StrongReferenceObject base_ref(copy ctor by L1_ref) info : %s use_count = %d(Should be 3)", base_ref.GetRef().ToString().c_str(), base_ref.UseCount());

		StrongReferenceObject<BaseClass> base_ref2;
		base_ref2 = L1_ref.StaticCastTo<BaseClass>();
		SDLOG("* StrongReferenceObject base_ref2(assigned by static cast BaseClass from L1_ref) info : %s use_count = %d(Should be 4)", 
			base_ref2.GetRef().ToString().c_str(), base_ref2.use_count());

		//StrongReferenceObject<L2Class> L2_ref2(L1);// compile error
		SDLOG("* StrongReferenceObject<L2Class> L2_ref2(L1) will cause complie error!!");

		//StrongReferenceObject<L2Class> L2_ref2(L1_ref);// compile error
		SDLOG("* StrongReferenceObject<L2Class> L2_ref2(L1_ref) will cause complie error!!");

		StrongReferenceObject<L1Class> L1_ref2;
		L1_ref2 = L1;
		SDLOG("* StrongReferenceObject L1_ref2(assign by L1) info : %s use_count = %d(Should be 5)", L1_ref2.GetRef().ToString().c_str(), L1_ref2.UseCount());

		StrongReferenceObject<L1Class> L1_ref3(L1_ref);
		SDLOG("* StrongReferenceObject L1_ref3(assign by L1_ref) info : %s use_count = %d(Should be 6)", L1_ref3.GetRef().ToString().c_str(), L1_ref3.UseCount());

		StrongReferenceObject<L1Class> L1_ref4;
		L1_ref4 = L1_ref;
		SDLOG("* StrongReferenceObject L1_ref4(copy ctor by L1_ref) info : %s use_count = %d(Should be 7)", L1_ref4.GetRef().ToString().c_str(), L1_ref4.UseCount());

		StrongReferenceObject<L1Class> L1_ref5;
		L1_ref5 = new L1Class("newL1forRef5");
		SDLOG("* StrongReferenceObject L1_ref5 info : %s use_count = %d(Should be 1)", L1_ref5.GetRef().ToString().c_str(), L1_ref5.UseCount());
	}

	{
		SDLOG("***** Static Cast Test. Cast newL2_ref to L2_ref should make L2_ref keep the instance.");
		StrongReferenceObject<L2Class> newL2_ref(new L2Class("NewL2"));
		StrongReferenceObject<L1Class> L1_ref;
		L1_ref = newL2_ref.StaticCastTo<L1Class>();
		if (L1_ref.IsNull() == false) {
			SDLOG("Valid Successful. StrongReferenceObject L1_ref(static assign by newL2_ref) isn't null. Print newL1_ref's info : %s use_count = %d(Should be 2)",
				L1_ref.GetRef().ToString().c_str(), L1_ref.UseCount());
		}
		else {
			SDLOG("Valid failure. StrongReferenceObject L1_ref(static assign by newL2_ref) is null. Print newL2_ref's info : %s use_count = %d",
				newL2_ref.GetRef().ToString().c_str(), newL2_ref.UseCount());
		}
		SDLOG("");
	}

	{
		SDLOG("***** Dynamic Cast Test. Cast newL1_ref(initialized L1Class obj) to L2_ref should make L2_ref is nullptr.");
		StrongReferenceObject<L1Class> newL1_ref(new L1Class("NewL1"));
		StrongReferenceObject<L2Class> L2_ref;
		L2_ref = newL1_ref.DynamicCastTo<L2Class>();
		if (L2_ref.IsNull() == true) {
			SDLOG("Valid Successful. StrongReferenceObject L2_ref(dynamic_cast assign by newL1_ref) is null. Print newL1_ref's info : %s use_count = %d(Should be 1)", 
				newL1_ref.GetRef().ToString().c_str(), newL1_ref.UseCount());
		}
		else {
			SDLOG("Valid failure. StrongReferenceObject L2_ref(dynamic_cast assign by newL1_ref) is not null. Print L2_ref's info : %s use_count = %d",
				L2_ref.GetRef().ToString().c_str(), L2_ref.UseCount());
		}
		SDLOG("");
	}

	{
		SDLOG("***** Dynamic Cast Test 2. Cast newL3_1_in_L1_ref(initialized L3_1Class obj) to L1_ref should make L1_ref isn't nullptr.");
		StrongReferenceObject<L1Class> newL3_1_in_L1_ref(new L3_1Class("NewL3_1"));
		StrongReferenceObject<L1Class> L1_ref;
		L1_ref = newL3_1_in_L1_ref.DynamicCastTo<L1Class>();
		if (L1_ref.IsNull() == false) {
			SDLOG("Valid Successful. StrongReferenceObject L1_ref(dynamic_cast assign by newL1_ref) is not null. Print L1_ref's info : %s use_count = %d(Should be 2)",
				L1_ref.GetRef().ToString().c_str(), L1_ref.UseCount());
		}
		else {
			SDLOG("Valid failure. StrongReferenceObject L1_ref(dynamic_cast assign by newL1_ref) is null. Print newL3_1_in_L1_ref's info : %s use_count = %d",
				newL3_1_in_L1_ref.GetRef().ToString().c_str(), newL3_1_in_L1_ref.use_count());
		}
		SDLOG("");
	}

	{
		SDLOG("***** Null Ref Exception Test.");
		StrongReferenceObject<L1Class> null_ref;
		try 
		{
			SDLOG("Valid failure !!! ref info : %s. use_count = %d", null_ref.GetRef().ToString(), null_ref.UseCount());
		}
		catch (std::exception &e)
		{
			SDLOG("Valid successful !!! Exception %s.", e.what());
		}
	}

	/*
	{
		try {
			//Bad Practice (Even we add exception, it's still crash. So mark this block.)
			StrongReferenceObject<L2Class> newL2_ref = new L2Class("newL2");
			SDLOG("c1 = %d", newL2_ref.use_count());
			StrongReferenceObject<L1Class> newL2_ref2(newL2_ref.get());
			SDLOG("c1 = %d, c2 = %d will crash at releasing newL2_ref.", newL2_ref.use_count(), newL2_ref2.use_count());
		}
		catch (std::exception &e) {
			SDLOG("Exception %s. This is bad practice. We should assign shared_ptr by allocate an object or another shared_ptr.", e.what());
		}

	}
	//*/

	SDLOG("****************************** WeakReferenceObject Test ******************************");
	{
		SDLOG("***** series test");
		StrongReferenceObject<L2Class> L2_ref = L2;
		WeakReferenceObject<L2Class> L2_wref = L2_ref;
		SDLOG("* Assign L2_ref to L2_wref. L2_wref info : %s. use_count %d.(should be 2)", L2_wref.GetRef().ToString().c_str(), L2_wref.UseCount());
		WeakReferenceObject<L1Class> L2_L1_wref = L2_ref.StaticCastTo<L1Class>();
		SDLOG("* Assign L2_ref to L2_L1_wref by static cast. L2_L1_wref info : %s. use_count %d.(should be 2)", 
			L2_L1_wref.GetRef().ToString().c_str(), L2_L1_wref.UseCount());
	}

	{
		SDLOG("***** dynamic cast test");
		StrongReferenceObject<L3_1Class> L3_1_ref = L3_1;
		StrongReferenceObject<L2Class> L3_1_in_L2_ref = L3_1_ref.StaticCastTo<L2Class>();
		WeakReferenceObject<L3_1Class> L2_L3_1_wref = L3_1_in_L2_ref.DynamicCastTo<L3_1Class>();
		SDLOG("* Assign L2_ref to L2_L3_1_wref by dynamic cast. L2_L3_1_wref info : %s. use_count %d.(should be 3)",
			L2_L3_1_wref.GetRef().ToString().c_str(), L2_L3_1_wref.UseCount());

		WeakReferenceObject<L1Class> L3_1_in_L1_wref;
		L3_1_in_L1_wref = L3_1_in_L2_ref.StaticCastTo<L1Class>();
		SDLOG("* Assign L3_1_in_L2_ref to L3_1_in_L1_wref by static cast. L3_1_in_L1_wref info : %s. use_count %d.(should be 3)",
			L3_1_in_L1_wref.GetRef().ToString().c_str(), L3_1_in_L1_wref.UseCount());

		WeakReferenceObject<L3_1Class> L3_1_wref;
		L3_1_wref = L3_1_in_L1_wref.DynamicCastTo<L3_1Class>();
		SDLOG("* Assign L3_1_in_L1_wref to L3_1_wref by dynamic cast. L3_1_wref info : %s. use_count %d.(should be 3)",
			L3_1_wref.GetRef().ToString().c_str(), L3_1_wref.UseCount());
	}
}