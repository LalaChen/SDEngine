#include "SDEnginePlatform.h"
#include "VulkanWrapper.h"
#include "AndroidApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

________________SD_START_APP_NAMESPACE_______________

AndroidApplication::AndroidApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, Resolution(2,2), true, i_adopt_library, i_argc, i_argv)
, m_current_state(AppState_CREATE)
, m_window(nullptr)
, m_asset_mgr(i_asset_mgr)
{
}

AndroidApplication::~AndroidApplication()
{
}

void AndroidApplication::Initialize()
{
    m_current_state = AppState_INITIALIZE;

    new AndroidLogManager();
    SDLOG("Initialize Android Application!!!");

    new AndroidTimer();
    Timer::GetRef().Start();

    new AndroidAssetResourceManger(m_asset_mgr);

    new ImageLoader();

    ImageLoader::GetRef().Initialize();

    if (m_adopt_library == SDE::Graphics::GraphicsLibrary_Vulkan) {
        new VulkanManager();
    }
    else {
    }
}

void AndroidApplication::InitializeGraphicsSystem()
{
    if (m_adopt_library == SDE::Graphics::GraphicsLibrary_Vulkan) {
        VkInstance instance = VK_NULL_HANDLE;
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        SDLOG("Adjust extension layers.");
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

        SDLOG("Adjust valid layers.");
        uint32_t layer_count = 0;
        std::vector<VkLayerProperties> avaiable_valid_layers;
        std::vector<const char*> desired_valid_layer_names;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        if (layer_count > 0) {
            avaiable_valid_layers.resize(layer_count);
            vkEnumerateInstanceLayerProperties(&layer_count, avaiable_valid_layers.data());
            for (uint32_t ext_id = 0; ext_id < avaiable_valid_layers.size(); ext_id++) {
                SDLOG("--- Avaiable valid layer :%s[%s](%d)(%d)",
                      avaiable_valid_layers[ext_id].layerName,
                      avaiable_valid_layers[ext_id].description,
                      avaiable_valid_layers[ext_id].implementationVersion,
                      avaiable_valid_layers[ext_id].specVersion);

                for (const char *desired_name : VulkanManager::GetDesiredValidLayers()) {
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

        SDLOG("Vulkan instance creation !!!");
        SDLOG("--- Vulkan create instance.");
        if (vkCreateInstance(&ins_c_info, nullptr, (VkInstance*)&instance) != VK_SUCCESS) {
            SDLOG("--- failed to create instance.");
            throw std::runtime_error("failed to create instance!");
        }
        SDLOG("--- Vulkan create surface.");
        VkAndroidSurfaceCreateInfoKHR surface_c_info = {};
        surface_c_info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        surface_c_info.pNext = nullptr;
        surface_c_info.flags = 0;
        surface_c_info.window = m_window;
        SDLOG("vkCreateAndroidSurfaceKHR(%p)", (void*)vkCreateAndroidSurfaceKHR);
        if (vkCreateAndroidSurfaceKHR(instance, &surface_c_info, nullptr, &surface) != VK_SUCCESS){
            SDLOG("--- failed to create surface.");
            throw std::runtime_error("failed to create surface!");
        }
        SDLOG("Vulkan instance creation end!!!");
        m_graphics_app_instance = reinterpret_cast<CompHandle>(instance);

        VulkanCreationArg arg;
        arg.m_instance = instance;
        arg.m_surface = surface;
        GraphicsManager::GetRef().InitializeGraphicsSystem(arg);
        GraphicsManager::GetRef().Initialize();
    }
    else {
    }
}

void AndroidApplication::ReleaseGraphicsSystem()
{
    m_window = nullptr;
}

void AndroidApplication::TerminateApplication()
{
    m_current_state = AppState_TERMINATE;
    m_render_thread.join();
    //destroy Timer.
    Timer::GetRef().End();
    SDLOG("APP Ending at %lf.", Timer::GetRef().GetEndProgramTime());
    Timer::Destroy();
    //destroy File Manager
    AssetResourceManager::Destroy();
    //destroy LogManager
    LogManager::Destroy();
    //
    m_asset_mgr = nullptr;
}

void AndroidApplication::InitializeNativeWindow(ANativeWindow *i_window)
{
    m_window = i_window;
}

void AndroidApplication::RefreshNativeWindow(ANativeWindow *i_window, int i_width, int i_height)
{
    if (i_window != nullptr) {
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

        GraphicsManager::GetRef().Resize(reinterpret_cast<CompHandle>(new_surface), i_width, i_height);
        m_current_state = AppState_RUN;
        m_pause_cv.notify_all();
    }
    else {
        SDLOGE("i_window isn't nullptr. We can't register.");
    }
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
                SDLOG("App Pause Start");
                m_pause_cv.wait(mtx_lock, [this](){return (m_current_state == AppState_RUN);});
                SDLOG("App Pause End - Go to Resume");
                Resume();
                SDLOG("Resume End");
            }
        }
    });
}

void AndroidApplication::Pause()
{
    if (m_current_state == AppState_RUN) {
        m_current_state = AppState_PAUSE;
    }
    else {
        SDLOG("Old state(%d) isn't AppState_Run. We can't stop.", m_current_state);
    }
}

void AndroidApplication::Resume()
{
    Application::Resume();
}
_________________SD_END_APP_NAMESPACE________________
