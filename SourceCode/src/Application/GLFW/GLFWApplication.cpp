#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanWrapper.h"
#include "GLFWApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

________________SD_START_APP_NAMESPACE_______________

GLFWApplication::GLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
, m_window(nullptr)
, m_monitor(nullptr)
{
    //1. set error callback.
    glfwSetErrorCallback(GLFWApplication::ErrorCallback);
    //2. create glfw window.
    //--- i. initialize glfw.
    if (!glfwInit())
        exit(EXIT_FAILURE);

    //3. According engine type to set API.
    if (glfwVulkanSupported() && i_adopt_library == Graphics::GraphicsLibrary_Vulkan) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    else {
        i_adopt_library = Graphics::GraphicsLibrary_OpenGL4;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    }

    //4. create window.
    if (i_full_window == true) {
        m_monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

        m_win_res.SetResolution(mode->width, mode->height);
    }

    m_window = glfwCreateWindow(m_win_res.GetWidth(), m_win_res.GetHeight(), i_win_title.c_str(), m_monitor, NULL);
    if (!m_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //5. register key cbk.
    //--- keyboard
    glfwSetKeyCallback(m_window, GLFWApplication::KeyEventCallback);
    //--- Mouse
    glfwSetCursorPosCallback(m_window, GLFWApplication::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, GLFWApplication::CursorMouseButtonCallback);
    glfwSetCursorPosCallback(m_window, GLFWApplication::CursorPositionCallback);
    glfwSetCursorEnterCallback(m_window, GLFWApplication::CursorEnterCallback);
    //------ Windows
    glfwSetWindowCloseCallback(m_window, GLFWApplication::WindowCloseCallback);
    glfwSetWindowMaximizeCallback(m_window, GLFWApplication::WindowMaximizeCallback);
    glfwSetFramebufferSizeCallback(m_window, GLFWApplication::WindowSizeCallback);
    glfwSetWindowPosCallback(m_window, GLFWApplication::WindowPosCallback);
    glfwSetWindowFocusCallback(m_window, GLFWApplication::WindowFocusCallback);
    glfwSetDropCallback(m_window, GLFWApplication::DropCallback);
    glfwSetScrollCallback(m_window, GLFWApplication::ScrollCallback);
}

GLFWApplication::~GLFWApplication()
{
    // Terminate GLFW
    glfwTerminate();
}

void GLFWApplication::Initialize()
{
    //new LogManager.
    new WindowsLogManager();
    SDLOG("Initialize Application.");
    //new WindowsFileSystemManager.
    new WindowsFileSystemManager();
    FileSystemManager::GetRef().Initialize();
    //new Timer.
    new WindowsTimer();
    Timer::GetRef().Start();
    SDLOG("APP Starting at %lf.", Timer::GetRef().GetProgramStartTime());
    //new ImageLoader
    new ImageLoader();
    ImageLoader::GetRef().Initialize();
    //new graphics engine.
    if (m_adopt_library == GraphicsLibrary_OpenGL4) {
        new OpenGL4Manager();
    }
    else {
        new VulkanManager();
    }
    //Initialize KeyBoard Mapping.
}

void GLFWApplication::InitializeGraphicsSystem()
{
    SDLOG("Initialize Graphics System of Application.");
    //new Graphics Manager.
    //--- iv. make current.
    if (m_adopt_library == GraphicsLibrary_OpenGL4) {
        glfwMakeContextCurrent(m_window);
        GraphicsManager::GetRef().InitializeGraphicsSystem(EventArg());
        GraphicsManager::GetRef().Initialize();
        glfwSwapInterval(1);
    }
    else if (m_adopt_library == GraphicsLibrary_Vulkan) {
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
            throw std::runtime_error("failed to create instance!");
        }

        SDLOG("--- Vulkan create surface.");
        if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }

        SDLOG("Vulkan instance creation end!!!");

        m_graphics_app_instance = reinterpret_cast<CompHandle>(instance);

        VulkanCreationArg arg;
        arg.m_instance = instance;
        arg.m_surface = surface;
        GraphicsManager::GetRef().InitializeGraphicsSystem(arg);
        GraphicsManager::GetRef().Initialize();

    }
    else 
    {
        glfwMakeContextCurrent(m_window);
        GraphicsManager::GetRef().InitializeGraphicsSystem(EventArg());
        GraphicsManager::GetRef().Initialize();
        glfwSwapInterval(1);
    }
}

void GLFWApplication::ReleaseGraphicsSystem()
{
    SDLOG("Release Graphics System of Application.");
    GraphicsManager::GetRef().Release();
    GraphicsManager::GetRef().ReleaseGraphicsSystem();
    //destroy Graphics Manager.
    GraphicsManager::Destroy();
}

void GLFWApplication::TerminateApplication()
{
    SDLOG("Terminate Application.");
    //destroy Timer.
    Timer::GetRef().End();
    SDLOG("APP Ending at %lf.", Timer::GetRef().GetEndProgramTime());
    Timer::Destroy();
    //destroy File Manager
    FileSystemManager::Destroy();
    //destroy LogManager
    LogManager::Destroy();
}

void GLFWApplication::RunMainLoop()
{
    //--- v. initialize glew.
    Application::GetRef().InitializeGraphicsSystem();

    //3. launch main loop.
    if (m_adopt_library == Graphics::GraphicsLibrary_OpenGL4) {
        while (!glfwWindowShouldClose(m_window)) {
            try {
                // Update Game.
                Application::GetRef().Update();
            }
            catch (std::exception &e) {
                SDLOGE("Execption occur !!! %s.", e.what());
            }

            // Swap buffers
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }
    else {
        while (!glfwWindowShouldClose(m_window)) {
            try {
                // Update Game.
                Application::GetRef().Update();
            }
            catch (std::exception &e) {
                SDLOGE("Execption occur !!! %s.", e.what());
            }
            glfwPollEvents();
        }
    }

    Application::GetRef().ReleaseGraphicsSystem();
  
    //Destroy App.
    Application::GetRef().TerminateApplication();
}

_________________SD_END_APP_NAMESPACE________________