#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	void CreateInstance();
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	VkResult DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);
private:
	VkInstance mInstance{};
	VkApplicationInfo mVkAppInfo{};
	VkInstanceCreateInfo mVkCreateInfo{};
	VkDebugUtilsMessengerEXT mDebugMessenger{};

	const bool mEnableValidationLayers = true;
};
