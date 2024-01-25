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
	// false if any fail, true if all succeed
	if (mVkRenderer.createInstance() &&
		mVkRenderer.setupDebugMessenger() &&
		mVkRenderer.createSurface(mWindow) &&
		mVkRenderer.pickPhysicalDevice() &&
		mVkRenderer.createLogicalDevice() &&
		mVkRenderer.createSwapChain(mWindow) &&
		mVkRenderer.createImageViews() &&
		mVkRenderer.createRenderPass() &&
		mVkRenderer.createGraphicsPipeline() &&
		mVkRenderer.createFrameBuffers() &&
		mVkRenderer.createCommandPool() &&
		mVkRenderer.createCommandBuffer()
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

