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

/*! \file      FunctionSlot.h
 *  \brief     Introduce of class FunctionSlotBase, GlobalFunctionSlot and MemberFunctionSlot.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/07
 *  \copyright MIT License.
 */

#pragma once

#include "Object.h"
#include "EventArg.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class FunctionSlotBase \n
 *  \brief In our event system, we will use the slot function after something finished.
 *  For a example, we needed to exexuted something after button clicked. So the kind
 *  of slot function class object will be used to handled those situation.
 *  We have some principle of using. we list those rule behind:
 *  <ul>
 *  <li>1. The format of all event callback function is bool FunctionName(cosnt EventArg &i_src)
 *         or bool ClassName::FunctionName(cosnt EventArg &i_src). So the EventArg only can declare
 *         with variable type. \n
 *  <li>2. One function slot is only existed in one event. We don't allow programmer
 *         set one slot in two event object. So function slot only can declare with variable type.
 *  </ul>
 *  Class FunctionSlotBase is the based class of all function slot class. It's interface class.
 *
 *  Example : (<A HREF=".\_demo_platform_2_unit_test-_event_slot_system_2main_8cpp-example.html">DemoPlatform/UnitTest-EventSlotSystem/main.cpp</a>)\n
 *
 *  \sa More introduction about event and slot system at \ref EventSlotSystem
 */

class SDENGINE_CLASS FunctionSlotBase : public Object
{
public:
    /*! \fn FunctionSlotBase(const std::string &i_object_name)
     *  \param [in] i_object_name name of this object.
     *  \brief The constructor of FunctionSlotBase Class.
     */
    explicit FunctionSlotBase(const ObjectName &i_object_name)
    : Object(i_object_name)
    {
    }
    
    /*! \fn ~FunctionSlotBase()
     *  \brief The destructor of FunctionSlotBase Class.
     */
    virtual ~FunctionSlotBase()
    {
    }
public:
//-------------- Behavior of slot ----------------
    /*! \fn bool NotifyFunction(const EventArg &i_src_arg)
     *  \param [in] i_src_arg The event arg we want to send out.
     *  \brief This is the behavior of all slots. We will deliver params by arg to target slot function.
     */
    virtual bool NotifyFunction(const EventArg &i_src_arg) = 0;

    /*! \fn bool IsInvalid()
     *  \brief This is the behavior of all slots. Return the result about that this slot is invalid or not.
     */
    virtual bool IsInvalid() const = 0;
private:
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(FunctionSlotBase);

/*! \class GlobalFunctionSlot
 *  This class is the derived class of all function slot class.
 *  Global function slot is used to record function NOT IN CLASS.
 *
 *  Example : (<A HREF=".\_demo_platform_2_unit_test-_event_slot_system_2main_8cpp-example.html">DemoPlatform/UnitTest-EventSlotSystem/main.cpp</a>)
 *
 *  \sa    More introduction about event and slot system at \ref EventSlotSystem
 */
class GlobalFunctionSlot : public FunctionSlotBase
{
public:
    /*! \var typedef bool(*GlobalSlotFunction)(const EventArg &i_src_arg)
     *  \brief GlobalSlotFunction is the type for stored function pointer of slot. We type-define it for using conveniently.
     */
    typedef bool(*GlobalSlotFunction)(const EventArg &i_src_arg); 
public:
    /*! \fn GlobalFunctionSlot(const std::string &i_object_name, GlobalSlotFunction i_fp)
     *  \param [in] i_object_name name of this object.
     *  \param [in] i_fp the receiving function.
     *  \brief The constructor of GlobalFunctionSlot Class.
     */
    explicit GlobalFunctionSlot(const ObjectName &i_object_name, GlobalSlotFunction i_fp);
    
    /*! \fn ~GlobalFunctionSlot()
     *  \brief The destructor of GlobalFunctionSlot Class.
     */
    virtual ~GlobalFunctionSlot();
public:
    /*! \fn bool NotifyFunction(const EventArg &i_src_arg) override;
     *  \param [in] i_src_arg The event arg we want to send out.
     *  \brief We will deliver params by arg to target global slot function.
     */
    bool NotifyFunction(const EventArg &i_src_arg) override;
    
    /*! \fn bool IsInvalid() const override;
     *  \brief Return the result about that this slot is invalid or not.
     *         Invalid global slot is that m_function_pointer is nullptr.
     */
    bool IsInvalid() const override;

    /*! \fn bool IsEqualTo(const Object &i_src) const override;
     *  \param [in] i_src The another slot instance.
     *  \brief  Return the result about i_src.m_fp is equal to this->m_fp or not.
     */
    bool IsEqualTo(const Object &i_src) const override;
public:
    /*! \fn void RegisterFunction(GlobalSlotFunction i_function_pointer);
     *  \param [in] i_fp The target slot function.
     *  \brief  Register the i_function_pointer as the slot function.
     */
    void RegisterFunction(GlobalSlotFunction i_fp);
public:
    /*! \var GlobalSlotFunction m_fp;
     *  \brief Slot callback function. Default is nullptr.
     */
    GlobalSlotFunction m_fp;
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GlobalFunctionSlot);

inline bool GlobalFunctionSlot::IsInvalid() const{ return (m_fp == nullptr); }

/*! \class MemberFunctionSlot
 *  MemberFunctionSlot is a template class for making member function slot.
 *  We register a reference and its function into this slot.
 *  
 *  <B>Example :</B> (<A HREF=".\_demo_platform_2_unit_test-_event_slot_system_2main_8cpp-example.html">DemoPlatform/UnitTest-EventSlotSystem/main.cpp</a>)
 *  
 *  \sa More introduction about event and slot system at \ref EventSlotSystem.
 *  
 *  \tparam Type Target Type.
 */
template <class Type>
class MemberFunctionSlot : public FunctionSlotBase
{
public:
    /*! \var typedef bool(*MemberSlotFunction)(const EventArg &i_src_arg)
     *  \brief MemberFunctionSlot is the type for stored function pointer and its ref of slot. We type-define it for using conveniently.
     */
    typedef bool(Type::*MemberSlotFunction)(const EventArg &i_src_arg);
public:
    /*! \fn explicit MemberFunctionSlot<Type>(const ObjectName &i_object_name, const WeakReferenceObject<T> &i, MemberSlotFunction i_fp);
     *  \param [in] i_object_name name of this object.
     *  \param [in] i the weak reference of receiver.
     *  \param [in] i_fp the receiving function.
     *  \tparam Type Target Type.
     *  \brief The constructor of MemberFunctionSlot Class.
     */
    explicit MemberFunctionSlot<Type>(const ObjectName &i_object_name, const WeakReferenceObject<Type> &i_target_obj, MemberSlotFunction i_fp)
    : FunctionSlotBase(i_object_name)
    , m_fp(i_fp)
    , m_target_obj(i_target_obj)
    {
    }

    /*! \fn virtual ~MemberFunctionSlot<Type>();
     *  \brief The destructor of MemberFunctionSlot Class.
     */
    virtual ~MemberFunctionSlot<Type>()
    {
    }
public:
    /*! \fn bool NotifyFunction(const EventArg& i_src_arg) override;
     *  \param [in] i_src_arg The event arg we want to send out.
     *  \brief We will deliver params by arg to target member slot function.
     */
    bool NotifyFunction(const EventArg & i_src_arg) override
    {
        if (m_target_obj.IsNull() == false) {
            if (m_fp != nullptr) {
                (SD_WREF(m_target_obj).*m_fp)(i_src_arg);
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    
    /*! \fn bool IsInvalid() override;
     *  \brief Return the result about that this slot is invalid or not. Condition of invalid member slot is one of m_fp or m is nullptr.
     */
    bool IsInvalid() const override
    {
        return (m_target_obj.IsNull() == true);
    }

    /*! \fn bool IsEqualTo(const Object &i_src) const;
     *  \param [in] i_src The another slot instance.
     *  \brief Return the result about i_src.m_fp and i_src.m \n
     *         are equal to this.m_fp and this->m or not.
     */
    bool IsEqualTo(const Object &i_src) const override
    {
        const MemberFunctionSlot<Type> *src_ptr = dynamic_cast<const MemberFunctionSlot<Type>* >(&i_src);
        //Compare owner and function pointer of two current slot .
        if (src_ptr != nullptr) {
            //----- both same.
            return (m_fp == src_ptr->m_fp && m_target_obj.GetPtr() == src_ptr->m_target_obj.GetPtr());
        }
        else {
            //error : Is different type.
            return false;
        }
    }
public:
    /*! \fn void RegisterFunction(const WeakReferenceObject<Type> &i, MemberSlotFunction m_fp);
     *  \param [in] i_target_obj The target slot object.
     *  \param [in] i_fp The target slot function.
     *  \tparam Type Target Type.
     *  \brief  Register the i_fp and i_src as the slot object and function.
     */
    void RegisterFunction(const WeakReferenceObject<Type> &i_target_obj, MemberSlotFunction i_fp)
    {
        m_target_obj = i_target_obj;
        m_fp = i_fp;
    }
public:
    /*! \var MemberSlotFunction m_fp;
     *  \brief Slot callback function. Default is nullptr.
     */
    MemberSlotFunction m_fp;

    /*! \var WeakReferenceObject<Type> m_target_obj;
     *  \brief Slot object. Default is null shared reference.
     */
    WeakReferenceObject<Type> m_target_obj;
};

_______________SD_END_BASIC_NAMESPACE________________