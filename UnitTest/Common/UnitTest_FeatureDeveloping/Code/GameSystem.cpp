#include "SampleDrawObjects.h"
#include "GameSystem.h"

GameSystem::GameSystem(const ObjectName &i_object_name)
: System(i_object_name)
, m_cur_sample_idx(0)
{
}

GameSystem::~GameSystem()
{
}

void GameSystem::Initialize()
{
    m_samples.push_back(
        ECSManager::GetRef().RegisterSystem<SampleDrawObjects>("SampleDrawObjects").
        DynamicCastTo<Sample>());

    SD_WREF(Application::GetRef().GetEventNotifier()).RegisterSlotFunctionIntoEvent(
        "AppEvent", 
        new MemberFunctionSlot<GameSystem>(
            "GameSystem::OnAppEventTriggered",
            GetThisWeakPtrByType<GameSystem>(),
            &GameSystem::OnAppEventTriggered));
}

void GameSystem::Update()
{
    if (m_cur_sample_idx < m_samples.size()) {
        SD_WREF(m_samples[m_cur_sample_idx]).UpdateScene();
    }
}

void GameSystem::Destroy()
{
    for (uint32_t sid = 0; sid < m_samples.size(); ++sid) {
        SD_WREF(m_samples[sid]).DestroyScene();
    }
    m_samples.clear();
}

void GameSystem::Resize()
{
}

bool GameSystem::OnAppEventTriggered(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(AppEventArg).hash_code()) {
        const AppEventArg &arg = dynamic_cast<const AppEventArg&>(i_arg);
        if (arg.m_app_event == AppEvent_GRAPHICS_INITIALIZED) {
            for (uint32_t sid = 0; sid < m_samples.size(); ++sid) {
                SD_WREF(m_samples[sid]).InitializeScene();
            }
        }
        return true;
    }
    else {
        SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
        return false;
    }

}