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
	if (mGLFWWindow != nullptr)
	{
		glfwDestroyWindow(mGLFWWindow);
		glfwTerminate();
		mGLFWWindow = nullptr;
	}
}

VkExtent2D vulkanWindow::getFrameBufferSize() const
{
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(mGLFWWindow, &width, &height);

	return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
}

VkSurfaceKHR vulkanWindow::createVulkanSurface(VkInstance instance)
{
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkResult res = glfwCreateWindowSurface(instance, mGLFWWindow, nullptr, &surface);
	Logger::getInstance().checkVulkanResult(res);

	return surface;
}

std::vector<const char*> vulkanWindow::getRequiredExtensions()
{
	// OS에 따라 필요한 Extension을 GLFW 통해 가져오기
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> instanceExtensions;

	// 메모리 재할당 방지를 위해 필요한 크기를 미리 할당 (+1은 PORTABILITY 확장 기능용)
	instanceExtensions.reserve(glfwExtensionCount + 1);

	// GLFW 확장 기능은 중복이 없으므로 반복문과 std::find 불필요
	if ((glfwExtensions != nullptr) && (glfwExtensionCount > 0))
	{
		instanceExtensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
	}

	// 필요한 추가 확장 기능 등록
	instanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	printLog("GlfwRequiredInstanceExtensions count: ", glfwExtensionCount);

	for (const char* extension : instanceExtensions)
	{
		printLog("  - ", extension);
	}

	return instanceExtensions;
}

void vulkanWindow::pollEvents() const
{
	glfwPollEvents();
	// 키보드 이벤트를 실제로 처리하는 것은 glfwSetKeyCallback()에서 등록한 콜백함수
}

bool vulkanWindow::isCloseRequested() const
{
	return glfwWindowShouldClose(mGLFWWindow);
}

bool vulkanWindow::isMinimized() const
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(mGLFWWindow, &width, &height);

	return (width == 0 || height == 0);
}
