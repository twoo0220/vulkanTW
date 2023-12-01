#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vulkan/vulkan.h>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "VulkanRenderer.h"
#include "ImGuiRenderer.h"

class MainEntry
{
public:
	MainEntry(const uint32_t width = 800, const uint32_t height = 600);
	~MainEntry();

	void run();

private:
	void initWindow();
	bool initVulkan();
	void mainLoop();
	void cleanup();

	GLFWwindow* mWindow;
	VulkanRenderer mVkRenderer;
	ImGuiRenderer mImGuiRenderer;
	const uint32_t mWidth;
	const uint32_t mHeight;
};
