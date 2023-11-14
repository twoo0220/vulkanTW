#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
#ifdef NDEBUG
	: mEnableValidationLayers(false)
#endif
{
}

VulkanRenderer::~VulkanRenderer()
{
	if (mEnableValidationLayers == true)
	{
		DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
	}

	vkDestroyInstance(mInstance, nullptr);
}

void VulkanRenderer::CreateInstance()
{
	if (mEnableValidationLayers && !checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available");
	}

	mVkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	mVkAppInfo.pApplicationName = "vulkanTW";
	mVkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	mVkAppInfo.pEngineName = "No_Engine";
	mVkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	mVkAppInfo.apiVersion = VK_API_VERSION_1_3;

	mVkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	mVkCreateInfo.pApplicationInfo = &mVkAppInfo;

	std::vector<const char*> extensions = getRequireExtensions();
	mVkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	mVkCreateInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (mEnableValidationLayers == true)
	{
		mVkCreateInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
		mVkCreateInfo.ppEnabledLayerNames = mValidationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		mVkCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		mVkCreateInfo.enabledLayerCount = 0;
		mVkCreateInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&mVkCreateInfo, nullptr, &mInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("falied to create instance!");
	}
}

void VulkanRenderer::setupDebugMessenger()
{
	if (mEnableValidationLayers == false)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void VulkanRenderer::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			mPhysicalDevice = device;
			break;
		}
	}
}

void VulkanRenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VulkanRenderer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

std::vector<const char*> VulkanRenderer::getRequireExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (mEnableValidationLayers == true)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool VulkanRenderer::checkValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : mValidationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (layerFound == false)
		{
			return false;
		}
	}

	return true;
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	return indices.isComplete();
}

QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	//vkGetPhysicalDeviceQueueFamilyProperties(device, &mQueueFamilyCount, nullptr);
	//
	//std::vector<VkQueueFamilyProperties> queueFamilies(mQueueFamilyCount);
	//vkGetPhysicalDeviceQueueFamilyProperties(device, &mQueueFamilyCount, queueFamilies.data());

	//int i = 0;
	//for (const auto& queueFamily : queueFamilies)
	//{
	//	if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
	//	{
	//		indices.graphicsFamily = i;
	//	}

	//	if (indices.isComplete())
	//	{
	//		break;
	//	}

	//	++i;
	//}

	return indices;
}
