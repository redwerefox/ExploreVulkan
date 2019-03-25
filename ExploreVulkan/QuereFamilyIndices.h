#pragma once

#include <optional>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices findQuereFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	uint32_t quereFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &quereFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> quereFamilies(quereFamilyCount);
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