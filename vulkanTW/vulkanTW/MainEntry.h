#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vulkan/vulkan.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>

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

	void printFPS();

private:
	GLFWwindow* mWindow{};
	VulkanRenderer mVkRenderer{};
	//ImGuiRenderer mImGuiRenderer;

	const uint32_t mWidth;
	const uint32_t mHeight;
};
