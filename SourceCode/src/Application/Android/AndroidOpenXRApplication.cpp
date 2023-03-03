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

#include "AndroidOpenXRApplication.h"

#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanWrapper.h"
#include "OpenXRApplication.h"
#include "OpenXRVulkanManager.h"
#include "AndroidEnvAttacher.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::GUI;
using namespace SDE::OPENXR;

________________SD_START_APP_NAMESPACE_______________

AndroidOpenXRApplication::AndroidOpenXRApplication(
    const std::string &i_win_title,
    JavaVM *i_javaVM, AAssetManager *i_asset_mgr,
    GraphicsLibraryEnum i_adopt_library, int i_argc, char** i_argv)
: OpenXRApplication(i_win_title, Resolution(2, 2), true, i_adopt_library, i_argc, i_argv)
, m_current_state(AppState_CREATE)
, m_window(nullptr)
, m_javaVM(i_javaVM)
, m_asset_mgr(i_asset_mgr)
, m_resize_signal(false)
{
    if (m_javaVM != nullptr) {
    }
    else {
        SDLOG("Java VM is nullptr!!!");
    }
}

AndroidOpenXRApplication::~AndroidOpenXRApplication()
{
    if (m_javaVM != nullptr) {
    }
    else {
        SDLOG("Java VM is nullptr!!!");
    }
}

void AndroidOpenXRApplication::Initialize()
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
        new OpenXRVulkanManager();
    }

    new IMGUIRenderer();

    OpenXRApplication::Initialize();

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_INITIALIZED));
}

void AndroidOpenXRApplication::InitializeGraphicsSystem()
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

                for (const char* desired_name : desired_layer_names) {
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

        VulkanCreationArg arg;
        arg.m_instance = instance;
        arg.m_surface = surface;
        GraphicsManager::GetRef().InitializeGraphicsSystem(arg);
        GraphicsManager::GetRef().InitializeBasicResource();
    }
    else {
    }

    IMGUIRenderer::GetRef().InitializeGraphicsSystem();

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_GRAPHICS_INITIALIZED));
}

void AndroidOpenXRApplication::ReleaseGraphicsSystem()
{
    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_GRAPHICS_RELESAED));
    IMGUIRenderer::GetRef().ReleaseGraphicsSystem();

    GraphicsManager::GetRef().ReleaseBasicResource();
    GraphicsManager::GetRef().ReleaseGraphicsSystem();

    m_window = nullptr;
}

void AndroidOpenXRApplication::TerminateApplication()
{
    m_current_state = AppState_TERMINATE;
    m_pause_cv.notify_all();
    m_render_thread.join();
    OpenXRApplication::TerminateApplication();
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

void AndroidOpenXRApplication::InitializeNativeWindow(ANativeWindow* i_window)
{
    m_window = i_window;
}

void AndroidOpenXRApplication::RefreshNativeWindow(ANativeWindow* i_window, int i_width, int i_height)
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
        if (vkCreateAndroidSurfaceKHR(app_VK_instance, &surface_c_info, nullptr, &new_surface) != VK_SUCCESS) {
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

KeyStatusEnum AndroidOpenXRApplication::GetKeyStateByCode(KeyCodeEnum i_code)
{
    return KEY_STATUS_NOT_SUPPORT;
}

void AndroidOpenXRApplication::RunMainLoop()
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
                m_pause_cv.wait(mtx_lock, [this]() {return (m_current_state == AppState_RUN || m_current_state == AppState_TERMINATE); });
                SDLOG("[AppFlow] App pause end - go to Resume");
                Resume();
                SDLOG("[AppFlow] App resume end");
            }
        }
        });
}

void AndroidOpenXRApplication::Pause()
{
    if (m_current_state == AppState_RUN) {
        SDLOG("[AppFlow] Set app pause.");
        m_current_state = AppState_PAUSE;
    }
    else {
        SDLOG("[AppFlow] Old state(%d) isn't AppState_Run. We can't stop.", m_current_state);
    }
}

void AndroidOpenXRApplication::Resume()
{
    Application::Resume();
}

_________________SD_END_APP_NAMESPACE________________