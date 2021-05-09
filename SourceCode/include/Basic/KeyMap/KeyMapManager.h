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

enum KeyCodeEnum
{
    KEY_BACKSPACE = 0x08u,
    KEY_TAB = 0x09u,
    KEY_ENTER = 0x0Du,
    KEY_CAPS_LOCK = 0x14u, //caps lock.
    KEY_PAUSE = 0x13u,
    KEY_SPACE = 0x20u,
    KEY_PAGE_UP = 0x21u,
    KEY_PAGE_DOWN = 0x22u,
    KEY_END = 0x23u,
    KEY_HOME = 0x24u,
    //
    KEY_LEFT = 0x25u,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    //
    KEY_PRINT_SNAPSHOT = 0x2Cu,
    KEY_INSERT = 0x2Du,
    KEY_DELETE = 0x2Eu,

    //--- Digital
    KEY_0 = 0x30u,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    //-- Alphabet.
    KEY_A = 0x41u,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    //Numpad.
    KEY_NUMPAD0 = 0x60u,
    KEY_NUMPAD1,
    KEY_NUMPAD2,
    KEY_NUMPAD3,
    KEY_NUMPAD4,
    KEY_NUMPAD5,
    KEY_NUMPAD6,
    KEY_NUMPAD7,
    KEY_NUMPAD8,
    KEY_NUMPAD9,
    KEY_MULTIPLY, // '*'
    KEY_ADD, //'+'
    KEY_SEPARATOR,
    KEY_SUBTRACT,
    KEY_DECIMAL,
    KEY_DIVIDE,
    //function.
    KEY_F1 = 0x70u,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,

    //
    KEY_LEFT_SHIFT = 0xA0u,
    KEY_RIGHT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_RIGHT_CONTROL,
    KEY_LEFT_ALT,
    KEY_RIGHT_ALT,

    KEY_SEMICOLON = 0xBAu, // ';:'
    KEY_EQUAL = 0xBBu, // '='
    KEY_COMMA = 0xBCu, // ',<'
    KEY_MINUS = 0xBDu, // '-_'
    KEY_PERIOD = 0xBEu, // '.>'
    KEY_SLASH = 0xBFu, // '/?'
    //
    KEY_APOSTROPHE = 0xC0u, //~
    //
    KEY_LEFT_BRACKET = 0xDBu, //'['
    KEY_BACKSLASH = 0xDCu, // '\'
    KEY_RIGHT_BRACKET = 0xDDu, //']'
    KEY_GRAVE_ACCENT = 0xDE, //'''
    //
    KEY_MAX_NUMBER = 256
};

enum KeyStatusEnum
{
    KEY_STATUS_RELEASE = 0,
    KEY_STATUS_PRESS,
    KEY_STATUS_REPEAT,
    KEY_STATUS_NOT_SUPPORT,
    KEY_STATUS_MAX_DEFINE_VALUE
};

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(KeyMapManager);

class SDENGINE_CLASS KeyMapManager : public EventObject
{
public:
    KeyMapManager(const ObjectName &i_object_name);

    virtual ~KeyMapManager();
public:
    /*! \fn virtual void SetKeyboardStatus(uint32_t i_key_id, bool i_is_pressed);
     *  \param [in] i_key_id Ascii Code KeyID.
     *  \param [in] i_is_pressed Pressed.
     *  \brief Set key status.
     */
    virtual void SetKeyboardStatus(int32_t i_key_id, bool i_is_pressed);
protected:
    /*! \var EventWeakReferenceObject m_key_event_wref;
     *  \brief weak reference of key event.
     */
    EventWeakReferenceObject m_key_event;
};

_______________SD_END_BASIC_NAMESPACE________________