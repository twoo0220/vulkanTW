#include "MainEntry.h"

MainEntry::MainEntry(const uint32_t width, const uint32_t height)
	: mWindow(nullptr)
	, mWidth(width)
	, mHeight(height)
{
}

MainEntry::~MainEntry()
{
}

void MainEntry::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void MainEntry::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(mWidth, mHeight, "VulkanTW", nullptr, nullptr);
}

void MainEntry::initVulkan()
{
	mVkRenderer.CreateInstance();
}

void MainEntry::mainLoop()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
	}
}

void MainEntry::cleanup()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

