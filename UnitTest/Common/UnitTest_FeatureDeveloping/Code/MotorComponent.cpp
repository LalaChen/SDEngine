#include "MotorComponent.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::Math;

MotorComponent::MotorComponent(const ObjectName &i_name)
: Component(i_name)
{
}

MotorComponent::~MotorComponent()
{
}

void MotorComponent::Initialize()
{
    m_trans_comp_wref = SD_GET_COMP_WREF(m_entity_wref, TransformComponent);
    if (m_trans_comp_wref.IsNull() == false) {
        m_current_trans = SD_WREF(m_trans_comp_wref).GetWorldTransform();
    }
}

void MotorComponent::Update()
{
    if (Application::GetRef().GetKeyStateByCode(KEY_A) == KEY_STATUS_PRESS) {
        m_current_trans.AddRotation(m_current_trans.GetTop(), static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 10.0f);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_D) == KEY_STATUS_PRESS) {
        m_current_trans.AddRotation(m_current_trans.GetTop(), static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * -10.0f);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_W) == KEY_STATUS_PRESS) {
        //Camera forward is -z axis.
        Vector3f offset = m_current_trans.GetForward().negative().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_current_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_S) == KEY_STATUS_PRESS) {
        Vector3f offset = m_current_trans.GetForward().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_current_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_Q) == KEY_STATUS_PRESS) {
        Vector3f offset = m_current_trans.GetRight().negative().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 1.0f);//speed 2m/s
        m_current_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_E) == KEY_STATUS_PRESS) {
        Vector3f offset = m_current_trans.GetRight().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 1.0f);//speed 2m/s
        m_current_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_R) == KEY_STATUS_PRESS) {
        Vector3f offset = m_current_trans.GetTop().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_current_trans.AddTranslation(offset);
    }

    if (Application::GetRef().GetKeyStateByCode(KEY_F) == KEY_STATUS_PRESS) {
        Vector3f offset = m_current_trans.GetTop().negative().scale(static_cast<float>(Timer::GetRef().GetProgramDeltaTime()) * 2.0f);//speed 2m/s
        m_current_trans.AddTranslation(offset);
    }

    if (m_trans_comp_wref.IsNull() == false) {
        SD_WREF(m_trans_comp_wref).SetWorldTransform(m_current_trans);
    }
}