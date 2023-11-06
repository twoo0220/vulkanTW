#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <vector>

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	void CreateInstance();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);
private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	std::vector<const char*> getRequireExtensions();

	VkInstance mInstance{};
	VkApplicationInfo mVkAppInfo{};
	VkInstanceCreateInfo mVkCreateInfo{};
	VkDebugUtilsMessengerEXT mDebugMessenger{};
	VkDebugUtilsMessengerCreateInfoEXT mCreateInfo{};

	const bool mEnableValidationLayers = true;
	const std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };
};
