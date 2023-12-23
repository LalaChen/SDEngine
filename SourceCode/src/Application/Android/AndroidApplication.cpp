/*==============  SD Engine License ==============
MIT License

Copyright(c) 2019 Kuan - Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include "AndroidApplication.h"

#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanWrapper.h"
#include "AndroidEnvAttacher.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::GUI;

________________SD_START_APP_NAMESPACE_______________

AndroidApplication::AndroidApplication(
    const std::string &i_win_title,
    JavaVM *i_javaVM, AAssetManager *i_asset_mgr,
    GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, Resolution(2,2), true, i_adopt_library, i_argc, i_argv)
, m_current_state(AppState_CREATE)
, m_window(nullptr)
, m_javaVM(i_javaVM)
, m_asset_mgr(i_asset_mgr)
, m_resize_signal(false)
{
    if (m_javaVM != nullptr) {
        AndroidEnvAttacher env_attacher(m_javaVM);
        JNIEnv *env = env_attacher.GetEnv();
        if (env != nullptr) {
            jClass_MotionEvent = env->FindClass("android/view/MotionEvent");
            jMethod_MotionEvent_getActionMasked = env->GetMethodID(jClass_MotionEvent, "getActionMasked", "()I");
            jMethod_MotionEvent_getPointerCount = env->GetMethodID(jClass_MotionEvent, "getPointerCount", "()I");
            jMethod_MotionEvent_getPointerId = env->GetMethodID(jClass_MotionEvent, "getPointerId", "(I)I");
            jMethod_MotionEvent_getX = env->GetMethodID(jClass_MotionEvent, "getX", "(I)F");
            jMethod_MotionEvent_getY = env->GetMethodID(jClass_MotionEvent, "getY", "(I)F");
        }
        else {
            SDLOG("env is nullptr!!!");
        }
    }
    else {
        SDLOG("Java VM is nullptr!!!");
    }
}

AndroidApplication::~AndroidApplication()
{
    if (m_javaVM != nullptr) {
        AndroidEnvAttacher env_attacher(m_javaVM);
        JNIEnv* env = env_attacher.GetEnv();
        if (env != nullptr) {
            env->DeleteLocalRef(jClass_MotionEvent);
        }
        else {
            SDLOG("env is nullptr!!!");
        }
    }
    else {
        SDLOG("Java VM is nullptr!!!");
    }
}

void AndroidApplication::Initialize()
{
    m_current_state = AppState_INITIALIZE;

    new AndroidLogManager();
    SDLOG("Initialize Android Application!!!");
    SDLOG("APP Starting at %lf.", Timer::GetRef().GetProgramStartTime());

    new AndroidAssetResourceManger(m_asset_mgr);

    //
    if (m_adopt_library == GraphicsLibrary_OpenGL4) {

    }
    else if (m_adopt_library == GraphicsLibrary_Vulkan) {
        new VulkanManager();
    }

    new IMGUIRenderer();

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_INITIALIZED));
}

void AndroidApplication::InitializeGraphicsSystem()
{
    if (m_adopt_library == SDE::Graphics::GraphicsLibrary_Vulkan) {
        VkInstance instance = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        SDLOG("[AppFlow] Adjust extension layers.");
        uint32_t ins_ext_prop_count = 0;
        std::vector<VkExtensionProperties> ins_ext_props;
        std::vector<const char*> ins_ext_prop_names;
        vkEnumerateInstanceExtensionProperties(nullptr, &ins_ext_prop_count, nullptr);
        if (ins_ext_prop_count > 0) {
            ins_ext_props.resize(ins_ext_prop_count);
            ins_ext_prop_names.resize(ins_ext_prop_count);
            vkEnumerateInstanceExtensionProperties(nullptr, &ins_ext_prop_count, ins_ext_props.data());
            for (uint32_t ext_id = 0; ext_id < ins_ext_prop_names.size(); ext_id++) {
                ins_ext_prop_names[ext_id] = ins_ext_props[ext_id].extensionName;
                SDLOGD("--- Ext[%s](%d)", ins_ext_props[ext_id].extensionName, ins_ext_props[ext_id].specVersion);
            }
        }

        SDLOG("[AppFlow] Adjust valid layers.");
        uint32_t layer_count = 0;
        std::vector<VkLayerProperties> avaiable_valid_layers;
        std::vector<const char*> desired_valid_layer_names;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        if (layer_count > 0) {
            avaiable_valid_layers.resize(layer_count);
            vkEnumerateInstanceLayerProperties(&layer_count, avaiable_valid_layers.data());
            for (uint32_t ext_id = 0; ext_id < avaiable_valid_layers.size(); ext_id++) {
                SDLOG("[AppFlow] --- Avaiable valid layer :%s[%s](%d)(%d)",
                      avaiable_valid_layers[ext_id].layerName,
                      avaiable_valid_layers[ext_id].description,
                      avaiable_valid_layers[ext_id].implementationVersion,
                      avaiable_valid_layers[ext_id].specVersion);

                std::vector<const char*> desired_layer_names;
                GraphicsManager::GetRef().GetDesiredVulkanValidLayers(desired_layer_names);

                for (const char *desired_name : desired_layer_names) {
                    if (strcmp(desired_name, avaiable_valid_layers[ext_id].layerName) == 0) {
                        desired_valid_layer_names.push_back(desired_name);
                        break;
                    }
                }
            }
        }

        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = m_win_title.c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "SD Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo ins_c_info = {};
        ins_c_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        ins_c_info.pApplicationInfo = &app_info;
        ins_c_info.enabledExtensionCount = static_cast<uint32_t>(ins_ext_prop_names.size());
        ins_c_info.ppEnabledExtensionNames = ins_ext_prop_names.data();
#ifdef NDEBUG
        ins_c_info.enabledLayerCount = 0;
        ins_c_info.ppEnabledLayerNames = nullptr;
#else
        ins_c_info.enabledLayerCount = static_cast<uint32_t>(desired_valid_layer_names.size());
        ins_c_info.ppEnabledLayerNames = desired_valid_layer_names.data();
#endif

        SDLOG("[AppFlow] Vulkan instance creation !!!");
        SDLOG("[AppFlow] --- Vulkan create instance.");
        if (vkCreateInstance(&ins_c_info, nullptr, (VkInstance*)&instance) != VK_SUCCESS) {
            SDLOG("[AppFlow] --- failed to create instance.");
            throw std::runtime_error("failed to create instance!");
        }
        SDLOG("[AppFlow] --- Vulkan create surface.");
        VkAndroidSurfaceCreateInfoKHR surface_c_info = {};
        surface_c_info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        surface_c_info.pNext = nullptr;
        surface_c_info.flags = 0;
        surface_c_info.window = m_window;
        SDLOG("[AppFlow] vkCreateAndroidSurfaceKHR(%p)", (void*)vkCreateAndroidSurfaceKHR);
        if (vkCreateAndroidSurfaceKHR(instance, &surface_c_info, nullptr, &surface) != VK_SUCCESS){
            SDLOG("[AppFlow] --- failed to create surface.");
            throw std::runtime_error("failed to create surface!");
        }
        SDLOG("[AppFlow] Vulkan instance creation end!!!");
        m_graphics_app_instance = reinterpret_cast<CompHandle>(instance);

        VulkanCreationArg arg;
        arg.m_instance = instance;
        arg.m_surface = surface;
        GraphicsManager::GetRef().InitializeGraphicsSystem(arg);
    }
    else {
    }

    IMGUIRenderer::GetRef().InitializeGraphicsSystem();

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_GRAPHICS_INITIALIZED));
}

void AndroidApplication::ReleaseGraphicsSystem()
{
    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_GRAPHICS_RELESAED));
    IMGUIRenderer::GetRef().ReleaseGraphicsSystem();
    GraphicsManager::GetRef().ReleaseGraphicsSystem();
    m_window = nullptr;
}

void AndroidApplication::TerminateApplication()
{
    m_current_state = AppState_TERMINATE;
    m_pause_cv.notify_all();
    m_render_thread.join();
    SDLOG("[AppFlow] Terminate SceneManager.");
    SceneManager::GetRef().Terminate();
    SDLOG("[AppFlow] Terminate ECSManager.");
    ECSManager::GetRef().Terminate();
    SDLOG("[AppFlow] Destroy GraphicsManager.");
    ReleaseGraphicsSystem();
    GraphicsManager::Destroy();
    //destroy Timer.
    Timer::GetRef().End();
    SDLOG("[AppFlow] APP Ending at %lf.", Timer::GetRef().GetProgramEndTime());
    //destroy File Manager
    SDLOG("[AppFlow] Destroy AssetResourceManager.");
    AssetResourceManager::Destroy();
    //destroy LogManager
    SDLOG("[AppFlow] Destroy LogManager.");
    LogManager::Destroy();
    //
    m_asset_mgr = nullptr;

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_TERMINATED));
}

void AndroidApplication::InitializeNativeWindow(ANativeWindow *i_window)
{
    m_window = i_window;
}

void AndroidApplication::RefreshNativeWindow(ANativeWindow *i_window, int i_width, int i_height)
{
    if (i_window != nullptr) {
        SDLOG("[AppFlow] Refresh native for android.");
        VkSurfaceKHR new_surface = VK_NULL_HANDLE;
        m_window = i_window;
        //Update surface in graphics manager.
        VkAndroidSurfaceCreateInfoKHR surface_c_info = {};
        surface_c_info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        surface_c_info.pNext = nullptr;
        surface_c_info.flags = 0;
        surface_c_info.window = m_window;
        VkInstance app_VK_instance = reinterpret_cast<VkInstance>(m_graphics_app_instance);
        if (vkCreateAndroidSurfaceKHR(app_VK_instance, &surface_c_info, nullptr, &new_surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create surface!");
        }
        m_win_res.SetResolution(i_width, i_height);
        Resize(reinterpret_cast<CompHandle>(new_surface), i_width, i_height);
        m_current_state = AppState_RUN;
        m_pause_cv.notify_all();
    }
    else {
        SDLOGE("[AppFlow] i_window isn't nullptr. We can't register.");
    }
}

KeyStatusEnum AndroidApplication::GetKeyStateByCode(KeyCodeEnum i_code)
{
    return KEY_STATUS_NOT_SUPPORT;
}

void AndroidApplication::RunMainLoop()
{
    m_current_state = AppState_INITIALIZE_GRAPHICS;
    InitializeGraphicsSystem();

    m_render_thread = std::thread([this]() {
        m_current_state = AppState_RUN;
        while (m_current_state == AppState_RUN || m_current_state == AppState_PAUSE) {
            if (m_current_state == AppState_RUN) {
                Update();
            }
            else {
                std::unique_lock<std::mutex> mtx_lock(m_pause_mtx);
                SDLOG("[AppFlow] App pause now");
                m_pause_cv.wait(mtx_lock, [this](){return (m_current_state == AppState_RUN || m_current_state == AppState_TERMINATE);});
                SDLOG("[AppFlow] App pause end - go to Resume");
                Resume();
                SDLOG("[AppFlow] App resume end");
            }
        }
    });
}

void AndroidApplication::Pause()
{
    if (m_current_state == AppState_RUN) {
        SDLOG("[AppFlow] Set app pause.");
        m_current_state = AppState_PAUSE;
    }
    else {
        SDLOG("[AppFlow] Old state(%d) isn't AppState_Run. We can't stop.", m_current_state);
    }
}

void AndroidApplication::Resume()
{
    Application::Resume();
}

void AndroidApplication::ReceiveMotionEvent(jobject i_jMotionEvent_mv)
{
    if (m_javaVM != nullptr) {
        AndroidEnvAttacher env_attacher(m_javaVM);
        JNIEnv* env = env_attacher.GetEnv();
        if (env != nullptr) {
            //for (jint tID = 0; tID < SDE::Basic::TouchButton_MAX_DEFINE_VALUE; tID++) {
            //    SetTouchPosition(static_cast<SDE::Basic::TouchButtonEnum>(tID), -1.0f, -1.0f);
            //    SetTouchStatus(static_cast<SDE::Basic::TouchButtonEnum>(tID), SDE::Basic::TouchButtonState_RELEASED);
            //}

            jint touch_count = env->CallIntMethod(i_jMotionEvent_mv, jMethod_MotionEvent_getPointerCount);
            jint event_action = env->CallIntMethod(i_jMotionEvent_mv, jMethod_MotionEvent_getActionMasked);
            for (jint tID = 0; tID < touch_count; ++tID) {
                jint touchID = env->CallIntMethod(i_jMotionEvent_mv, jMethod_MotionEvent_getPointerId, tID);
                jfloat x = env->CallFloatMethod(i_jMotionEvent_mv, jMethod_MotionEvent_getX, tID);
                jfloat y = env->CallFloatMethod(i_jMotionEvent_mv, jMethod_MotionEvent_getY, tID);

                SDLOG("Touch(%d) ID(%d) A:%d, (%lf, %lf)", tID, touchID, event_action, x, y);
                SetTouchPosition(static_cast<SDE::Basic::TouchButtonEnum>(touchID), x, y);
                if (event_action == 1 || event_action == 6) {
                    SetTouchStatus(static_cast<SDE::Basic::TouchButtonEnum>(touchID), SDE::Basic::TouchButtonState_RELEASED);
                }
                else if(event_action == 0 || event_action == 2 || event_action == 5) {
                    SetTouchStatus(static_cast<SDE::Basic::TouchButtonEnum>(touchID), SDE::Basic::TouchButtonState_PRESSED);
                }
                else {
                    SetTouchStatus(static_cast<SDE::Basic::TouchButtonEnum>(touchID), SDE::Basic::TouchButtonState_RELEASED);
                }
            }

        }
        else {
            SDLOG("env is nullptr!!!");
        }
    }
    else {
        SDLOG("Java VM is nullptr!!!");
    }
}
_________________SD_END_APP_NAMESPACE________________
