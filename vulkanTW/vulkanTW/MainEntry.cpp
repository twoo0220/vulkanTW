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
	if (!glfwVulkanSupported())
	{
		std::cout << "GLFW: Vulkan Not Supported!" << std::endl;
		return;
	}
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
		mVkRenderer.createCommandBuffer() &&
		mVkRenderer.createSyncObjects()
		)
	{
		std::cout << "Vulkan initialization Success!" << std::endl;
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

		// Wait for the previous frame to finish
		// Acquire an image from the swap chain
		// Record a command buffer which draws the scene onto that image
		// Submit the recorded command buffer
		// Present the swap chain image

		if (false == mVkRenderer.drawFrame())
		{
			//rendering fail
			std::cout << "Rendering Failed " << std::endl;
		}
	}

	mVkRenderer.WaitIdle();
}

void MainEntry::cleanup()
{
	//mVkRenderer.cleanUp();
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

