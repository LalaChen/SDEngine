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

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

#include "EventObject.h"

______________SD_START_BASIC_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(KeyMapManager);

class SDENGINE_CLASS KeyMapManager : public EventObject
{
public:
    KeyMapManager(const ObjectName &i_object_name);

    virtual ~KeyMapManager();
public:
    /*! \fn virtual void SetKeyboardStatus(uint32_t i_key_id, bool i_is_pressed);
     *  \param [in] i_key_id KeyID.
     *  \param [in] i_is_pressed Pressed.
     *  \brief Set key status.
     */
    virtual void SetKeyboardStatus(int32_t i_key_id, bool i_is_pressed);
protected:
    /*! \var bool m_key_maps[256];
     *  \brief Cache press state of each key.
     */
    bool m_key_maps[256];

    /*! \var EventWeakReferenceObject m_key_event_wref;
     *  \brief weak reference of key event.
     */
    EventWeakReferenceObject m_key_event_wref;
};

_______________SD_END_BASIC_NAMESPACE________________