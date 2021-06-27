/* ==============  SD Engine License ==============
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

/*! \file      SDEngineMacro.h
 *  \brief     SDEngineMacro collects a lot of useful macro about attribute getter/setter.
 *             Those macro will be used widely in SDEngine and others side project made by me.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/01
 *  \copyright MIT License.
 */
#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning( disable : 4251 ) //For dllexport warning of STL
#endif

//DLL Import/Export
#if defined(SDENGINE_EXPORTS)
    #if defined(_WIN32) || defined(_WIN64)
        #define SDENGINE_API __declspec(dllexport)
    #else
        #define SDENGINE_API
    #endif
#else
    #if defined(_WIN32) || defined(_WIN64)
        #define SDENGINE_API __declspec(dllimport)
    #else
        #define SDENGINE_API
    #endif
#endif

#ifdef SDENGINE_EXPORTS
    #if defined(_WIN32) || defined(_WIN64)
        #define SDENGINE_CLASS __declspec(dllexport)
    #else
        #define SDENGINE_CLASS
    #endif
#else
    #if defined(_WIN32) || defined(_WIN64)
        #define SDENGINE_CLASS __declspec(dllimport)
    #else
        #define SDENGINE_CLASS
    #endif
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define SDENGINE_STDCALL_API __stdcall
#else
    #define SDENGINE_STDCALL_API
#endif

//Code Page
#define SD_DEFAULT_CODE_PAGE 936

#ifndef SD_CODE_PAGE
#define SD_CODE_PAGE SD_DEFAULT_CODE_PAGE
#endif

#ifdef UNICODE 
#define SD_ADT_OS_STR(str) SDE::Basic::StringToWString(str, SD_CODE_PAGE)
#define SD_ADT_OS_STRCSTR(str) SDE::Basic::StringToWString(str, SD_CODE_PAGE).c_str()
#define SD_CVT_OS_CHARS_TOSTR(chars) SDE::Basic::WStringToString(chars, SD_CODE_PAGE)
typedef wchar_t SDStrChar;
#elif _UNICODE
#define SD_ADT_OS_STR(str) SDE::Basic::StringToWString(  str, SD_CODE_PAGE)
#define SD_ADT_OS_STRCSTR(str) SDE::Basic::StringToWString(  str, SD_CODE_PAGE).c_str()
#define SD_CVT_OS_CHARS_TOSTR(chars) SDE::Basic::WStringToString(chars, SD_CODE_PAGE)
typedef wchar_t SDStrChar;
#else
#define SD_ADT_OS_STR(str) (str)
#define SD_ADT_OS_STRCSTR(str) (str).c_str()
#define SD_CVT_OS_CHARS_TOSTR(chars) std::string(chars)
typedef char SDStrChar;
#endif

//Namespace
#define ______________SD_START_BASIC_NAMESPACE_______________ \
namespace SDE \
{ \
namespace Basic \
{

#define _______________SD_END_BASIC_NAMESPACE________________ \
} \
}

#define _____________SD_START_GRAPHICS_NAMESPACE_____________ \
namespace SDE \
{ \
namespace Graphics \
{

#define ______________SD_END_GRAPHICS_NAMESPACE______________ \
} \
}

#define _______________SD_START_MATH_NAMESPACE_______________ \
namespace SDE \
{ \
namespace Math \
{

#define ________________SD_END_MATH_NAMESPACE________________ \
} \
}

#define ________________SD_START_APP_NAMESPACE_______________ \
namespace SDE \
{ \
namespace App \
{

#define _________________SD_END_APP_NAMESPACE________________ \
} \
}

//Strong And Weak Reference.
#define SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Type) \
    class Type; \
    typedef SDE::Basic::StrongReferenceObject<Type> Type##StrongReferenceObject; \
    typedef SDE::Basic::WeakReferenceObject<Type> Type##WeakReferenceObject;

//Singleton Define
/*! \def SD_SINGLETON_DECLARATION(ClassName)
 *  \brief Declare singleton necessary method and member. Use this macro on the class declaration(.h).
 */
#define SD_SINGLETON_DECLARATION( ClassName ) \
    public: \
        static ClassName &GetRef(); \
        static ClassName *GetPtr(); \
        static bool IsNull(); \
        static void Destroy(); \
    public: \
        template<class DerivedType> static DerivedType* GetDynamicCastPtr() \
        { \
            return dynamic_cast<DerivedType*>(ClassName::GetPtr()); \
        } \
        template<class DerivedType> static DerivedType& GetDynamicCastRef() \
        { \
            return dynamic_cast<DerivedType&>(ClassName::GetRef()); \
        } \
    protected: \
        static ClassName *m_instance;

/*! \def SD_SINGLETON_DECLARATION_IMPL(ClassName)
 *  \brief implement singleton necessary method and member. Use this macro at class implementation(.cpp).
 */
#define SD_SINGLETON_DECLARATION_IMPL( ClassName ) \
    ClassName* ClassName::m_instance = nullptr; \
    \
    ClassName& ClassName::GetRef() \
    { \
        return *m_instance;  \
    } \
    \
    ClassName* ClassName::GetPtr() \
    { \
        return m_instance; \
    } \
    \
    bool ClassName::IsNull() \
    { \
        return (m_instance == nullptr);  \
    } \
    void ClassName::Destroy() \
    { \
        if(m_instance != nullptr){ \
             delete m_instance; \
             m_instance = nullptr; \
        } \
    }

/*! \def SD_SINGLETON_DECLARATION_REGISTER
 *  \brief assign instance. Use this macro at class ctor(.cpp).
 */
#define SD_SINGLETON_DECLARATION_REGISTER \
    if (m_instance != nullptr) \
    { \
        /*SDLOGE("m_instance isn't nullptr!!!.");*/ \
    } \
    m_instance = this;

#define SD_COMPONENT_POOL_TYPE_DECLARATION(CompType, CompPoolType) \
    static std::type_index GetComponentPoolType() { \
         return std::type_index(typeid(CompPoolType)); \
    };

//Variable Get/Set Declaration
/*! \def SD_DECLARE_ATTRIBUTE_VAR_GET(Type,VarName,FunctionName)
 *   \brief Implement a c++ attribute with an getter. \n
 *          Type is the type of attribute. \n
 *          VarName is the name of class member var. \n
 *          FunctionName is name of getter. \n
 *          The getter return variable. So we suggest we use it to declare a based type attribute.
 */
#define SD_DECLARE_ATTRIBUTE_VAR_GET(Type,VarName,FunctionName) \
    protected: Type VarName; \
    public:    Type Get##FunctionName() const{ return VarName; }

/*! \def SD_DECLARE_ATTRIBUTE_CONSTREF_GET(Type,VarName,FunctionName)
 *  \brief Implement a c++ attribute with an getter. \n
 *         Type is the type of attribute. \n
 *         VarName is the name of class member var. \n
 *         FunctionName is name of getter. \n
 *         The getter return referenece.
 */
#define SD_DECLARE_ATTRIBUTE_CONSTREF_GET(Type,VarName,FunctionName) \
    protected: Type VarName; \
    public:    const Type& Get##FunctionName() const{return VarName;}

/*! \def SD_DECLARE_ATTRIBUTE_VAR_SET_VAR_GET(Type,VarName,FunctionName)
 *  \brief Implement a c++ attribute with an getter and an getter. \n
 *         Type is the type of attribute. \n
 *         VarName is the name of class member var. \n
 *         FunctionName is name of getter and setter. \n
 *         The getter return variable. So we suggest we use it to declare a based type attribute. \n
 *         The setter is setted by variable.
 */

#define SD_DECLARE_ATTRIBUTE_VAR_SET_VAR_GET(Type,VarName,FunctionName) \
    protected: Type VarName; \
    public:    Type Get##FunctionName() const{return VarName;} \
    public:    void Set##FunctionName(Type i_##VarName){VarName = i_##VarName;}

/*! \def SD_DECLARE_ATTRIBUTE_CONSTREF_SET_VAR_GET(Type,VarName,FunctionName)
 *  \brief Implement a c++ attribute with an getter and an getter. \n
 *         Type is the type of attribute. \n
 *         VarName is the name of class member var. \n
 *         FunctionName is name of getter and setter. \n
 *         The getter return variable. So we suggest we use it to declare a based type attribute. \n
 *         The setter is setted by variable.
 */
#define SD_DECLARE_ATTRIBUTE_CONSTREF_SET_VAR_GET(Type,VarName,FunctionName) \
    protected: Type VarName; \
    public:    Type Get##FunctionName() const{return VarName;} \
    public:    void Set##FunctionName(const Type &i_##VarName){VarName = i_##VarName;}

/*! \def SD_DECLARE_ATTRIBUTE_VAR_SET_CONSTREF_GET(Type,VarName,FunctionName)
 *  \brief Implement a c++ attribute with an getter and an getter. \n
 *         Type is the type of attribute. \n
 *         VarName is the name of class member var. \n
 *         FunctionName is name of getter and setter. \n
 *         The getter return reference. \n
 *         The setter is setted by variable.
 */
#define SD_DECLARE_ATTRIBUTE_VAR_SET_CONSTREF_GET(Type,VarName,FunctionName) \
    protected: Type VarName; \
    public:    const Type& Get##FunctionName() const{return VarName;} \
    public:    void        Set##FunctionName(Type i_##VarName){VarName = i_##VarName;}

/*! \def SD_DECLARE_ATTRIBUTE_CONSTREF_SET_CONSTREF_GET(Type,VarName,FunctionName)
 *  \brief Implement a c++ attribute with an getter and an getter. \n
 *         Type is the type of attribute. \n
 *         VarName is the name of class member var. \n
 *         FunctionName is name of getter and setter. \n
 *         The getter return reference. \n
 *         The setter is setted by reference.
 */
#define SD_DECLARE_ATTRIBUTE_CONSTREF_SET_CONSTREF_GET(Type,VarName,FunctionName) \
    protected: Type VarName; \
    public:    const Type& Get##FunctionName() const{return VarName;} \
    public:    void        Set##FunctionName(const Type &i_##VarName){VarName = i_##VarName;}

/*! \def ENUM_TO_UINT( var )
 *  \brief convert enum to unsigned int.
 */
#define SD_ENUM_TO_UINT( var ) static_cast<unsigned int>(var)

/*! \def ENUM_TO_INT( var )
 *  \brief convert enum to int.
 */
#define SD_ENUM_TO_INT( var )  static_cast<int>(var)

/*! \def SD_REF( var )
 *  \brief get object by reference.
 */
#define SD_REF( var ) var.GetRef()
#define SD_WREF( var ) SD_REF( var )
#define SD_SREF( var ) SD_REF( var )

#define SD_GET_COMP_WREF( var , type ) \
   SD_REF(var).GetComponent(typeid(type)).DynamicCastTo<type>()

#define SD_COMP_WREF( var , type ) \
    SD_GET_COMP_WREF(var, type).GetRef()

#define SD_GET_TYPE_COMP_WREF( var , type ) \
   SD_REF(var).GetComponentByType<type>()

#define SD_TYPE_COMP_WREF( var , type ) \
    SD_GET_TYPE_COMP_WREF(var, type).GetRef()

 /*! \def SD_REF( var )
  *  \brief get object by reference.
  */
#define SD_CONST_REF( var ) var.GetConstRef()
#define SD_CONST_WREF( var ) SD_CONST_REF( var )
#define SD_CONST_SREF( var ) SD_CONST_REF( var )

 /* \def SD_NULL_HANDLE
  * \brief null handle.
  */
#define SD_NULL_HANDLE 0

/* \def SD_SUBPASS_EXTERNAL
 * \brief external subpass id.
 */
#define SD_SUBPASS_EXTERNAL ~0U

 /* \def SD_ERROR_ATTACHMENT_REF
  * \brief initial value about attachment reference.
  */
#define SD_ERROR_ATTACHMENT_REF ~0U
