#include "SDEngine.h"
#include "VulkanCreationArg.h"
#include "GLFWApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace App ----------------------------
namespace App
{

GLFWApplication::GLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: Application(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
, m_window(nullptr)
, m_monitor(nullptr)
{

}

GLFWApplication::~GLFWApplication()
{

}

void GLFWApplication::Initialize()
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
	//Initialize KeyBoard Mapping.
}

void GLFWApplication::InitializeGraphicsSystem()
{
	SDLOG("Initialize Graphics System of Application.");
	//new Graphics Manager.
	//--- iv. make current.
	if (m_adopt_library == GraphicsLibrary_OpenGL4) 
	{
		new OpenGL4Manager();

		glfwMakeContextCurrent(m_window);
		GraphicsManager::GetRef().InitializeGraphicsSystem(EventArg());
		glfwSwapInterval(1);
	}
	else if (m_adopt_library == GraphicsLibrary_Vulkan)
	{
		new VulkanManager();

		VkApplicationInfo appInfo = {};
		VkInstanceCreateInfo ins_c_info = {};
		VkInstance instance = nullptr;
		VkSurfaceKHR surface = nullptr;

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = m_win_title.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "SD Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		ins_c_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		ins_c_info.pApplicationInfo = &appInfo;

		uint32_t glfw_ext_count = 0;
		const char** glfw_exts;
		glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
		std::vector<const char*> glfw_extensions(glfw_exts, glfw_exts + glfw_ext_count);
		glfw_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		ins_c_info.enabledExtensionCount = static_cast<uint32_t>(glfw_extensions.size());
		ins_c_info.ppEnabledExtensionNames = glfw_extensions.data();
		SDLOG("Vulkan instance creation !!!");
		SDLOG("--- Adjust valid layers.");
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		std::vector<VkLayerProperties> avaiable_valid_layer(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, avaiable_valid_layer.data());
		for (auto &layer : avaiable_valid_layer) {
			SDLOG("--- Avaiable valid layer :%s[%s](%d)(%d)", layer.layerName, layer.description, layer.implementationVersion, layer.specVersion);
		}
#ifdef _NDEBUG
		ins_c_info.enabledLayerCount = 0;
#else
		bool layer_valid = false;
		for (VkLayerProperties &layer : avaiable_valid_layer) {
			for (const char* layer_name : VulkanManager::GetValidLayers()) {
				if (strcmp(layer.layerName, layer_name) == 0) {
					layer_valid = true;
					break;
				}
			}
			if (layer_valid == true)
				break;
		}
		ins_c_info.enabledLayerCount = static_cast<uint32_t>(VulkanManager::GetValidLayers().size());
		ins_c_info.ppEnabledLayerNames = VulkanManager::GetValidLayers().data();
		if (layer_valid == false) {
			throw std::runtime_error("failed to valid!");
		}
#endif 
		SDLOG("--- Vulkan create instance.");
		if (vkCreateInstance(&ins_c_info, nullptr, (VkInstance*)&instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

		SDLOG("--- Vulkan create surface.");
		if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
		SDLOG("Vulkan instance creation end!!!");


		VulkanCreationArg arg;
		arg.m_instance = instance;
		arg.m_surface = surface;
		GraphicsManager::GetRef().InitializeGraphicsSystem(arg);

	}
	else 
	{
		glfwMakeContextCurrent(m_window);
		GraphicsManager::GetRef().InitializeGraphicsSystem(EventArg());
		glfwSwapInterval(1);
	}
}

void GLFWApplication::ReleaseGraphicsSystem()
{
	SDLOG("Release Graphics System of Application.");
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

void GLFWApplication::RegisterGLFW(GLFWwindow *i_window, GLFWmonitor *i_monitor)
{
	m_window = i_window;
	m_monitor = i_monitor;
}

}
//---------------------------- end of namespace App ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------