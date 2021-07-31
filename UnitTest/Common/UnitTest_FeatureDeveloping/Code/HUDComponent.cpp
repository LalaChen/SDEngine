#include "HUDComponent.h"

HUDComponent::HUDComponent(const ObjectName &i_name)
: Component(i_name)
{
}

HUDComponent::~HUDComponent()
{
}

void HUDComponent::Initialize()
{
    EntityWeakReferenceObject entity = GetEntity();
    m_GUI = SD_GET_COMP_WREF(entity, WorldGUIComponent);

    m_FPS_label = SD_WREF(m_GUI).GetGUINode<IMGUITextLabel>("FPSLabel");

    if (m_FPS_label.IsNull() == true) {
        SDLOGE("No FPSLabel !!!");
    }
}

void HUDComponent::Update()
{
    if (m_FPS_label.IsNull() == false) {
        float fps = GraphicsManager::GetRef().GetFPS();
        SD_WREF(m_FPS_label).SetText(SDE::Basic::StringFormat("FPS : %lf", fps));
    }
}