#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
#ifdef NDEBUG
	: mEnableValidationLayers(false)
#endif
{
}

VulkanRenderer::~VulkanRenderer()
{
	if (nullptr != mCommandPool) vkDestroyCommandPool(mDevice, mCommandPool, nullptr);

	for (VkFramebuffer framebuffer : mSwapChainFrameBuffers)
	{
		if (nullptr != framebuffer) vkDestroyFramebuffer(mDevice, framebuffer, nullptr);
	}

	if (nullptr != mGraphicsPipeline) vkDestroyPipeline(mDevice, mGraphicsPipeline, nullptr);
	if (nullptr != mPipelineLayout) vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);
	if (nullptr != mRenderPass) vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

	for (VkImageView imageView : mSwapChainImageViewVector)
	{
		if (nullptr != imageView) vkDestroyImageView(mDevice, imageView, nullptr);
	}

	if (nullptr != mSwapChain)	vkDestroySwapchainKHR(mDevice, mSwapChain, nullptr);
	if (nullptr != mDevice)		vkDestroyDevice(mDevice, nullptr);
	if (true == mEnableValidationLayers)	DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
	if (nullptr != mSurface)	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	if (nullptr != mInstance)	vkDestroyInstance(mInstance, nullptr);
}

bool VulkanRenderer::createInstance()
{
	if ((true == mEnableValidationLayers) && !checkValidationLayerSupport())
	{
		std::cerr << "validation layers requested, but not available " << mEnableValidationLayers << std::endl;
		std::cerr << "Make sure you have the Vulkan SDK installed." << std::endl;
		return false;
		//throw std::runtime_error("validation layers requested, but not available");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "vulkanTW";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No_Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = getRequireExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (true == mEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
		createInfo.ppEnabledLayerNames = mValidationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (VK_SUCCESS != vkCreateInstance(&createInfo, nullptr, &mInstance))
	{
		std::cerr << "failed to create instance!" << std::endl;
		//throw std::runtime_error("failed to create instance!");
		return false;
	}

	return true;
}

bool VulkanRenderer::createSurface(GLFWwindow* window)
{
	if (VK_SUCCESS != glfwCreateWindowSurface(mInstance, window, nullptr, &mSurface))
	{
		std::cerr << "failed to create window surface!" << std::endl;
		return false;
	}

	return true;
}

bool VulkanRenderer::setupDebugMessenger()
{
	if (false == mEnableValidationLayers)
	{
		return false;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);

	if (VK_SUCCESS != CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger))
	{
		std::cerr << "failed to set up debug messenger!" << std::endl;
		return false;
		//throw std::runtime_error("failed to set up debug messenger!");
	}

	return true;
}

bool VulkanRenderer::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	if (0 == deviceCount)
	{
		std::cerr << "failed to find GUPs with Vulkan support!" << std::endl;
		return false;
		//throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	for (const VkPhysicalDevice& dev : devices)
	{
		if (isDeviceSuitable(dev))
		{
			mPhysicalDevice = dev;
			break;
		}
	}

	if (VK_NULL_HANDLE == mPhysicalDevice)
	{
		std::cerr << "failed to find a suitable GPU!" << std::endl;
		return false;
	}

	return true;
}

bool VulkanRenderer::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(mPhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfoVector{};
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfoVector.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoVector.size());
	createInfo.pQueueCreateInfos = queueCreateInfoVector.data();
	createInfo.enabledLayerCount = 0;

	//createInfo.enabledExtensionCount = 0; //이거 오류 블로그에 적어둘것
	createInfo.enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = mDeviceExtensions.data(); 

	VkPhysicalDeviceFeatures deviceFeatures{};
	createInfo.pEnabledFeatures = &deviceFeatures;

	if (true == mEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
		createInfo.ppEnabledLayerNames = mValidationLayers.data();
	}

	if (VK_SUCCESS != vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice))
	{
		std::cerr << "failedto create logical device!" << std::endl;
		return false;
	}

	vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &mPresentQueue);
	return true;
}

bool VulkanRenderer::createSwapChain(GLFWwindow* window)
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(mPhysicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if ((swapChainSupport.capabilities.maxImageCount > 0) && (imageCount > swapChainSupport.capabilities.maxImageCount))
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapChainInfo{};
	swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainInfo.surface = mSurface;
	swapChainInfo.minImageCount = imageCount;
	swapChainInfo.imageFormat = surfaceFormat.format;
	swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainInfo.imageExtent = extent;
	swapChainInfo.imageArrayLayers = 1;
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(mPhysicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainInfo.queueFamilyIndexCount = 2;
		swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.queueFamilyIndexCount = 0;
		swapChainInfo.pQueueFamilyIndices = nullptr;
	}

	swapChainInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainInfo.presentMode = presentMode;
	swapChainInfo.clipped = VK_TRUE;
	swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

	if (VK_SUCCESS != vkCreateSwapchainKHR(mDevice, &swapChainInfo, nullptr, &mSwapChain))
	{
		std::cerr << "failed to create swap chain" << std::endl;
		return false;
	}

	vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, nullptr);
	mSwapChainImageVector.resize(imageCount);
	vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, mSwapChainImageVector.data());
	mSwapChainImageFormat = surfaceFormat.format;
	mSwapChainExtent = extent;

	return true;
}

bool VulkanRenderer::createImageViews()
{
	size_t scSize = mSwapChainImageVector.size();
	mSwapChainImageViewVector.resize(scSize);
	for (size_t i = 0; i < scSize; ++i)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = mSwapChainImageVector[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = mSwapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (VK_SUCCESS != vkCreateImageView(mDevice, &createInfo, nullptr, &mSwapChainImageViewVector[i]))
		{
			std::cerr << "failed to create image view!" << std::endl;
			return false;
		}
	}

	return true;
}

bool VulkanRenderer::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = mSwapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subPass{};
	subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPass.colorAttachmentCount = 1;
	subPass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subPass;

	if (VK_SUCCESS != vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass))
	{
		std::cerr << "Failed to create render pass" << std::endl;
		return false;
	}

	return true;
}

bool VulkanRenderer::createGraphicsPipeline()
{
	std::vector<char> vertShaderCode = vertexShader.readShaderFile("shaders/vert.spv");
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	if (nullptr == vertShaderModule)
	{
		std::cerr << "Failed to createGraphicsPipeline" << std::endl;
		return false;
	}
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	std::vector<char> fragShaderCode = fragShader.readShaderFile("shaders/frag.spv");
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
	if (nullptr == fragShaderModule)
	{
		std::cerr << "Failed to createGraphicsPipeline" << std::endl;
		return false;
	}
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> vectorDynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(vectorDynamicStates.size());
	dynamicState.pDynamicStates = vectorDynamicStates.data();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (VK_SUCCESS != vkCreatePipelineLayout(mDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout))
	{
		std::cerr << "Failed to create pipeline layout" << std::endl;
		return false;
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = mPipelineLayout;
	pipelineInfo.renderPass = mRenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (VK_SUCCESS != vkCreateGraphicsPipelines(mDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mGraphicsPipeline))
	{
		std::cerr << "Failed to create graphics pipeline" << std::endl;
		return false;
	}

	vkDestroyShaderModule(mDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(mDevice, vertShaderModule, nullptr);
	return true;
}

bool VulkanRenderer::createFrameBuffers()
{
	size_t framebufferSize = mSwapChainImageViewVector.size();
	mSwapChainFrameBuffers.resize(framebufferSize);

	for (size_t i = 0; i < framebufferSize; ++i)
	{
		VkImageView attachments[] = {
			mSwapChainImageViewVector[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = mRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = mSwapChainExtent.width;
		framebufferInfo.height = mSwapChainExtent.height;
		framebufferInfo.layers = 1;

		if (VK_SUCCESS != vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr, &mSwapChainFrameBuffers[i]))
		{
			std::cerr << "Failed to create framebuffer[" << i << "]" << std::endl;
			return false;
		}
	}

	return true;
}

bool VulkanRenderer::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(mPhysicalDevice);
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (VK_SUCCESS != vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mCommandPool))
	{
		std::cerr << "Failed to create command pool" << std::endl;
		return false;
	}

	return true;
}

bool VulkanRenderer::createCommandBuffer()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = mCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (VK_SUCCESS != vkAllocateCommandBuffers(mDevice, &allocInfo, &mCommandBuffer))
	{
		std::cerr << "Failed to allocate command buffer" << std::endl;
		return false;
	}

	return true;
}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (VK_SUCCESS != vkCreateShaderModule(mDevice, &createInfo, nullptr, &shaderModule))
	{
		std::cerr << "Failed to create shader module" << std::endl;
		return nullptr;
	}

	return shaderModule;
}

void VulkanRenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (nullptr != func)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VulkanRenderer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (nullptr != func)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

std::vector<const char*> VulkanRenderer::getRequireExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (true == mEnableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool VulkanRenderer::checkValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : mValidationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (false == layerFound)
		{
			return false;
		}
	}

	return true;
}

bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(mDeviceExtensions.begin(), mDeviceExtensions.end());
	for (const VkExtensionProperties& ext : availableExtensions)
	{
		requiredExtensions.erase(ext.extensionName);
	}

	return requiredExtensions.empty();
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (true == extensionSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionSupported && swapChainAdequate;
}

bool VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (VK_SUCCESS != vkBeginCommandBuffer(commandBuffer, &beginInfo))
	{
		std::cerr << "Failed to begin recording command buffer" << std::endl;
		return false;
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = mRenderPass;
	renderPassInfo.framebuffer = mSwapChainFrameBuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = mSwapChainExtent;

	VkClearValue clearColor = { {{ 0.0f, 0.0f, 0.0f, 1.0f }} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float_t>(mSwapChainExtent.width);
	viewport.height = static_cast<float_t>(mSwapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = mSwapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(commandBuffer);

	if (VK_SUCCESS != vkEndCommandBuffer(commandBuffer))
	{
		std::cerr << "Failed to record command buffer" << std::endl;
		return false;
	}

	return true;
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const VkSurfaceFormatKHR& avformat : availableFormats)
	{
		if ((VK_FORMAT_B8G8R8A8_SRGB == avformat.format) && (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == avformat.colorSpace))
		{
			return avformat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const VkPresentModeKHR& avPresentMode : availablePresentModes)
	{
		if (VK_PRESENT_MODE_MAILBOX_KHR == avPresentMode)
		{
			return avPresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRenderer::chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width(0), height(0);
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const VkQueueFamilyProperties& queueFam : queueFamilies)
	{
		if (queueFam.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (true == indices.isComplete())
		{
			break;
		}

		++i;
	}

	return indices;
}

SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);
	if (0 != formatCount)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr);
	if (0 != presentModeCount)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, details.presentModes.data());
	}

	return details;
}
