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

/*! \file      Object.h
 *  \brief     Introduce of class Object
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/07
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "StrongReferenceObject.h"
#include "WeakReferenceObject.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Object);

/*! \class Object
 *  \brief In our system, class object is the root of all classes those instances will be kept
 *  in several places. What's more, we will define some common behavior for those object.
 */
class SDENGINE_CLASS Object : public std::enable_shared_from_this<Object>
{
public:
    /*! \fn explicit Object(const ObjectName &i_object_name);
     *  \brief The constructor of Object Class.
     */
    explicit Object(const ObjectName &i_object_name);

    /*! \fn virtual ~Object();
     *  \brief The destructor of Object Class.
     */
    virtual ~Object();
public:

    /*! \fn virtual bool IsEqualTo(const Object &i_src) const;
     *  \param [in] i_src Target object.
     *  \brief Common behavior of all derived class. It used to compare two
     *         objects is equivalent. In class object, we return true if
     *         the result that compare name of i_src and this is equal 0.
     */
    virtual bool IsEqualTo(const Object &i_src) const;

    /*! \fn virtual std::string ToString() const;
     *  \brief Common behavior of all derived class
     */
    virtual std::string ToString() const;
public:
    /*! \fn std::shared_ptr<Object> GetThisSharedPtr();
     *  \brief Return this shared_ptr.
     */
    std::shared_ptr<Object> GetThisSharedPtr()
    {
        return shared_from_this();
    }

    /*! \fn template<class T> std::shared_ptr<T> GetThisSharedPtrByType();
     *  \brief Return this shared_ptr by type T.
     */
    template<class T> std::shared_ptr<T> GetThisSharedPtrByType()
    {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    /*! \fn template<class T> std::weak_ptr<T> GetThisWeakPtrByType();
     *  \brief Return this weak_ptr by type T.
     */
    template<class T> std::weak_ptr<T> GetThisWeakPtrByType()
    {
        std::weak_ptr<T> this_obj(std::dynamic_pointer_cast<T>(shared_from_this()));
        return this_obj;
    }

protected:

    /*! \var ObjectName m_object_name;
     *  \brief object name. [VarGet Attribute]
     */
    SD_DECLARE_ATTRIBUTE_VAR_GET(ObjectName, m_object_name, ObjectName);
};

_______________SD_END_BASIC_NAMESPACE________________