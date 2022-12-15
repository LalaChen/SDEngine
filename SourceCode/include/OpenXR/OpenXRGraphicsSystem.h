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

/*! \file      OpenXRGraphicsSystem.h
 *  \brief     The class OpenXRGraphicsSystem is used to perform rendering commands in OpenXR.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/25
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "GraphicsSystem.h"

using SDE::Graphics::GraphicsSystem;
using SDE::Graphics::GraphicsSystemStrongReferenceObject;
using SDE::Graphics::GraphicsSystemWeakReferenceObject;

______________SD_START_OPENXR_NAMESPACE______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(OpenXRGraphicsSystem)

class SDENGINE_CLASS OpenXRGraphicsSystem : public GraphicsSystem
{
public:
    explicit OpenXRGraphicsSystem(const ObjectName &i_object_name);
    virtual ~OpenXRGraphicsSystem();
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
    void Resize() override;
protected:
    EntityGroupWeakReferenceObject m_space_eg;
};


_______________SD_END_OPENXR_NAMESPACE_______________