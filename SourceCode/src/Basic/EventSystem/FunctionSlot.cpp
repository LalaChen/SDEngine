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

#include "FunctionSlot.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

//class GlobalFunctionSlot

GlobalFunctionSlot::GlobalFunctionSlot(const ObjectName &i_object_name, GlobalSlotFunction i_fp)
: FunctionSlotBase(i_object_name)
, m_fp(i_fp)
{
}

GlobalFunctionSlot::~GlobalFunctionSlot()
{
}

void GlobalFunctionSlot::RegisterFunction(GlobalFunctionSlot::GlobalSlotFunction i_function_pointer)
{
    m_fp = i_function_pointer;
}

bool GlobalFunctionSlot::NotifyFunction(const EventArg &i_src_arg)
{
    if(m_fp != 0){
        m_fp(i_src_arg);
        return true;
    }
    else
    {
        return false;
    }
}

bool GlobalFunctionSlot::IsEqualTo(const Object &i_src) const
{
    const GlobalFunctionSlot *src_ptr = dynamic_cast<const GlobalFunctionSlot*>(&i_src);

    if(src_ptr != nullptr){
        return (m_fp == src_ptr->m_fp);
    }
    else{
        //error : Is not GlobalFunctionSlot object.
        return false;
    }
}

//---------------------------- end of namespace Basic ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}