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
private:
	VkInstance mInstance;
	VkApplicationInfo mVkAppInfo;
	VkInstanceCreateInfo mVkCreateInfo;
};
