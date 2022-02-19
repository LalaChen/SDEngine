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

#include <SDEngine.h>
#include <SDEnginePlatform.h>

#include <wvr/wvr.h>
#include <wvr/wvr_types.h>
#include <wvr/wvr_render.h>
#include <wvr/wvr_device.h>
#include <wvr/wvr_projection.h>
#include <wvr/wvr_overlay.h>
#include <wvr/wvr_system.h>
#include <wvr/wvr_events.h>
#include <wvr/wvr_vulkan.h>

#include "WaveVRVulkanManager.h"
#include "WaveVRApplication.h"
#include "WaveSystem.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::GUI;

WaveVRApplication::WaveVRApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, Resolution(2,2), true, i_adopt_library, i_argc, i_argv)
, m_asset_mgr(i_asset_mgr)
, m_is_app_terminate(false)
{
    SD_SREF(m_app_event_notifier).RegisterEvent(new Event(WaveVREventArg::sWaveVREventName));
}

WaveVRApplication::~WaveVRApplication()
{
}


void WaveVRApplication::Initialize()
{
    SDLOG("Initialze application.");
    //1. Initialize super class and necessary manager.
    new AndroidLogManager();
    SDLOG("Initialize Android Application!!!");
    SDLOG("APP Starting at %lf.", Timer::GetRef().GetProgramStartTime());

    new AndroidAssetResourceManger(m_asset_mgr);

    if (m_adopt_library == GraphicsLibrary_OpenGL4) {
        //Not support.
    } else if (m_adopt_library == GraphicsLibrary_Vulkan) {
        new WaveVRVulkanManager(); //new WaveVRVulkanManager.
    }

    new IMGUIRenderer();

    //2. Initialize wvr.
    WVR_InitError init_result = WVR_Init(WVR_AppType_VRContent);
    if (init_result != WVR_InitError_None) {
        SDLOG("Initialze application failure(%d).", init_result);
        throw std::runtime_error("WVRInit Error");
    }

    WVR_RenderInitParams_t param;
    param = { WVR_GraphicsApiType_OpenGL, WVR_RenderConfig_Default };

    WVR_RenderError r_init_result = WVR_RenderInit(&param);
    if (r_init_result != WVR_RenderError_None) {
        SDLOGE("Initialize renderer failure(%d).", r_init_result);
        throw std::runtime_error("WVRRenderInit Error");
    }

    ECSManager::GetRef().RegisterSystem<WaveSystem>("WaveSystem");

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_INITIALIZED));
}

void WaveVRApplication::InitializeGraphicsSystem()
{
    //1. initialize renderer.
    if (m_adopt_library == SDE::Graphics::GraphicsLibrary_Vulkan) {
        VkInstance instance = VK_NULL_HANDLE;

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
        SDLOG("[AppFlow] Vulkan instance creation end!!!");
        m_graphics_app_instance = reinterpret_cast<CompHandle>(instance);

        VulkanCreationArg arg;
        arg.m_instance = instance;
        arg.m_surface = VK_NULL_HANDLE;
        GraphicsManager::GetRef().InitializeGraphicsSystem(arg);
        GraphicsManager::GetRef().InitializeBasicResource();
    }
    else {
    }

    IMGUIRenderer::GetRef().InitializeGraphicsSystem();

    //2. call wvr_vkinit.
    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_GRAPHICS_INITIALIZED));
}

void WaveVRApplication::ReleaseGraphicsSystem()
{
    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_GRAPHICS_RELESAED));
    IMGUIRenderer::GetRef().ReleaseGraphicsSystem();
    GraphicsManager::GetRef().ReleaseBasicResource();
    GraphicsManager::GetRef().ReleaseGraphicsSystem();
}

void WaveVRApplication::TerminateApplication()
{
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

    SD_WREF(m_app_event_notifier).NotifyEvent(sAppEventName, AppEventArg(AppEvent_TERMINATED));
}

KeyStatusEnum WaveVRApplication::GetKeyStateByCode(KeyCodeEnum i_code)
{
    return KEY_STATUS_NOT_SUPPORT;
}

void WaveVRApplication::HandleInput()
{
    WVR_Event_t event;
    while (WVR_PollEventQueue(&event) == true) {
        WaveVREventArg event_arg;
        event_arg.m_event = event;
        SD_SREF(m_app_event_notifier).NotifyEvent(WaveVREventArg::sWaveVREventName, event_arg);
        if (event.common.type == WVR_EventType_Quit) {
            NotifyTerminating();
            return;
        }
    }
}

void WaveVRApplication::RunMainLoop()
{
    SDLOG("%s : InitializeGraphicsSystem start!!!", __FUNCTION__);
    InitializeGraphicsSystem();
    SDLOG("%s : MainLoop start!!!", __FUNCTION__);
    bool current_ap_term = m_is_app_terminate;
    while (current_ap_term == false) {
        {
            std::lock_guard<std::mutex> lock(m_main_loop_mtx);
            current_ap_term = m_is_app_terminate;
        }
        HandleInput();
        Update();//Pause will block this thread.
    }
    SDLOG("%s : MainLoop end!!!", __FUNCTION__);
    TerminateApplication();
    WVR_Quit();
}

void WaveVRApplication::Pause()
{
}

void WaveVRApplication::Resume()
{
}