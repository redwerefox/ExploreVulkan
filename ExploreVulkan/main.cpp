
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
#include <set>

#include "QuereFamilyIndices.h"

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

//load Extension functions
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT DebugMessenger, VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
		func(instance, DebugMessenger, pAllocator);
}


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
	VkDebugUtilsMessengerEXT debugMessenger;

	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "ExploreVulcan", nullptr, nullptr);

	}
	void initVulcan() {

		initializeInstance();
		setupDebugMessengers();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();

	}
	void mainLoop(){
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
	void cleanup() {

		vkDestroyDevice(device,nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);

		if (enableValidationLayers)
			DestroyDebugUtilsMessengerExt(instance, debugMessenger, nullptr);

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

	void initializeInstance()
	{
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
			std::cout << "initialization of instance successful" << std::endl;
	}

	void setupDebugMessengers()
	{
		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT ;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr; //optional

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to set up debug messenger");
		}

	}

	//picks gpu
	void pickPhysicalDevice()
	{
		
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs that support Vulkan");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			if (isDeviceSuitable(device))
			{
				physicalDevice = device;
				break;
			}
		}
		if (physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("failed to find suitable GPU");
	}

	bool isDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indicies = findQuereFamilies(device);
		return indicies.isComplete();
	}

	QueueFamilyIndices findQuereFamilies(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices;

		uint32_t quereFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &quereFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> quereFamilies (quereFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &quereFamilyCount, quereFamilies.data());

		int i = 0;
		for (const auto& quereFamily : quereFamilies) {

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

			if (quereFamily.queueCount > 0 && quereFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (quereFamily.queueCount > 0 && presentSupport)
				indices.presentFamily = i;

			//This approach allows to pick two seperate queues for graphics and presentation
			//Note its less performant to do so

			if (indices.isComplete())
				break;
			i++;
		}

		return indices;
	}

	void createLogicalDevice()
	{

		QueueFamilyIndices indices = findQuereFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = uniqueQueueFamilies.size();
		createInfo.pEnabledFeatures = &deviceFeatures;

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device");

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}

	void createSurface() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
			throw std::runtime_error("failed to create window surface");
	}

	//Challenge 1
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

	

	//CALLBACKS

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "! validation layer:" << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
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


