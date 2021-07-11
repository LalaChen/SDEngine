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

/*! \file      WorldGUISystem.h
 *  \brief     The class WorldGUISystem is used to perform rendering commands about all WorldGUI.
 *             This system priority will be higher than GraphicsSystem.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/04
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "EntitiyGroup.h"
#include "CommandPool.h"
#include "IMGUIRenderer.h"
#include "System.h"

using SDE::Basic::ObjectName;

using SDE::Basic::EntityGroup;
using SDE::Basic::EntityGroupStrongReferenceObject;
using SDE::Basic::EntityGroupWeakReferenceObject;

using SDE::Basic::System;
using SDE::Basic::SystemStrongReferenceObject;
using SDE::Basic::SystemWeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(WorldGUISystem)

class SDENGINE_CLASS WorldGUISystem : public System
{
public:
    explicit WorldGUISystem(const ObjectName &i_object_name);
    virtual ~WorldGUISystem();
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
    void Resize() override;
protected:
    EntityGroupWeakReferenceObject m_world_GUI_eg;
    CommandPoolStrongReferenceObject m_GUI_cp;
    CommandBufferWeakReferenceObject m_GUI_cb;
};

______________SD_END_GRAPHICS_NAMESPACE______________