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

/*! \file      LightComponent.h
 *  \brief     The class LightComponent is used to keep light data.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/10/18
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "LightUniforms.h"
#include "TransformComponent.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"
#include "Component.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

using SDE::Basic::Component;
using SDE::Basic::ComponentStrongReferenceObject;
using SDE::Basic::ComponentWeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(LightComponent);

class SDENGINE_CLASS LightComponent : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(LightComponent, LightComponent);
public:
    explicit LightComponent(const ObjectName &i_object_name);
    virtual ~LightComponent();
public:
    void Initialize();
    bool OnGeometryChanged(const EventArg &i_arg);
public:
    const DescriptorSetWeakReferenceObject GetDescriptorSet() const;
    void SetLightParameter(const LightUniforms &i_light_data);
protected:
    void InitializeDescriptorSetAndPool();
    void InitializeShadowMappingWorkspace();
protected:
    DescriptorPoolStrongReferenceObject m_dp_sref;
    UniformBufferWeakReferenceObject m_buffer_wref;
    DescriptorSetWeakReferenceObject m_ds_wref;
protected:
    std::vector<TextureStrongReferenceObject> m_cascade_map_srefs;
protected:
    DescriptorSetWeakReferenceObject m_sp_ds_wref;
    UniformImagesWeakReferenceObject m_cascade_map_ui_wref;
protected:
    LightUniforms m_light_params;
protected:
    TransformComponentWeakReferenceObject m_geo_comp_wref;
};

inline const DescriptorSetWeakReferenceObject LightComponent::GetDescriptorSet() const
{
    return m_ds_wref;
}

inline void LightComponent::SetLightParameter(const LightUniforms &i_light_data)
{
    m_light_params = i_light_data;
}

______________SD_END_GRAPHICS_NAMESPACE______________