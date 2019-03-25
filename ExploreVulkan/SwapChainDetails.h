#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <algorithm>

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails details;

	
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	details.formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	details.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());

	return details;
}

bool swapChainAdequate(SwapChainSupportDetails details)
{
	return !details.formats.empty() && !details.presentModes.empty();
}

//Choosing the right settings for swap chain

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &avaiableformats)
{
	if (avaiableformats.size() == 1 && avaiableformats[0].format == VK_FORMAT_UNDEFINED)
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

	for (const auto& format : avaiableformats)
	{
		if (format.format == VK_FORMAT_R8G8B8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return format;
	}

	return avaiableformats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector < VkPresentModeKHR > availableModes)
{
	VkPresentModeKHR bestmode = VK_PRESENT_MODE_FIFO_KHR;
	//Check for triple buffering
	for (const auto& availableMode : availableModes)
	{
		if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availableMode;
		}
		else if (availableMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			bestmode = availableMode;
	}
	return bestmode;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites, uint32_t width, uint32_t height)
{
	if (capabilites.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilites.currentExtent;
	else {
		VkExtent2D actualExtent = { width,height };

		
		actualExtent.width = std::max(capabilites.minImageExtent.width, std::min(capabilites.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilites.minImageExtent.height, std::min(capabilites.maxImageExtent.width, actualExtent.height));
		return actualExtent;
	}
}