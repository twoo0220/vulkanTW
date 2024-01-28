#include "ImGuiRenderer.h"

static void check_vk_result(VkResult err)
{
	if (err == 0) return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0) abort();
}

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

static void SetupVulkan(ImVector<const char*> instance_extensions)
{
}

ImGuiRenderer::ImGuiRenderer(GLFWwindow* window)
	: mGLFWWindow(window)
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
	
}

void ImGuiRenderer::update()
{

}

