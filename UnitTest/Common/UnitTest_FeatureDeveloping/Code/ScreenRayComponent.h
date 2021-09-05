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

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(ScreenRayComponent);

class ScreenRayComponent : public SDE::Basic::Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(ScreenRayComponent, ScreenRayComponent);
public:
    explicit ScreenRayComponent(const ObjectName &i_name);
    virtual ~ScreenRayComponent();
public:
    virtual void Initialize();
    virtual void Update();
protected:
    MeshStrongReferenceObject m_mesh;
    MeshRenderComponentWeakReferenceObject m_mesh_render;
    MaterialStrongReferenceObject m_material;
    TransformComponentWeakReferenceObject m_transform;
};