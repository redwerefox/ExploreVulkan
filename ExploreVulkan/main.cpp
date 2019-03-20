
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string>

int WIDTH = 800;
int HEIGHT = 600;

//basic range of diagonstic layers
const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else 
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulcan();
		mainLoop();
		cleanup();
	}

private:

	GLFWwindow* window;

	VkInstance instance;

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "ExploreVulcan", nullptr, nullptr);

	}
	void initVulcan() {

		//App Info Creation

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		appInfo.pEngineName = "KitsuBox";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		
		std::vector<const char*> glfwExtentions = getRequiredExtensions();

		createInfo.ppEnabledExtensionNames = glfwExtentions.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtentions.size());

		createInfo.enabledLayerCount = 0;

		// Checking Extentions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "available extensions:" << std::endl;

		if (checkExtensions(glfwExtentions, extensions) != VK_SUCCESS)
		{
			std::cerr << "Missing at least one extension"; 
		}

		//Checking Validation Layers
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		if (enableValidationLayers && !checkValidationLayers(availableLayers, validationLayers))
		{
			throw std::runtime_error("validation layers requested but not available");
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to initialize vulkan instance");
		}
		else
			std::cout << "initialization of instance successful" ;

	}
	void mainLoop(){
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
	void cleanup() {

		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}


	std::vector <const char*> getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char ** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}

	//Challenge 1#
	int checkExtensions (std::vector<const char*> glfwExtensions, std::vector<VkExtensionProperties> vkExtensions){
		int32_t extensionsFound = 0;
		for (const auto& extension : vkExtensions)
		{
			std::cout << "\n" << extension.extensionName << " ";

			for (const std::string& glfwExtensionName : glfwExtensions)
			{
				if (glfwExtensionName == extension.extensionName) {
					std::cout << "as required glfw extension ";
					std::cout << "found";
					extensionsFound ++;
					break;
				}
			}
			
		}
		if (extensionsFound == static_cast<int32_t>(glfwExtensions.size()))
			return VK_SUCCESS;
		else
			return 1;
	}

	bool checkValidationLayers(std::vector<VkLayerProperties> availableLayers,const std::vector<const char*> validationLayers)
	{
		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;
			for (const auto& layer : availableLayers)
			{
				if (strcmp(layer.layerName, layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
				return false;
		}
		return true;
		
	}

};

	




int main() {

	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
		}

	return EXIT_SUCCESS;
}


