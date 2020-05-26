#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "VulkanWrapper.h"
#include "WindowsApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

enum UserMessage {
    USER_MESSAGE_RESIZE = WM_USER + 1,
    USER_MESSAGE_QUIT,
    USER_MESSAGE_MOUSE_CLICK,
    USER_MESSAGE_MOUSE_MOVE,
    USER_MESSAGE_MOUSE_WHEEL
};

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 0, 1);
        break;
    case WM_LBUTTONUP:
        PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 0, 0);
        break;
    case WM_RBUTTONDOWN:
        PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 1, 1);
        break;
    case WM_RBUTTONUP:
        PostMessage(hWnd, USER_MESSAGE_MOUSE_CLICK, 1, 0);
        break;
    case WM_MOUSEMOVE:
        PostMessage(hWnd, USER_MESSAGE_MOUSE_MOVE, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:
        PostMessage(hWnd, USER_MESSAGE_MOUSE_WHEEL, HIWORD(wParam), 0);
        break;
    case WM_SIZE:
    case WM_EXITSIZEMOVE:
        PostMessage(hWnd, USER_MESSAGE_RESIZE, wParam, lParam);
        break;
    case WM_KEYDOWN:
        if (VK_ESCAPE == wParam) {
            PostMessage(hWnd, USER_MESSAGE_QUIT, wParam, lParam);
        }
        break;
    case WM_CLOSE:
        PostMessage(hWnd, USER_MESSAGE_QUIT, wParam, lParam);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

WindowsApplication::WindowsApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
, m_hwnd(NULL)
, m_h_ins(NULL)
{
    m_h_ins = GetModuleHandle(nullptr);

    WNDCLASSEX win_c_info = {};
    win_c_info.cbSize = sizeof(WNDCLASSEX);
    win_c_info.style = CS_HREDRAW | CS_VREDRAW;
    win_c_info.lpfnWndProc = WindowProcedure;
    win_c_info.cbClsExtra = 0;
    win_c_info.cbWndExtra = 0;
    win_c_info.hInstance = m_h_ins;
    win_c_info.hIcon = nullptr;
    win_c_info.hCursor = LoadCursor(nullptr, IDC_ARROW);
    win_c_info.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    win_c_info.lpszMenuName = nullptr;
    win_c_info.lpszClassName = L"SDWindowsAPP";
    win_c_info.hIconSm = nullptr;

    if (!RegisterClassEx(&win_c_info)) {
        throw std::runtime_error("Cannot register class!");
    }

    m_hwnd = CreateWindow(
        L"SDWindowsAPP",
        SD_ADT_OS_STRCSTR(i_win_title),
        WS_OVERLAPPEDWINDOW,
        0, 0,
        i_win_res.GetWidth(), i_win_res.GetHeight(),
        nullptr,
        nullptr,
        m_h_ins,
        nullptr);

    if (m_hwnd == nullptr) {
        throw std::runtime_error("Cannot create handle!!!");
    }
}

WindowsApplication::~WindowsApplication()
{
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
    }

    if (m_h_ins) {
        UnregisterClass(L"SDWindowsAPP", m_h_ins);
    }
}

void WindowsApplication::Initialize()
{
    SDLOG("Initialize Application.");
    //new LogManager.
    new WindowsLogManager();
    //new WindowsFileSystemManager.
    new WindowsFileSystemManager();
    FileSystemManager::GetRef().Initialize();
    //new FileResourceRequester.
    new FileResourceRequester();
    //new Timer.
    new Timer();
    Timer::GetRef().Start();
    SDLOG("APP Starting at %lf.", Timer::GetRef().GetProgramStartTime());
    //Initialize KeyBoard Mapping.
}

void WindowsApplication::InitializeGraphicsSystem()
{
    SDLOG("Initialize Graphics System of Application.");
    //new Graphics Manager.
    //--- iv. make current.
    if (m_adopt_library == GraphicsLibrary_OpenGL4) {
        new OpenGL4Manager();

        //Create wgl context.

        //
        GraphicsManager::GetRef().InitializeGraphicsSystem(EventArg());
        GraphicsManager::GetRef().Initialize();

    }
    else if (m_adopt_library == GraphicsLibrary_Vulkan) {
        new VulkanManager();

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
        app_info.pNext = nullptr;
        app_info.pApplicationName = m_win_title.c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "SD Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo ins_c_info = {};
        ins_c_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        ins_c_info.pNext = nullptr;
        ins_c_info.flags = 0;
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
        VkWin32SurfaceCreateInfoKHR sur_c_info = {};
        sur_c_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        sur_c_info.pNext = nullptr;
        sur_c_info.flags = 0;
        sur_c_info.hwnd = m_hwnd;
        sur_c_info.hinstance = m_h_ins;

        if (vkCreateWin32SurfaceKHR(instance, &sur_c_info, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Cannot create surface!");
        }
        SDLOG("Vulkan instance creation end!!!");

        VulkanCreationArg arg;
        arg.m_instance = instance;
        arg.m_surface = surface;
        GraphicsManager::GetRef().InitializeGraphicsSystem(arg);
        GraphicsManager::GetRef().Initialize();

    } else {
        SDLOGE("Error engine type!!!");
        throw std::runtime_error("Error engine type!!!");
    }
}

void WindowsApplication::ReleaseGraphicsSystem()
{
    SDLOG("Release Graphics System of Application.");
    GraphicsManager::GetRef().Release();
    GraphicsManager::GetRef().ReleaseGraphicsSystem();
    //destroy Graphics Manager.
    GraphicsManager::Destroy();
}

void WindowsApplication::TerminateApplication()
{
    SDLOG("Terminate Application.");
    //destroy Timer.
    Timer::GetRef().End();
    SDLOG("APP Ending at %lf.", Timer::GetRef().GetProgramEndTime());
    Timer::Destroy();
    //destroy File Manager
    FileSystemManager::Destroy();
    //destroy FileResourceRequester
    FileResourceRequester::Destroy();
    //destroy LogManager
    LogManager::Destroy();
}

KeyStatusEnum WindowsApplication::GetKeyStateByCode(KeyCodeEnum i_code)
{
    if (i_code != KEY_MAX_NUMBER) {
        SHORT status = GetKeyState(SD_ENUM_TO_INT(i_code));
        if (status < 0) {
            return KEY_STATUS_PRESS;
        } else {
            return KEY_STATUS_RELEASE;
        }
    }
    else {
        return KEY_STATUS_NOT_SUPPORT;
    }
}

//======================= Run Function =======================
void WindowsApplication::RunMainLoop()
{
    ShowWindow(m_hwnd, SW_SHOWNORMAL);

    UpdateWindow(m_hwnd);

    //Initialize Graphics System.
    InitializeGraphicsSystem();

    MSG message;
    bool loop = true;

    while (loop) {
        //Receive 
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            switch (message.message) {
            case USER_MESSAGE_MOUSE_CLICK:
                //MouseClick btn : static_cast<size_t>(message.wParam) , state : message.lParam > 0 
                break;
            case USER_MESSAGE_MOUSE_MOVE:
                //MouseClick x : static_cast<int>(message.wParam), y : static_cast<int>(message.lParam)
                break;
            case USER_MESSAGE_MOUSE_WHEEL:
                //MouseWheel wheel static_cast<short>(message.wParam)
                break;
            case USER_MESSAGE_RESIZE:
                //Resize w : wParam, y : lParam
                Resize(SD_NULL_HANDLE, message.wParam, message.lParam);
                break;
            case USER_MESSAGE_QUIT:
                loop = false;
                break;
            }

            TranslateMessage(&message);

            DispatchMessage(&message);
        } else {
            Application::GetRef().Update();
        }
    }
}
