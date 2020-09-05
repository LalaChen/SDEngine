#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "SampleDrawObjects.h"
#include "FeatureApplication.h"

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

bool ApplicationManipulater::OnReceiveKeyStateChanged(const EventArg &i_arg)
{
    if (typeid(i_arg).hash_code() == typeid(KeyEventArg).hash_code()) {
        const KeyEventArg& arg = dynamic_cast<const KeyEventArg&>(i_arg);
        SDLOG("Key[%d](%d).", arg.m_key_id, arg.m_key_state);

        if (arg.m_key_state == 0) {
            if (arg.m_key_id >= GLFW_KEY_0 && arg.m_key_id <= GLFW_KEY_9) {
                uint32_t idx = arg.m_key_id - GLFW_KEY_0;

                FeatureApplication* app = dynamic_cast<FeatureApplication*>(Application::GetPtr());
                if (app != nullptr) {
                    app->SetSampleIdx(idx);
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


FeatureApplication::FeatureApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
, m_cur_sample_idx(0)
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
    //Create Sample
    m_sample_srefs.push_back(new SampleDrawObjects("SampleDrawObject"));
}

void FeatureApplication::InitializeGraphicsSystem()
{
    GLFWApplication::InitializeGraphicsSystem();

    std::vector<SampleStrongReferenceObject>::iterator iter;
    for (iter = m_sample_srefs.begin(); iter != m_sample_srefs.end(); ++iter) {
        (*iter).GetRef().Initialize();
    }

}

void FeatureApplication::Update()
{
    // Override Update for temporary.
    Timer::GetRef().Update();
    //
    if (m_sample_srefs[m_cur_sample_idx].IsNull() == false) {
        m_sample_srefs[m_cur_sample_idx].GetRef().Render();
        //
        GraphicsManager::GetRef().RenderTexture2DToScreen(m_sample_srefs[m_cur_sample_idx].GetRef().GetColorBuffer());
    }
}

void FeatureApplication::Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h)
{
    GraphicsManager::GetRef().Resize(i_ns_handle, i_w, i_h);

    for (uint32_t sample_idx = 0; sample_idx < m_sample_srefs.size(); ++sample_idx) {
        m_sample_srefs[sample_idx]->Resize(i_w, i_h);
    }
}

void FeatureApplication::ReleaseGraphicsSystem()
{
    //
    for (SampleStrongReferenceObject &sample_sref : m_sample_srefs) {
        if (sample_sref != nullptr) {
            sample_sref.GetRef().Destroy();
            sample_sref.Reset();
        }
    }
    m_sample_srefs.resize(0);
    m_sample_srefs.shrink_to_fit();

    GLFWApplication::ReleaseGraphicsSystem();
}

void FeatureApplication::SetSampleIdx(uint32_t i_target_idx)
{
    if (i_target_idx < m_sample_srefs.size()) {
        m_cur_sample_idx = i_target_idx;
    }
}