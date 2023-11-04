#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "VulkanRenderer.h"

class MainEntry
{
public:
	MainEntry(const uint32_t width = 800, const uint32_t height = 600);
	~MainEntry();

	void run();

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	GLFWwindow* mWindow;
	VulkanRenderer mVkRenderer;
	const uint32_t mWidth;
	const uint32_t mHeight;
};
