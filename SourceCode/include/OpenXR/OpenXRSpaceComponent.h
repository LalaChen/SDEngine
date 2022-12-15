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

/*! \file      OpenXRSpaceComponent.h
 *  \brief     The class OpenXRSpaceComponent is used to perform rendering for XR at location of owner(Entity).
 *             We will register it to Entity for performing application logic.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/10/16
 *  \copyright MIT License.
 */

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>

#include "VRCameraComponent.h"

using SDE::Math::Transform;
using SDE::Math::Quaternion;
using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;

using SDE::Basic::Component;

using SDE::Graphics::TransformComponent;
using SDE::Graphics::TransformComponentStrongReferenceObject;
using SDE::Graphics::TransformComponentWeakReferenceObject;

using SDE::Graphics::VRCameraComponent;
using SDE::Graphics::VRCameraComponentStrongReferenceObject;
using SDE::Graphics::VRCameraComponentWeakReferenceObject;

______________SD_START_OPENXR_NAMESPACE______________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(OpenXRSpaceComponent);

class OpenXRSpaceComponent : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(OpenXRSpaceComponent, OpenXRSpaceComponent);
public:
    explicit OpenXRSpaceComponent(const ObjectName &i_object_name, XrReferenceSpaceType i_space_type);
    virtual ~OpenXRSpaceComponent();
public:
    void SetPoseOffset(const Transform &i_offset);
    void SetFramePredictTimePerFrame(float i_frame);
protected:
    void InitializeImpl() override;
protected:
    TransformComponentWeakReferenceObject m_xform_comp;
protected:
    XrSpace m_space;
    XrPosef m_world_offset;
    XrReferenceSpaceType m_space_type;
};

_______________SD_END_OPENXR_NAMESPACE_______________
