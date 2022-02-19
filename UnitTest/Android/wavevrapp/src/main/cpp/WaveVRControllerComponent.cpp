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

