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

/*! \file      StrongReferenceObject.h
 *  \brief     Introduce of class StrongReferenceObject
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

/*! \class StrongReferenceObject
 *  In our system, StrongReferenceObject is a template class for making strong reference object of \n
 *  Type class. Actually, this class is extended class about std::shared_ptr.
 */
template <class Type> class StrongReferenceObject : public std::shared_ptr<Type>
{
public:
    /*! \fn StrongReferenceObject<Type>();
     *  \tparam Type class Type.
     *  \brief The constructor of StrongReferenceObject Class.
     */
    StrongReferenceObject<Type>()
    : std::shared_ptr<Type>(nullptr)
    {
    }

    /*! \fn StrongReferenceObject<Type>(const StrongReferenceObject<Type> &i_src);
     *  \tparam Type class Type.
     *  \param i_src Target reference.
     *  \brief The copy constructor of StrongReferenceObject Class.
     */
    StrongReferenceObject<Type>(const StrongReferenceObject<Type> &i_src)
    : std::shared_ptr<Type>(i_src)
    {
    }

    /*! \fn StrongReferenceObject<Type>(const std::shared_ptr<Type> &i_src);
     *  \tparam Type class Type.
     *  \param i_src Target std::shared_ptr.
     *  \brief The copy constructor of StrongReferenceObject Class.
     */
    StrongReferenceObject<Type>(const std::shared_ptr<Type> &i_src)
    : std::shared_ptr<Type>(i_src)
    {
    }

    /*! \fn StrongReferenceObject<Type>(Type *i_ptr);
     *  \tparam Type class Type.
     *  \param i_ptr Target object.
     *  \brief The copy constructor of StrongReferenceObject Class.
     */
    StrongReferenceObject<Type>(Type *i_ptr)
    : std::shared_ptr<Type>(i_ptr)
    {
    }

public:

    /*! \fn template<class DerivedType> StrongReferenceObject<DerivedType> DynamicCastTo() const;
     *  \tparam DerivedType target derived class type.
     *  \brief Try to dynamic cast this to derived class type.
     */
    template<class DerivedType> StrongReferenceObject<DerivedType> DynamicCastTo() const
    {
        return std::dynamic_pointer_cast<DerivedType>(*this);
    }

    /*! \fn template<class DerivedType> std::weak_ptr<DerivedType> DynamicCastToPtr() const;
     *  \tparam DerivedType target derived class type.
     *  \brief Try to dynamic cast this to derived class type.
     */
    template<class DerivedType> std::weak_ptr<DerivedType> DynamicCastToPtr() const
    {
        return std::weak_ptr<DerivedType>( std::dynamic_pointer_cast<DerivedType>(*this) );
    }

    /*! \fn template<class BasedType> StrongReferenceObject<BasedType> StaticCastTo() const;
     *  \tparam BasedType target derived class type.
     *  \brief Try to static cast this to based class type.
     */
    template<class BasedType> StrongReferenceObject<BasedType> StaticCastTo() const
    {
        return std::static_pointer_cast<BasedType>(*this);
    }

    /*! \fn template<class BasedType> std::weak_ptr<BasedType> StaticCastToWeakPtr() const;
     *  \tparam BasedType target based class type.
     *  \brief Try to static cast this to based class type.
     */
    template<class BasedType> std::weak_ptr<BasedType> StaticCastToWeakPtr() const
    {
        return std::weak_ptr<BasedType>( std::static_pointer_cast<BasedType>(*this) );
    }
public:
    /*! \fn bool IsEqualTo(const StrongReferenceObject<Type> &i_src) const
     *  \param i_src target ref.
     *  \tparam Type target class type.
     *  \brief Return that this instance is equal to i_src or not.
     */
    bool IsEqualTo(const StrongReferenceObject<Type> &i_src) const
    {
        Type *this_ptr = this->get();
        Type *src_ptr = i_src.get();
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
        if ((*this))
            return *(this->get());
        else
            throw NullReferenceException();
    }

    /*! \fn const Type& GetConstRef() const;
     *  \brief return const reference of this.
     */
    const Type& GetConstRef() const
    {
        if ((*this))
            return *(this->get());
        else
            throw NullReferenceException();
    }

    /*! \fn Type* GetPtr() const;
     *  \brief return pointer of this.
     */
    Type* GetPtr() const
    {
        return this->get();
    }

    /*! \fn const Type* GetConstPtr() const;
     *  \brief return const pointer of this.
     */
    const Type* GetConstPtr() const
    {
        return this->get();
    }

    /*! \fn bool IsNull() const;
     *  \brief return true if this reference is null.
     */
    bool IsNull() const
    {
        return (this->get() == nullptr);
    }

    /*! \fn long UseCount() const;
     *  \brief return how many references keeping this instance.
     */
    long UseCount() const
    {
        if (this->get() == nullptr) return 0;
        else return this->use_count();
    }
};

_______________SD_END_BASIC_NAMESPACE________________