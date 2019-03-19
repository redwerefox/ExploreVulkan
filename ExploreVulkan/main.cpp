
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

		uint32_t glfwExtensionCount = 0;
		const char ** glfwExtentions;


		glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.ppEnabledExtensionNames = glfwExtentions;
		createInfo.enabledExtensionCount = glfwExtensionCount;

		createInfo.enabledLayerCount = 0;

		// Checking Extentions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "available extensions:" << std::endl;

		if (checkExtensions(glfwExtentions, glfwExtensionCount, extensions) != VK_SUCCESS)
		{
			std::cerr << "Missing at least one extension"; 
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

	//Challenge 1#
	int checkExtensions (const char** glfwExtensions, int glfwExtensionCount, std::vector<VkExtensionProperties> vkExtensions){

		std::vector<std::string> v(glfwExtensions, glfwExtensions + glfwExtensionCount);
		int32_t extensionsFound = 0;

		for (const auto& extension : vkExtensions)
		{
			std::cout << "\n" << extension.extensionName << " ";

			for (const std::string& glfwExtensionName : v)
			{
				if (glfwExtensionName == extension.extensionName) {
					std::cout << "as required glfw extension ";
					std::cout << "found";
					extensionsFound ++; 
				}
			}
			
		}
		if (extensionsFound == glfwExtensionCount)
			return VK_SUCCESS;
		else
			return 1;
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


