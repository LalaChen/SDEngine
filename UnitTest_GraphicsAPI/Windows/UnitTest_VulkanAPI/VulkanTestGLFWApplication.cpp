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

bool ApplicationManipulater::OnReceiveKeyStateChanged(const EventArg& i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(KeyEventArg).hash_code()) {
        const KeyEventArg& arg = dynamic_cast<const KeyEventArg&>(i_arg);
        SDLOG("Key[%d](%d).", arg.m_key_id, arg.m_key_state);

        VulkanAPITestManager* mgr = GraphicsManager::GetDynamicCastPtr<VulkanAPITestManager>();
        if (mgr != nullptr) {
            if (arg.m_key_state == 0) {
                int32_t idx = arg.m_key_id - GLFW_KEY_0;
                mgr->SetCurrentSampleIndex(idx);
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
    //new Timer.
    new WindowsTimer();
    Timer::GetRef().Start();
    SDLOG("APP Starting at %lf.", Timer::GetRef().GetProgramStartTime());
    //new ImageWindowsLoader
    new ImageLoader();
    ImageLoader::GetRef().Initialize();
    //new graphics engine.
    if (m_adopt_library == GraphicsLibrary_OpenGL4) {
        new OpenGL4Manager();
    }
    else {
        new VulkanAPITestManager();
    }
}