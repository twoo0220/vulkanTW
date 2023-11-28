#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value();
	}
};

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	bool CreateInstance();
	bool setupDebugMessenger();
	void pickPhysicalDevice();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	std::vector<const char*> getRequireExtensions();
	bool checkValidationLayerSupport();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	VkInstance mInstance{};
	VkApplicationInfo mVkAppInfo{};
	VkInstanceCreateInfo mVkCreateInfo{};
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT mDebugMessenger{};
	VkDebugUtilsMessengerCreateInfoEXT mCreateInfo{};

	uint32_t mQueueFamilyCount = 0;

	const bool mEnableValidationLayers = true;
	const std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };
};
