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

/*! \file      WorldGUIComponentBase.h
 *  \brief     The class WorldGUIComponentBase is the based class of GUI in world.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/07/03
 *  \copyright MIT License.
 */

#pragma once

#include "Area.h"
#include "Ray.h"

#include "Component.h"
#include "TransformComponent.h"

using SDE::Basic::EventArg;
using SDE::Basic::Component;
using SDE::Basic::ComponentStrongReferenceObject;
using SDE::Basic::ComponentWeakReferenceObject;

using SDE::Physics::Ray;


_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(WorldGUIComponentBase);

class SDENGINE_CLASS WorldGUIComponentBase : public Component
{
public:
    explicit WorldGUIComponentBase(const ObjectName &i_object_name);
    virtual ~WorldGUIComponentBase();
public:
    void SetGUIAreaInWorldSpace(uint32_t i_width, uint32_t i_height, float i_world_w, float i_world_h);
    void SetGUIAreaInScreenSpace(AreaAlignOrientationEnum i_orientation, const Area2D &i_area);
public:
    virtual void RefreshTouchDataByRay(const Ray &i_ray, const TouchButton &i_tb) = 0;
protected:
    void InitializeImpl() override;
protected:
    void UpdateArea();
protected:
    TransformComponentWeakReferenceObject m_transform;
    TouchButton m_touch_data;
protected:
    Vector3f m_UI_vertices[4];
    uint32_t m_buffer_size[2];
    float    m_world_size[2];
protected:
    bool m_is_ncp;
    Area2D m_screen_area;
    DepthArea2D m_world_area;
    AreaAlignOrientationEnum m_area_orientation;
};


______________SD_END_GRAPHICS_NAMESPACE______________