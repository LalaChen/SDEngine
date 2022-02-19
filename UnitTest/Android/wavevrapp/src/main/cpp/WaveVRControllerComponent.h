#pragma once

#include <SDEngineMacro.h>
#include <SDEngineCommonType.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

#include "WaveCommonFunction.h"

using namespace SDE;
using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::GUI;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(WaveVRControllerComponent);

class WaveVRControllerComponent : public SDE::Basic::Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION(WaveVRControllerComponent, WaveVRControllerComponent);
public:
    explicit WaveVRControllerComponent(const ObjectName &i_object_name);
    virtual ~WaveVRControllerComponent();
public:
    void Initialize();
    void Update();
    void OnConnected();
    void OnDisconnected();
    void LoadModelAsync();
    void ReleaseModel();
    void SetControllerPose(const Transform &i_xform);
protected:
    std::list<MeshRenderComponentWeakReferenceObject> m_mrs;
    std::list<EntityWeakReferenceObject> m_entities;
    std::list<MeshStrongReferenceObject> m_models;
    std::list<MaterialStrongReferenceObject> m_materials;
    std::list<TextureStrongReferenceObject> m_texture;
protected:
    TransformComponentWeakReferenceObject m_transform;

};

inline void WaveVRControllerComponent::SetControllerPose(const Transform &i_xform)
{
    SD_WREF(m_transform).SetWorldTransform(i_xform);
}