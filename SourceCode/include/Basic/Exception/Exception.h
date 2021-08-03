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

/*! \file      Exception.h
 *  \brief     Introduce of classes about all exception.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/31
 *  \copyright MIT License.
 */

#pragma once

#include <exception>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \class ReasonableException
 *  \brief In our system, class ReasonableException throw when we need to throw exception for some reason.
 *  or weak reference object.
 */
class ReasonableException : public std::exception
{
public:
    ReasonableException(const std::string &i_msg);
    ~ReasonableException();
public:
    /*! \fn virtual const char* what() const throw();
     *  \brief override what function for using.
     */
    virtual const char* what() const throw() { return m_msg.c_str(); }
protected:
    /*! \fn std::string m_msg;
     *  \brief exception msg.
     */
    std::string m_msg;
};

inline ReasonableException::ReasonableException(const std::string& i_msg)
{
    m_msg = StringFormat("ReasonableException : %s.", i_msg.c_str());
}

inline ReasonableException::~ReasonableException()
{
}

/*! \class NullReferenceException
 *  \brief In our system, class NullReferenceException throw when we try to GetRef of null strong
 *  or weak reference object.
 */
class NullReferenceException : public std::exception
{
public:
    /*! \fn virtual const char* what() const throw();
     *  \brief override what function for using.
     */
    virtual const char* what() const throw() { return "NullReferenceException"; }
};

_______________SD_END_BASIC_NAMESPACE________________