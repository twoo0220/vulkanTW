#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::~VulkanRenderer()
{
	vkDestroyInstance(mInstance, nullptr);
}

void VulkanRenderer::CreateInstance()
{
	mVkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	mVkAppInfo.pApplicationName = "vulkanTW";
	mVkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	mVkAppInfo.pEngineName = "No_Engine";
	mVkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	mVkAppInfo.apiVersion = VK_API_VERSION_1_3;

	mVkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	mVkCreateInfo.pApplicationInfo = &mVkAppInfo;
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	mVkCreateInfo.enabledExtensionCount = glfwExtensionCount;
	mVkCreateInfo.ppEnabledExtensionNames = glfwExtensions;
	mVkCreateInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&mVkCreateInfo, nullptr, &mInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("falied to create instance!");
	}
}
