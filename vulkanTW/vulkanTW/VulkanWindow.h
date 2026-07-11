#pragma once

#include "Logger.h"
#include <vulkan/vulkan.h>	// Note: vulkan.h must be included before glfw3.h to enable Vulkan-related GLFW functions
#include <GLFW/glfw3.h>
#include <vector>

class vulkanWindow
{
public:
	vulkanWindow();
	~vulkanWindow();

	VkExtent2D getFrameBufferSize() const;
	VkSurfaceKHR createVulkanSurface(VkInstance instance);
	std::vector<const char*> getRequiredExtensions();
	void pollEvents() const;
	bool isCloseRequested() const;
	bool isMinimized() const;

private:
	GLFWwindow* mGLFWWindow = nullptr;
};
