#include "VulkanWindow.h"

vulkanWindow::vulkanWindow()
{
	constexpr float aspectRatio = 16.0f / 9.0f;
	constexpr float outRatio = 0.8f;

	if (!glfwInit())
	{
		exitWithMessage("glfwInit failed");
	}

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int displayWidth = videoMode->width;
	int displayHeight = videoMode->height;
	int windowWidth = 0;
	int windowHeight = 0;
	if (displayWidth > displayHeight)
	{
		// Landscape: base on height
		windowHeight = static_cast<int>(displayHeight * outRatio);
		windowWidth = static_cast<int>(windowHeight * aspectRatio);
	}
	else
	{
		// Portrait or square: base on width
		windowWidth = static_cast<int>(displayWidth * outRatio);
		windowHeight = static_cast<int>(windowWidth / aspectRatio);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	mGLFWWindow = glfwCreateWindow(windowWidth, windowHeight, "vulkanTW", nullptr, nullptr);

	if (mGLFWWindow == nullptr)
	{
		glfwTerminate();
		exitWithMessage("glfwCreateWindow failed");
	}
}

vulkanWindow::~vulkanWindow()
{
}
