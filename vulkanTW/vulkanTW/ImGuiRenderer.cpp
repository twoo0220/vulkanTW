#include "ImGuiRenderer.h"

static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice()
{
	uint32_t gpu_count = 1;
	VkResult result = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, nullptr);
	check_vk_result(result);

	ImVector<VkPhysicalDevice> gpus;
	gpus.resize(gpu_count);
	result = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus.Data);
	check_vk_result(result);
	
	// If a number > 1 of GPUs got reported, find discreate GPU if present, or use first one available.

	for (VkPhysicalDevice& device : gpus)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);
		if (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU == properties.deviceType)
		{
			return device;
		}
	}

	// Use first GPU (Integrated) is a Discrete one is not available.
	if (gpu_count > 0)
		return gpus[0];
	return VK_NULL_HANDLE;
}

ImGuiRenderer::ImGuiRenderer(GLFWwindow* window)
	: mGLFWWindow(window)
	, mWD(&g_ImGuiVulkanWindowData)
{
}

void ImGuiRenderer::initialize()
{
	VkDescriptorPoolSize poolSize[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = 1000;
	poolInfo.poolSizeCount = sizeof(poolSize);
	poolInfo.pPoolSizes = poolSize;

	//VkDescriptorPool imguiPool;
	//ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

	//// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	////ImGui::StyleColorsLight();

	//// Setup Platform/Renderer backends
	//ImGui_ImplGlfw_InitForVulkan(mGLFWWindow, true);
	//ImGui_ImplVulkan_InitInfo init_info{};
	//init_info.Instance = g_Instance;
	//init_info.PhysicalDevice = g_PhysicalDevice;
	//init_info.Device = g_Device;
	//init_info.QueueFamily = g_QueueFamily;
	//init_info.Queue = g_Queue;
	//init_info.PipelineCache = g_PipelineCache;
	//init_info.DescriptorPool = g_DescriptorPool;
	//init_info.Subpass = 0;
	//init_info.MinImageCount = g_MinImageCount;
	//init_info.ImageCount = wd->ImageCount;
	//init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	//init_info.Allocator = g_Allocator;
	//init_info.CheckVkResultFn = check_vk_result;
	//ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

	glfwSetErrorCallback(glfw_error_callback);
	
}

// FrameRenderer
void ImGuiRenderer::update()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	const bool is_minized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
	if (!is_minized)
	{
		//mWD->ClearValue.color.float32[0] = clear_color.x * clear_color.w;

		VkResult err;
		VkSemaphore image_acquired_semaphore = mWD->FrameSemaphores[mWD->SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = mWD->FrameSemaphores[mWD->SemaphoreIndex].RenderCompleteSemaphore;
		// err = vkAcquireNextImageKHR(
		//	g_Device,
		//	mWD->Swapchain,
		//	UINT64_MAX,
		//	image_acquired_semaphore,
		//	VK_NULL_HANDLE,
		//	&mWD->FrameIndex);
		//if ((VK_ERROR_OUT_OF_DATE_KHR == err) || (VK_SUBOPTIMAL_KHR == err))
		//{
		//	g_SwapChainRebuild = true;
		//	return;
		//}

		//ImGui_ImplVulkanH_Frame* fd = &mWD->Frames[mWD->FrameIndex];
		//{
		//	err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
		//	check_vk_result(err);

		//	err = vkResetFences(g_Device, 1, &fd->Fence);
		//	check_vk_result(err);
		//}
		//{
		//	err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
		//	check_vk_result(err);
		//	VkCommandBufferBeginInfo info{};
		//	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		//	err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		//	check_vk_result(err);
		//}
	}
}
