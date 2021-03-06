#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanAPITestManager.h"
#include "VulkanTestGLFWApplication.h"

using namespace::SDE::App;
using namespace::SDE::Basic;
using namespace::SDE::Graphics;

ApplicationManipulater::ApplicationManipulater(const ObjectName &i_object_name)
: EventObject(i_object_name)
{
}

ApplicationManipulater::~ApplicationManipulater()
{
}

void ApplicationManipulater::Initalize()
{
    ApplicationManipulaterWeakReferenceObject this_obj = GetThisWeakPtrByType<ApplicationManipulater>();
    FunctionSlotBaseStrongReferenceObject func_slot =
        new MemberFunctionSlot<ApplicationManipulater>(
            "ApplicationManipulater::OnReceiveKeyStateChanged",
            this_obj,
            &ApplicationManipulater::OnReceiveKeyStateChanged);

    Application::GetRef().RegisterSlotFunctionIntoKeyEvent(func_slot);
}

void ApplicationManipulater::Release()
{
    ApplicationManipulaterWeakReferenceObject this_obj = GetThisWeakPtrByType<ApplicationManipulater>();

    FunctionSlotBaseStrongReferenceObject func_slot =
        new MemberFunctionSlot<ApplicationManipulater>(
            "ApplicationManipulater::OnReceiveKeyStateChanged",
            this_obj,
            &ApplicationManipulater::OnReceiveKeyStateChanged);

    Application::GetRef().UnregisterSlotFunctionFromKeyEvent(func_slot);
}

bool ApplicationManipulater::OnReceiveKeyStateChanged(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(KeyEventArg).hash_code()) {
        const KeyEventArg& arg = dynamic_cast<const KeyEventArg&>(i_arg);
        SDLOG("Key[%d](%d).", arg.m_key_id, arg.m_key_state);

        VulkanAPITestManager *mgr = GraphicsManager::GetDynamicCastPtr<VulkanAPITestManager>();
        if (mgr != nullptr) {
            if (arg.m_key_state == 0) {
                if (arg.m_key_id >= GLFW_KEY_0 && arg.m_key_id <= GLFW_KEY_9) {
                    int32_t idx = arg.m_key_id - GLFW_KEY_0;
                    mgr->SetCurrentSampleIndex(idx);
                }
            }
        }

        return true;
    }
    else {
        SDLOG("Valid Failure !!! Receive error event arg. Error type %s.", typeid(i_arg).name());
        return false;
    }
}


VulkanTestGLFWApplication::VulkanTestGLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
{
    m_app_manipulater = new ApplicationManipulater("APManipulater");
    m_app_manipulater.GetRef().Initalize();
}

VulkanTestGLFWApplication::~VulkanTestGLFWApplication()
{
    m_app_manipulater.GetRef().Release();
    m_app_manipulater.reset();
}

void VulkanTestGLFWApplication::Initialize()
{
    SDLOG("Initialize Application.");
    //new LogManager.
    new WindowsLogManager();
    //new WindowsFileSystemManager.
    new WindowsFileSystemManager();
    FileSystemManager::GetRef().Initialize();
    //Initialize KeyBoard Mapping.

    //new Graphics Manager.
    new VulkanAPITestManager();
}

void VulkanTestGLFWApplication::Update()
{
    //1. Update Timer.
    Timer::GetRef().Update();
    //2. 
    VulkanAPITestManager::GetRef().Render();
}