#include "SampleDrawObjects.h"
#include "HUDComponent.h"
#include "GameSystem.h"

GameSystem::GameSystem(const ObjectName &i_object_name)
: System(i_object_name)
, m_cur_sample_idx(0)
{
    m_scene_names.push_back("SampleDrawObjects");
}

GameSystem::~GameSystem()
{
}

void GameSystem::Initialize()
{

    SD_WREF(Application::GetRef().GetEventNotifier()).RegisterSlotFunctionIntoEvent(
        sAppEventName, 
        new MemberFunctionSlot<GameSystem>(
            "GameSystem::OnAppEventTriggered",
            GetThisWeakPtrByType<GameSystem>(),
            &GameSystem::OnAppEventTriggered));

     m_camera_motor_group = ECSManager::GetRef().AddEntityGroup(
        "MotorComponent",
        {
            std::type_index(typeid(MotorComponent))
        }
    );

    m_HUD_group = ECSManager::GetRef().AddEntityGroup(
        "HUDComponent",
        {
            std::type_index(typeid(HUDComponent))
        }
    );
}

void GameSystem::Update()
{
    std::list<EntityWeakReferenceObject> entities;
    entities = SD_WREF(m_camera_motor_group).GetEntities();
    for (EntityWeakReferenceObject &entity : entities) {
        MotorComponentWeakReferenceObject motor = SD_WREF(entity).GetComponent(typeid(MotorComponent)).DynamicCastTo<MotorComponent>();
        if (motor.IsNull() == false) {
            SD_WREF(motor).Update();
        }
    }

    entities = SD_WREF(m_HUD_group).GetEntities();
    for (EntityWeakReferenceObject &entity : entities) {
        HUDComponentWeakReferenceObject hud = SD_WREF(entity).GetComponent(typeid(HUDComponent)).DynamicCastTo<HUDComponent>();
        if (hud.IsNull() == false) {
            SD_WREF(hud).Update();
        }
    }
}

void GameSystem::Destroy()
{
    for (std::string &scene_name : m_scene_names) {
        SceneManager::GetRef().UnloadScene(m_scene_names[0]);
    }
}

void GameSystem::Resize()
{
}

bool GameSystem::OnAppEventTriggered(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(AppEventArg).hash_code()) {
        const AppEventArg &arg = dynamic_cast<const AppEventArg&>(i_arg);
        if (arg.m_app_event == AppEvent_GRAPHICS_INITIALIZED) {
            SceneManager::GetRef().RegisterScene(new SampleDrawObjects(m_scene_names[0]));
            SceneManager::GetRef().LoadScene(m_scene_names[0]);
        }
        return true;
    }
    else {
        SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
        return false;
    }

}