#pragma once

#include "Logger.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.h>

class vulkanWindow
{
public:
	vulkanWindow();
	~vulkanWindow();

	VkExtent2D getFrameBufferSize() const;
	void pollEvents() const;

private:
	GLFWwindow* mGLFWWindow = nullptr;
};
