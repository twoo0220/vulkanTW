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
	if (true == initVulkan())
	{
		//mImGuiRenderer.initialize();

		mainLoop();
	}

	cleanup();
}

void MainEntry::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(mWidth, mHeight, "VulkanTW", nullptr, nullptr);
}

bool MainEntry::initVulkan()
{
	// 이 중 하나라도 실패하면 false
	// 모두 성공하면 true
	if (mVkRenderer.createInstance() &&
		mVkRenderer.setupDebugMessenger() &&
		mVkRenderer.createSurface(mWindow) &&
		mVkRenderer.pickPhysicalDevice() &&
		mVkRenderer.createLogicalDevice() &&
		mVkRenderer.createSwapChain(mWindow) &&
		mVkRenderer.createImageViews() &&
		mVkRenderer.createGraphicsPipeline()
		)
	{
		return true;
	}

	std::cerr << "Vulkan initialization failed" << std::endl;
	return false;
}

void MainEntry::mainLoop()
{
	glm::mat4 matrix{};
	glm::vec4 vec{};
	auto test = matrix * vec;
	std::cout << test.a << std::endl;

	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
		//mImGuiRenderer.update();
	}
}

void MainEntry::cleanup()
{
	//mVkRenderer.cleanUp();
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

