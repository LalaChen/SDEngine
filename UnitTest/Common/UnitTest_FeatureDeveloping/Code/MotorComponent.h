#pragma once

#include <SDEngineMacro.h>
#include <SDEngineCommonType.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

using SDE::Basic::ObjectName;
using SDE::Graphics::TransformComponentWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(MotorComponent);

class MotorComponent : public SDE::Basic::Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(MotorComponent, MotorComponent);
public:
    explicit MotorComponent(const ObjectName &i_name);
    virtual ~MotorComponent();
public:
    virtual void Initialize();
    virtual void Update();
protected:
    TransformComponentWeakReferenceObject m_trans_comp;
    Transform m_current_trans;
};