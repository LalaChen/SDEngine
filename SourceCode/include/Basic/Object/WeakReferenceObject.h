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

/*! \file      WeakReferenceObject.h
 *  \brief     Introduce of class WeakRefernceObject
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/07
 *  \copyright MIT License.
 */

#pragma once

#include <memory>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "NullRefException.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class WeakReferenceObject
 *  \brief In our system, WeakReferenceObject is a template class for making weak reference object of \n
 *  Type class. It means the reference shouldn't add use count. Actually, this class is extended \n
 *  class about std::weak_ptr.
 */
template <class Type> class WeakReferenceObject : public std::weak_ptr<Type>
{
public:
    /*! \fn WeakReferenceObject<Type>()
     *  \tparam Type class Type.
     *  \brief The constructor of WeakReferenceObject Class.
     */
    WeakReferenceObject<Type>()
    : std::weak_ptr<Type>()
    {
    }

    /*! \fn WeakReferenceObject<Type>(const std::shared_ptr<Type> &i_src);
     *  \tparam Type class Type.
     *  \param [in] i_src Target std::shared_ptr.
     *  \brief The copy constructor of WeakReferenceObject Class.
     */
    WeakReferenceObject<Type>(const std::shared_ptr<Type> &i_src)
    : std::weak_ptr<Type>(i_src)
    {
    }

    /*! \fn WeakReferenceObject<Type>(const std::weak_ptr<Type> &i_src)
     *  \tparam Type class Type.
     *  \param [in] i_src Target std::weak_ptr.
     *  \brief The copy constructor of WeakReferenceObject Class.
     */
    WeakReferenceObject<Type>(const WeakReferenceObject<Type> &i_src)
    : std::weak_ptr<Type>(i_src)
    {
    }

    /*! \fn WeakReferenceObject<Type>(const std::weak_ptr<Type> &i_src)
     *  \tparam Type class Type.
     *  \param [in] i_src Target WeakReferenceObject.
     *  \brief The copy constructor of WeakReferenceObject Class.
     */
    WeakReferenceObject<Type>(const std::weak_ptr<Type> &i_src)
    : std::weak_ptr<Type>(i_src)
    {
    }
public:

    /*! \fn template<class DerivedType> WeakReferenceObject<DerivedType> DynamicCastTo() const;
     *  \tparam DerivedType target derived class type.
     *  \brief Try to dynamic cast this to derived class type.
     */
    template<class DerivedType> WeakReferenceObject<DerivedType> DynamicCastTo() const
    {
        return std::weak_ptr<DerivedType>(std::dynamic_pointer_cast<DerivedType>(this->lock()));
    }

    /*! \fn template<class DerivedType> std::shared_ptr<DerivedType> DynamicCastToSharedPtr() const
     *  \tparam DerivedType target derived class type.
     *  \brief Try to dynamic cast this to derived class type.
     */
    template<class DerivedType> std::shared_ptr<DerivedType> DynamicCastToSharedPtr() const
    {
        return std::dynamic_pointer_cast<DerivedType>(this->lock());
    }

    /*! \fn template<class BasedType> WeakReferenceObject<BasedType> StaticCastTo() const
     *  \tparam BasedType target based class type.
     *  \brief Try to static cast this to based class type.
     */
    template<class BasedType> WeakReferenceObject<BasedType> StaticCastTo() const
    {
        return std::weak_ptr<BasedType>(std::static_pointer_cast<BasedType>(this->lock()));
    }

    /*! \fn template<class BasedType> std::shared_ptr<BasedType> StaticCastToSharedPtr() const
     *  \tparam BasedType target based class type.
     *  \brief Try to static cast this to based class type.
     */
    template<class BasedType> std::shared_ptr<BasedType> StaticCastToSharedPtr() const
    {
        return std::static_pointer_cast<BasedType>(this->lock());
    }
public:
    /*! \fn bool IsEqualTo(const WeakReferenceObject<Type> &i_src) const
     *  \tparam Type target class type.
     *  \param [in] i_src target weak ref.
     *  \brief Try to static cast this to based class type. Please note we should use this 
     *         template on class with IsEqualTo method(Inherit class Object).
     */
    bool IsEqualTo(const WeakReferenceObject<Type> &i_src) const
    {
        Type *this_ptr = this->lock().get();
        Type *src_ptr = i_src.lock().get();
        if (this_ptr != nullptr && src_ptr != nullptr) {
            if (this_ptr != src_ptr)
                return this_ptr->IsEqualTo(*src_ptr);
            else
                return true;
        }
        else {
            return false;
        }
    }
public:
    /*! \fn Type& GetRef() const;
     *  \brief return reference of this.
     */
    Type& GetRef() const
    {
        if (this->lock())
            return *(this->lock().get());
        else throw NullReferenceException();
    }

    /*! \fn const Type& GetConstRef() const;
     *  \brief return const reference of this.
     */
    const Type& GetConstRef() const
    {
        if (this->lock())
            return *(this->lock().get());
        else throw NullReferenceException();
    }

    /*! \fn Type* GetPtr() const;
     *  \brief return pointer of this.
     */
    Type* GetPtr() const
    {
        return this->lock().get();
    }

    /*! \fn const Type* GetConstPtr() const;
     *  \brief return const pointer of this.
     */
    const Type* GetConstPtr() const
    {
        return this->lock().get();
    }

    /*! \fn bool IsNull() const;
     *  \brief return true if this reference is null.
     */
    bool IsNull() const
    {
        return (this->lock().get() == nullptr);
    }

    /*! \fn long UseCount() const;
     *  \brief return how many shared references keeping this instance.
     */
    long UseCount() const
    {
        if (this->lock().get() == nullptr) return 0;
        else return this->use_count();
    }
};

_______________SD_END_BASIC_NAMESPACE________________