#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "GameSystem.h"
#include "SampleDrawObjects.h"
#include "FeatureApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::App;

ApplicationManipulater::ApplicationManipulater(const ObjectName &i_object_name)
: EventObject(i_object_name)
{
}

ApplicationManipulater::~ApplicationManipulater()
{
}

void ApplicationManipulater::Initalize()
{
    ApplicationManipulaterWeakReferenceObject this_wref = GetThisWeakPtrByType<ApplicationManipulater>();
    FunctionSlotBaseStrongReferenceObject func_slot_sref =
        new MemberFunctionSlot<ApplicationManipulater>(
            "ApplicationManipulater::OnReceiveKeyStateChanged",
            this_wref,
            &ApplicationManipulater::OnReceiveKeyStateChanged);

    Application::GetRef().RegisterSlotFunctionIntoKeyEvent(func_slot_sref);
}

void ApplicationManipulater::Release()
{
    ApplicationManipulaterWeakReferenceObject this_wref = GetThisWeakPtrByType<ApplicationManipulater>();

    FunctionSlotBaseStrongReferenceObject func_slot_sref =
        new MemberFunctionSlot<ApplicationManipulater>(
            "ApplicationManipulater::OnReceiveKeyStateChanged",
            this_wref,
            &ApplicationManipulater::OnReceiveKeyStateChanged);

    Application::GetRef().UnregisterSlotFunctionFromKeyEvent(func_slot_sref);
}

bool ApplicationManipulater::OnReceiveKeyStateChanged(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(KeyEventArg).hash_code()) {
        const KeyEventArg& arg = dynamic_cast<const KeyEventArg&>(i_arg);
        SDLOG("Key[%d](%d).", arg.m_key_id, arg.m_key_state);

        if (arg.m_key_state == 0) {
            if (arg.m_key_id >= GLFW_KEY_0 && arg.m_key_id <= GLFW_KEY_9) {
                uint32_t idx = arg.m_key_id - GLFW_KEY_0;
            }
        }

        return true;
    }
    else {
        SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
        return false;
    }
}


FeatureApplication::FeatureApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
{
    m_app_manipulater = new ApplicationManipulater("APManipulater");
    m_app_manipulater.GetRef().Initalize();
}

FeatureApplication::~FeatureApplication()
{
    m_app_manipulater.GetRef().Release();
    m_app_manipulater.reset();
}

void FeatureApplication::Initialize()
{
    GLFWApplication::Initialize();
    //
    ECSManager::GetRef().RegisterSystem<GameSystem>("GameSystem");
}