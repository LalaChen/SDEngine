#include "WaveVRControllerComponent.h"

WaveVRControllerComponent::WaveVRControllerComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
}

WaveVRControllerComponent::~WaveVRControllerComponent()
{
}

void WaveVRControllerComponent::Initialize()
{
    EntityWeakReferenceObject entity = GetEntity();
    m_transform = SD_GET_COMP_WREF(entity, TransformComponent);
    //1. Load material.
    m_material = new Material("CtrlerDefaultMat");
    SD_SREF(m_material).BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("NoLightShader"));
    //Set data done. Link with shader program.(Write descirptor)
    SD_SREF(m_GUI_material).LinkWithShaderProgram();
    MaterialUniforms mat_ub; //use default color.
    SD_SREF(m_GUI_material).SetDataToUniformBuffer(sUniformBuffer_Material, &mat_ub, sizeof(MaterialUniforms));
}

void WaveVRControllerComponent::Update()
{

}

void WaveVRControllerComponent::OnConnected()
{

}

void WaveVRControllerComponent::OnDisconnected()
{

}

void WaveVRControllerComponent::LoadModelAsync()
{

}

