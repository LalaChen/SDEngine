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

/*! \file SDEngineMacro.h
    \brief SDEngineMacro collects a lot of useful macro about attribute getter/setter.
	       Those macro will be used widely in SDEngine and others side project made by me.
*/
#pragma once

#ifdef SDENGINE_EXPORTS
#define SDENGINE_API __declspec(dllexport)
#else
#define SDENGINE_API __declspec(dllimport)
#endif

#ifdef SDENGINE_EXPORTS
#define SDENGINE_CLASS __declspec(dllexport)
#else
#define SDENGINE_CLASS __declspec(dllimport)
#endif

//Variable Get/Set Declaration
/*! \def DECLARE_ATTRIBUTE_VAR_GET(Type,VarName,FunctionName)
    \brief Implement a c++ attribute with an getter. \n
	       Type is the type of attribute. \n
		   VarName is the name of class member var. \n
		   FunctionName is name of getter. \n
		   The getter return variable. So we suggest we use it to declare a based type attribute.
*/
#define DECLARE_ATTRIBUTE_VAR_GET(Type,VarName,FunctionName) \
	protected: Type VarName; \
	public:    Type Get##FunctionName() const{ return VarName; }

/*! \def DECLARE_ATTRIBUTE_CONSTREF_GET(Type,VarName,FunctionName)
    \brief Implement a c++ attribute with an getter. \n
	       Type is the type of attribute. \n
		   VarName is the name of class member var. \n
		   FunctionName is name of getter. \n
		   The getter return referenece.
*/
#define DECLARE_ATTRIBUTE_CONSTREF_GET(Type,VarName,FunctionName) \
	protected: Type VarName; \
	public:    const Type& Get##FunctionName() const{return VarName;}

/*! \def DECLARE_ATTRIBUTE_VAR_SET_VAR_GET(Type,VarName,FunctionName)
    \brief Implement a c++ attribute with an getter and an getter. \n
	       Type is the type of attribute. \n
		   VarName is the name of class member var. \n
		   FunctionName is name of getter and setter. \n
		   The getter return variable. So we suggest we use it to declare a based type attribute. \n
		   The setter is setted by variable.
*/
#define DECLARE_ATTRIBUTE_VAR_SET_VAR_GET(Type,VarName,FunctionName) \
	protected: Type VarName; \
	public:    Type Get##FunctionName() const{return VarName;} \
	public:    void Set##FunctionName(Type i_##VarName){VarName = i_##VarName;}

/*! \def DECLARE_ATTRIBUTE_CONSTREF_SET_VAR_GET(Type,VarName,FunctionName)
    \brief Implement a c++ attribute with an getter and an getter. \n
	       Type is the type of attribute. \n
		   VarName is the name of class member var. \n
		   FunctionName is name of getter and setter. \n
		   The getter return variable. So we suggest we use it to declare a based type attribute. \n
		   The setter is setted by variable.
*/
#define DECLARE_ATTRIBUTE_CONSTREF_SET_VAR_GET(Type,VarName,FunctionName) \
	protected: Type VarName; \
	public:    Type Get##FunctionName() const{return VarName;} \
	public:    void Set##FunctionName(const Type &i_##VarName){VarName = i_##VarName;}

/*! \def DECLARE_ATTRIBUTE_VAR_SET_CONSTREF_GET(Type,VarName,FunctionName)
    \brief Implement a c++ attribute with an getter and an getter. \n
	       Type is the type of attribute. \n
		   VarName is the name of class member var. \n
		   FunctionName is name of getter and setter. \n
		   The getter return reference. \n
		   The setter is setted by variable.
*/
#define DECLARE_ATTRIBUTE_VAR_SET_CONSTREF_GET(Type,VarName,FunctionName) \
	protected: Type VarName; \
	public:    const Type& Get##FunctionName() const{return VarName;} \
	public:    void        Set##FunctionName(Type i_##VarName){VarName = i_##VarName;}

/*! \def DECLARE_ATTRIBUTE_CONSTREF_SET_CONSTREF_GET(Type,VarName,FunctionName)
    \brief Implement a c++ attribute with an getter and an getter. \n
	       Type is the type of attribute. \n
		   VarName is the name of class member var. \n
		   FunctionName is name of getter and setter. \n
		   The getter return reference. \n
		   The setter is setted by reference.
*/
#define DECLARE_ATTRIBUTE_CONSTREF_SET_CONSTREF_GET(Type,VarName,FunctionName) \
	protected: Type VarName; \
	public:    const Type& Get##FunctionName() const{return VarName;} \
	public:    void        Set##FunctionName(const Type &i_##VarName){VarName = i_##VarName;}

/*! \def ENUM_TO_UINT( var )
    \brief convert enum to unsigned int.
*/
#define ENUM_TO_UINT( var ) static_cast<unsigned int>(var)

/*! \def ENUM_TO_INT( var )
    \brief convert enum to int.
*/
#define ENUM_TO_INT( var )  static_cast<int>(var)

//Reference Object Get/Set Declaration