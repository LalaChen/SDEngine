#pragma once

#include <SDEngineMacro.h>
#include <SDEngineCommonType.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

using namespace SDE;
using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::GUI;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(HUDComponent);

class HUDComponent : public SDE::Basic::Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(HUDComponent, HUDComponent);
public:
    explicit HUDComponent(const ObjectName &i_name);
    virtual ~HUDComponent();
public:
    virtual void Initialize();
    virtual void Update();
protected:
    WorldGUIComponentWeakReferenceObject m_GUI;
    TransformComponentWeakReferenceObject m_transform;
    IMGUITextLabelWeakReferenceObject m_FPS_label;
    IMGUIVectorLabelWeakReferenceObject m_camera_pos_label;
    IMGUIVectorLabelWeakReferenceObject m_camera_rot_label;
};