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

/*! \file      CameraComponentBase.h
 *  \brief     The class CameraComponentBase is the interface.
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/10
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "RenderFlow.h"
#include "Transform.h"
#include "Resolution.h"
#include "LightComponent.h"
#include "MeshRenderComponent.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

using SDE::Basic::Component;
using SDE::Basic::ComponentStrongReferenceObject;
using SDE::Basic::ComponentWeakReferenceObject;

using SDE::Math::Transform;
using SDE::Math::Vector3f;
using SDE::Math::Quaternion;
using SDE::Math::Matrix4X4f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(CameraComponentBase);

enum CameraWorkspaceType {
    CameraWorkspaceType_Forward = 0,
    CameraWorkspaceType_Deferred,
    CameraWorkspaceType_Other
};

class SDENGINE_CLASS CameraComponentBase : public Component
{
public:
    explicit CameraComponentBase(const ObjectName &i_object_name);
    virtual ~CameraComponentBase();
public:
    virtual bool OnGeometryChanged(const EventArg &i_arg) = 0;
public:
    virtual TextureWeakReferenceObject GetColorBuffer() const = 0;
    virtual TextureWeakReferenceObject GetDepthBuffer() const = 0;
public:
    virtual void Resize() = 0;
    virtual void RecordCommand(
        const CommandBufferWeakReferenceObject &i_cb,
        const std::list<LightComponentWeakReferenceObject> &i_light_list,
        const std::map<uint32_t, std::list<MeshRenderComponentWeakReferenceObject> > &i_mr_groups) = 0;
};

______________SD_END_GRAPHICS_NAMESPACE______________