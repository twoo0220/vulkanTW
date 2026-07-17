#pragma once

#include <vector>
#include "vulkan/vulkan.h"

class VulkanContext
{
public:
	VulkanContext(const std::vector<const char*>& requiredInstanceExtensions, bool useSwapChain);
	~VulkanContext();

	void cleanup();
	bool createQueues();
	bool createInstance(std::vector<const char*> instanceExtensions);
	bool selectPhysicalDevice();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	std::vector<const char*> getRequireExtensions() const;
	bool checkValidationLayerSupport() const;
	bool isDeviceSuitable(VkPhysicalDevice device);

private:
	VkInstance mInstance = VK_NULL_HANDLE;
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	const std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
	bool mEnableValidationLayers = false;
#else
	bool mEnableValidationLayers = true;
#endif

};
