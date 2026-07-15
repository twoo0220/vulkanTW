#pragma once

#include "Logger.h"
#include <vulkan/vulkan.h>	// Note: vulkan.h must be included before glfw3.h to enable Vulkan-related GLFW functions
#include <GLFW/glfw3.h>
#include <vector>

class VulkanWindow
{
public:
	VulkanWindow();
	~VulkanWindow();

	VkExtent2D getFrameBufferSize() const;
	VkSurfaceKHR createVulkanSurface(VkInstance instance);
	std::vector<const char*> getRequiredExtensions();
	void pollEvents() const;
	bool isCloseRequested() const;
	bool isMinimized() const;

	void setUserPointer(void* userPointer);
	void setKeyCallback(GLFWkeyfun callback);
	void setMouseButtonCallback(GLFWmousebuttonfun callback);
	void setCursorPosCallback(GLFWcursorposfun callback);
	void setScrollCallback(GLFWscrollfun callback);
	void setFramebufferSizeCallback(GLFWframebuffersizefun callback);

private:
	GLFWwindow* mGLFWWindow = nullptr;
};
