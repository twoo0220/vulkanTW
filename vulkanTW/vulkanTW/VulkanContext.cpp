#include <string>

#include "VulkanContext.h"
#include "Logger.h"

VulkanContext::VulkanContext(const std::vector<const char*>& requiredInstanceExtensions, bool useSwapChain)
{
	createInstance(requiredInstanceExtensions);
}

VulkanContext::~VulkanContext()
{
	cleanup();
}

void VulkanContext::createQueues()
{
	// Create Vulkan queues here
}

void VulkanContext::cleanup()
{
	// Cleanup Vulkan resources here
}

void VulkanContext::createInstance(std::vector<const char*> instanceExtensions)
{
	if ((true == mEnableValidationLayers) && !checkValidationLayerSupport())
	{
		std::cerr << "validation layers requested, but not available " << mEnableValidationLayers << std::endl;
		std::cerr << "Make sure you have the Vulkan SDK installed." << std::endl;
		return false;
		//throw std::runtime_error("validation layers requested, but not available");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "vulkanTW";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No_Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = getRequireExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (true == mEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
		createInfo.ppEnabledLayerNames = mValidationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (VK_SUCCESS != vkCreateInstance(&createInfo, nullptr, &g_Instance))
	{
		std::cerr << "failed to create instance!" << std::endl;
		//throw std::runtime_error("failed to create instance!");
		return false;
	}

	return true;
}