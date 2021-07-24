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

/*! \file      IMGUITextLabel.h
 *  \brief     The class IMGUITextLabel is used to show text.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/01
 *  \copyright MIT License.
 */

#pragma once

#include "IMGUINode.h"

using SDE::Basic::ObjectName;

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectWeakReferenceObject;
using SDE::Basic::EventObjectStrongReferenceObject;

________________SD_START_GUI_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(IMGUITextLabel);

class SDENGINE_CLASS IMGUITextLabel : public IMGUINode
{
public:
    explicit IMGUITextLabel(const ObjectName &i_name, const std::string &i_text);
    virtual ~IMGUITextLabel();
public:
    void SetText(const std::string &i_text);
public:
    void RecordCommand() override;
    void Append(const IMGUINodeStrongReferenceObject &i_child) override;
protected:
    std::string m_text;
};

_________________SD_END_GUI_NAMESPACE________________