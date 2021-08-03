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

/*! \file      IMGUIVectorLabel.h
 *  \brief     The class IMGUIVectorLabel is used to show vector3f label.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/01
 *  \copyright MIT License.
 */

#pragma once

#include "Vector3f.h"
#include "IMGUINode.h"

using SDE::Math::Vector3f;

using SDE::Basic::ObjectName;

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectWeakReferenceObject;
using SDE::Basic::EventObjectStrongReferenceObject;

________________SD_START_GUI_NAMESPACE_______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(IMGUIVectorLabel);

class SDENGINE_CLASS IMGUIVectorLabel : public IMGUINode
{
public:
    explicit IMGUIVectorLabel(const ObjectName &i_name, const std::string &i_text);
    virtual ~IMGUIVectorLabel();
public:
    void SetText(const std::string &i_text);
    void SetValue(const Vector3f &i_value);
public:
    void RecordCommand() override;
    void Append(const IMGUINodeStrongReferenceObject &i_child) override;
protected:
    std::string m_text;
    Vector3f m_value;
};

_________________SD_END_GUI_NAMESPACE________________